# c) Tecnologías utilizadas

## c.1 Cómo se eligieron

La propuesta v2.0 comparó tres alternativas por cada necesidad con una matriz ponderada (cumplimiento de los requisitos de entrega 35 %, control del render y los shaders 20 %, experiencia del equipo 20 %, rendimiento en gráficos integrados 15 % y costo de licencia 10 %). Para el motor, **OpenGL 3.3 con C++** obtuvo 4.85 puntos contra 2.55 de Unity y 2.20 de Unreal Engine 5: es la única opción que entrega un proyecto de Visual Studio donde el modelado jerárquico, la iluminación y la interpolación de KeyFrames quedan programados por el estudio, que es lo que pide el cliente. La tabla de decisión y las demás comparaciones (ventana, carga de modelos, modelado, texturas, fichas, audio, gestión) están en la propuesta, sección 7.

## c.2 Tecnologías del proyecto

| Tecnología | Para qué se usa | Licencia | Cómo se integra |
|---|---|---|---|
| C++17 | Lenguaje de toda la aplicación | Estándar ISO/IEC 14882:2017 | Visual Studio 2022, solo x64 (Debug/Release) |
| OpenGL 3.3 Core y GLSL 330 | API gráfica y shaders (iluminación, olas, llama, cielo) | Estándar abierto de Khronos | GLEW carga las funciones |
| Visual Studio 2022 | Edición, compilación y depuración | Comercial (Professional, según la propuesta) | `NierikaStudio.sln` |
| GLFW | Ventana, contexto, teclado y ratón | zlib/libpng | vcpkg |
| GLEW | Carga de funciones de OpenGL | Sitio sin licencia explícita; en la práctica, licencias tipo MIT/BSD | vcpkg (cambio frente a la propuesta, que usaba GLAD) |
| GLM | Vectores, matrices y cuaterniones | Happy Bunny / MIT | vcpkg |
| Assimp | Lectura de `.obj` y materiales | BSD de 3 cláusulas modificada | vcpkg |
| SOIL2 y stb_image | Lectura de texturas (RGBA) | MIT-0 y dominio público / MIT | `vendor/SOIL2` en código fuente (stb_image v2.15) |
| Dear ImGui | Fichas, menús y subtítulos | MIT | `vendor/imgui` v1.91.9 en código fuente |
| miniaudio | Audio con volumen y paneo | Dominio público / MIT sin atribución | `vendor/miniaudio` v0.11.26 |
| vcpkg | Instala Assimp, GLFW, GLEW y GLM con versiones fijas (`builtin-baseline` `664933b8…`) | MIT | `vcpkg.json` (no estaba en la propuesta) |
| Blender | Modelado, UV y exportación `.obj`; versión 5.2 en la PC de D2 y 4.4.3 para la fachada original | GPL | Además se usó como módulo de Python (`bpy`) sin ventana |
| Python (numpy, Pillow, `bpy`) | Scripts de `tools/` que generan modelos, texturas y datos de la escena | PSF, BSD y GPL | Cada resultado conserva su script |
| Autodesk Maya | Modelos de D1: calavera, flor, pan, humo, fuego, mariposas y otros del altar | Comercial (suscripción) | No cuentan en el inventario de modelos propios de Blender |
| Git y GitHub | Control de versiones: ramas `main`, `develop` y `feature/*`, y *pull requests* | GPL; plan Team según la propuesta | Repositorio público |
| Trello | Tablero Kanban (Pendiente, En proceso, En revisión, Terminado) | Standard, de pago | Una tarjeta por tarea con su código |
| Poly Haven | Tres texturas (arena, piso, barda) | CC0 | Con crédito |
| ElevenLabs (modelo Eleven v4) | Voz sintética de las 12 narraciones del recorrido | Términos de uso de ElevenLabs | Uso educativo, no comercial |
| Claude (Anthropic) | Asistente de planeación, programación, scripts, documentación y revisión (Claude Code y un Proyecto de Claude) | Servicio de Anthropic | Reglas escritas en `CLAUDE.md`; ver sección l) |

## c.3 Cambios frente a la propuesta

| Cambio | Motivo |
|---|---|
| GLEW en lugar de GLAD; se agrega SOIL2 | La base de la Práctica 6 ya compilaba con GLEW y SOIL2 |
| Se agrega vcpkg | La solución recibida estaba en 32 bits; así el equipo usa las mismas versiones |
| Se agrega Python y `bpy` | Generar de forma reproducible modelos, texturas y datos; cada archivo generado tiene su script en `tools/` |
| Se agrega Autodesk Maya (solo D1) | D1 hizo calavera, flor, pan y otros modelos en Maya; se integraron como extras |
| Se agrega ElevenLabs | La narración del recorrido guiado usa una voz sintética |
| GIMP sin uso registrado | Las texturas propias se generaron con scripts de Python, no con GIMP |
| OBS Studio | Se previó para los videos; ver la sección de pruebas y los videos de la entrega |

## c.4 Dependencias externas verificadas

Las versiones de `vendor/` salen de los encabezados de sus archivos (Dear ImGui 1.91.9, miniaudio 0.11.26, stb_image 2.15). Las versiones que instala vcpkg quedan fijadas por el `builtin-baseline` del archivo `vcpkg.json`; al decidir el baseline (19/10) se anotaron Assimp 6.0.5, GLFW 3.5.1, GLEW 2.3.1 y GLM 1.0.3. No se registró la versión exacta de Visual Studio 2022, de Maya ni de Python.
