# 3. Herramientas y librerías

Tabla 16 de la propuesta v2.0 con **lo que cambió** respecto a ella (✅ **[repo]**: propuesta, `CLAUDE.md` §3, `vcpkg.json`, `vendor/`, ESTADO §4). Estado al **12/11/2026**.

| Herramienta | Uso en el proyecto | Licencia (propuesta) | Estado real |
|---|---|---|---|
| Visual Studio 2022 | Edición, compilación Debug/Release **x64** | Professional, de pago | ✅ solo x64 (decisión del 19/10) |
| C++17 | Lenguaje | Estándar abierto | ✅ |
| OpenGL 3.3 Core y GLSL 330 | API gráfica; shaders | Estándar abierto (Khronos) | ✅ |
| GLFW | Ventana, contexto, teclado y ratón | zlib | ✅ por vcpkg |
| ~~GLAD~~ → **GLEW** | Carga de funciones de OpenGL | MIT (GLAD en la propuesta) | ✅ **cambio** (18/10): la base de la Práctica 6 ya compila con GLEW |
| GLM | Vectores, matrices, cuaterniones | MIT | ✅ por vcpkg |
| Assimp | Lectura de `.obj` | BSD | ✅ por vcpkg |
| stb_image + **SOIL2** | Lectura de texturas | Dominio público / ver SOIL2 | ✅ **cambio** (18/10): se agrega SOIL2; ambos en `vendor/SOIL2` |
| **vcpkg** | Instala Assimp, GLFW, GLEW y GLM con versiones fijas (`builtin-baseline` `664933b8…`) | MIT | ✅ **no estaba en la propuesta** (decisión del 19/10) |
| Dear ImGui | Fichas culturales | MIT | ✅ **integrada en `vendor/imgui/`** (v1.91.9, copiada el 11/11); 🔄 falta compilarla y probarla en VS |
| miniaudio | Audio ambiental | Dominio público / MIT-0 | ✅ en `vendor/miniaudio/`; pistas propias en `Audio/` (D-6) |
| Blender | Modelado, UV, export `.obj` | GPL | ✅ playa y mar con 5.2.2 (D2); fachada hecha en 4.4.3 y reexportada desde 5.2.2. Los modelos de B-3, B-4, B-7, B-9, B-10, B-12 y E-2 se generaron con `bpy` 5.3 (Blender como módulo de Python, sin ventana; decisión del 11/11). Misma versión en las dos PC: X-1 🔄 |
| GIMP | Texturas y canales alfa | GPL | ⏳ **sin uso registrado**: las texturas propias de B-12 se generaron con scripts de Python (`tools/b12_texturas.py`) |
| Python (numpy, `bpy`) | Scripts que generan modelos, texturas y audio (`tools/`) | PSF / GPL | ✅ **no estaba en la propuesta** (ver abajo) |
| Git y GitHub | Repositorio, ramas por funcionalidad, PR | GPL; plan Team de pago | ✅ repositorio **público** (ESTADO, encabezado) |
| Trello | Tablero Kanban | Standard, de pago | ✅ |
| OBS Studio | Videos | GPL | ⏳ E-4 |
| Google Docs y Sheets | Hoja del plan | Cuenta gratuita | ✅ (enlace en el ESTADO) |

## 3.1 Herramientas que **no estaban** en la propuesta

| Herramienta | Uso | Nota de transparencia |
|---|---|---|
| **Autodesk Maya** | Calavera, pan, flor (D1, extras X-3) y capilla provisional (ya retirada de la escena en B-13) | La propuesta eligió Blender. Esos modelos **no cuentan** como propios (decisión «solo Blender», 19/10) |
| **Scripts de Python / `bpy`** | Generan modelos B-3, B-4, B-7, B-9, B-10, texturas (B-12) y el audio (D-6) | Todo lo generado está en el repo con su script; es trabajo del equipo con ayuda de Claude |
| **Claude (Claude Code y Proyecto web)** | Planeación, código, documentación y revisión | ❓ redacción final por D1 y D2 ([§9.3](09_conclusiones.md)) |
| Poly Haven | Textura de arena (CC0) | Crédito APA 7 en [§11](11_referencias.md) |

## 3.2 Alternativas descartadas

La comparación de tres alternativas por necesidad es la Tabla 15 de la propuesta (`docs/propuesta/Propuesta_Nierika_v2.0.pdf`, §7); aquí se cita, no se repite. Resumen: Blender sobre 3ds Max y Maya. ✅ **[repo]**

📷 **CAPTURA pendiente (X-1):** versión de Blender y de GIMP en cada PC.
