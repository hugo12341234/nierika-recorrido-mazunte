# 2. Architecture

## 2.1 Application layers

Layered architecture from the proposal (Figure 15) ✅, with each module's status on **Nov 12** (code of `develop` with PRs #59–#68 merged):

```
Tour application (C++17, Visual Studio 2022)
 ├─ Camera and collisions   (GLFW · GLM)           ✅ Camera.h · Colisiones.cpp (AABB, A-9)
 ├─ Model loading           (Assimp · SOIL2/stb)   ✅ Model.h / Mesh.h (+ bump map, B-12)
 ├─ Shaders and lighting    (GLSL 330)             ✅ lighting.vs/.frag: Blinn-Phong (A-8), waves (C-1), flame (C-2)
 ├─ Animation               (KeyFrames · slerp)    ✅ Animacion.cpp · Silueta.cpp · DejarFlor.h; simple: Olas.h, AnimVelas.h, AnimFlores.h
 ├─ Cultural cards          (Dear ImGui)           🔄 Fichas.cpp (ImGui is in vendor/; not yet compiled in VS)
 └─ Audio                   (miniaudio)            ✅ Audio.cpp; own tracks Audio/oleaje.wav and musica.wav (D-6)
OpenGL 3.3 Core Profile (GLEW)
GPU / Resources (.obj, textures, audio, keyframes)
```

All motion is computed in the application (KeyFrames in C++); the sea waves are computed in the *vertex shader*, with the same formula on the CPU (`Olas.h`) so candles and flowers can sit on the wave. ✅ `src/Olas.h`

📷 **FIGURE pending:** layer diagram in the Nierika Studio style.

## 2.2 Existing code

Measured with `wc -l` on `develop` on Nov 12, 2026.

| File | Lines | What it does | Origin |
|---|---|---|---|
| `src/main.cpp` | 727 | 1280×720 GLFW window, GLEW, model and shader loading, collisions, render loop with `deltaTime`, input, cleanup | Course Practice 6, extended |
| `src/Camera.h` | 171 | First-person camera: WASD/arrows and mouse, pitch ±89° | Course class |
| `src/Shader.h` | 112 | Compiles and links GLSL programs from files; `cleanup()` | Course class |
| `src/Model.h`, `src/Mesh.h` | 272 · 152 | Loads `.obj` with Assimp, RGBA textures with SOIL2/stb; `Draw(Shader&)`; `cleanup()` | Course classes, extended |
| `src/Colisiones.h/.cpp` | 44 · 46 | Body AABB (0.5 × 0.5 × 1.7 m) against boxes; per-axis sliding; ground height from the beach slope | Own (A-9) |
| `src/Animacion.h/.cpp` | 69 · 83 | `KeyFrame`, `Pista`, own `Slerp`, easing curves | Own (D-2) |
| `src/Silueta.h/.cpp` | 75 · 190 | 10-piece hierarchy, 18.33 s walk and flower drop | Own (D-1, D-2, D-4) |
| `src/DejarFlor.h` | 36 | Quadratic Bézier = free fall of the flower | Own (D-4) |
| `src/Olas.h`, `AnimVelas.h`, `AnimFlores.h` | 90 · 48 · 70 | Formulas of the 3 simple animations (`<cmath>` only) | Own (C-1, C-3, C-4) |
| `src/Fichas.h/.cpp` | 18 · 140 | 5 Dear ImGui cards at < 2 m (nearest wins) | Own (C-7b) |
| `src/Audio.h/.cpp` | 23 · 98 | Two looping tracks with miniaudio; runs silently if files are missing | Own (D-6) |
| `Shaders/lighting.vs/.frag` | 50 · 200 | Blinn-Phong with 1 directional, 4 point, 1 spot and 6 step spot lights; waves, Perlin flame and alpha discard < 0.1 | A-8, C-1, C-2 |
| `Shaders/lamp.vs/.frag` | — | Light cubes (debug, key L) | A-8 |
| `Shaders/modelLoading.*` | — | Unused; kept in the repo | Practice 6 |

## 2.3 Render loop (summary)

1. Initialization: GLFW (1280×720 non-resizable window, captured cursor), GLEW, ImGui, shaders, ≈ 30 models, collision boxes, audio.
2. Per frame: compute `deltaTime` and the FPS title → process input (`DoMovement`, with collisions) → clear → send camera and lights → draw facade, arch, roof, floor, beach, sea (`oleaje = 1`), altar with offering, cemetery, wall, floating flowers and candles, flames (`llama = 1`) and the silhouette with its flower → cultural card if one is within 2 m → swap buffers.
3. Shutdown: explicit `cleanup()` of `Shader`, `Mesh` and `Model` (Oct 19 decision, implemented in E-1a on Nov 4; **real leaks have not been checked with a tool**, STATE §6 problem #7) before `glfwTerminate()`.

📷 **FIGURE pending:** loop flow diagram.

## 2.4 Camera and projection parameters

Current values live in `ESTADO_PROYECTO.md` §5 and are copied here when the manual closes: window 1280×720, FOV 45°, near/far 0.1/300 m; eye height 1.6 m, speed 2.0 m/s (**initial values**, adjustable). ✅

## 2.5 Repository structure

| Path | Content |
|---|---|
| `src/` | C++ code |
| `vendor/` | SOIL2 + stb_image, Dear ImGui, miniaudio (source) |
| `Models/<object>/` | `.obj`, `.mtl`, textures and `.blend` of each model |
| `Shaders/` | GLSL 330 (`.vs`, `.frag`) |
| `Audio/` | Own tracks and README |
| `tools/` | Scripts that generate models (`bpy`), textures and audio, and the Release packager |
| `docs/` | STATE, CONTEXT, PLAN, WORKFLOW, proposal (PDF), manuals, videos |
| `build/`, `dist/` | Build output and standalone Release; **not versioned** |

## 2.6 Architecture decisions (excerpt of STATE §4)

| Date | Decision | Why |
|---|---|---|
| Oct 18 | GLEW instead of GLAD; SOIL2 besides stb_image | Base already compiled and mastered |
| Oct 19 | x64 solution with vcpkg (`builtin-baseline`) | The received solution was 32-bit; same versions on every PC |
| Oct 19 | Explicit `cleanup()`, not destructors | Classes are copied (`Draw` by value, `vector<Mesh>`) |
| Oct 19 | `Draw(Shader&)` by reference | Avoid copying the shader |
| Oct 20 | Beach and sea in world coordinates, identity matrix | STATE §4 ("Beach and sea") |
| Nov 4 | D1's lighting (Blinn-Phong) | D2's decision; replaces "warm ambient + 3 key lights" |
