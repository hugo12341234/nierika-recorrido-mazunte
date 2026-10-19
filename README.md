# Nierika Studio — Virtual Tour: Día de Muertos in Mazunte, Oaxaca

*«Ver más allá de lo visible»*

Real-time, first-person 3D virtual tour that recreates the Day of the Dead celebration in **Mazunte, Oaxaca (Mexico)**: the beach where candles and marigolds are set adrift on the sea at sunset, the village cemetery, and the cemetery chapel with its three-tier offering altar. Visitors walk freely and open short cultural cards that explain the meaning of each element, with its source.

> Final project for *Computer Graphics and Human-Computer Interaction* — School of Engineering, UNAM (semester 2027-1).
> Delivery: **November 18, 2026**.

## Features (planned scope)

- One continuous scene with three zones: **beach and shoreline**, **cemetery** with stone wall, **cemetery chapel** with altar.
- **First-person navigation** (keyboard + mouse) with AABB collisions.
- **Lighting**: warm ambient light + 3 key lights, Blinn-Phong.
- **5 animations**
  - Keyframed (hierarchical silhouette): walking cycle (slerp + smoothstep) and placing a flower on the altar (quadratic Bézier).
  - Procedural: sea waves (sum of sines in the vertex shader), floating candles (Bézier path + sine bobbing, noise-based flame), floating marigolds (circular motion + spin).
- **5 cultural information cards** (Dear ImGui) shown within 2 m of an object, each citing its source.
- Ambient audio loop (waves + distant music).

## Tech stack

| Area | Tools |
|---|---|
| Language / API | C++17 · OpenGL 3.3 Core · GLSL 330 |
| Libraries | GLFW · GLEW · GLM · Assimp · SOIL2 / stb_image · Dear ImGui · miniaudio |
| IDE | Visual Studio 2022 (x64) |
| Assets | Blender · GIMP · CC0 textures (Poly Haven, ambientCG) |
| Management | Git / GitHub (feature branches + pull requests) · Kanban |

## Repository layout

```
docs/                    Project context, plan and live status (Spanish)
  CONTEXTO_PROYECTO.md   Scope, requirements, acceptance criteria, conventions
  PLAN_ACTIVIDADES.md    Work breakdown (60 activities)
  ESTADO_PROYECTO.md     Current plan, progress, decisions, fixed parameters
  manual_tecnico/        Technical manual (ES/EN)
  manual_usuario/        User manual (ES/EN)
  videos/                Demo videos
.claude/                 AI-assistant configuration (agents and commands)
CLAUDE.md                Project rules for the AI assistant
```

```
NierikaStudio.sln / .vcxproj   Visual Studio 2022 solution (x64 only)
vcpkg.json                     Binary dependencies: assimp, glfw3, glew, glm
src/                           C++ source (main.cpp, Camera.h, Shader.h, Model.h, Mesh.h)
vendor/SOIL2/                  SOIL2 + stb_image, compiled with the project
Models/<object>/<object>.obj   3D models (+ .mtl and textures)
Shaders/                       GLSL 330 core shaders (.vs / .frag)
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

## Branching

- `main` — tested, deliverable versions only.
- `develop` — daily integration.
- `feature/act-<N>-<name>` — one branch per task; merged into `develop` through a reviewed pull request.

## Team — Nierika Studio (Team 15)

| Member | Student ID |
|---|---|
| D1 | 319266894 |
| D2 | 321332086 |

---

### Resumen (español)

Recorrido virtual 3D en tiempo real del Día de Muertos en Mazunte, Oaxaca: playa con velas y cempasúchil en el mar, panteón y cuarto del cementerio con altar de tres niveles. Navegación en primera persona con colisiones, iluminación Blinn-Phong, cinco animaciones (dos por cuadros clave) y cinco fichas culturales con fuente. Desarrollado en C++17 con OpenGL 3.3 en Visual Studio 2022. La documentación del proyecto está en `docs/`.
