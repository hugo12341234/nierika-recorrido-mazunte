# Videos de demostración (E-4, act. 57)

> **Estado (18/11/2026):** los tres videos **están grabados** (según el hilo de grabación en la PC de D2, 18/11). Los MP4 **no se suben a Git**
> (pesan ≈ 450 MB en total): **se entregan aparte** (archivos o enlace sin listar). El enlace, si se usa, se anota en la última columna.

## Los tres videos

| # | Archivo (nombre exacto) | Duración | Guion | Rúbrica que cubre | Enlace |
|---|---|---|---|---|---|
| 1 | `Evidencia1_Equipo15_319266894_321332086.mp4` | 2:50 | [`GUION_1_entorno.md`](GUION_1_entorno.md) | **(a) entorno y ambientación** | ⏳ (entrega aparte) |
| 2 | `Evidencia2_Equipo15_319266894_321332086.mp4` | 7:51 | [`GUION_2_recorrido.md`](GUION_2_recorrido.md) | **(b) recorrido e interacción** | ⏳ (entrega aparte) |
| 3 | `Evidencia3_Equipo15_319266894_321332086.mp4` | 3:04 | [`GUION_3_animaciones.md`](GUION_3_animaciones.md) | **(c) animaciones y elementos culturales interactivos** | ⏳ (entrega aparte) |

Los puntos (a), (b) y (c) son los de `CONTEXTO_PROYECTO.md` §9 («3 videos MP4: (a) entorno, (b) recorrido, (c) animaciones e interactividad») y
`PLAN_ACTIVIDADES.md` act. 57. La redacción larga de cada punto es la del hilo de grabación.

## Cómo se grabaron (según el hilo de grabación, 18/11/2026)
- Corriendo **el programa real** (el juego, no maquetas) en su **modo de video** (`NIERIKA_VIDEO`, tarea X-16; ver `tools/x16_generar_videos.ps1`), con reloj fijo,
  desde `develop` en el commit `4e03274`. **No se usó OBS** (la guía [`GUIA_GRABACION_OBS.md`](GUIA_GRABACION_OBS.md) quedó como alternativa manual).
- Formato de los tres: **1280×720, 30 fps, H.264 + AAC**, con **audio real** del programa (mar, música y las voces del recorrido guiado).
- Los videos 1 y 3 llevan **solo rótulos, sin narración hablada**. El video 2 incluye las voces del recorrido guiado (tecla **T**) con subtítulos.
- Descripción original del hilo de grabación: `LEEME_VIDEOS.md` en la carpeta `entrega_videos` de la PC de D2 (**no está en el repo**).

## Teclas vigentes del programa (las que aparecen en los videos)
**W A S D** o flechas (moverse) · **ratón** (mirar) · **V** (velas del altar, a menos de 3 m) · **M** (sonido) · **R** (reinicia la silueta) · **F1** (ayuda de controles) ·
**T** (recorrido guiado) · **Esc** (pausa; con el recorrido activo solo lo cancela). Fuente: `docs/manual_usuario/02_controles.md`. La barra espaciadora ya no hace nada.

## Lo que se verificó y lo que no
- **Verificado en el repo (develop):** nombres de las teclas, las 13 fichas, las 10 animaciones (ESTADO, decisión 17/11 X-55), las 12 estaciones y voces del recorrido (`docs/RECORRIDO_GUIADO.md`, `Audio/narracion/narracion_00..11.mp3`), el menú (Iniciar recorrido, Controles, Créditos, Salir).
- **Solo según el hilo de grabación (no se pudo ver el MP4 desde la nube):** duraciones, orden de las escenas dentro de cada video, que Evidencia 2 incluya el recorrido guiado completo (5:48) y que las 12 voces se oigan.
- **No se afirma** ningún minuto exacto de una escena concreta: los guiones no traen tiempos por escena porque no se tienen verificados.

## Reglas que valen para los tres videos
- Decir **solo lo que se ve o está en los documentos**; lo que sea adaptación del estudio, decirlo (como en las fichas).
- No mencionar nada fuera de alcance como si existiera (pueblo, Punta Cometa, tapete de arena, humo de copal con partículas, VR, multijugador, IA).
- La voz del recorrido es una narración sintética que lee un texto; no es un personaje con IA (ESTADO, decisión 17/11 X-62).
- Sin logos de la escuela ni datos personales; solo los números de cuenta (van en el nombre de los archivos).
