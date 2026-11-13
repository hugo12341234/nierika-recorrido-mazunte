# 10. Evidencias

Qué prueba qué, y dónde está (estado al 12/11/2026). La carpeta `evidencias/` la fija el ESTADO §4 (19/10).

| Evidencia | Qué demuestra | Dónde está | Estado |
|---|---|---|---|
| Capturas de escena de B-12, B-13, C-6 y E-2 (12/11) | Texturas propias, ofrenda, flores y llamas, barda | [`evidencias/`](evidencias/) (8 `.jpg`) | 🔄 tomadas en **Linux con Mesa (software)**, con el código de los PR; **no** son capturas de la PC de D2: repetirlas con el aspecto final |
| Capturas del manual de usuario (6) | Ventana de inicio, orilla, panteón, altar y silueta | [`../manual_usuario/capturas/`](../manual_usuario/capturas/) | 🔄 mismo origen que arriba |
| Capturas de las tareas A–D (04/11) | Funcionamiento en la PC de D2 | Carpeta compartida del proyecto de Claude (`plan/evidencias_2026-11-04/`, ~70 archivos) | ❓ **no están en el repo**; decidir cuáles se copian a `evidencias/` |
| Captura semanal del Trello | Gestión y límites de «En proceso» | `evidencias/AAAA-MM-DD_trello.png` | ⏳ ninguna guardada (el Trello no es accesible desde los hilos en la nube) |
| Minuta semanal (3–5 líneas) | Avance, atrasos y acuerdos | `evidencias/AAAA-MM-DD_minuta.md` | ⏳ ninguna guardada |
| Export JSON de Trello (19/10) | Estado del tablero ese día (56 tarjetas) | ❓ no está en el repo | ❓ decidir si se guarda |
| Historial de commits y PR | Quién cambió qué y cuándo | GitHub, PR #1 a #68 | ✅ (a nombre de D2; ver [§5](05_raci.md)) |
| Veredictos de `/verificar` | Revisión independiente | Comentarios en los PR | 🔄 hay APROBAR en los PR #9, #11, #13–#17, #40 y #41; los PR #59–#68 sin veredicto |
| Archivos `.blend` | Modelado propio | `Models/<objeto>/*.blend` | ✅ 23 de 23 presentes (el de la fachada fue reconstruido del `.obj`) |
| Scripts de generación | Cómo se hicieron modelos, texturas y audio | `tools/` | ✅ |
| Pruebas numéricas | Matemática de las animaciones | `tools/d7_pruebas_matematicas.cpp` | ✅ (se corre con `g++`, ver ESTADO D-7) |
| Auditorías y autorrevisión | Hallazgos de calidad | Carpeta compartida (`auditoria/`, `revisiones/`) | ❓ fuera del repo |
| Bocetos del cuarto (X-2) | Diseño de fachada, interior y ofrenda | — | ❓ sin confirmar |
| Versiones de Blender y GIMP (X-1) | Misma versión en las dos PC | — | 🔄 |
| Videos | Entorno, recorrido, animaciones | `docs/videos/` | 🔄 E-4: guiones listos, sin grabar |

## Reglas

- Lo que se cite en el manual debe estar **en el repositorio** o enlazado desde él.
- Sin datos personales en las capturas (nombres, correos, estados de cuenta): solo números de cuenta. (Ver problema #14: un PDF con datos personales sigue en el historial de Git.)
- Las capturas llevan fecha en el nombre.

## Capturas reales del programa (12/11/2026, PC de D2, modo de captura de `src/main.cpp`)

| Archivo (`evidencias/`) | Qué muestra |
|---|---|
| `X-4_caminata_4_cuadros_12-11.jpg` | La silueta en 4 instantes de un cuarto de ciclo (t = 6.00, 6.25, 6.50, 6.75 s): se ve el balanceo de brazos y piernas |
| `X-5_panteon_noche_12-11.jpg` | Panteón con cielo, luna y estrellas |
| `X-7_silueta_orilla_12-11.jpg` | Orilla y mar con la silueta orgánica |
| `X-8_altar_luz_calida_12-11.jpg` | Altar con la luz cálida de las velas |
| `X-9_palmas_playa_12-11.jpg` | Palmas y arbustos decorativos en la playa |
| `X-11_piso_cuarto_12-11.jpg` | Piso del cuarto (Poly Haven, CC0) |
| `X-4_luces_cubos_L_12-11.jpg` | Luces de colores con los cubos visibles (tecla L) |
| `D-2_caminata_8_cuadros_12-11.jpg` | Caminata: 8 cuadros de un ciclo completo (t = 6.000 a 6.875 s, cada 0.125 s) |
| `D-4_dejar_flor_6_cuadros_12-11.jpg` | Dejar la flor: brazo que se extiende (tau 0.9 s), se sostiene (1.7 a 2.9 s) y regresa (3.7 s) |
| `X-13_arena_manchas_12-11.jpg` | Arena con manchas de gran escala y arena mojada bajo el nivel del agua |
