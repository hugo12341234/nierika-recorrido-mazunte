# 3. Herramientas y librerías

Tabla 16 de la propuesta v2.0 con **lo que cambió y lo que aún no está integrado** (✅ **[repo]**: propuesta, `CLAUDE.md` §3, `vcpkg.json`, ESTADO §4).

| Herramienta | Uso en el proyecto | Licencia (propuesta) | Estado real al 04/11 |
|---|---|---|---|
| Visual Studio 2022 | Edición, compilación Debug/Release **x64** | Professional, de pago | ✅ en uso; solo x64 (decisión del 19/10) |
| C++17 | Lenguaje | Estándar abierto | ✅ |
| OpenGL 3.3 Core y GLSL 330 | API gráfica; shaders | Estándar abierto (Khronos) | ✅ |
| GLFW | Ventana, contexto, teclado y ratón | zlib | ✅ por vcpkg |
| ~~GLAD~~ → **GLEW** | Carga de funciones de OpenGL | MIT (GLAD en la propuesta) | ✅ **cambio**: la propuesta decía GLAD; se usa GLEW (decisión del 18/10) porque la base de la Práctica 6 ya compila con él |
| GLM | Vectores, matrices, cuaterniones | MIT | ✅ por vcpkg |
| Assimp | Lectura de `.obj` | BSD | ✅ por vcpkg (versión de vcpkg, no la 4.x del curso) |
| stb_image + **SOIL2** | Lectura de texturas | Dominio público / ver SOIL2 | ✅ **cambio**: la propuesta nombraba solo stb_image; se agrega SOIL2 (decisión del 18/10); ambos en `vendor/SOIL2` |
| **vcpkg** | Instala Assimp, GLFW, GLEW y GLM con versiones fijas (`builtin-baseline` `664933b8…`: Assimp 6.0.5, GLFW 3.5.1, GLEW 2.3.1, GLM 1.0.3) | MIT | ✅ **no estaba en la propuesta** (decisión del 19/10) |
| Dear ImGui | Fichas culturales | MIT | ⏳ **aún no integrada** (C-7); cómo entra (vcpkg o `vendor/`) ❓ **POR DEFINIR** |
| miniaudio | Audio ambiental | Dominio público / MIT-0 | ⏳ **aún no integrada** (D-6); qué audios ❓ **POR DEFINIR** |
| Blender | Modelado, UV, export `.obj` | GPL | ✅ playa y mar con Blender 5.2.2 (D2); la fachada se hizo en Blender 4.4.3 y se reexportó desde 5.2.2 en A-2. Blender y GIMP con la misma versión en las dos PC: X-1 🔄 |
| GIMP | Texturas y canales alfa | GPL | ⏳ sin uso registrado aún; X-1 🔄 |
| Git y GitHub | Repositorio, ramas por funcionalidad, PR | GPL; plan Team de pago | ✅ (repositorio público; ❓ público vs privado sin decidir, ESTADO §4) |
| Trello | Tablero Kanban | Standard, de pago | ✅ |
| OBS Studio | Videos | GPL | ⏳ E-4 |
| Google Docs y Sheets | Hoja del plan | Cuenta gratuita | ✅ la hoja del plan (enlace en el ESTADO) |

## 3.1 Herramientas que **no estaban** en la propuesta

| Herramienta | Uso | Declararla en el manual porque… |
|---|---|---|
| **Autodesk Maya** | Calavera, pan, flor y capilla provisional (D1/D2) | La propuesta eligió Blender y descartó Maya (Tabla 15). Esos modelos **no cuentan** como propios (decisión «solo Blender», 19/10) y la capilla se retira cuando B-9 la reemplace. Hay que decirlo con transparencia |
| **Claude (Claude Code y Proyecto web)** | Planeación, código, documentación y revisión | Regla del ESTADO (el uso de Claude es visible en `CLAUDE.md` y `.claude/`). Redacción final ❓ **POR DEFINIR** |
| Poly Haven | Textura de arena (CC0) | Crédito APA 7 en [§11](11_referencias.md) |

## 3.2 Alternativas descartadas

La comparación de tres alternativas por necesidad es la Tabla 15 de la propuesta (`docs/propuesta/Propuesta_Nierika_v2.0.pdf`, §7). Aquí **no se repite**: se cita. Resumen de la elección de modelado: Blender sobre 3ds Max y Maya. ✅ **[repo]**

📷 **CAPTURA pendiente (X-1):** versión de Blender y de GIMP en cada PC.
