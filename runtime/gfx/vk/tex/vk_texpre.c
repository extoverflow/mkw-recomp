// Precomputed BC7 of ASTC textures. Without native ASTC, vk_astc.c decodes each ASTC texture of 256x256 or more on the
// GPU and encodes it again as BC7 with mode 6 only; here the same texture comes ready, encoded offline with every mode
// bc7enc.cpp has (runtime/third_party/bc7enc, MIT).
//
// Only what the game loads: the whole romfs in BC7 would take 34.5 GiB, 17 GiB with zstd (23 545 textures; sample of 40).
// - The upload hashes the texture's storage (XXH3 of every byte, swizzled as NVN keeps it). When <dir>/<hash>.bc7.zst
//   exists it holds the BC7 blocks of every region of the upload, in its order (levels, then layers; slices of a 3D level
//   one after the other), zstd-compressed, and goes to the image as is.
// - Otherwise the upload goes on through the GPU encoder, and a writer thread stores the storage with its descriptor and
//   regions in <dir>/src/<hash>.astc.zst (MKW_TEXPRE_RECORD=0: not).
// - build/mkw --texpre encodes every src file into its .bc7.zst on all cores, with the runtime's own deswizzle and ASTC
//   decoder (bit-exact with the GPU decoder: --probe-astc), and deletes the src file; a .bc7.zst of one frame (written
//   before CHUNK) is split into frames.
// MKW_TEXPRE=1 (directory <exe dir>/texpre) or =<dir>; off by default: in the menus up to frame 1400 the uploads took
// the render thread 3 088 ms with it against 1 543 ms without (live21 / live22, 31/10/2026; tools/run/texms.sh), as the
// BC7 is 4 times the bytes of the ASTC and the GPU encoder costs the CPU little. What it gives is quality (every BC7
// mode). The directory listing is read once at the first upload.
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zstd.h>
#define XXH_INLINE_ALL
#include <xxhash.h>

#include "cfg.h"
#include "host.h"
#include "nvn_hmap.h"
#include "tex.h"
#include "vk.h"
#include "platform/platform.h"

void bc7enc_c_init(void);
void bc7enc_c_block(void *out16, const void *rgba64);

#define SRC_MAGIC 0x3150585457544b4dull   // "MKTWXTP1"
typedef struct {
    uint64_t magic, size;   // size: bytes of storage after the regions
    nvn_texdesc d;
    uint32_t n, pad;
} SrcHead;

static struct {
    char dir[PLAT_PATH_MAX];
    int on, record;
    nvn_hmap have;          // hashes with a .bc7.zst (value bit 0) or a .bc1.zst (bit 1)
    uint32_t n_bc7, n_bc1;
    nvn_hmap seen;          // hashes recorded (or found) in this run
    PlatMutex lock;         // seen, and pending
    uint64_t pending;       // bytes held by writer threads
    uint64_t n_hit, n_rec, hit_bytes;
} T = {.have = {.klen = 8}, .seen = {.klen = 8}};
static PlatOnce t_once = PLAT_ONCE_INIT;

static int list_cb(const char *name, void *ctx) {   // plat_file_list: 1 goes on
    (void)ctx;
    char *end;
    uint64_t h = strtoull(name, &end, 16);
    uintptr_t kind = end != name + 16 ? 0 : !strcmp(end, ".bc7.zst") ? 1 : !strcmp(end, ".bc1.zst") ? 2 : 0;
    if (!kind) return 1;
    nvn_hm_put(&T.have, &h, (void *)((uintptr_t)nvn_hm_get(&T.have, &h) | kind));
    if (kind == 1) T.n_bc7++;
    else T.n_bc1++;
    return 1;
}

static void t_init(void *arg) {
    (void)arg;
    const char *e = cfg_get("MKW_TEXPRE");
    if (!e || !*e || !strcmp(e, "0")) return;
    if (strcmp(e, "1") && strlen(e) < sizeof T.dir) {
        strcpy(T.dir, e);
    } else {
        snprintf(T.dir, sizeof T.dir, "%s/texpre", cfg_game_dir());
    }
    const char *r = cfg_get("MKW_TEXPRE_RECORD");
    T.record = !(r && !strcmp(r, "0"));
    plat_file_list(T.dir, ".zst", list_cb, NULL);
    T.on = 1;
    rt_log("vk: precomputed textures in %s: %u BC7, %u BC1 (recording new ones: %s)", T.dir, T.n_bc7, T.n_bc1,
           T.record ? "on" : "off");
}

int vk_texpre_on(void) {
    plat_once(&t_once, t_init, NULL);
    return T.on;
}

uint64_t vk_texpre_hash(const void *p, uint64_t n) { return XXH3_64bits(p, (size_t)n); }

uint64_t vk_texpre_region_bytes(const VkTexpreRegion *r, int bc1) {
    return (uint64_t)((r->w + 3) / 4) * ((r->h + 3) / 4) * (bc1 ? 8 : 16) * r->slices;
}

int vk_texpre_has(uint64_t h, int bc1) {
    return vk_texpre_on() && ((uintptr_t)nvn_hm_get(&T.have, &h) & (bc1 ? 2 : 1));
}

// A .bc7.zst is zstd frames of at most CHUNK bytes each, one after the other: one thread decodes 1 GiB in ~2.4 s (zstd
// -b on a 121 MiB file, 505 MB/s), and the menus load 1.2 GiB in one frame (live19), so the frames go to LOAD_THREADS.
#define CHUNK (4u << 20)
#define LOAD_THREADS 8
// skip/take: a frame that only part of a read covers decodes into a scratch block, and its bytes [skip, skip + take)
// go to dst; a whole one (take 0) decodes into dst.
typedef struct { const uint8_t *src; size_t sn; uint8_t *dst; size_t dn, skip, take; } Frame;
typedef struct { Frame *f; int n; volatile int32_t next, bad; } FrameJob;

static void frame_worker(void *arg) {
    plat_thread_background();   // off the frame's fast cores (MKW_FAST_CORES)
    FrameJob *j = arg;
    for (int i; (i = plat_inc32(&j->next) - 1) < j->n;) {
        Frame *f = &j->f[i];
        uint8_t *tmp = f->take ? malloc(f->dn) : NULL;
        if ((f->take && !tmp) || ZSTD_decompress(tmp ? tmp : f->dst, f->dn, f->src, f->sn) != f->dn) plat_inc32(&j->bad);
        else if (tmp) memcpy(f->dst, tmp + f->skip, f->take);
        free(tmp);
    }
}

static int run_frames(Frame *f, int nf) {
    FrameJob j = {f, nf};
    PlatThread *th[LOAD_THREADS - 1];
    int nt = nf - 1 < LOAD_THREADS - 1 ? nf - 1 : LOAD_THREADS - 1;
    for (int i = 0; i < nt; i++) th[i] = plat_thread_new(frame_worker, &j, "texpre-load");
    frame_worker(&j);
    for (int i = 0; i < nt; i++) if (th[i]) plat_thread_join(th[i]);
    return !j.bad;
}

// buf[0, n) as frames of CHUNK bytes at zstd level: the compressed bytes (malloc) and their size in *zn; NULL on failure.
static uint8_t *encode_frames(const uint8_t *buf, size_t n, int level, size_t *zn) {
    size_t cap = 0;
    for (size_t at = 0; at < n; at += CHUNK) cap += ZSTD_compressBound(n - at < CHUNK ? n - at : CHUNK);
    uint8_t *z = malloc(cap ? cap : 1);
    *zn = 0;
    for (size_t at = 0; z && at < n; at += CHUNK) {
        size_t r = ZSTD_compress(z + *zn, cap - *zn, buf + at, n - at < CHUNK ? n - at : CHUNK, level);
        if (ZSTD_isError(r)) { free(z); return NULL; }
        *zn += r;
    }
    return z;
}

// A .bc7.zst read in pieces: its frames found once (vk_texpre_open), then any range of its bytes decoded
// (vk_texpre_read), so a texture larger than a staging block goes up in groups as upload() sends the GPU path's
// (vk_tex_upload.c): the 8188x7292 map texture's 121 MB at once failed on the PS5 ("staging buffer of 126670752 bytes
// failed", 19/11/2026).
struct VkTexpreFile {
    uint8_t *z;
    uint64_t size;
    int nf;
    struct { size_t zoff, zn; uint64_t at, n; } *f;   // each frame: its compressed bytes and the bytes it decodes to
    int bad;
};

static void texpre_bad(const char *path, uint64_t size) {
    static volatile int32_t n_bad;
    if (plat_inc32(&n_bad) <= 16) rt_log("vk: %s does not hold %llu bytes of blocks; not used", path, (unsigned long long)size);
}

VkTexpreFile *vk_texpre_open(uint64_t h, int bc1, uint64_t size) {
    if (!vk_texpre_has(h, bc1)) return NULL;
    char path[PLAT_PATH_MAX + 32];
    snprintf(path, sizeof path, "%s/%016llx.%s.zst", T.dir, (unsigned long long)h, bc1 ? "bc1" : "bc7");
    uint64_t n = 0;
    VkTexpreFile *t = calloc(1, sizeof *t);
    int cap = (int)(size / CHUNK) + 2;
    if (!t || !(t->z = plat_file_read(path, &n)) || !(t->f = malloc(sizeof *t->f * (size_t)cap))) goto bad;
    uint64_t out = 0;
    for (size_t off = 0; off < n; off += t->f[t->nf++].zn) {
        size_t fs = ZSTD_findFrameCompressedSize(t->z + off, n - off);
        unsigned long long cs = ZSTD_isError(fs) ? ZSTD_CONTENTSIZE_ERROR : ZSTD_getFrameContentSize(t->z + off, fs);
        if (t->nf == cap || cs == ZSTD_CONTENTSIZE_ERROR || cs == ZSTD_CONTENTSIZE_UNKNOWN || out + cs > size) goto bad;
        t->f[t->nf].zoff = off, t->f[t->nf].zn = fs, t->f[t->nf].at = out, t->f[t->nf].n = cs;
        out += cs;
    }
    if (!t->nf || out != size) goto bad;
    t->size = size;
    return t;
bad:
    texpre_bad(path, size);
    if (t) free(t->z), free(t->f), free(t);
    return NULL;
}

// Bytes [at, at + n) of t's BC7 into dst; 1 on success.
int vk_texpre_read(VkTexpreFile *t, uint64_t at, void *dst, uint64_t n) {
    if (t->bad || at + n > t->size) return 0;
    Frame *f = malloc(sizeof *f * (size_t)t->nf);
    int nf = 0;
    for (int i = 0; f && i < t->nf; i++) {
        uint64_t a = t->f[i].at, b = a + t->f[i].n, lo = a > at ? a : at, hi = b < at + n ? b : at + n;
        if (lo >= hi) continue;
        int whole = lo == a && hi == b;
        f[nf++] = (Frame){t->z + t->f[i].zoff, t->f[i].zn, (uint8_t *)dst + (lo - at), t->f[i].n, lo - a,
                          whole ? 0 : hi - lo};
    }
    int ok = f && nf && run_frames(f, nf);
    free(f);
    if (!ok) t->bad = 1;
    return ok;
}

// 1 when every read succeeded (counted as a hit)
int vk_texpre_close(VkTexpreFile *t) {
    if (!t) return 0;
    int ok = !t->bad;
    if (ok) {
        plat_mutex_lock(&T.lock);
        T.n_hit++, T.hit_bytes += t->size;
        if (!(T.n_hit & (T.n_hit - 1)))
            rt_log("vk: %llu precomputed textures loaded (%.0f MiB)", (unsigned long long)T.n_hit, T.hit_bytes / 1048576.0);
        plat_mutex_unlock(&T.lock);
    }
    free(t->z), free(t->f), free(t);
    return ok;
}

typedef struct { uint64_t h; size_t n; uint8_t *buf; } WriteJob;

static void writer(void *arg) {
    WriteJob *j = arg;
    size_t cap = ZSTD_compressBound(j->n);
    uint8_t *z = malloc(cap);
    size_t zn = z ? ZSTD_compress(z, cap, j->buf, j->n, 3) : 0;
    char dir[PLAT_PATH_MAX + 8], path[PLAT_PATH_MAX + 48];
    snprintf(dir, sizeof dir, "%s/src", T.dir);
    snprintf(path, sizeof path, "%s/%016llx.astc.zst", dir, (unsigned long long)j->h);
    plat_file_mkdir_p(dir);
    if (z && !ZSTD_isError(zn) && !plat_file_write_atomic(path, z, zn)) rt_log("vk: cannot write %s", path);
    free(z);
    plat_mutex_lock(&T.lock);
    T.pending -= j->n;
    plat_mutex_unlock(&T.lock);
    free(j->buf);
    free(j);
}

// At most this many bytes wait for writer threads; a texture past it is recorded in a later run.
#define PENDING_MAX (1ull << 30)

void vk_texpre_record(uint64_t h, const nvn_texdesc *d, const VkTexpreRegion *rg, int n, const void *mem, uint64_t size) {
    if (!vk_texpre_on() || !T.record) return;
    size_t total = sizeof(SrcHead) + sizeof *rg * (size_t)n + (size_t)size;
    plat_mutex_lock(&T.lock);
    int seen = nvn_hm_get(&T.seen, &h) != NULL, full = !seen && T.pending + total > PENDING_MAX;
    if (!seen && !full) nvn_hm_put(&T.seen, &h, (void *)1), T.pending += total, T.n_rec++;
    static uint64_t n_full;
    uint64_t nf = full ? ++n_full : 0;
    if (nf && !(nf & (nf - 1)))
        rt_log("vk: %llu textures not recorded: %.0f MiB already wait for the disk (next run records them)",
               (unsigned long long)n_full, T.pending / 1048576.0);
    plat_mutex_unlock(&T.lock);
    if (seen || full) return;
    WriteJob *j = malloc(sizeof *j);
    uint8_t *b = malloc(total);
    if (!j || !b) rt_fatal("vk: out of memory recording a texture");
    SrcHead hd = {.magic = SRC_MAGIC, .size = size, .d = *d, .n = (uint32_t)n};
    memcpy(b, &hd, sizeof hd);
    memcpy(b + sizeof hd, rg, sizeof *rg * (size_t)n);
    memcpy(b + sizeof hd + sizeof *rg * (size_t)n, mem, (size_t)size);
    *j = (WriteJob){h, total, b};
    if (!plat_thread_spawn(writer, j, "texpre", 0)) writer(j);
}

// ---- build/mkw --texpre
typedef struct {
    char (*names)[64];
    int n, cap;
    volatile int32_t next;
    uint64_t in_bytes, out_bytes, texels;
    int failed, n_bc1, n_small;
} EncList;

// One 4x4 block of RGBA8 texels -> BC1 (four-colour mode, alpha ignored): bc1_encode.comp's method on the CPU (the
// GPU encoder this file stands in for when the texture becomes BC1, vk_tex_image.c): the bounding segment along the
// block's principal RGB axis, then one least-squares refinement on the chosen indices.
static unsigned pack565(const float c[3]) {
    float m[3] = {31.f, 63.f, 31.f};
    unsigned q[3];
    for (int i = 0; i < 3; i++) {
        float v = roundf(c[i] * m[i] / 255.f);
        q[i] = (unsigned)(v < 0 ? 0 : v > m[i] ? m[i] : v);
    }
    return q[0] << 11 | q[1] << 5 | q[2];
}
static void unpack565(unsigned v, float c[3]) {
    c[0] = (float)((v >> 11) * 255u / 31u), c[1] = (float)(((v >> 5) & 63u) * 255u / 63u), c[2] = (float)((v & 31u) * 255u / 31u);
}
static float bc1_fit(const float c[16][3], unsigned e0, unsigned e1, unsigned idx[16]) {
    float pal[4][3], total = 0;
    unpack565(e0, pal[0]), unpack565(e1, pal[1]);
    for (int k = 0; k < 3; k++)
        pal[2][k] = (2 * pal[0][k] + pal[1][k]) / 3, pal[3][k] = (pal[0][k] + 2 * pal[1][k]) / 3;
    for (int i = 0; i < 16; i++) {
        float best = 1e9f;
        unsigned bi = 0;
        for (unsigned k = 0; k < 4; k++) {
            float d0 = pal[k][0] - c[i][0], d1 = pal[k][1] - c[i][1], d2 = pal[k][2] - c[i][2], err = d0 * d0 + d1 * d1 + d2 * d2;
            if (err < best) best = err, bi = k;
        }
        idx[i] = bi, total += best;
    }
    return total;
}
static void clamp255(float v[3]) {
    for (int k = 0; k < 3; k++) v[k] = v[k] < 0 ? 0 : v[k] > 255 ? 255 : v[k];
}
static void bc1_block(uint8_t out[8], const uint8_t rgba[64]) {
    float c[16][3], mean[3] = {0}, cov[3][3] = {{0}};
    for (int i = 0; i < 16; i++)
        for (int k = 0; k < 3; k++) c[i][k] = rgba[4 * i + k], mean[k] += c[i][k] / 16;
    for (int i = 0; i < 16; i++)
        for (int a = 0; a < 3; a++)
            for (int b = 0; b < 3; b++) cov[a][b] += (c[i][a] - mean[a]) * (c[i][b] - mean[b]);
    float ax[3] = {1, 1, 1};
    for (int it = 0; it < 6; it++) {
        float n[3];
        for (int a = 0; a < 3; a++) n[a] = cov[a][0] * ax[0] + cov[a][1] * ax[1] + cov[a][2] * ax[2];
        float l = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
        for (int a = 0; a < 3; a++) ax[a] = l > 1e-6f ? n[a] / l : 0.577f;
    }
    float tmin = 1e9f, tmax = -1e9f;
    for (int i = 0; i < 16; i++) {
        float t = (c[i][0] - mean[0]) * ax[0] + (c[i][1] - mean[1]) * ax[1] + (c[i][2] - mean[2]) * ax[2];
        tmin = t < tmin ? t : tmin, tmax = t > tmax ? t : tmax;
    }
    float p0[3], p1[3];
    for (int k = 0; k < 3; k++) p0[k] = mean[k] + ax[k] * tmax, p1[k] = mean[k] + ax[k] * tmin;
    clamp255(p0), clamp255(p1);
    unsigned e0 = pack565(p0), e1 = pack565(p1), idx[16];
    if (e0 < e1) { unsigned t = e0; e0 = e1, e1 = t; }
    float err = bc1_fit(c, e0, e1, idx);
    static const float W[4] = {1.f, 0.f, 2.f / 3.f, 1.f / 3.f};
    float aa = 0, bb = 0, ab = 0, sa[3] = {0}, sb[3] = {0};
    for (int i = 0; i < 16; i++) {
        float w = W[idx[i]];
        aa += w * w, bb += (1 - w) * (1 - w), ab += w * (1 - w);
        for (int k = 0; k < 3; k++) sa[k] += w * c[i][k], sb[k] += (1 - w) * c[i][k];
    }
    float det = aa * bb - ab * ab;
    if (fabsf(det) > 1e-4f && e0 != e1) {
        float r0[3], r1[3];
        for (int k = 0; k < 3; k++) r0[k] = (sa[k] * bb - sb[k] * ab) / det, r1[k] = (sb[k] * aa - sa[k] * ab) / det;
        clamp255(r0), clamp255(r1);
        unsigned f0 = pack565(r0), f1 = pack565(r1), idx2[16];
        if (f0 < f1) { unsigned t = f0; f0 = f1, f1 = t; }
        float err2 = f0 != f1 ? bc1_fit(c, f0, f1, idx2) : 1e9f;
        if (err2 < err) {
            e0 = f0, e1 = f1;
            memcpy(idx, idx2, sizeof idx);
        }
    }
    uint32_t bits = 0;
    if (e0 != e1)
        for (int i = 0; i < 16; i++) bits |= idx[i] << (2 * i);
    uint32_t lo = e0 | e1 << 16;
    memcpy(out, &lo, 4), memcpy(out + 4, &bits, 4);
}

// What the runtime makes of this ASTC texture (vk_tex_image.c's choice for storage the CPU reads, MKW_ASTC_BC1 on):
// 0 RGBA8 (under 256x256: not cached), 1 BC7, 2 BC1.
static int target_kind(const nvn_texdesc *d, const uint8_t *mem, uint64_t size, const TexVkFormat *f) {
    if ((uint64_t)d->width * d->height < 256u * 256u) return 0;
    const char *e = cfg_get("MKW_ASTC_BC1");
    int bc1_on = !e || !*e || atoi(e);
    if (!bc1_on || d->width >= 4096 || d->height >= 4096 || (d->flags & NVN_TEXTURE_FLAGS_SPARSE)) return 1;
    return vk_astc_storage_bc1(mem, size, f->astc_bw, f->astc_bh) ? 2 : 1;
}
static PlatMutex enc_lock;

static int src_cb(const char *name, void *ctx) {   // also the .bc7.zst of the directory (rechunk_one)
    EncList *L = ctx;
    if (strlen(name) >= 64 || !(strstr(name, ".astc.zst") || strstr(name, ".bc7.zst") || strstr(name, ".bc1.zst"))) return 1;
    if (L->n == L->cap && !(L->names = realloc(L->names, sizeof *L->names * (size_t)(L->cap = L->cap ? 2 * L->cap : 256))))
        rt_fatal("texpre: out of memory");
    strcpy(L->names[L->n++], name);
    return 1;
}

// One src file into its .bc7.zst; 1 on success.
// 1 done (encoded, or not cached: *kind 0), 0 failed; *kind: 0 RGBA8 (none), 1 BC7, 2 BC1
static int encode_one(const char *name, uint64_t *in_b, uint64_t *out_b, uint64_t *texels, int *kind) {
    char path[PLAT_PATH_MAX + 96], out[PLAT_PATH_MAX + 96];
    snprintf(path, sizeof path, "%s/src/%s", T.dir, name);
    *kind = -1;
    uint64_t zn = 0;
    uint8_t *z = plat_file_read(path, &zn), *b = NULL, *bc = NULL;
    unsigned long long raw = z ? ZSTD_getFrameContentSize(z, zn) : 0;
    int ok = 0;
    if (!z || raw == ZSTD_CONTENTSIZE_ERROR || raw == ZSTD_CONTENTSIZE_UNKNOWN || raw < sizeof(SrcHead)) goto done;
    if (!(b = malloc(raw)) || ZSTD_decompress(b, raw, z, zn) != raw) goto done;
    SrcHead hd;
    memcpy(&hd, b, sizeof hd);
    const VkTexpreRegion *rg = (const VkTexpreRegion *)(b + sizeof hd);
    const uint8_t *mem = b + sizeof hd + sizeof *rg * hd.n;
    TexVkFormat f;
    if (hd.magic != SRC_MAGIC || sizeof hd + sizeof *rg * hd.n + hd.size != raw || !tex_vk_format(hd.d.format, &f) ||
        !f.astc_bw)
        goto done;
    int bc1 = (*kind = target_kind(&hd.d, mem, hd.size, &f)) == 2;
    if (!*kind) {   // the runtime keeps it RGBA8 and never looks for it
        plat_file_delete(path);
        ok = 1;
        goto done;
    }
    snprintf(out, sizeof out, "%s/%.16s.%s.zst", T.dir, name, bc1 ? "bc1" : "bc7");
    uint64_t total = 0;
    for (uint32_t k = 0; k < hd.n; k++) total += vk_texpre_region_bytes(&rg[k], bc1);
    if (!(bc = malloc(total))) goto done;
    uint8_t *o = bc;
    for (uint32_t k = 0; k < hd.n; k++) {
        const VkTexpreRegion *r = &rg[k];
        uint32_t rb = (r->w + f.astc_bw - 1) / f.astc_bw * 16, rows = (r->h + f.astc_bh - 1) / f.astc_bh;
        uint8_t *lin = malloc((size_t)rb * rows * r->slices), *px = malloc((size_t)r->w * r->h * 4);
        int bad = !lin || !px || nvn_deswizzle(&hd.d, r->level, r->layer, mem, lin, rb);
        for (uint32_t s = 0; !bad && s < r->slices; s++) {
            astc_decode_ldr(lin + (size_t)s * rb * rows, r->w, r->h, f.astc_bw, f.astc_bh, f.astc_srgb, px);
            for (uint32_t by = 0; by < r->h; by += 4)
                for (uint32_t bx = 0; bx < r->w; bx += 4, o += bc1 ? 8 : 16) {
                    uint8_t blk[64];
                    for (uint32_t y = 0; y < 4; y++)   // edge blocks repeat the last row and column
                        for (uint32_t x = 0; x < 4; x++) {
                            uint32_t sx = bx + x < r->w ? bx + x : r->w - 1, sy = by + y < r->h ? by + y : r->h - 1;
                            memcpy(blk + 4 * (4 * y + x), px + 4 * ((size_t)sy * r->w + sx), 4);
                        }
                    if (bc1) bc1_block(o, blk);
                    else bc7enc_c_block(o, blk);
                }
        }
        free(lin), free(px);
        if (bad) goto done;
        *texels += (uint64_t)r->w * r->h * r->slices;
    }
    size_t zon;
    uint8_t *zo = encode_frames(bc, total, 19, &zon);
    ok = zo && plat_file_write_atomic(out, zo, zon);
    free(zo);
    if (ok) {
        plat_file_delete(path);
        *in_b += zn, *out_b += zon;
    }
done:
    if (!ok) printf("texpre: %s could not be encoded\n", name);
    free(z), free(b), free(bc);
    return ok;
}

// A .bc7.zst written as one frame (before CHUNK) and larger than CHUNK, written again as frames; 1 unless it fails.
static int rechunk_one(const char *name) {
    char path[PLAT_PATH_MAX + 96];
    snprintf(path, sizeof path, "%s/%s", T.dir, name);
    uint64_t zn = 0;
    uint8_t *z = plat_file_read(path, &zn), *b = NULL, *zo = NULL;
    unsigned long long raw = z ? ZSTD_getFrameContentSize(z, zn) : ZSTD_CONTENTSIZE_ERROR;
    int ok = z && raw != ZSTD_CONTENTSIZE_ERROR && raw != ZSTD_CONTENTSIZE_UNKNOWN;
    if (ok && raw > CHUNK && ZSTD_findFrameCompressedSize(z, zn) == zn) {
        size_t on;
        ok = (b = malloc(raw)) && ZSTD_decompress(b, raw, z, zn) == raw && (zo = encode_frames(b, raw, 19, &on)) &&
             plat_file_write_atomic(path, zo, on);
    }
    if (!ok) printf("texpre: %s could not be split into frames\n", name);
    free(z), free(b), free(zo);
    return ok;
}

static void enc_worker(void *arg) {
    plat_thread_background();   // off the frame's fast cores (MKW_FAST_CORES)
    EncList *L = arg;
    uint64_t in_b = 0, out_b = 0, tx = 0;
    int failed = 0, n_bc1 = 0, n_small = 0;
    for (int i; (i = plat_inc32(&L->next) - 1) < L->n;) {
        int kind = -1;
        failed += strstr(L->names[i], ".astc.zst") ? !encode_one(L->names[i], &in_b, &out_b, &tx, &kind)
                                                   : !rechunk_one(L->names[i]);
        n_bc1 += kind == 2, n_small += kind == 0;
        if ((i + 1) % 100 == 0) printf("texpre: %d of %d\n", i + 1, L->n), fflush(stdout);
    }
    plat_mutex_lock(&enc_lock);
    L->in_bytes += in_b, L->out_bytes += out_b, L->texels += tx, L->failed += failed, L->n_bc1 += n_bc1, L->n_small += n_small;
    plat_mutex_unlock(&enc_lock);
}

int vk_texpre_encode(void) {
    if (!vk_texpre_on()) { printf("texpre: off (set MKW_TEXPRE=1 or MKW_TEXPRE=<dir>)\n"); return 1; }
    bc7enc_c_init();
    char dir[PLAT_PATH_MAX + 8];
    snprintf(dir, sizeof dir, "%s/src", T.dir);
    EncList L = {0};
    plat_file_list(dir, ".zst", src_cb, &L);
    plat_file_list(T.dir, ".bc7.zst", src_cb, &L);
    plat_file_list(T.dir, ".bc1.zst", src_cb, &L);
    const char *j = cfg_get("MKW_TEXPRE_JOBS");
    int jobs = j && atoi(j) > 0 ? atoi(j) : plat_cpu_count();
    printf("texpre: %d files in %s (to encode) and %s (to check for frames) with %d threads\n", L.n, dir, T.dir, jobs), fflush(stdout);
    uint64_t t0 = kern_now_ns();
    PlatThread **th = calloc((size_t)jobs, sizeof *th);
    for (int i = 0; i < jobs; i++) th[i] = plat_thread_new(enc_worker, &L, "texpre");
    for (int i = 0; i < jobs; i++) if (th[i]) plat_thread_join(th[i]);
    double s = (kern_now_ns() - t0) / 1e9;
    printf("texpre: %d done (%d of them BC1, %d under 256x256 not cached), %d failed, %.1f M texels in %.1f s; %.1f MiB "
           "of ASTC (zstd) -> %.1f MiB of BC7/BC1 (zstd)\n", L.n - L.failed, L.n_bc1, L.n_small, L.failed, L.texels / 1e6,
           s, L.in_bytes / 1048576.0, L.out_bytes / 1048576.0);
    free(th), free(L.names);
    return L.failed != 0;
}
