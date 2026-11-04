# 4. Cronograma

## 4.1 Plan original (propuesta v2.0) ✅ **[repo, CONTEXTO §8]**

| Sem | Fechas | Fase | Hito |
|---|---|---|---|
| 1 | 28/09–02/10 | Arranque e infraestructura | Repo, proyecto base compilando, cuarto documentado |
| 2 | 05/10–09/10 | Escenario base y navegación | Playa y mar navegables en 1.ª persona |
| 3 | 12/10–16/10 | Cuarto del cementerio y panteón | Fachada, arco, interior y barda modelados y texturizados |
| 4 | 19/10–23/10 | Objetos de la ofrenda | Altar de 3 niveles y 7 objetos modelados |
| 5 | 26/10–30/10 | Texturizado, integración, iluminación | Escena completa cargada, texturizada e iluminada |
| 6 | 02/11–06/11 | Animaciones simples | Olas, velas y flores integradas |
| 7 | 09/11–13/11 | Animaciones complejas, interacción, audio | 2 animaciones KeyFrames, fichas y audio |
| 8 | 16/11–18/11 | Pruebas, empaquetado, entrega | Ejecutable, videos y manuales entregados |

Esfuerzo planeado: 60 actividades, 173 h de ejecución + 30 h de verificación = 203 h. ✅ **[repo]**

## 4.2 Replaneación del 18/10 ✅ **[repo, ESTADO §2]**

Se sustituyen las fechas por cinco bloques con códigos de tarea (`A-1`…`E-7`, más `X-1…X-3`):

| Bloque | Fechas | Contenido |
|---|---|---|
| A | 19–25 oct | Escena base correcta y navegable con luz |
| B | 26 oct–1 nov | Panteón + altar con ofrenda |
| C | 2–8 nov | 3 animaciones simples + fichas |
| D | 9–15 nov | 2 animaciones por KeyFrames + audio |
| E | 16–18 nov | Entrega |

## 4.3 Avance real (al 04/11/2026) ✅ **[repo, ESTADO §2–3]**

| Tarea | Qué | Fecha | Evidencia |
|---|---|---|---|
| (act. 1–5) | Fuentes, propuesta v2.0, repositorio, VS + dependencias, Trello | hasta 19/10 | Propuesta v2.0 (04/10); PR #1 (act. 4); export de Trello del 19/10 |
| A-1 | Quitar calavera; capilla provisional | 19/10 | PR #9, `/verificar A-1`: APROBAR |
| A-2 | Escalas reales (fachada ×1.31) | 20/10 | PR #11, `/verificar A-2`: APROBAR |
| A-3, A-4 | Ventana 1280×720; *far plane* 300 m | 20/10 | PR #12 |
| A-5 | Playa 100×50 m | 20/10 | PR #13, `/verificar A-5`: APROBAR |
| A-6 | Textura de arena 1K | 20/10 | PR #14, `/verificar A-6`: APROBAR |
| A-7 | Mar 100×100 m | 25/10 | rama `feature/a-7-mar` |
| A-8 | Iluminación Blinn-Phong (luces de D1) | 04/11 | PR #16, `/verificar A-8`: APROBAR (según D2) |
| X-3 | Extras de D1: pan, flor, calavera | 04/11 | PR #17, `/verificar X-3`: APROBAR (según D2) |

Fechas con el criterio del ESTADO. Los números de PR de A-7 no figuran en el ESTADO; completar al hacer el cierre de jornada.

## 4.4 Comparación plan vs. real

| Bloque | Previsto | Real al 04/11 |
|---|---|---|
| A (19–25 oct) | A-1…A-12 | ✅ A-1…A-8 · ⏳ **A-9** (colisiones), **A-10** (arco con dovelas), **A-11** (piso y techo), **A-12** (piedra) |
| B (26 oct–1 nov) | B-1…B-14 (14 tareas, 23 modelos planeados) | ⏳ ninguna tarea cerrada |
| C (2–8 nov) | C-1…C-8 | ⏳ ninguna cerrada |
| D, E | 9–18 nov | ⏳ |

**Lectura honesta:** el Bloque A cerró 8 de 12 tareas; los bloques B y C, en su fecha original, aún no empiezan. El ESTADO registra que faltan **14 días** a la entrega (el encabezado del ESTADO dice 24: está desactualizado; se corrige en el próximo `/cierre`). El repositorio no registra las causas del atraso; **cómo se cuenta** (qué se dice de la disponibilidad de cada integrante) ❓ **lo deciden D1 y D2** antes de entregar.

## 4.5 Horas reales

❓ **POR DEFINIR.** El repo **no guarda horas reales**; las horas de la propuesta (173 + 30) son estimaciones. Para poner horas reales en este manual hay que decidir cómo se registran (p. ej. una columna en la hoja del plan) a partir de ahora. Sin ese dato, esta sección se queda con fechas de cierre y no con horas.

## 4.6 Cronograma de lo que falta

Se completa cuando D1 y D2 decidan el orden final de las tareas B, C, D y E (ESTADO §2 y §7). ⏳
