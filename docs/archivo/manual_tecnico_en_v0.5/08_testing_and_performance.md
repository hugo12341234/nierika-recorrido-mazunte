# 8. Testing and performance

## 8.1 How testing is done today ✅

- Each task is **built in Release x64** on D2's PC and run with F5; screenshots and a clean console are documented.
- Each PR goes through `/verificar <code>` (the `verificador` subagent) with an APPROVE verdict before merging (since Oct 20); **exception:** PRs #59–#68 were merged on Nov 12 without it (STATE §4).
- There is no CI or automated testing in the repository. Claude cannot build on Windows or see the screen, so building and visual inspection are done by a person.

## 8.2 Performance

| Criterion | Value | Status |
|---|---|---|
| ≥ 30 FPS at 1280×720 on integrated graphics | client target | ❓ **not measured**: A-8 records it as "FPS are fine" (D2's word, no value). The counter exists since E-1a (not compiled) |
| Measurement PC | ❓ **UNDECIDED** (integrated or dedicated?) | — |
| Measured value | ⏳ | The counter exists (window title, average every 0.5 s; E-1a). **D2 still has to write the values down** |

**Table for D2 to fill (empty today):**

| Place | FPS (title) | GPU | Plugged in | Date |
|---|---|---|---|---|
| (1) Shore, z ≈ 23, looking at the sea | ⏳ | | | |
| (2) Cemetery, looking at the room | ⏳ | | | |
| (3) Room, facing the altar | ⏳ | | | |

📷 **Screenshot pending** of each place with the counter visible.

### 8.2.1 Integration of the 3 simple animations (C-5) — measured on Nov 12

> The measurements pre-date the merge of B-13 and C-6 (PRs #59 and #63, now in `develop`): the "with B-13 + C-6" row describes the current code; the first row, the previous state.

Waves (C-1, vertex shader), floating candles (C-3), flames (C-2, fragment shader) and floating flowers (C-4) run **simultaneously** in the draw loop of `src/main.cpp`; no animation depends on another and the `uniform`s do not collide (`oleaje` and `llama` are switched on and off around each draw).

**Triangles per frame** (counted from the `.obj` files × the number of times `main.cpp` draws them; a code-reading count, not a GPU measurement):

| Code state | Triangles per frame | Of which: Maya flower (18 874 tris each) |
|---|---|---|
| `develop` of Nov 12 (before) | ≈ 226 000 | 9 flowers = 169 866 (75 %) |
| with B-13 + C-6 (removes Maya's chapel and bread; floating flowers with the own 6-tri flower) | ≈ 78 000 | 2 flowers = 37 748 |

**Time per frame in a software renderer** (Mesa llvmpipe, 4 CPU threads, 1280×720, 300 frames): **not an integrated GPU**; only useful to compare views and changes with each other.

| View (camera at 1.6 m) | With B-13 + C-6 |
|---|---|
| Looking at the sky (mostly geometry and vertices) | 8.2 ms |
| Looking at the sea from z = 6 | 25.0 ms |
| Looking at the facade from z = 6 | 43.3 ms |
| Inside the room, facing the altar | 67.7 ms (before B-13 + C-6: 77.1 ms) |

**Reading (inferred):** cost is dominated by the *fragment shader* (each pixel evaluates 1 directional light, 4 point lights, 1 spot and 6 step spots), not by triangles: removing ~148 000 triangles lowered the altar view time by only 12 %. On an integrated GPU, the screen area covered by walls near the camera will probably weigh most.

**What is NOT known:** the FPS on the client's GPU. C-5's criterion (≥ 30 FPS with the three animations) **remains unchecked** until D2 measures with the title counter.

**Protocol for D2 (3 minutes):** build Release x64, run without the debugger (Ctrl+F5) and note the title number in 3 places, 10 s each: (1) on the shore (z ≈ 23) looking at the sea, candles and flowers in view; (2) inside the cemetery looking at the room; (3) inside the room facing the altar. Also note the GPU model and whether the laptop was plugged in. If any drops below 30, the first candidate to cut is the step spot lights in the fragment shader.

## 8.3 GPU memory

An explicit `cleanup()` exists in `Shader`, `Mesh` and `Model` and is called at the end of `main` before `glfwTerminate()` (E-1a, Nov 4; Oct 19 decision: no destructors, because the classes are copied). **Nobody has checked with a tool (RenderDoc or similar) that no leaks remain**: problem #7 is still "to verify". ⏳ E-1.

## 8.4 STATE §6 problems on Nov 12 ✅

| # | Problem | Status |
|---|---|---|
| 3 | 2048 px textures of the provisional chapel | Closed in B-9 (Nov 4); the chapel is no longer loaded (B-13) |
| 5 | Missing lighting | Closed in A-8 (Nov 4) |
| 7 | GPU memory | Implemented (E-1a); **to compile and verify with a tool** |
| 8 | `Model.h` without bump maps | Solved in B-12 (Nov 12); not yet seen compiled |
| 9 | `fachada.obj` included the roof | Split in A-11 (Nov 3); to confirm |
| 12 | Arch not semicircular | Fixed in A-10 (Nov 3); to confirm |
| 13 | Animation of the 4 lights possibly from Practice 8 | Closed **on D2's word** ("it's our own"); unverified |
| 14 | 8 unrelated PDFs in `Models/playa/` | Deleted on Nov 11; **still in the history of a public repository** (including a questionnaire); removing them from history would require rewriting it: D2's decision |

Closed earlier: #1, #2 (A-1), #4, #6, #11 (A-2). **New findings of E-5 (Nov 12):** `laminado1.jpg` is 512×512 (the convention asks 1K) and E-2 fixed the wall being mirrored in z. ✅

## 8.5 Final-test error log

⏳ E-1: prioritized error list (STATE criterion). Filled after D2 builds and tests; not written beforehand.
