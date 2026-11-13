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

### 8.2.1 Integración de las 3 animaciones simples (C-5) — lo que se midió el 12/11

Olas (C-1, vertex shader), velas flotando (C-3), llamas (C-2, fragment shader) y flores flotando (C-4) ya corren **a la vez** en el ciclo de dibujo de `src/main.cpp`; no hay animación que dependa de otra ni se pisen los `uniform` (`oleaje` y `llama` se encienden y apagan alrededor de cada dibujo).

**Triángulos por cuadro** (contados de los `.obj` × las veces que `main.cpp` los dibuja; es un conteo por lectura del código, no una medición de la GPU):

| Estado del código | Triángulos por cuadro | De ellos: flor de Maya (18 874 tris cada una) |
|---|---|---|
| `develop` del 12/11 | ≈ 226 000 | 9 flores = 169 866 (75 %) |
| con B-13 + C-6 (retira capilla y pan de Maya; flores flotantes con la flor propia de 6 tris) | ≈ 78 000 | 2 flores = 37 748 |

**Tiempo por cuadro medido en un renderizador por software** (Mesa llvmpipe, 4 hilos de CPU, ventana 1280×720, 300 cuadros): **no es una GPU integrada**, solo sirve para comparar vistas y cambios entre sí.

| Vista (cámara a 1.6 m) | Con B-13 + C-6 |
|---|---|
| Mirando al cielo (casi solo geometría y vértices) | 8.2 ms |
| Mirando al mar desde z = 6 | 25.0 ms |
| Mirando la fachada desde z = 6 | 43.3 ms |
| Dentro del cuarto, frente al altar | 67.7 ms (antes de B-13 + C-6, 77.1 ms) |

**Lectura (inferida):** el costo domina por el *fragment shader* (cada píxel evalúa 1 luz direccional, 4 puntuales, 1 reflector y 6 reflectores de escalón), no por los triángulos: quitar ~148 000 triángulos solo bajó un 12 % el tiempo en la vista del altar. En una GPU integrada lo que más pesa será entonces la superficie de pantalla cubierta por paredes cerca de la cámara.

**Lo que NO se sabe:** los FPS en la GPU del cliente. El criterio de C-5 (≥ 30 FPS con las tres animaciones) **sigue sin comprobarse** hasta que D2 mida con el contador del título de la ventana.

**Protocolo para D2 (3 minutos):** compilar Release x64, ejecutar sin depurador (Ctrl+F5) y anotar el número del título en 3 lugares, 10 s en cada uno: (1) en la orilla (z ≈ 23) mirando al mar, con velas y flores a la vista; (2) dentro del panteón mirando hacia el cuarto; (3) dentro del cuarto de frente al altar. Anotar también modelo de GPU y si la laptop iba conectada. Si alguno baja de 30, el primer candidato a recortar son las luces de escalón del *fragment shader*.

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
