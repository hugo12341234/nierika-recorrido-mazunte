# CLAUDE.md — Nierika Studio · Recorrido virtual Día de Muertos en Mazunte

Proyecto final de CGeIHC (FI UNAM, 2027-1). Equipo de 2: **D1 = 319266894**, **D2 = 321332086**.
Entrega final: **mié 18/11/2026**. Responde siempre **en español**, explicando paso a paso.

## Contexto obligatorio (léelo antes de tareas no triviales)
- `docs/CONTEXTO_PROYECTO.md` — alcance, criterios de aceptación, stack, convenciones.
- `docs/PLAN_ACTIVIDADES.md` — 60 actividades (responsable, verificador, criterio).
- `docs/ESTADO_PROYECTO.md` — **plan vigente (sección 2)**, hecho/pendiente, decisiones, parámetros fijados. **Mantenlo al día** (`/cierre`).
- `docs/FLUJO_DE_TRABAJO.md` — ciclo de cada tarea (Trello → rama → `/actividad` → `/cierre` → PR → `/verificar` → merge). Síguelo y recuérdale al usuario el paso que falte.
- Se trabaja por disponibilidad (no por Resp./Verif. de la propuesta); el otro integrante revisa con `/verificar`.

## Stack (no cambiar)
C++17 · OpenGL 3.3 **Core** · GLSL `#version 330 core` · Visual Studio 2022 · x64 Debug/Release ·
GLFW · **GLEW** · GLM · Assimp · SOIL2 / stb_image · Dear ImGui · miniaudio.
Base: proyecto de la Práctica 6 del laboratorio, con las clases del curso `Shader`, `Camera`, `Model`
(respeta su estilo y amplíalas; no las reescribas desde cero sin necesidad).
Prohibido: OpenGL legacy (`glBegin`, `glMatrixMode`…), nuevas librerías sin registrar decisión, motores (Unity/Unreal).

## Convenciones
- 1 unidad = 1 metro. Cámara 1ª persona, pitch ±89°, colisiones AABB, movimiento × `deltaTime`.
- Rutas **relativas**: `Models/<objeto>/<objeto>.obj` (con su .mtl y texturas), `Shaders/*.vs` y `*.frag`, `Audio/`.
- Shaders en archivos dentro de `Shaders/`, no en strings de C++.
- Escala real en el .obj (1 u = 1 m); en `main.cpp` solo escalas cercanas a 1.
- Toda la memoria de GPU (VAO, VBO, EBO, texturas, programas) se libera en destructores o `cleanup()`.
- Comentarios en español. Una clase por par `.h/.cpp`.
- No hagas `git commit` ni `git push` sin que el usuario lo pida. Ramas: `main` (releases), `develop`, `feature/act-<N>-<nombre>`. Todo entra a `develop` por PR.

## Restricciones del cliente (profesor)
- Fuera de alcance: pueblo, Punta Cometa, tapete de arena, VR/web/móvil, multijugador, IA, **partículas de humo**.
- KeyFrames **solo** en caminata de silueta y "dejar flores". Velas, fuego, papel picado y humo **no** cuentan como KeyFrames.
- Ninguna animación copiada de las prácticas del curso; cada una con justificación matemática.
- Fichas (Dear ImGui) a < 2 m, citando fuente APA 7. Rendimiento ≥ 30 FPS a 1280×720 en gráficos integrados.

## Compilar y probar
- Compilar: Visual Studio → configuración `Release | x64` (o `Debug | x64`), Build Solution.
  Desde terminal (Developer PowerShell): `msbuild <Solucion>.sln /p:Configuration=Release /p:Platform=x64`
  *(ajusta el nombre de la solución cuando exista; luego actualiza esta línea).*
- Ejecutar con el *Working Directory* en la carpeta que contiene `Models/` y `Shaders/` para que las rutas relativas funcionen.
- Antes de dar algo por terminado: compila sin errores y explica al usuario qué debe ver al ejecutar.

## Agentes y comandos del proyecto
- `/actividad <N>` — arranca el trabajo de una actividad del plan.
- `/cierre` — actualiza `docs/ESTADO_PROYECTO.md` con lo hecho en la sesión.
- `/verificar <N>` — revisa la actividad como verificador antes de aprobar el PR.
- Subagentes: `verificador` (revisión contra criterio de aceptación), `justificacion-matematica` (fórmulas para el manual técnico), `revisor-opengl` (errores de render, fugas de GPU, rendimiento).
