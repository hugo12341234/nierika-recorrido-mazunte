# 6. Modelos y recursos

## 6.1 Definición de «modelo propio» ✅ **[repo, ESTADO §4, 19/10]**

Cada objeto distinto modelado por el equipo **en Blender** y exportado a su propio `.obj` en `Models/<objeto>/`. Las copias o instancias cuentan una vez; las variantes con geometría distinta (cada tumba) cuentan por separado. Se guarda el archivo fuente (`.blend`) como evidencia. Los modelos externos solo son secundarios y con crédito.

## 6.2 Conteo al 12/11/2026

| Concepto | Valor | Fuente |
|---|---|---|
| Meta | ≥ 18 | CONTEXTO §3 |
| Planeados | 23 | ESTADO §2 (inventario) |
| Escenario estricto (sin las 5 mallas planas: piso, playa, mar, papel picado, cempasúchil) | 18, sin margen: no se recortan la 4.ª tumba, la roca de la barda ni el copalero | ESTADO §2 |
| **Archivos presentes en `develop`** | **23 de 23 con `.obj` y `.blend`** (calculado de los archivos con `tools/e5_tabla_modelos.py`) | repositorio, 12/11 |
| Con estado ✅ en el ESTADO | techo, arco, piso, playa, mar, 4 tumbas, barda y roca (11); los otros 12 están 🔄 por falta de `/verificar` o de verlos compilados | ESTADO §2 |

**Lo que NO prueba este conteo:** que cada `.blend` sea de autoría del equipo (el de la fachada se reconstruyó del `.obj`, ver ESTADO), ni que la lista pase la revisión del profesor: el criterio de «modelo propio» es el del ESTADO §4 (19/10).

Medidas reales de cada archivo (generadas el 12/11/2026 con `python3 tools/e5_tabla_modelos.py`; se regeneran con el mismo comando):

| # | Modelo | Triángulos | Vértices | Dimensiones x · y · z (m) | `.blend` | Texturas (px) |
|---|---|---|---|---|---|---|
| 1 | `fachada` | 140 | 68 | 3.93 · 3.28 · 2.62 | sí | `laminado1.jpg` 512×512, `piedra.jpg` 1024×1024 |
| 2 | `techo` | 64 | 34 | 4.18 · 0.70 · 3.43 | sí | `laminado1.jpg` 512×512 |
| 3 | `arco` | 108 | 72 | 1.57 · 0.77 · 0.08 | sí | `piedra.jpg` 1024×1024 |
| 4 | `piso` | 2 | 4 | 3.93 · 0.00 · 2.62 | sí | `piedra.jpg` 1024×1024 |
| 5 | `playa` | 2500 | 1326 | 100.00 · 0.50 · 50.00 | sí | `arena.jpg` 1024×1024 |
| 6 | `mar` | 20000 | 10201 | 100.00 · 0.00 · 100.00 | sí | `mar_provisional.png` 1024×1024 |
| 7 | `tumba1` | 310 | 169 | 1.30 · 1.90 · 0.90 | sí | `tumba1_provisional.png` 1024×1024 |
| 8 | `tumba2` | 80 | 46 | 1.00 · 1.05 · 2.00 | sí | `tumba2_provisional.png` 1024×1024 |
| 9 | `tumba3` | 60 | 40 | 1.10 · 1.16 · 2.00 | sí | `tumba3_provisional.png` 1024×1024 |
| 10 | `tumba4` | 32 | 22 | 1.20 · 0.90 · 1.70 | sí | `tumba4_provisional.png` 1024×1024 |
| 11 | `barda` | 60 | 40 | 14.00 · 0.70 · 9.00 | sí | `piedra.jpg` 1024×1024 |
| 12 | `roca` | 108 | 56 | 0.56 · 0.33 · 0.47 | sí | `piedra.jpg` 1024×1024 |
| 13 | `cruz` | 48 | 32 | 0.60 · 1.50 · 0.60 | sí | `cruz_provisional.png` 1024×1024 |
| 14 | `soportes` | 36 | 24 | 1.85 · 2.00 · 0.05 | sí | `soportes_provisional.png` 1024×1024 |
| 15 | `papel_picado` | 680 | 385 | 1.70 · 0.50 · 0.00 | sí | `papel_picado.png` 1024×1024 |
| 16 | `altar3` | 36 | 24 | 1.60 · 1.20 · 0.90 | sí | `altar3_provisional.png` 1024×1024 |
| 17 | `vela` | 60 | 32 | 0.06 · 0.15 · 0.06 | sí | `vela_provisional.png` 1024×1024 |
| 18 | `cempasuchil` | 6 | 12 | 0.07 · 0.07 · 0.07 | sí | `cempasuchil.png` 1024×1024 |
| 19 | `pan_muerto` | 624 | 352 | 0.16 · 0.08 · 0.16 | sí | `pan_muerto.png` 1024×1024 |
| 20 | `botella` | 672 | 338 | 0.08 · 0.30 · 0.08 | sí | `botella_provisional.png` 1024×1024 |
| 21 | `marco` | 50 | 36 | 0.20 · 0.25 · 0.02 | sí | `foto_provisional.png` 1024×1024, `marco_provisional.png` 1024×1024 |
| 22 | `silueta` | 276 | 158 | 0.36 · 1.03 · 0.20 | sí | `silueta_provisional.png` 1024×1024 |
| 23 | `copalero` | 432 | 218 | 0.17 · 0.09 · 0.17 | sí | `copalero_provisional.png` 1024×1024 |

Notas: las dimensiones de la silueta suman las 10 piezas con su pivote en el origen (no es el tamaño de la figura armada, ≈ 1.7 m, ver `Silueta.cpp`); los triángulos cuentan una cara de *n* lados como *n − 2*. Texturas `*_provisional.png`: de un solo color, sustituidas por texturas propias en el PR de B-12 (altar3, vela, botella, marco y copalero); siguen provisionales las de tumbas, cruz, soportes, silueta, mar y la foto del marco. `laminado1.jpg` de `fachada` y `techo` mide 512×512 (la convención pide 1K); el ESTADO no registra su origen (§6.4).

## 6.3 Modelos que **no** cuentan (extras y provisionales)

| Modelo | Origen | Por qué no cuenta |
|---|---|---|
| `altar/altar.obj` (capilla con domo y cruz) | Maya, D2 | Decisión «solo Blender» (19/10); B-9 ya la reemplazó con `tumba1`; el PR de B-13 la quita de la escena |
| `calavera/calavera.obj`, `pan/pan.obj`, `flor/flor.obj` | Maya, D1 (X-3, 04/11) | Extras sin tarea en el plan; el pan se retira de la escena en el PR de B-13 (lo reemplaza `pan_muerto`) y la flor de Maya queda sobre el altar |

La capilla provisional no sale en videos ni capturas finales (ESTADO, decisión del 19/10); el PR de B-13 la retira de la escena.

## 6.4 Texturas y créditos

| Textura | Uso | Origen | Crédito |
|---|---|---|---|
| `arena.jpg` (1024×1024) | Playa | *Sandy Gravel*, Poly Haven, CC0 | Ver [§11](11_referencias.md) |
| `mar_provisional.png` (1024×1024, color `1E5A78`) | Mar | Hecha por el equipo; **provisional** hasta C-1 | — |
| `concreto.jpg`, `laminado1.jpg` (512×512) | Fachada | ❓ el ESTADO no registra su origen | ❓ **POR DEFINIR**; se reemplazan en A-12 |
| `texture_diffuse.png` / `shaded.png` | Capilla, calavera, pan y flor | Maya | La capilla pesa 2048 px (problema #3); las de X-3 se redujeron a 1024 |

📷 **CAPTURA pendiente por modelo** (vista 3D en Blender y en la escena) a medida que se cierre cada tarea.
