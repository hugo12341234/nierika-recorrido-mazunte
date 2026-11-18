# Video 2 — Recorrido e interacción

- **Archivo:** `Evidencia2_Equipo15_319266894_321332086.mp4` · **Duración:** 7:51 · 1280×720, 30 fps, H.264 + AAC
- **Rúbrica:** (b) recorrido e interacción (`CONTEXTO_PROYECTO.md` §9).
- **Audio:** real del programa (mar, música) y las **12 voces** del recorrido guiado, con subtítulos. *Según el hilo de grabación.*

## Qué se ve (según el hilo de grabación)
1. **Movimiento y cámara:** **W A S D** (o flechas) y **ratón** (primera persona, colisiones; 2.0 m/s).
2. **Teclas de interacción:** **F1** (ayuda de controles), **V** (velas del altar), **M** (sonido: mar y música / solo mar / solo música / silencio), **R** (reinicia la silueta).
3. **Fichas culturales** que aparecen a **menos de 2 m** del objeto.
4. **Recorrido guiado completo con la tecla T:** dura **5:48** con las 12 estaciones y las 12 voces (`Audio/narracion/narracion_00..11.mp3`) y subtítulos.

Los 5:48 del recorrido dejan ≈ 2:03 para los puntos 1 a 3 (diferencia calculada: 7:51 − 5:48; el orden y el minuto de cada cosa **no están verificados**).

## Las 12 estaciones del recorrido guiado (de `docs/RECORRIDO_GUIADO.md` y `Audio/narracion/`)
| # | Estación | Archivo de voz |
|---|---|---|
| 0 | Apertura: Mazunte y el mar (tortugas) | `narracion_00.mp3` |
| 1 | Velas y flores al mar | `narracion_01.mp3` |
| 2 | Comparsa en la orilla | `narracion_02.mp3` |
| 3 | Camino de flores al arco | `narracion_03.mp3` |
| 4 | Banda de viento | `narracion_04.mp3` |
| 5 | Panteón | `narracion_05.mp3` |
| 6 | Copal | `narracion_06.mp3` |
| 7 | El altar | `narracion_07.mp3` |
| 8 | Ofrendas de las familias | `narracion_08.mp3` |
| 9 | Camino de las ánimas | `narracion_09.mp3` |
| 10 | Mariposas | `narracion_10.mp3` |
| 11 | Mirador y cierre | `narracion_11.mp3` |

Durante el recorrido el ratón no mueve la cámara y **no salen las fichas** (solo subtítulos y voz); **T**, **Esc** o **W A S D** lo cancelan. Las fichas se ven en modo libre (ESTADO X-64; `docs/manual_usuario/02_controles.md`).

## Las 13 fichas (`src/Fichas.cpp`)
Velas · Flor de cempasúchil · Papel picado · Altar de ofrenda · Pan de muerto · Banda de viento · Copal y copalero · Panteón y tumbas · Ofrendas de las familias · Comparsa en la orilla · Camino de las ánimas · Mariposas monarca · Tortugas de Mazunte. Cada una cita su fuente en APA 7 y dice cuándo es adaptación del estudio.
Cuáles de las 13 se muestran en este video: **no verificado** (el hilo no lo dijo).

## Narración
La narración hablada es la del propio programa (voz del recorrido guiado); el video no añade otra voz. *Según el hilo de grabación.* La voz es sintética y lee un texto: no es un personaje con IA (ESTADO, X-62).

## Antes de entregar, confirmar al ver el MP4
- [ ] Que el recorrido de **T** esté completo (12 estaciones) y se oigan las 12 voces.
- [ ] Que se vean **F1, V, M y R** funcionando y al menos una ficha a < 2 m.
- Nota: `narracion_07` y `narracion_08` duran lo mismo pero son distintas (confirmado por D2, 18/11).
- [ ] Que no se afirme nada fuera de alcance.
