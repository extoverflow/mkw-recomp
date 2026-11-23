// From mkw-pit-stop (MIT, Copyright (c) 2026 Adrià; PS5 work by exploitizm) runtime/core/diag/bench.c @ 0a0259d
// (commit cdae122), unchanged but for these lines and the MKW_WITH_BENCH gate: the same benchmark on both PS5 ports
// for a like-for-like comparison.
// Benchmark mode: MKW_BENCH=tt|gp (Hacks tab, or mkw.ini [hacks] bench = gp) runs races with no player and reports how
// smoothly they ran (tools/ps5/bench.py runs it on the PS5 unattended). Every piece is something the console or the
// runtime already does:
//
// - The way in: MKW_QUICK_RACE=<mode>, MKW_SEED=1 and MKW_SKIP_MOVIES=1 unless the player set them (cfg.c, BENCH
//   preset). tt: the same time trial (Mario, standard kart, the course the seed gives). gp: a Grand Prix with every
//   default choice, A pressed every 1.5 s through the menus, the races and the results between them (input_script.c).
// - The driver: the game's own Smart Steering and Auto-Accelerate (pause menu > Settings), switched on in the save as
//   the player would switch them. GameDataList.Product.100 has no names, only murmur3 hashes; the settings are the
//   nine bools of the struct whose members include murmur3("MinimapOn") and murmur3("CameraReverseX") (hash 664761721,
//   3932007048). main+0x79ec7c copies each into the byte array at [[main+0x32f1e18]] + 0xb84 in the pause menu's
//   order: +0 Smart Steering (main+0x79f058), +1 Tilt Controls, +2 Auto-Accelerate (main+0x79f76c), +3 Auto-Use Item,
//   +4 and +5 the cameras (the bytes CameraReverseX and Y set: the menu's order holds). account.sav keeps each bool as
//   a (hash, value) pair of u32 in the table between its header and the data (offset at +8, 0x620 in 1.0.0).
// - The window starts with the first race: ReadyRace for tt, the first IsSequenceFlag after the cup movie for gp
//   (seqwatch.c turns its watch off there, so the races run without the entry hook). It ends with what fs_host.c sees
//   for free, where a phase watch would cost a probe on every guest call of the race:
//     tt: a time trial saves its result when it ends: the first save file opened for writing at least
//         BENCH_MIN_RACE_S into the race (the earlier ones are the menus').
//     gp: the cup's ending, romfs Movie/EdEmblem_<cup>_00.webm, opened after its last race.
//   MKW_BENCH_MAX=<s> (0: 300 for tt, 1800 for gp) ends a run that never sees it.
// - Marks: every save write and movie opened in the window is logged with its time and frame ("bench: mark"); save
//   writes more than BENCH_MARK_GAP_S apart split the window into segments with their own statistics, so a Grand
//   Prix's races show apart if the game saves after each (to confirm on the first run: the marks say).
// - The report: time, frames, average frame time and rate, the 1 % and 0.1 % lows (the frame time 99 % and 99.9 % of
//   frames stay under, as a rate), the worst frame and the frames over 33.3 and 50 ms, the same per segment, then
//   exit. The run exits before the game commits the last save, so the result and the assist change are not kept
//   unless the game committed the save earlier (the menus save too): the log says what the assists were before.
// Test builds only (bench.h): empty without MKW_WITH_BENCH.
#ifdef MKW_WITH_BENCH
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bench.h"
#include "cfg.h"
#include "host.h"
#include "platform/platform.h"

#define BENCH_MIN_RACE_S 45      // 3 laps of a time trial take longer than this with any driver
#define BENCH_MARK_GAP_S 10      // save writes closer than this are one save (several files)
#define BENCH_FRAMES 108000      // 30 min at 60 fps
#define BENCH_MARKS 64
#define BENCH_SMART_STEERING 3029987336u   // murmur3 of its GameData name, member 3087104944 of the settings struct
#define BENCH_AUTO_ACCELERATE 4206093005u  // member 3690836888
#define SAVE_HEADER_MAGIC 0x01020304u

enum { MODE_OFF, MODE_TT, MODE_GP };
typedef struct {
    uint32_t frame;   // frames recorded before it
    uint64_t ns;
    int save;         // a save write (segment boundary), else a movie
    char what[56];
} Mark;

static float frame_ms[BENCH_FRAMES];
static volatile uint32_t nframes;
static uint64_t last_ns, race_t0;
static volatile uint64_t finish_ns;   // the end (fs_host.c thread), 0 before
static int save_edited, done;
static Mark marks[BENCH_MARKS];
static int nmarks;
static PlatMutex mark_lock = PLAT_MUTEX_INIT;   // marks: fs threads; the report reads them after finish_ns

static int bench_mode(void) {
    static int mode = -1;
    if (mode < 0) {
        const char *v = cfg_value("MKW_BENCH");
        mode = !strcmp(v, "tt") ? MODE_TT : !strcmp(v, "gp") ? MODE_GP : MODE_OFF;
    }
    return mode;
}

int bench_on(void) { return bench_mode() != MODE_OFF; }

// The value slot of a bool's (hash, value) pair in the save's table, or NULL.
static uint8_t *save_bool(uint8_t *b, uint64_t n, uint32_t hash) {
    uint32_t end;
    memcpy(&end, b + 8, 4);
    if (end > n) return NULL;
    for (uint32_t o = 0x20; o + 8 <= end; o += 8) {
        uint32_t h;
        memcpy(&h, b + o, 4);
        if (h == hash) return b + o + 4;
    }
    return NULL;
}

int bench_save(uint8_t *b, uint64_t n) {
    uint32_t magic = 0;
    if (n >= 0x20) memcpy(&magic, b, 4);
    uint8_t *smart = magic == SAVE_HEADER_MAGIC ? save_bool(b, n, BENCH_SMART_STEERING) : NULL;
    uint8_t *accel = magic == SAVE_HEADER_MAGIC ? save_bool(b, n, BENCH_AUTO_ACCELERATE) : NULL;
    if (!smart || !accel) {
        rt_log("bench: account.sav (%llu bytes) has no assist settings where 1.0.0 keeps them: left as it is; the "
               "kart will not drive itself", (unsigned long long)n);
        return 0;
    }
    uint32_t s, a, one = 1;
    memcpy(&s, smart, 4);
    memcpy(&a, accel, 4);
    rt_log("bench: Smart Steering %s -> on, Auto-Accelerate %s -> on (MKW_BENCH; the save keeps them only if the "
           "game commits it before the run ends)", s ? "on" : "off", a ? "on" : "off");
    memcpy(smart, &one, 4);
    memcpy(accel, &one, 4);
    save_edited = 1;
    return s != 1 || a != 1;
}

// A mark in the window; 0 before the window or after the end.
static int mark(int save, const char *path, uint64_t *t0_out) {
    uint64_t t0 = __atomic_load_n(&race_t0, __ATOMIC_ACQUIRE);   // set by the present thread
    if (!bench_on() || !t0 || __atomic_load_n(&finish_ns, __ATOMIC_ACQUIRE)) return 0;
    uint64_t now = kern_now_ns();
    uint32_t fr = __atomic_load_n(&nframes, __ATOMIC_RELAXED);
    const char *name = strrchr(path, '/');
    name = name ? name + 1 : path;
    plat_mutex_lock(&mark_lock);
    if (nmarks < BENCH_MARKS) {
        Mark *m = &marks[nmarks++];
        m->frame = fr, m->ns = now, m->save = save;
        snprintf(m->what, sizeof m->what, "%s", name);
    }
    plat_mutex_unlock(&mark_lock);
    rt_log("bench: mark %.1f s frame %u: %s %s", (now - t0) / 1e9, fr, save ? "save" : "movie", name);
    *t0_out = t0;
    return 1;
}

void bench_save_write(const char *host_path) {
    uint64_t t0;
    if (!mark(1, host_path, &t0) || bench_mode() != MODE_TT) return;
    uint64_t now = kern_now_ns();
    if (now - t0 < (uint64_t)BENCH_MIN_RACE_S * 1000000000ull) return;
    __atomic_store_n(&finish_ns, now, __ATOMIC_RELEASE);
    rt_log("bench: finish at %.1f s (the game saves %s)", (now - t0) / 1e9, host_path);
}

void bench_romfs_opened(const char *path) {
    if (!bench_on() || !strstr(path, "Movie/")) return;   // every romfs open passes here
    uint64_t t0;
    if (!mark(0, path, &t0) || bench_mode() != MODE_GP || !strstr(path, "Movie/EdEmblem_")) return;
    uint64_t now = kern_now_ns();
    __atomic_store_n(&finish_ns, now, __ATOMIC_RELEASE);
    rt_log("bench: the cup ends at %.1f s (%s)", (now - t0) / 1e9, path);
}

static int cmp_float(const void *a, const void *b) {
    float x = *(const float *)a, y = *(const float *)b;
    return (x > y) - (x < y);
}

// Statistics of frames [a, b) over secs; label names them in the lines ("" for the whole window).
static void stats(const char *label, uint32_t a, uint32_t b, double secs, int finished) {
    static float sorted[BENCH_FRAMES];
    uint32_t n = b > a ? b - a : 0;
    if (!n) {
        rt_log("bench: %sno frames", label);
        return;
    }
    memcpy(sorted, frame_ms + a, n * sizeof *sorted);
    qsort(sorted, n, sizeof *sorted, cmp_float);
    double sum = 0;
    uint32_t over33 = 0, over50 = 0;
    for (uint32_t i = 0; i < n; i++) {
        sum += sorted[i];
        over33 += sorted[i] > 33.4f;
        over50 += sorted[i] > 50.0f;
    }
    double avg = sum / n, p99 = sorted[(uint32_t)(n * 0.99)], p999 = sorted[(uint32_t)(n * 0.999)];
    if (!*label) {
        const char *how = finished ? (bench_mode() == MODE_GP ? "Grand Prix finished" : "race finished")
                                   : "no finish seen (MKW_BENCH_MAX)";
        rt_log("bench: %s, %.1f s, %u frames", how, secs, n);
        rt_log("bench: average %.2f ms (%.1f fps), 1%% low %.1f fps, 0.1%% low %.1f fps, worst %.1f ms", avg,
               1000.0 / avg, 1000.0 / p99, 1000.0 / p999, sorted[n - 1]);
        rt_log("bench: frames over 33.3 ms %u, over 50 ms %u", over33, over50);
        rt_log("bench: result mode=%s avg_ms=%.2f fps=%.1f low1=%.1f low01=%.1f worst_ms=%.1f frames=%u secs=%.1f "
               "finished=%d", bench_mode() == MODE_GP ? "gp" : "tt", avg, 1000.0 / avg, 1000.0 / p99, 1000.0 / p999,
               sorted[n - 1], n, secs, finished);
    } else {
        rt_log("bench: %s%.1f s, %u frames, average %.2f ms (%.1f fps), 1%% low %.1f fps, worst %.1f ms, over 33.3 ms "
               "%u", label, secs, n, avg, 1000.0 / avg, 1000.0 / p99, sorted[n - 1], over33);
    }
}

static void report(uint64_t end_ns, int finished) {
    uint32_t n = nframes;
    stats("", 0, n, (end_ns - race_t0) / 1e9, finished);
    // segments: split at save writes BENCH_MARK_GAP_S or more after the previous boundary
    plat_mutex_lock(&mark_lock);
    uint32_t from = 0;
    uint64_t from_ns = race_t0, last_save = 0;
    int seg = 0;
    for (int i = 0; i < nmarks; i++) {
        if (!marks[i].save || (last_save && marks[i].ns - last_save < BENCH_MARK_GAP_S * 1000000000ull)) continue;
        last_save = marks[i].ns;
        char label[48];
        snprintf(label, sizeof label, "segment %d (to %s): ", ++seg, marks[i].what);
        stats(label, from, marks[i].frame, (marks[i].ns - from_ns) / 1e9, 1);
        from = marks[i].frame, from_ns = marks[i].ns;
    }
    plat_mutex_unlock(&mark_lock);
    if (seg) {
        char label[48];
        snprintf(label, sizeof label, "segment %d (to the end): ", seg + 1);
        stats(label, from, n, (end_ns - from_ns) / 1e9, finished);
    }
}

void bench_frame(uint64_t frame) {
    (void)frame;
    if (!bench_on() || done) return;
    int64_t since_us = seqwatch_since_us();   // -1 before the first race
    if (since_us < 0) return;
    uint64_t now = kern_now_ns();
    if (!race_t0) {
        __atomic_store_n(&race_t0, now - (uint64_t)since_us * 1000, __ATOMIC_RELEASE);
        last_ns = now;
        if (!save_edited) rt_log("bench: account.sav was not edited (no save yet?): the assists are the game's defaults");
        rt_log("bench: window starts (%s)", bench_mode() == MODE_GP ? "the Grand Prix's first race" : "ReadyRace");
        return;
    }
    uint32_t n = nframes;
    if (n < BENCH_FRAMES) frame_ms[n] = (float)((now - last_ns) / 1e6), __atomic_store_n(&nframes, n + 1, __ATOMIC_RELEASE);
    last_ns = now;
    uint64_t fin = __atomic_load_n(&finish_ns, __ATOMIC_ACQUIRE);
    double max_s = atof(cfg_value("MKW_BENCH_MAX"));
    if (max_s <= 0) max_s = bench_mode() == MODE_GP ? 1800 : 300;
    if (fin || now - race_t0 >= (uint64_t)(max_s * 1e9)) {
        done = 1;
        __atomic_store_n(&finish_ns, fin ? fin : now, __ATOMIC_RELEASE);   // no more marks
        report(fin ? fin : now, fin != 0);
        fflush(stderr);
        plat_exit(0);
    }
}
#else
typedef int bench_c_not_built;   // ISO C: a translation unit holds at least one declaration
#endif
