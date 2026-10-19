# CLAUDE.md — Nierika Studio · Recorrido virtual Día de Muertos en Mazunte

> **Qué es este archivo:** las reglas que Claude Code lee al empezar cada sesión en este repositorio.
> Es corto a propósito: el detalle vive en `docs/` (ver tabla de abajo).

Proyecto final de CGeIHC (FI UNAM, 2027-1). Equipo de 2: **D1 = 319266894**, **D2 = 321332086**.
Entrega final: **mié 18/11/2026**. Responde siempre **en español**, súper claro y paso a paso.

## 1. Prioridad número uno: integridad y coherencia
- **No inventes** datos, valores, fuentes ni estados: todo sale del código o de `docs/`. Si algo no está, dilo.
- Antes de cambiar algo, compáralo con los documentos. Si encuentras una **contradicción** (entre documentos,
  o entre un documento y el código), **detente, señálala** y propone registrar una decisión. No elijas en silencio.
- Todo cambio de alcance, valor o regla se anota primero en `docs/ESTADO_PROYECTO.md` (sección 4 o 5)
  y después se refleja en los demás archivos afectados, en el mismo commit.
- No des una tarea por terminada sin compilar y sin comprobar su criterio de terminado.

## 2. Documentos: qué es cada uno y cuál manda

| Archivo | Qué es | Cuándo se lee |
|---|---|---|
| `docs/ESTADO_PROYECTO.md` | **Dónde vamos:** plan vigente con códigos de tarea (`A-1`…), estado, decisiones, parámetros, problemas | Siempre, al empezar cualquier tarea |
| `docs/CONTEXTO_PROYECTO.md` | **Qué prometimos:** alcance, criterios de aceptación, stack y convenciones (resumen de la propuesta) | Antes de tareas no triviales |
| `docs/PLAN_ACTIVIDADES.md` | Las 60 actividades **originales** de la propuesta (referencia; no cambia) | Para criterios y entregables originales |
| `docs/FLUJO_DE_TRABAJO.md` | **Cómo trabajamos:** ciclo Trello → rama → PR, reglas de Git, mapa del repositorio | Para recordar al usuario el paso que falte |
| `docs/INSTRUCCIONES_PROYECTO_CLAUDE.md` | Texto que se pega en el Proyecto de Claude (web) | Solo si se cambian las reglas de Claude |
| `docs/propuesta/*.pdf` | Propuesta v2.0 y plan entregados al profesor (no se editan) | Para citar el documento original |

**Orden de autoridad** si dos documentos dicen cosas distintas:
(1) Registro de decisiones del ESTADO → (2) `CONTEXTO_PROYECTO.md` → (3) `PLAN_ACTIVIDADES.md` y PDFs.

## 3. Stack (no cambiar sin registrar decisión)
C++17 · OpenGL 3.3 **Core** · GLSL `#version 330 core` · Visual Studio 2022 · **solo x64** (Debug/Release) ·
GLFW · **GLEW** · GLM · Assimp (por **vcpkg**) · SOIL2 / stb_image (en `vendor/`) · Dear ImGui · miniaudio.
Base: proyecto de la Práctica 6 del laboratorio, con las clases del curso `Shader`, `Camera`, `Model`, `Mesh`
(respeta su estilo y amplíalas; no las reescribas desde cero sin necesidad).
Prohibido: OpenGL legacy (`glBegin`, `glMatrixMode`…), nuevas librerías sin registrar decisión, motores (Unity/Unreal).

## 4. Convenciones
- 1 unidad = 1 metro. Cámara 1ª persona, pitch ±89°, colisiones AABB, movimiento × `deltaTime`.
- Carpetas: código en `src/`; `Models/<objeto>/<objeto>.obj` (con su .mtl y **sus texturas en la misma carpeta**;
  no hay carpeta `textures/` aunque la propuesta la mencione); `Shaders/*.vs` y `*.frag`; `Audio/`; `vendor/`.
- Rutas **relativas** siempre. Shaders en archivos dentro de `Shaders/`, no en strings de C++.
- Escala real en el .obj (1 u = 1 m); en `main.cpp` solo escalas cercanas a 1.
- Texturas 1K (1024 px), cargadas en RGBA (el fragment shader descarta alfa < 0.1).
- Toda la memoria de GPU (VAO, VBO, EBO, texturas, programas) se libera en destructores o `cleanup()`.
- Comentarios en español. Una clase por par `.h/.cpp` (las clases heredadas del curso siguen solo en `.h`;
  si una función queda definida en un `.h`, debe ser `inline`).

## 5. Git y autoría
- No hagas `git commit` ni `git push` sin que el usuario lo pida.
- Ramas: `main` (solo entregas), `develop` (integración), `feature/<código>-<nombre-corto>` en minúsculas,
  p. ej. `feature/a-10-dovelas` (para la act. 4 se usó `feature/act-4-base-solucion`). Todo entra a `develop` por PR.
- Título del PR: `<código> · <tarea> (act. N)`, p. ej. `A-10 · Arco con dovelas (act. 17)`.
- Autoría: los commits y PR van **solo a nombre del integrante** (su cuenta de GitHub). **No agregues** líneas
  `Co-Authored-By`, `Claude-Session` ni "Generated with Claude Code" en commits ni en descripciones de PR.

## 6. Restricciones del cliente (profesor)
- Fuera de alcance: pueblo, Punta Cometa, tapete de arena, VR/web/móvil, multijugador, IA, **partículas de humo**.
- KeyFrames **solo** en caminata de silueta y "dejar flores". Velas, fuego, papel picado y humo **no** cuentan como KeyFrames.
- Ninguna animación copiada de las prácticas del curso; cada una con justificación matemática.
- ≥ 18 modelos **propios**; los externos solo como secundarios y con crédito.
- Fichas (Dear ImGui) a < 2 m, citando fuente APA 7. Rendimiento ≥ 30 FPS a 1280×720 en gráficos integrados.

## 7. Compilar y probar
- Compilar: abrir `NierikaStudio.sln` en Visual Studio 2022 → `Release | x64` (o `Debug | x64`) → Build Solution.
  Desde terminal (Developer PowerShell): `msbuild NierikaStudio.sln /p:Configuration=Release /p:Platform=x64`
- Dependencias: Assimp, GLFW, GLEW y GLM las instala **vcpkg** (manifiesto `vcpkg.json` con `builtin-baseline`;
  instalación una sola vez en `C:\vcpkg` con `bootstrap-vcpkg.bat` y `vcpkg integrate install`, ver README;
  si sale un error del *baseline*, actualizar con `git -C C:\vcpkg pull`). SOIL2/stb viven en `vendor/` y se compilan con el proyecto.
- Salida en `build/x64/<Configuración>/` (ignorada por git). F5 ya fija el *Working Directory* en la raíz del repo.
- Claude no puede compilar en Windows ni ver la pantalla: pide al usuario que compile (F5) y te diga qué ve o qué error sale.
- Antes de dar algo por terminado: compila sin errores y explica al usuario qué debe ver al ejecutar.

## 8. Comandos y subagentes del proyecto
- `/actividad <código o N>` — arranca una tarea (`/actividad A-10`) o una actividad del plan (`/actividad 17`).
- `/cierre` — actualiza `docs/ESTADO_PROYECTO.md` con lo hecho en la sesión.
- `/verificar <código o N>` — revisa la tarea como verificador antes de aprobar el PR.
- Subagentes: `verificador` (revisión contra el criterio de terminado), `justificacion-matematica`
  (fórmulas para el manual técnico), `revisor-opengl` (errores de render, fugas de GPU, rendimiento).
