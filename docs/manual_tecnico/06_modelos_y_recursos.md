# 6. Modelos y recursos

## 6.1 Definición de «modelo propio» ✅ **[repo, ESTADO §4, 19/10]**

Cada objeto distinto modelado por el equipo **en Blender** y exportado a su propio `.obj` en `Models/<objeto>/`. Las copias o instancias cuentan una vez; las variantes con geometría distinta (cada tumba) cuentan por separado. Se guarda el archivo fuente (`.blend`) como evidencia. Los modelos externos solo son secundarios y con crédito.

## 6.2 Conteo al 04/11/2026

| Concepto | Valor | Fuente |
|---|---|---|
| Meta | ≥ 18 | CONTEXTO §3 |
| Planeados | 23 | ESTADO §2 (inventario) |
| Escenario estricto (sin las 5 mallas planas) | 18, sin margen: no se recortan la 4.ª tumba, la roca de la barda ni el copalero | ESTADO §2 |
| **Existen y cuentan hoy** | **3 en el conteo amplio (fachada, playa, mar); 1 en el estricto (la fachada)** | ESTADO §2, filas 1, 5 y 6 (el ESTADO escribe «existe 1 que cuenta» el 19/10, antes de playa y mar; la cuenta de 3 y de 1 se deriva de las filas del inventario y debe confirmarse en el próximo `/cierre`) |

Detalle de lo existente:

| # | Modelo | Archivo | Herramienta | Evidencia fuente |
|---|---|---|---|---|
| 1 | Fachada y muros (con el techo adentro hasta A-11) | `Models/fachada/fachada.obj` | Blender 4.4.3; reexportada con 5.2.2 en A-2 | `fachada.blend` en el repo, **reconstruido desde el `.obj`: no prueba autoría**; el original no apareció (20/10) |
| 5 | Playa 100×50 m (plano) | `Models/playa/playa.obj` | Blender 5.2.2 | `playa.blend` |
| 6 | Mar 100×100 m (plano) | `Models/mar/mar.obj` | Blender 5.2.2 | `mar.blend` |

Los 20 restantes del inventario (techo, arco, piso, 4 tumbas, barda, roca, cruz, soportes, papel picado, altar de 3 niveles, vela, cempasúchil, pan, botella, marco, silueta, copalero) están en ⏳ y los llena cada tarea B/D. **Esta sección no debe afirmar más que eso.** La prueba de autoría (problema #10) se cerró **solo por la palabra de D2**, sin verificar archivos (ESTADO §4, 19/10); el manual no debe decir más.

## 6.3 Modelos que **no** cuentan (extras y provisionales)

| Modelo | Origen | Por qué no cuenta |
|---|---|---|
| `altar/altar.obj` (capilla con domo y cruz) | Maya, D2 | Decisión «solo Blender» (19/10); se retira cuando B-9 la reemplace |
| `calavera/calavera.obj`, `pan/pan.obj`, `flor/flor.obj` | Maya, D1 (X-3, 04/11) | Extras sin tarea en el plan; B-3 y B-4 siguen ⏳ |

La capilla provisional no sale en videos ni capturas finales (ESTADO, decisión del 19/10).

## 6.4 Texturas y créditos

| Textura | Uso | Origen | Crédito |
|---|---|---|---|
| `arena.jpg` (1024×1024) | Playa | *Sandy Gravel*, Poly Haven, CC0 | Ver [§11](11_referencias.md) |
| `mar_provisional.png` (1024×1024, color `1E5A78`) | Mar | Hecha por el equipo; **provisional** hasta C-1 | — |
| `concreto.jpg`, `laminado1.jpg` (512×512) | Fachada | ❓ el ESTADO no registra su origen | ❓ **POR DEFINIR**; se reemplazan en A-12 |
| `texture_diffuse.png` / `shaded.png` | Capilla, calavera, pan y flor | Maya | La capilla pesa 2048 px (problema #3); las de X-3 se redujeron a 1024 |

📷 **CAPTURA pendiente por modelo** (vista 3D en Blender y en la escena) a medida que se cierre cada tarea.
