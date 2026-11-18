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
- **10 animations** (5 keyframed + 5 simple; see `docs/ESTADO_PROYECTO.md` and the technical manual): walking silhouette, placing a flower on the altar, sea waves, floating candles and flowers, flames, and more.
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

Full details: `docs/manual_usuario/en/` (English) and `docs/manual_usuario/` (Spanish).

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
src/                           C++ source (main.cpp, Camera.h, Shader.h, Model.h, Mesh.h)
vendor/SOIL2/                  SOIL2 + stb_image, compiled with the project
Models/<object>/               3D model: <object>.obj + .mtl + its textures
Shaders/                       GLSL 330 core shaders (.vs / .frag)
```

**Documentation** (Spanish — the full map is in `docs/FLUJO_DE_TRABAJO.md`, section 1)

```
docs/
  ESTADO_PROYECTO.md                Live status: current plan with task codes, progress, decisions, parameters
  CONTEXTO_PROYECTO.md              What was promised: scope, acceptance criteria, stack, conventions
  PLAN_ACTIVIDADES.md               Original work breakdown (60 activities, reference only)
  FLUJO_DE_TRABAJO.md               How the team works: repository map, task cycle, Git rules
  INSTRUCCIONES_PROYECTO_CLAUDE.md  Instructions for the team's Claude project
  propuesta/                        Submitted proposal and work plan (PDF)
  manual_tecnico/                   Technical manual (ES/EN)
  manual_usuario/                   User manual (ES/EN)
  videos/                           Demo videos
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

## License and credits

- **Source code** (`src/`, `Shaders/`, `tools/`) is released under the **MIT License** (see `LICENSE`).
- **Models, textures and audio are not covered by that license.** Some are third-party material with their own
  terms (Poly Haven textures are CC0; the two ambient audio tracks are used for non-commercial educational purposes
  with no free license confirmed). Details and credits: `Audio/README.md`, `docs/manual_tecnico/06_modelos_y_recursos.md`
  and the in-app Credits screen. Do not redistribute them commercially.
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
