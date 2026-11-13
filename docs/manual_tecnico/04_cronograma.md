# 4. Cronograma

> Estado al **12/11/2026, hora de México**. Las fechas reales son las que el ESTADO registra por tarea; **no hay horas reales** en el repositorio (§4.5).

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

Cinco bloques con códigos de tarea (`A-1`…`E-7`, más `X-1…X-3`): A (19–25 oct) escena base con luz · B (26 oct–1 nov) panteón y altar con ofrenda · C (2–8 nov) 3 animaciones simples y fichas · D (9–15 nov) 2 animaciones por KeyFrames y audio · E (16–18 nov) entrega.

## 4.3 Avance real por tarea (ESTADO §2) ✅ **[repo]**

Leyenda: ✅ cerrada con su criterio · 🔄 hecha en código/archivos, con algo por confirmar (casi siempre «falta compilar en Release x64 en la PC de D2») · ⏳ no empezada. Las fechas son las del ESTADO.

| Bloque | Tarea | Fecha | Estado | PR |
|---|---|---|---|---|
| A | A-1 Quitar calavera; capilla provisional | 19/10 | ✅ `/verificar` APROBAR | #9 |
| A | A-2 Escalas reales (fachada ×1.31) | 20/10 | ✅ APROBAR | #11 |
| A | A-3, A-4 Ventana 1280×720; *far plane* 300 m | 20/10 | ✅ | #12 |
| A | A-5 Playa 100×50 m · A-6 Arena 1K | 20/10 | ✅ APROBAR | #13, #14 |
| A | A-7 Mar 100×100 m | 25/10 | ✅ APROBAR CON OBSERVACIONES | #15 |
| A | A-8 Iluminación Blinn-Phong (luces de D1) | 04/11 | ✅ APROBAR (según D2) | #16 |
| A | A-9 Colisiones (04/11) · A-10 Arco con dovelas (03/11) | 03–04/11 | ✅ | #18, #30 |
| A | A-11 Piso y techo · A-12 Texturas de piedra | 03–04/11 | ✅ (A-12: falta compilar y `/verificar`) | #31, #32 |
| B | B-9 Tumbas (4) · B-10 Barda y roca | 04/11 | ✅ APROBAR | #40, #41 |
| B | B-1, B-2, B-5, B-6, B-8, B-11, B-14 Modelos propios en Blender | 04/11 | 🔄 | #36 |
| B | B-3 Cempasúchil · B-4 Pan de muerto · B-7 Papel picado | 11/11 | 🔄 | #49, #50, #51 |
| B | B-12 Texturas propias y mapa de relieve | 12/11 | 🔄 | #66 |
| B | B-13 Ofrenda completa en el cuarto | 12/11 | 🔄 | #52, #59 |
| C | C-1 Olas · C-4 Flores flotando | 04/11 | 🔄 | #28, #26 |
| C | C-2 Llama (ruido) · C-3 Velas flotando | 11/11 | 🔄 | #54, #53 |
| C | C-5 Integración de las 3 simples · C-6 Descarte por alfa | 12/11 | 🔄 (C-5: FPS sin medir) | #64, #63 |
| C | C-7 Fichas (C-7a textos, C-7b ImGui) | 04/11 | 🔄 falta compilar en VS y verificar textos | #20, #24 |
| C | C-8 Justificación de las 3 simples | 11/11 | 🔄 | #55 |
| D | D-1 Silueta jerárquica · D-2 Caminata con slerp | 04/11 | D-2 ✅ · D-1 ⏳ (modelo de D1 integrado, sin compilar) | #29, #39 |
| D | D-3 Justificación de smoothstep | 11/11 | ✅ | #56 |
| D | D-4 Dejar flores (Bézier) | 11/11 | 🔄 | #57 |
| D | D-5 Integrar las 2 complejas · D-6 Audio · D-7 Justificación | 12/11 | 🔄 | #60, #62, #65 |
| E | E-1a `cleanup()` y contador de FPS | 04/11 | 🔄 falta compilar y verificar | #19 |
| E | E-2 Barda y colisiones | 12/11 | 🔄 | #61 |
| E | E-3 Empaquetado (`dist/`) | 04/11 | 🟡 prueba en otra PC pendiente | #21, #43 |
| E | E-4 Videos · E-7 Release | — | ⏳ | — |
| E | E-5 Manual técnico · E-6 Manual de usuario | 12–13/11 | 🔄 (este documento) | #67, #68 + PR actual |
| X | X-3 Extras de D1 (pan, flor, calavera) | 04/11 | ✅ APROBAR (según D2) | #17 |

Los PR #59 a #68 se fusionaron sin veredicto APROBAR (excepción del 12/11, ESTADO §4; [§1.5](01_introduccion.md)).

## 4.4 Comparación plan vs. real

| Bloque | Previsto | Real al 12/11 |
|---|---|---|
| A (19–25 oct) | A-1…A-12 | ✅ 12 de 12 (el último cierre, A-8 y A-12, el 04/11) |
| B (26 oct–1 nov) | 14 tareas | 2 ✅ (B-9, B-10); las 12 restantes 🔄: **hechas con 1–2 semanas de retraso** (04/11 y 11–12/11) |
| C (2–8 nov) | C-1…C-8 | Todas 🔄 (código hecho entre el 04/11 y el 12/11) |
| D (9–15 nov) | D-1…D-7 | D-2 y D-3 ✅; el resto 🔄; D-1 ⏳ por confirmar |
| E (16–18 nov) | E-1…E-7 | E-1a, E-2, E-3, E-5, E-6 en marcha; E-4 y E-7 ⏳ |

**Lectura honesta:** los cierres registrados se concentran entre el 03/11 y el 12/11 (fechas del ESTADO), lo que deja **poco margen de prueba**: casi todo lo marcado 🔄 depende de una sola compilación y revisión visual de D2. El repositorio no registra las causas del atraso respecto al plan original; qué se dice de ellas lo deciden D1 y D2 antes de entregar. ❓

## 4.5 Horas reales

❓ **POR DEFINIR.** El repo no guarda horas reales; las 173 + 30 h de la propuesta son estimaciones. Sin ese dato, esta sección entrega fechas de cierre, no horas. Si D1 y D2 quieren horas reales, deben reconstruirlas (p. ej. desde la hoja del plan) y anotarlas en una columna nueva.

## 4.6 Cronograma de lo que falta (hasta el 18/11)

Orden propuesto, **sin fechas fijadas por D1/D2**: (1) D2 compila `Release | x64` y revisa lo marcado 🔄; (2) medir FPS (§8.2.1); (3) E-1: lista de errores y verificar `cleanup()`; (4) E-3: probar el `.zip` en una PC sin Visual Studio; (5) E-4: tres videos; (6) E-5/E-6: capturas finales en la PC de D2 y PDF; (7) E-7: publicar el Release. ⏳
