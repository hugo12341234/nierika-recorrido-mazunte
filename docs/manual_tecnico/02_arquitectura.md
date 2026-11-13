# 2. Arquitectura

## 2.1 Capas de la aplicación

Arquitectura por capas de la propuesta (Figura 15) ✅ **[repo, CONTEXTO §6]**, con el estado de cada módulo al **12/11** (según el código de `develop`; lo marcado «PR» está abierto y aún no entra):

```
Aplicación del recorrido (C++17, Visual Studio 2022)
 ├─ Cámara y colisiones      (GLFW · GLM)              ✅ Camera.h · Colisiones.cpp (AABB, A-9)
 ├─ Carga de modelos         (Assimp · SOIL2/stb)      ✅ Model.h / Mesh.h (+ mapa de relieve, PR de B-12)
 ├─ Shaders e iluminación    (GLSL 330)                ✅ lighting.vs/.frag: Blinn-Phong (A-8), olas (C-1), llama (C-2)
 ├─ Animación                (KeyFrames · slerp)       ✅ Animacion.cpp · Silueta.cpp · DejarFlor.h; simples: Olas.h, AnimVelas.h, AnimFlores.h
 ├─ Fichas culturales        (Dear ImGui)              ✅ Fichas.cpp (falta compilar en VS y probar)
 └─ Audio                    (miniaudio)               ✅ Audio.cpp; pistas propias en el PR de D-6
OpenGL 3.3 Core Profile (GLEW)
GPU / Recursos (.obj, texturas, audio, keyframes)
```

Todo movimiento se calcula en la aplicación (KeyFrames en C++); el oleaje se calcula en el *vertex shader*, con la misma fórmula en CPU (`Olas.h`) para posar las velas y las flores sobre la ola. ✅ **[repo, CONTEXTO §6; `src/Olas.h`]**

📷 **FIGURA pendiente:** diagrama de capas con el estilo de Nierika Studio (la Figura 15 de la propuesta es la referencia; se redibuja al cerrar el manual).

## 2.2 Código existente

Medido con `wc -l` en `develop` el 12/11/2026.

| Archivo | Líneas | Qué hace | Origen |
|---|---|---|---|
| `src/main.cpp` | 727 | Ventana GLFW 1280×720, GLEW, carga de modelos y shaders, colisiones, ciclo de render con `deltaTime`, entrada y cleanup | Práctica 6 del curso, ampliada |
| `src/Camera.h` | 171 | Cámara de primera persona: WASD/flechas y ratón, pitch ±89° | Clase del curso |
| `src/Shader.h` | 112 | Compila y enlaza programas GLSL desde archivos; `cleanup()` | Clase del curso |
| `src/Model.h`, `src/Mesh.h` | 272 · 152 | Carga `.obj` con Assimp, texturas RGBA con SOIL2/stb; `Draw(Shader&)`; `cleanup()` | Clases del curso, ampliadas |
| `src/Colisiones.h/.cpp` | 44 · 46 | AABB de cuerpo (0.5 × 0.5 × 1.7 m) contra cajas; deslizamiento por eje; altura de suelo según la pendiente de la playa | Propio (A-9) |
| `src/Animacion.h/.cpp` | 69 · 83 | `KeyFrame`, `Pista`, `Slerp` propio y curvas de aceleración | Propio (D-2) |
| `src/Silueta.h/.cpp` | 75 · 190 | Jerarquía de 10 piezas, caminata de 18.33 s y dejar la flor | Propio (D-1, D-2, D-4) |
| `src/DejarFlor.h` | 36 | Bézier cuadrática = caída libre de la flor | Propio (D-4) |
| `src/Olas.h`, `AnimVelas.h`, `AnimFlores.h` | 90 · 48 · 70 | Fórmulas de las 3 animaciones simples (solo `<cmath>`) | Propio (C-1, C-3, C-4) |
| `src/Fichas.h/.cpp` | 18 · 140 | 5 fichas con Dear ImGui a < 2 m (la más cercana gana) | Propio (C-7b) |
| `src/Audio.h/.cpp` | 23 · 98 | Dos pistas en bucle con miniaudio; si faltan archivos sigue sin sonido | Propio (D-6) |
| `Shaders/lighting.vs/.frag` | 50 · 200 | Blinn-Phong con 1 direccional, 4 puntuales, 1 reflector y 6 reflectores de escalón; oleaje, llama de Perlin y descarte por alfa < 0.1 | A-8, C-1, C-2 |
| `Shaders/lamp.vs/.frag` | — | Cubos de luz (depuración, tecla L) | A-8 (04/11) |
| `Shaders/modelLoading.*` | — | Sin uso; queda en el repo | Práctica 6 |

Datos de ✅ **[repo]**.

## 2.3 Ciclo de render (resumen)

1. Inicialización: GLFW (ventana 1280×720, no redimensionable, cursor capturado), GLEW, ImGui, shaders, ≈ 30 modelos, cajas de colisión, audio.
2. Por cuadro: calcular `deltaTime` y el título con los FPS → procesar entrada (`DoMovement`, con colisiones) → limpiar → enviar cámara y luces → dibujar fachada, arco, techo, piso, playa, mar (con `oleaje = 1`), altar con su ofrenda, panteón, barda, flores y velas flotantes, llamas (`llama = 1`) y la silueta con su flor → ficha cultural si hay una a < 2 m → intercambiar buffers.
3. Cierre: `cleanup()` explícito de `Shader`, `Mesh` y `Model` (decisión del 19/10, implementada en E-1a el 04/11; **falta confirmar con una herramienta que no haya fugas reales**, ESTADO §6 problema #7) antes de `glfwTerminate()`.

✅ **[repo]** `main.cpp` y ESTADO §4 y §6. 📷 **FIGURA pendiente:** diagrama de flujo del ciclo.

## 2.4 Parámetros de cámara y proyección

Los valores vigentes están en `ESTADO_PROYECTO.md` §5 y se copian aquí **al cerrar el manual**, no antes, para no duplicarlos: ventana 1280×720, proyección FOV 45°, near/far 0.1/300 m; cámara a 1.6 m de altura de ojos y 2.0 m/s (**valores iniciales**, ajustables). ✅ **[repo]**

## 2.5 Estructura del repositorio

| Ruta | Contenido |
|---|---|
| `src/` | Código C++ |
| `vendor/SOIL2/` | SOIL2 + stb_image en código fuente |
| `Models/<objeto>/` | `.obj`, `.mtl`, texturas y `.blend` de cada modelo |
| `Shaders/` | GLSL 330 (`.vs`, `.frag`) |
| `docs/` | ESTADO, CONTEXTO, PLAN, FLUJO, propuesta (PDF), manuales, videos |
| `build/`, `dist/` | Salida de compilación y Release autónomo; **no se versionan** |

✅ **[repo, FLUJO_DE_TRABAJO §1]**. `Audio/` existe con su `README.md`; las pistas propias llegan con el PR de D-6. `tools/` guarda los scripts que generan modelos (`bpy`), texturas y audio.

## 2.6 Decisiones de arquitectura (extracto del ESTADO §4)

| Fecha | Decisión | Por qué |
|---|---|---|
| 18/10 | GLEW en lugar de GLAD; SOIL2 además de stb_image | Base ya compilada y dominada |
| 19/10 | Solución a x64 con vcpkg (`builtin-baseline`) | La solución recibida estaba en 32 bits; mismas versiones en todas las PC |
| 19/10 | `cleanup()` explícito, no destructores | Las clases se copian (`Draw` por valor, `vector<Mesh>`) |
| 19/10 | `Draw(Shader&)` por referencia | Evitar copias del shader |
| 20/10 | Playa y mar en coordenadas del mundo, matriz identidad | Ver ESTADO §4 («Playa y mar») |
| 04/11 | Iluminación de D1 (Blinn-Phong) | Decisión de D2; sustituye «ambiental cálida + 3 luces clave» |

El resto está en `ESTADO_PROYECTO.md` §4; este manual cita la fecha de cada decisión y no la reescribe.
