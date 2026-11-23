# docs/project

The project's own documents: where it comes from, how fast it runs, what is next and what every release says.

| File | What it is |
|---|---|
| [research.md](research.md) | Initial research: the executable, Switch 2 hardware, every source used with its license, NVN2 against NVN1, shaders |
| [ps5.md](ps5.md) | The PlayStation 5 homebrew build: what it needs, building, installing on the console, its settings and status |
| [performance.md](performance.md) | Frame times measured on real machines, with their date; users add theirs through the performance report form |
| [roadmap.md](roadmap.md), [roadmap.toml](roadmap.toml) | The open work in order of priority; the page is generated from the TOML (`tools/docs/roadmap.py`) |
| `release-template.md` | What every release's notes repeat (install, updating, known issues, reporting); `tools/build/notes.py` fills it with the version's section of `CHANGELOG.md` |
| [upscaling.md](upscaling.md) | DLSS, FSR 3.1, DLSS Frame Generation, FSR 3.1 frame generation and NVIDIA Reflex: settings and how they hook into the game |
