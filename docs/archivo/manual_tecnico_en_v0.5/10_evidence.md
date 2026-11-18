# 10. Evidence

What proves what, and where it is (status on Nov 12, 2026). The `evidencias/` folder is set by STATE §4 (Oct 19).

| Evidence | What it shows | Where | Status |
|---|---|---|---|
| Scene screenshots of B-12, B-13, C-6 and E-2 (Nov 12) | Own textures, offering, flowers and flames, wall | [`../evidencias/`](../evidencias/) (8 `.jpg`) | 🔄 taken on **Linux with Mesa (software)** with the PRs' code; **not** from D2's PC: repeat with the final look |
| User-manual screenshots (6) | Start window, shore, cemetery, altar, silhouette | [`../../manual_usuario/capturas/`](../../manual_usuario/capturas/) | 🔄 same origin |
| Task screenshots A–D (Nov 4) | Behavior on D2's PC | Claude Project shared folder (`plan/evidencias_2026-11-04/`, ~70 files) | ❓ **not in the repo**; decide which to copy into `evidencias/` |
| Weekly Trello screenshot | Management and "In progress" limits | `evidencias/YYYY-MM-DD_trello.png` | ⏳ none saved (Trello is not reachable from cloud threads) |
| Weekly minutes (3–5 lines) | Progress, delays, agreements | `evidencias/YYYY-MM-DD_minuta.md` | ⏳ none saved |
| Trello JSON export (Oct 19) | Board state that day (56 cards) | ❓ not in the repo | ❓ decide whether to save it |
| Commit and PR history | Who changed what and when | GitHub, PRs #1 to #68 | ✅ (under D2's name; see [§5](05_raci.md)) |
| `/verificar` verdicts | Independent review | PR comments | 🔄 APPROVE on PRs #9, #11, #13–#17, #40 and #41; PRs #59–#68 without verdict |
| `.blend` files | Own modeling | `Models/<object>/*.blend` | ✅ 23 of 23 present (the facade one was rebuilt from the `.obj`) |
| Generation scripts | How models, textures and audio were made | `tools/` | ✅ |
| Numerical tests | Animation math | `tools/d7_pruebas_matematicas.cpp` | ✅ (run with `g++`, see STATE D-7) |
| Audits and self-review | Quality findings | Shared folder (`auditoria/`, `revisiones/`) | ❓ outside the repo |
| Room sketches (X-2) | Facade, interior and offering design | — | ❓ unconfirmed |
| Blender and GIMP versions (X-1) | Same version on both PCs | — | 🔄 |
| Videos | Environment, tour, animations | `docs/videos/` (empty) | ⏳ E-4 |

## Rules

- Whatever the manual cites must be **in the repository** or linked from it.
- No personal data in screenshots (names, emails, statements): only account numbers. (See problem #14: a PDF with personal data is still in Git history.)
- Screenshots carry the date in their name.
