# Video 3 — Animaciones y elementos culturales interactivos

- **Archivo:** `Evidencia3_Equipo15_319266894_321332086.mp4` · **Duración:** 3:04 · 1280×720, 30 fps, H.264 + AAC
- **Rúbrica:** (c) animaciones y elementos culturales interactivos (el CONTEXTO §9 lo llama «animaciones e interactividad»).
- **Narración:** ninguna; solo **rótulos**, con el audio real del programa. *Según el hilo de grabación.*
- **Qué muestra (según el hilo):** las **10 animaciones**, las **fichas culturales** y la tecla **V**.

## Las 10 animaciones (ESTADO, decisión 17/11 X-55; valores de `docs/manual_tecnico/justificacion_*.md`)
| # | Animación | Tipo | Idea matemática | Fuente de los valores |
|---|---|---|---|---|
| 1 | Olas | Simple | Suma de 3 senoidales en el *vertex shader* (0.040 / 0.025 / 0.015 m; 12 / 7 / 4.5 m de longitud de onda) | `justificacion_simples.md` |
| 2 | Velas flotando (con su llama) | Simple | Trayectoria de Bézier + vaivén senoidal (0.012 m, 2.4 s); llama con ruido de Perlin | `justificacion_simples.md` |
| 3 | Flores flotando | Simple | Movimiento circular + rotación, con pausas | `justificacion_simples.md` |
| 4 | Caminata de la silueta | KeyFrames | Slerp de cuaterniones + smoothstep; jerarquía de articulaciones | `justificacion_complejas.md` |
| 5 | Dejar la flor | KeyFrames | Bézier cuadrática que coincide con una caída libre | `justificacion_complejas.md` |
| 6 | Baile familiar | KeyFrames | Órbita de 8 s con 12 KeyFrames de radio 0.95 m, paso de 2 s | `justificacion_personas.md` |
| 7 | Juego con el perro | KeyFrames | Pelota con tiro parabólico (1.5 s de vuelo), ciclo de 10 s | `justificacion_personas.md` |
| 8 | Sahumerio con copal | KeyFrames | Copalero como péndulo (14°, 1.1 s), ciclo de 9 s; sin humo con partículas | `justificacion_personas.md` |
| 9 | Banda al compás | Simple | Rebote al compás de 90 BPM, 1.2–3.6 cm según la sección | `justificacion_personas.md` |
| 10 | Anfitriona que saluda | Simple | Gira hacia el visitante a < 9 m con τ = 1/3 s | `justificacion_personas.md` |

Velas, llama, olas, flores flotantes, papel picado y humo **no** son KeyFrames. Conteo: 5 KeyFrames + 5 simples. Las 5 primeras son las de la propuesta; las otras 5 se agregaron el 17/11 (X-55).
Los valores 6 a 10 son del ESTADO (registrados el 17/11). Según el hilo de grabación, el video se hizo corriendo el juego real.

## Qué se ve, además
- **Fichas culturales** (a < 2 m): lista de las 13 en `GUION_2_recorrido.md`.
- **Tecla V:** enciende y apaga las velas del altar (a menos de 3 m).
- El orden y los minutos de cada animación dentro de los 3:04 **no están verificados**; se confirma al ver el MP4.

## Antes de entregar, confirmar al ver el MP4
- [ ] Que aparezcan las 10 animaciones de la tabla (no solo las 5 originales).
- [ ] Que cada rótulo diga KeyFrames o simple como en la tabla.
- [ ] Que no se diga «ninguna es del curso» sin el argumento de D-7 §5 (ESTADO lo deja pendiente de revisión).
- [ ] Que los valores de los rótulos coincidan con el código (`src/Olas.h`, `src/Animacion.cpp`, `src/DejarFlor.h`, `src/PersonasAnim.cpp`).
