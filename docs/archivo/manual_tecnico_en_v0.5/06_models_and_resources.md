# 6. Models and resources

## 6.1 Definition of "own model" ✅ (STATE §4, Oct 19)

Each distinct object modeled by the team **in Blender** and exported to its own `.obj` in `Models/<object>/`. Copies or instances count once; variants with different geometry (each tomb) count separately. The source `.blend` is kept as evidence. External models are secondary only and credited.

## 6.2 Count on Nov 12, 2026

| Item | Value | Source |
|---|---|---|
| Target | ≥ 18 | CONTEXT §3 |
| Planned | 23 | STATE §2 (inventory) |
| Strict scenario (without the 5 flat meshes: floor, beach, sea, papel picado, marigold) | 18, with no margin: the 4th tomb, the wall rock and the copal burner cannot be cut | STATE §2 |
| **Files present in `develop`** | **23 of 23 with `.obj` and `.blend`** (computed from the files with `tools/e5_tabla_modelos.py`) | repository, Nov 12 |
| ✅ in the STATE document | roof, arch, floor, beach, sea, 4 tombs, wall and rock (11); the other 12 are 🔄 for lack of `/verificar` or a compiled view | STATE §2 |

**What this count does NOT prove:** that each `.blend` was authored by the team (the facade one was rebuilt from the `.obj`), nor that the list will pass the professor's review: the "own model" criterion is STATE §4 (Oct 19).

Real measurements of each file (regenerated on Nov 12, 2026 on `develop` with B-12 merged, via `python3 tools/e5_tabla_modelos.py`):

| # | Model | Triangles | Vertices | Dimensions x · y · z (m) | `.blend` | Textures (px) |
|---|---|---|---|---|---|---|
| 1 | `fachada` | 140 | 68 | 3.93 · 3.28 · 2.62 | yes | `laminado1.jpg` 512×512, `piedra.jpg` 1024×1024 |
| 2 | `techo` | 64 | 34 | 4.18 · 0.70 · 3.43 | yes | `laminado1.jpg` 512×512 |
| 3 | `arco` | 108 | 72 | 1.57 · 0.77 · 0.08 | yes | `piedra.jpg` 1024×1024 |
| 4 | `piso` | 2 | 4 | 3.93 · 0.00 · 2.62 | yes | `piedra.jpg` 1024×1024 |
| 5 | `playa` | 2500 | 1326 | 100.00 · 0.50 · 50.00 | yes | `arena.jpg` 1024×1024 |
| 6 | `mar` | 20000 | 10201 | 100.00 · 0.00 · 100.00 | yes | `mar_provisional.png` 1024×1024 |
| 7 | `tumba1` | 310 | 169 | 1.30 · 1.90 · 0.90 | yes | `tumba1_provisional.png` 1024×1024 |
| 8 | `tumba2` | 80 | 46 | 1.00 · 1.05 · 2.00 | yes | `tumba2_provisional.png` 1024×1024 |
| 9 | `tumba3` | 60 | 40 | 1.10 · 1.16 · 2.00 | yes | `tumba3_provisional.png` 1024×1024 |
| 10 | `tumba4` | 32 | 22 | 1.20 · 0.90 · 1.70 | yes | `tumba4_provisional.png` 1024×1024 |
| 11 | `barda` | 72 | 48 | 14.00 · 0.70 · 9.00 | yes | `piedra.jpg` 1024×1024 |
| 12 | `roca` | 108 | 56 | 0.56 · 0.33 · 0.47 | yes | `piedra.jpg` 1024×1024 |
| 13 | `cruz` | 48 | 32 | 0.60 · 1.50 · 0.60 | yes | `cruz_provisional.png` 1024×1024 |
| 14 | `soportes` | 36 | 24 | 1.85 · 2.00 · 0.05 | yes | `soportes_provisional.png` 1024×1024 |
| 15 | `papel_picado` | 680 | 385 | 1.70 · 0.50 · 0.00 | yes | `papel_picado.png` 1024×1024 |
| 16 | `altar3` | 36 | 24 | 1.60 · 1.20 · 0.90 | yes | `altar3_tela.png` 1024×1024 |
| 17 | `vela` | 60 | 32 | 0.06 · 0.15 · 0.06 | yes | `vela_cera.png` 1024×1024 |
| 18 | `cempasuchil` | 6 | 12 | 0.07 · 0.07 · 0.07 | yes | `cempasuchil.png` 1024×1024 |
| 19 | `pan_muerto` | 624 | 352 | 0.16 · 0.08 · 0.16 | yes | `pan_muerto.png` 1024×1024, `pan_muerto_relieve.png` 1024×1024 |
| 20 | `botella` | 672 | 338 | 0.08 · 0.30 · 0.08 | yes | `botella_vidrio.png` 1024×1024 |
| 21 | `marco` | 50 | 36 | 0.20 · 0.25 · 0.02 | yes | `foto_provisional.png` 1024×1024, `marco_madera.png` 1024×1024 |
| 22 | `silueta` | 276 | 158 | 0.36 · 1.03 · 0.20 | yes | `silueta_provisional.png` 1024×1024 |
| 23 | `copalero` | 432 | 218 | 0.17 · 0.09 · 0.17 | yes | `copalero_barro.png` 1024×1024 |

Notes: silhouette dimensions add up its 10 pieces with the pivot at the origin (not the assembled figure's size, ≈ 1.7 m, see `Silueta.cpp`); triangles count an *n*-sided face as *n − 2*. `*_provisional.png` textures are flat colors; B-12 (PR #66) replaced those of altar3, candle, bottle, frame and copal burner with own 1024×1024 textures; those of tombs, cross, supports, silhouette, sea and the frame photo **remain provisional**. `laminado1.jpg` of `fachada` and `techo` is 512×512 (the convention asks 1K); its origin is not recorded (§6.4).

## 6.3 Models that do **not** count (extras and provisional)

| Model | Origin | Why it does not count |
|---|---|---|
| `altar/altar.obj` (chapel with dome and cross) | Maya, D2 | "Blender only" decision (Oct 19); B-9 replaced it with `tumba1`; B-13 (PR #59) removed it from the scene (the `Models/altar/` folder is still in the repo; PR #45 proposes deleting it and is still open) |
| `calavera/calavera.obj`, `pan/pan.obj`, `flor/flor.obj` | Maya, D1 (X-3, Nov 4) | Extras with no plan task; Maya's bread was removed from the scene in B-13 (replaced by `pan_muerto`); Maya's skull and flowers stay on the altar ledge |

The provisional chapel does not appear in final videos or screenshots (STATE, Oct 19 decision); B-13 already removed it from the scene.

## 6.4 Textures and credits

| Texture | Use | Origin | Credit |
|---|---|---|---|
| `arena.jpg` (1024×1024) | Beach | *Sandy Gravel*, Poly Haven, CC0 | See [§11](11_references.md) |
| `mar_provisional.png` (1024×1024, color `1E5A78`) | Sea | Made by the team; **still provisional** (flat color, Oct 25 decision) | — |
| `piedra.jpg` (1024×1024) | Walls, voussoirs, floor, wall and rock | Procedural (Voronoi), generated by the team (A-12) | Own |
| `altar3_tela.png`, `vela_cera.png`, `botella_vidrio.png`, `marco_madera.png`, `copalero_barro.png`, `pan_muerto_relieve.png` (1024×1024) | Offering | Generated with `tools/b12_texturas.py` (numpy, fixed seed) | Own |
| `cempasuchil.png`, `papel_picado.png`, `pan_muerto.png` (1024×1024 RGBA) | Flower, papel picado, bread | Generated with `tools/b3_*.py`, `b7_*.py`, `b4_*.py` | Own |
| `laminado1.jpg` (512×512) | Facade and roof | ❓ the STATE document does not record its origin; not 1K | ❓ **UNDECIDED**: replace or record the exception |
| `*_provisional.png` | Tombs, cross, supports, silhouette, frame photo | Made by the team, flat color | Provisional |
| `texture_diffuse.png` / `shaded.png` | Skull, flower, Maya's bread and chapel (the chapel is no longer loaded) | Maya (D1/D2) | Extras that do not count as own |

📷 **Screenshot pending per model** (3D view in Blender and in the scene) as each task closes.
