# 2. Arquitectura

## 2.1 Capas de la aplicación

Arquitectura por capas de la propuesta (Figura 15) ✅ **[repo, CONTEXTO §6]**, con el estado de cada módulo al 04/11:

```
Aplicación del recorrido (C++17, Visual Studio 2022)
 ├─ Cámara y colisiones      (GLFW · GLM)              🔄 cámara ✅ · colisiones AABB ⏳ A-9
 ├─ Carga de modelos         (Assimp · SOIL2/stb)      ✅ Model.h / Mesh.h
 ├─ Shaders e iluminación    (GLSL 330)                ✅ lighting.vs/.frag (A-8) · olas ⏳ C-1 · llama ⏳ C-2
 ├─ Animación                (KeyFrames · slerp)       ⏳ D-1…D-5
 ├─ Fichas culturales        (Dear ImGui)              ⏳ C-7
 └─ Audio                    (miniaudio)               ⏳ D-6
OpenGL 3.3 Core Profile (GLEW)
GPU / Recursos (.obj, texturas, audio, keyframes)
```

Todo movimiento se calcula en la aplicación (KeyFrames en C++); el oleaje se calcula en el *vertex shader*. ✅ **[repo, CONTEXTO §6]**

📷 **FIGURA pendiente:** diagrama de capas con el estilo de Nierika Studio (la Figura 15 de la propuesta es la referencia; se redibuja al cerrar el manual).

## 2.2 Código existente

| Archivo | Qué hace | Origen |
|---|---|---|
| `src/main.cpp` (426 líneas) | Ventana GLFW 1280×720, inicialización GLEW, carga de shaders y modelos, ciclo de render con `deltaTime`, entrada de teclado y ratón | Práctica 6 del curso, ampliada |
| `src/Camera.h` | Cámara de primera persona: WASD/flechas y ratón, pitch ±89° | Clase del curso |
| `src/Shader.h` | Compila y enlaza programas GLSL desde archivos | Clase del curso |
| `src/Model.h`, `src/Mesh.h` | Carga `.obj` con Assimp, texturas RGBA con SOIL2/stb; `Draw(Shader&)` | Clases del curso, ampliadas |
| `Shaders/lighting.vs`, `lighting.frag` | Blinn-Phong con 1 direccional, 4 puntuales, 1 reflector y 6 reflectores; descarte por alfa < 0.1 | A-8 (04/11), luces de D1 |
| `Shaders/lamp.vs`, `lamp.frag` | Cubos de luz (ayuda de depuración, tecla L) | A-8 (04/11) |
| `Shaders/modelLoading.*` | Sin uso (solo textura); queda en el repo | Práctica 6 |

Datos de ✅ **[repo, ESTADO §1]**. Tamaños de archivo medidos con `wc -l` en `develop`.

## 2.3 Ciclo de render (resumen)

1. Inicialización: GLFW (ventana 1280×720, no redimensionable, cursor capturado), GLEW, shaders, modelos.
2. Por cuadro: calcular `deltaTime` → procesar entrada (`DoMovement`) → limpiar pantalla → enviar al shader la cámara y las luces → dibujar playa, mar, fachada, capilla provisional, calavera, pan y flores → intercambiar buffers.
3. Cierre: se libera lo creado en `main`; la memoria de GPU de `Shader`/`Mesh`/`Model` se libera con `cleanup()` explícito (decisión del 19/10, tarea E-1; **hoy no está implementado** salvo para el cubo de luz).

✅ **[repo]** `main.cpp` y ESTADO §4 (decisión `cleanup()`) y §6 (problema #7). 📷 **FIGURA pendiente:** diagrama de flujo del ciclo.

## 2.4 Parámetros de cámara y proyección

Los valores vigentes están en `ESTADO_PROYECTO.md` §5 y se copian aquí **al cerrar el manual**, no antes, para no duplicarlos: ventana 1280×720, proyección FOV 45°, near/far 0.1/300 m; cámara 1.6 m de altura de ojos y 2.0 m/s (**valores iniciales**, se ajustan en A-9). ✅ **[repo]**

## 2.5 Estructura del repositorio

| Ruta | Contenido |
|---|---|
| `src/` | Código C++ |
| `vendor/SOIL2/` | SOIL2 + stb_image en código fuente |
| `Models/<objeto>/` | `.obj`, `.mtl`, texturas y `.blend` de cada modelo |
| `Shaders/` | GLSL 330 (`.vs`, `.frag`) |
| `docs/` | ESTADO, CONTEXTO, PLAN, FLUJO, propuesta (PDF), manuales, videos |
| `build/`, `dist/` | Salida de compilación y Release autónomo; **no se versionan** |

✅ **[repo, FLUJO_DE_TRABAJO §1]**. `Audio/` está previsto en `CLAUDE.md` pero **todavía no existe** (D-6).

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
