# Video 1 — Entorno y ambientación

- **Archivo:** `Evidencia1_Equipo15_319266894_321332086.mp4` · **Duración:** 2:50 · 1280×720, 30 fps, H.264 + AAC
- **Rúbrica:** (a) entorno y ambientación (`CONTEXTO_PROYECTO.md` §9).
- **Narración:** ninguna; solo **rótulos** en pantalla, con el audio real del programa (mar y música). *Según el hilo de grabación.*
- **Cómo se hizo:** programa real en modo de video (`NIERIKA_VIDEO`, X-16), desde `develop` 4e03274.

## Qué se ve (orden según el hilo de grabación; sin tiempos por escena, no están verificados)
| # | Escena | Qué demuestra | Verificado en el repo |
|---|---|---|---|
| 1 | **Menú de inicio** | Pantalla previa con *Iniciar recorrido, Controles, Créditos, Salir* (X-63) | `src/Menu.cpp` |
| 2 | **Playa con olas** | Mar con oleaje animado (vertex shader), arena y cielo nocturno | ESTADO X-68 / justificación de olas |
| 3 | **Velas y flores** | Velas con llama y flores de cempasúchil flotando en la orilla | `docs/manual_tecnico/justificacion_simples.md` |
| 4 | **Comparsa** | Grupo que desfila en la orilla | ficha «Comparsa en la orilla» |
| 5 | **Colinas** | Fondo de colinas | ESTADO (X-48 «colinas») |
| 6 | **Banda** | Banda de viento con sus músicos | ficha «Banda de viento» |
| 7 | **Camino** | Camino de flores / sendero hacia el arco y el cuarto | `docs/RECORRIDO_GUIADO.md` (estación 3) |
| 8 | **Cuarto del altar** | Altar de ofrenda de tres niveles con velas, flores y pan | ficha «Altar de ofrenda» |
| 9 | **Créditos** | Pantalla de créditos del programa | `src/Menu.cpp` |

## Lo que dicen los rótulos
No se tiene el texto exacto de los rótulos (el hilo de grabación no lo dio): **no se transcribe aquí**. Si se necesita, revisar el MP4.

## Qué NO hay en este video
Voz, recorrido guiado con **T**, fichas culturales abiertas ni las animaciones explicadas una por una (eso es del video 2 y 3).

## Antes de entregar, confirmar al ver el MP4
- [ ] Que el orden de las escenas sea el de la tabla.
- [ ] Que el cuarto del altar se presente como **adaptación del estudio** (ficha del altar y CONTEXTO §5).
- [ ] Que no aparezca nada fuera de alcance (pueblo, Punta Cometa, humo con partículas).
- [ ] Créditos: modelos propios; texturas y audio con su crédito (`docs/REFERENCIAS.md`, `Audio/README.md`).
