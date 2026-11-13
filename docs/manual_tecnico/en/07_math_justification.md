# 7. Mathematical justification of the 5 animations

> **Status (Nov 12, 2026):** all 5 animations have code and a justification using **the code's values** with `file:line`. Full derivations live in two Spanish documents that are part of this manual and are **not translated yet**:
> - [`../justificacion_simples.md`](../justificacion_simples.md): waves, candles, flame and flowers (C-8, Nov 11).
> - [`../justificacion_complejas.md`](../justificacion_complejas.md): slerp, smoothstep, hierarchy and Bézier (D-3, D-7, Nov 12; 23 numerical checks with `tools/d7_pruebas_matematicas.cpp`).
>
> **Not verified:** that any animation looks right with MSVC and the real model (nobody has compiled since the switch) and the "not from the course" comparison, because **the course practices are not in the repository**: D2 reviews it.

## 7.1 Summary ✅ (STATE §5)

| # | Animation | Type | Formula / technique | Values in the code | Task |
|---|---|---|---|---|---|
| 1 | Silhouette walk (shore → altar) | KeyFrames | Quaternions with own **slerp**; advance with **smoothstep** (SmoothIn/SmoothOut when starting and stopping); legs half a cycle apart, arms in opposition | z = +24 → +0.2 (23.8 m = 17 strides of 1.4 m), cruise 1.4 m/s, 2 s start and stop, **18.33 s** total, 1 s stride cycle; ends at x = +0.45 (D-5, Nov 12) | D-2, D-3 |
| 2 | Silhouette leaving a flower | KeyFrames | Right arm by KeyFrames with slerp; flower on a **quadratic Bézier** with P1 at hand height and halfway horizontally, so the curve **is** a horizontal projectile in free fall | after the walk: extends 1 s, releases 2 s, returns 3–4 s; shoulder +75°, forearm +10°; P2 = (1.05, 0.435, hand z); T_b = √(2h/g) ≈ 0.429 s with h ≈ 0.901 m | D-4 |
| 3 | Waves | Simple | Vertex shader: y = −0.2 + Σ A·sin(k·(d·x) − ωt + φ), ω = √(9.81·k); analytic normal | 3 waves: A 0.040 / 0.025 / 0.015 m · λ 12 / 7 / 4.5 m · θ 0° / 25° / −30° · φ 0 / 1.7 / 4.1 rad; height between −0.28 and −0.12 m | C-1 |
| 4 | Floating candles | Simple | **Quadratic Bézier** out to sea + vertical sinusoidal bobbing with per-candle phase; flame: 2D Perlin noise in the fragment shader | 4 candles: P0 (x0, 25), P1 (x0 ± 8, 40), P2 (x0 ± 3, 60), x0 = −2.5, 0.5, 2.5, 5.0; T 100–130 s; bobbing 0.012 m, period 2.4 s; flame: 3 octaves, noise rises at 2 units/s | C-3, C-2 |
| 5 | Floating marigolds | Simple | Small-radius circular motion + continuous rotation; variable speed with pauses | radius 0.45–0.75 m, motion cycle 4–5.5 s + pause 1.5–3 s, spin 0.6–1.0 rad/s, height = C-1 wave + 0.02 m | C-4 |

Simple animations depend on absolute time (`glfwGetTime()`) and do not accumulate `deltaTime`; the two KeyFrame animations share one clock `tSilueta` (D-5). ✅

## 7.2 The 6-second warning, resolved

The proposal fixed 6 s to reach the altar; the real distance (23.8 m) would need ≈ 4 m/s. The **Nov 4 decision** in the STATE document sets **18.33 s** at 1.4 m/s cruise and overrides CONTEXT §4. ✅

## 7.3 Where each derivation is

| Topic | Document and section (Spanish) |
|---|---|
| Sum of sines, choice of ω, analytic normal | `justificacion_simples.md` §1 |
| Candle Bézier, bobbing and phase, noise flame | `justificacion_simples.md` §2 |
| Circle, rotation and pauses of the flowers | `justificacion_simples.md` §3 |
| Slerp, silhouette hierarchy, keyframes | `justificacion_complejas.md` §1 |
| Smoothstep and leg offset | `justificacion_complejas.md` §2 |
| Flower Bézier and free fall | `justificacion_complejas.md` §3 |
| `deltaTime` integration and why they are not course animations | `justificacion_complejas.md` §4–5 |

## 7.4 Main technical reference

de Vries (2020), *Learn OpenGL* ([§11](11_references.md)). ❓ References for slerp, Bézier and Perlin noise are not added to §11 until D2 confirms they are the sources used.
