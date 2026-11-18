# Nierika Studio — Virtual Tour: Día de Muertos in Mazunte, Oaxaca

*«Ver más allá de lo visible»*

Real-time, first-person 3D virtual tour that recreates the Day of the Dead celebration in **Mazunte, Oaxaca (Mexico)**: the beach where candles and marigolds are set adrift on the sea at sunset, the village cemetery, and the cemetery chapel with its three-tier offering altar. Visitors walk freely and open short cultural cards that explain the meaning of each element, with its source.

> Final project for *Computer Graphics and Human-Computer Interaction* — School of Engineering, UNAM (semester 2027-1).
> Delivery: **November 18, 2026**.

## Features

- One continuous scene with three zones: **beach and shoreline**, **cemetery** with stone wall, **cemetery chapel** with altar.
- **First-person navigation** (keyboard + mouse) with AABB collisions, start menu, pause menu and on-screen help (**F1**).
- **Guided tour** (key **T**): 12 narrated stations with subtitles and voice (`Audio/narracion/`); any movement key cancels it.
- **Lighting**: warm ambient light + key lights, Blinn-Phong.
- **10 animations** (5 keyframed + 5 simple; details in `docs/manual_tecnico/09_i_animaciones.md`): walking silhouette, placing a flower on the altar, family dance, playing with the dog and copal censing (keyframed); sea waves, floating candles with flame, floating flowers, band beat and hostess (simple). Only the first five were in the approved proposal.
- **13 cultural information cards** (Dear ImGui) shown within 2 m of an object, each citing its source.
- Ambient audio by zone (waves + band music) and narration; **M** switches the sound mix.

## Controls

| Key | Action |
|---|---|
| W A S D / arrows + mouse | Walk and look around |
| T | Guided tour (T, Esc or W A S D cancel it) |
| V | Candles on/off (within 3 m of the altar) |
| M | Sound: sea + music / sea / music / mute |
| R | Restart the silhouette |
| F1 | Show/hide help |
| Esc | Pause menu |

Full details (Spanish): `docs/manual_usuario/`. The delivered PDFs are in `docs/entrega/`.

## Tech stack

| Area | Tools |
|---|---|
| Language / API | C++17 · OpenGL 3.3 Core · GLSL 330 |
| Libraries | GLFW · GLEW · GLM · Assimp · SOIL2 / stb_image · Dear ImGui · miniaudio |
| IDE | Visual Studio 2022 (x64) |
| Assets | Blender · GIMP · CC0 textures (Poly Haven, ambientCG) |
| Management | Git / GitHub (feature branches + pull requests) · Kanban |

## Repository layout

**Application**

```
NierikaStudio.sln / .vcxproj   Visual Studio 2022 solution (x64 only)
vcpkg.json                     Dependencies installed by vcpkg (assimp, glfw3, glew, glm), pinned by builtin-baseline
src/                           C++ source (main.cpp and one module per feature: Camera, Shader, Model, Mesh, Fichas, Audio, Menu, Recorrido, ...)
vendor/                        SOIL2 + stb_image, Dear ImGui and miniaudio, compiled with the project
Models/<object>/               3D model: <object>.obj + .mtl + its textures (+ .blend source for the team's own models)
Shaders/                       GLSL 330 core shaders (.vs / .frag)
Audio/                         Sea and music tracks (Audio/README.md) and the 12 guided-tour narration files (Audio/narracion/)
tools/                         Helper scripts (Blender/bpy generators, tests, release packaging); see tools/README.md
```

**Documentation** (Spanish; start with `docs/README.md`, which indexes every file)

```
docs/
  README.md                         Index of all documents and their status
  ESTADO_PROYECTO.md                Live status: current plan with task codes, progress, decisions, parameters, open problems
  CONTEXTO_PROYECTO.md              What was promised: scope, acceptance criteria, stack, conventions
  PLAN_ACTIVIDADES.md               Original work breakdown (60 activities, reference only)
  FLUJO_DE_TRABAJO.md               How the team works: repository map, task cycle, Git rules
  REFERENCIAS.md                    All APA 7 references of the project
  entrega/                          Delivery: the two manual PDFs, release notes and checklist
  manual_tecnico/, manual_usuario/  Source (Markdown) of the two manuals
  videos/                           Scripts and notes of the 3 demo videos (the MP4 files are delivered separately, not in Git)
  propuesta/                        Submitted proposal and work plan (PDF)
  archivo/                          Superseded material, kept for history
CLAUDE.md                           Project rules for the AI assistant (Claude Code)
.claude/                            AI-assistant configuration (settings, commands and agents)
```

## Build and run

1. Install **Visual Studio 2022** with the *Desktop development with C++* workload.
2. Install vcpkg once (this is what the team uses):
   ```
   git clone https://github.com/microsoft/vcpkg C:\vcpkg
   C:\vcpkg\bootstrap-vcpkg.bat
   C:\vcpkg\vcpkg integrate install
   ```
   If vcpkg was cloned before, update it with `git -C C:\vcpkg pull`: `vcpkg.json` pins the library versions with a
   `builtin-baseline`, and that commit must exist in your vcpkg copy.
3. Open `NierikaStudio.sln`, choose **Release | x64** (or Debug) and press **F5**.
   The first build downloads and compiles Assimp, GLFW, GLEW and GLM (about 10–20 min); later builds are fast.

## Delivery and current status (Nov 18, 2026)

| Item | Where | Status |
|---|---|---|
| Source code and Visual Studio project | this repository (`develop`) | Builds and runs in Visual Studio 2022 according to D2 (Nov 18); the game's behavior was not changed in the final repository review |
| User manual and technical manual (PDF) | `docs/entrega/` | Generated Nov 18 from the Markdown sources; the technical manual still lacks D1's individual sections (g and j) |
| 3 demo videos (MP4) | not in Git (~450 MB) | Recorded Nov 18 according to D2; delivered separately (`docs/videos/README.md`) |
| Performance | `docs/manual_tecnico/15_anexo_pruebas.md` | Measured by D2 on an Intel Core i5 (11th gen) with integrated graphics: minimum 45 FPS, about 60 FPS on average (before the culling optimization: about 15 FPS). Only one machine was measured |
| Standalone Release `.zip` | `tools/empaquetar_release.ps1` | **Not built or published yet**; not tested on a PC without Visual Studio |
| D1's participation | `docs/manual_tecnico/05_e_roles_raci.md` | **No evidence of D1's work in the repository** (no commits or pull requests under D1's account); D1's individual texts are pending and nobody else writes them |

Open items and decisions are tracked in `docs/ESTADO_PROYECTO.md` (section 6 and section 7).

## License and credits

- **Source code** (`src/`, `Shaders/`, `tools/`) is released under the **MIT License** (see `LICENSE`).
- **Models, textures and audio are not covered by that license.** The sea and music tracks are third-party recordings used
  for non-commercial educational purposes, with no free license confirmed; the 12 narration files were synthesized with
  ElevenLabs (voice and terms in `docs/manual_tecnico/13_m_creditos_licencias.md`); some textures are CC0 (Poly Haven).
  Full credits and APA 7 references: `docs/REFERENCIAS.md`, `Audio/README.md` and the in-app Credits screen.
  Do not redistribute them commercially.
- Libraries in `vendor/` and those installed by vcpkg keep their own licenses.

## Branching

- `main` — tested, deliverable versions only.
- `develop` — daily integration.
- `feature/<task-code>-<name>` (e.g. `feature/a-10-dovelas`) — one branch per task; merged into `develop` through a pull request reviewed by the other team member.

## Team — Nierika Studio (Team 15)

| Member | Student ID |
|---|---|
| D1 | 319266894 |
| D2 | 321332086 |

---

### Resumen (español)

Recorrido virtual 3D en tiempo real del Día de Muertos en Mazunte, Oaxaca: playa con velas y cempasúchil en el mar, panteón y cuarto del cementerio con altar de tres niveles. Navegación en primera persona con colisiones, iluminación Blinn-Phong, diez animaciones (cinco por cuadros clave), recorrido guiado narrado (tecla T) y 13 fichas culturales con fuente. Desarrollado en C++17 con OpenGL 3.3 en Visual Studio 2022. La documentación del proyecto está en `docs/`. El código tiene licencia MIT; los modelos, texturas y audio **no** (ver «License and credits»).
