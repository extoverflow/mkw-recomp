# runtime/core/diag

Diagnostics: flight recorder, counters and profiling, RenderDoc, game phases and the census of SDK calls.

| File | What it is |
|---|---|
| `flight.c`, `flight.h`, `perf.c`, `prof*`, `perfmon.*`, `rdoc.c` | Diagnostics: flight recorder, counters, profiling, RenderDoc |
| `frames.c` | `MKW_FRAMES=<file>`: one tab-separated row per present (frame, world clock, A/B side, interval, replay, render-thread waits) for `tools/runner/abstat.py`; flushed every 60 frames and at exit |
| `cpuctr.c` | `MKW_CPU_COUNTERS=1` (Linux): per-thread cycles, instructions and cache misses (`perf_event_open`, counting) with CPU time, summed for the replay thread, its chunk threads and the game's submitting threads, as extra `MKW_FRAMES` columns (`rep_cpu_us`, `rep_kcyc`, `rep_kins`, `rep_miss`, `chk_…`, `game_…`), each its own `abstat.py` metric: a slower clock, shared caches and more work read apart |
| `seqwatch.c` | `MKW_SEQ_WATCH=1`: entries and exits of the game's phases (`seq: enter TitleMenuMain frame N`) through the function-entry hook of the translated code; the `seq` command of the control channel; `MKW_SEQ_HOLD` holds the button script while a phase lasts; `MKW_SEQ_UNTIL`/`MKW_SEQ_EXIT`: goes off at a phase and exits N s later (`tools/run/race.sh`); `ctl exit` ends the run the same way at the next present |
| `graphchk.c` | `MKW_GRAPH_CHECK=1` (set `graphchk`, 1.0.0): the ModuleSystem's nested job graphs, their jobs, edges and pointer slots against their capacity at each build (`graphchk: ctx … new high` / `OVERFLOW`) jobs not done when a graph is reset, drops on its four rings, and 3 s after a stalled build its frontier
(`graphchk: STALL job …`: not-done jobs with no unfinished predecessor left, owner, vtable, `+` if added after the
build), for the hang at `MKW_LOGIC_HZ=120` |
| `graphchk_edge.c` | With `graphchk`: edges added onto a predecessor already done (`graphchk: LATE edge …`) and decrements of a job whose count is already 0 (`graphchk: decrement from 0 …`, in the finishing job's walk, the empty job's walk and the template's gate), the first 60 of each with graph, class and owner vtable; counts per adder and per decrement site at a stall, and in each stalled job's `STALL job` line its last late edge, decrement from 0 and gate openings |
| `bench.c`, `bench.h` | `MKW_BENCH=tt\|gp`: races with no player (the game's Smart Steering and Auto-Accelerate, A every 1.5 s for `gp`), timed from the first race to the finish, then a report (`bench:` lines) and exit; from mkw-pit-stop (MIT) for a like-for-like comparison of the two PS5 ports. Test builds only: compiled in with `tools/build/build.py --bench` (`MKW_WITH_BENCH`), absent from a release |
| `impcensus.c` | `MKW_IMPORT_CENSUS=<file>`: which SDK functions the game code calls during the run, how many times and from which frame, through the same entry hook as `seqwatch.c`; read by `tools/analisis/runtime/sdk_gaps.py --census` |
