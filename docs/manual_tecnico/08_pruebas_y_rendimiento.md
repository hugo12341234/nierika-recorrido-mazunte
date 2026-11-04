# 8. Pruebas y rendimiento

## 8.1 Cómo se prueba hoy ✅ **[repo]**

- Cada tarea se **compila en Release x64** en la PC de D2 y se corre con F5; se documentan capturas y la consola sin errores.
- Cada PR pasa `/verificar <código>` (subagente `verificador`) con veredicto APROBAR antes de fusionarse (desde el 20/10).
- No hay CI ni pruebas automáticas en el repositorio. Claude no puede compilar en Windows ni ver la pantalla, así que la compilación y la inspección visual las hace una persona.

## 8.2 Rendimiento

| Criterio | Valor | Estado |
|---|---|---|
| ≥ 30 FPS a 1280×720 en gráficos integrados | meta del cliente | ❓ **sin medir**: A-8 lo registra como «los FPS están bien» (palabra de D2, sin valor). No hay contador de FPS en el código |
| PC de medición | ❓ **POR DEFINIR** (¿integrada o dedicada?) | — |
| Valor medido | ⏳ | Se llena cuando exista un contador (tarea E-1a/E-1) |

Para el manual hace falta: modelo de GPU, FPS medio y mínimo, escena donde se midió y fecha. 📷 **CAPTURA pendiente.**

## 8.3 Memoria de GPU

Hoy no se libera la memoria de GPU de `Shader`, `Mesh` y `Model` (problema #7, ESTADO §6); la decisión del 19/10 es resolverlo con `cleanup()` explícito al final de `main`, **no con destructores**, porque las clases se copian. En `main.cpp` solo se libera lo creado ahí (el cubo de luz). ⏳ E-1.

## 8.4 Problemas abiertos al 04/11 ✅ **[repo, ESTADO §6]**

| # | Problema | Se resuelve en |
|---|---|---|
| 3 | Texturas de la capilla provisional de 2048 px (la propuesta pide 1K) | B-9 |
| 7 | No se libera memoria de GPU | E-1 |
| 8 | `Model.h` no carga mapas normales (necesario para el pan) | B-12 |
| 9 | `fachada.obj` incluye el techo | A-11 |
| 12 | La apertura del arco no es de medio punto | A-10 / A-12 |
| 13 | Animación de las 4 luces: cerrado por palabra de D2 («es propio»), sin verificar; `cleanup()` pendiente | E-1 |

Cerrados: #5 (iluminación, A-8), #4, #6 y #11 (A-2), #1 y #2 (A-1).

## 8.5 Registro de errores de la prueba final

⏳ E-1: lista de errores priorizada (tarea E-1, criterio del ESTADO). Se llena al cierre.
