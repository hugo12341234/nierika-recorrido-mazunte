# Flujo de trabajo — Nierika Studio

> **Qué es este archivo:** la guía para **nosotros** (los dos integrantes): qué es cada cosa del repositorio,
> qué herramienta usar para qué y los pasos para llevar una tarea de principio a fin sin perder trabajo.
> **Regla de oro:** `docs/ESTADO_PROYECTO.md` en la rama `develop` es la memoria del proyecto.
> Lo que no esté ahí, Claude no lo sabrá la próxima vez.

**Contenido:** 1. Mapa del repositorio · 2. Qué herramienta para qué · 3. Cómo se nombran las tareas ·
4. Ciclo de cada tarea · 5. Cuándo usar cada Claude · 6. Reglas · 7. Ejemplo · 8. Lista rápida

---

## 1. Mapa del repositorio (qué es cada cosa)

### Documentos (`docs/`)

| Archivo | Qué es | ¿Se edita? |
|---|---|---|
| `ESTADO_PROYECTO.md` | **Dónde vamos:** plan vigente con códigos de tarea, estado, decisiones, parámetros, problemas | Sí, en cada sesión (`/cierre`) |
| `CONTEXTO_PROYECTO.md` | **Qué prometimos:** resumen de la propuesta (alcance, criterios, stack, convenciones) | Solo si se registra una decisión que lo cambie |
| `PLAN_ACTIVIDADES.md` | Las 60 actividades **originales** (fechas, responsables y criterios de la propuesta) | No (es la referencia original) |
| `FLUJO_DE_TRABAJO.md` | **Cómo trabajamos** (este archivo) | Si cambia la forma de trabajar |
| `INSTRUCCIONES_PROYECTO_CLAUDE.md` | Texto que se pega en las instrucciones del Proyecto de Claude (web) | Si cambian las reglas de Claude |
| `propuesta/` | PDFs entregados al profesor: propuesta v2.0 y plan de trabajo | No |
| `manual_tecnico/`, `manual_usuario/` | Entregables finales; cada bloque deja aquí sus capturas y parámetros | Sí, al cerrar cada bloque |
| `videos/` | Los 3 MP4 finales | Al final (E-4) |

**Si dos documentos dicen cosas distintas, manda:** (1) el Registro de decisiones del ESTADO →
(2) `CONTEXTO_PROYECTO.md` → (3) `PLAN_ACTIVIDADES.md` y los PDFs. Y la contradicción se corrige, no se ignora.

### Código y recursos

| Ruta | Qué es |
|---|---|
| `NierikaStudio.sln` / `.vcxproj` | Solución de Visual Studio 2022 (solo x64) |
| `vcpkg.json` | Librerías que instala vcpkg: Assimp, GLFW, GLEW, GLM (versiones fijas por `builtin-baseline`) |
| `src/` | Código C++: `main.cpp` y las clases `Camera.h`, `Shader.h`, `Model.h`, `Mesh.h` |
| `vendor/SOIL2/` | SOIL2 + stb_image en código fuente (se compilan con el proyecto) |
| `Models/<objeto>/` | Cada modelo con su `.obj`, `.mtl` y **sus texturas** |
| `Shaders/` | Shaders GLSL 330 (`.vs` y `.frag`) |
| `build/` | Salida de compilación (no se sube a Git) |

### Configuración de Claude

| Ruta | Qué es |
|---|---|
| `CLAUDE.md` | Reglas que Claude Code lee al iniciar en este repo |
| `.claude/settings.json` | Ajustes compartidos de Claude Code (p. ej. sin co-autoría en commits) |
| `.claude/skills/` | Comandos: `/actividad`, `/cierre`, `/verificar` |
| `.claude/agents/` | Subagentes: `verificador`, `justificacion-matematica`, `revisor-opengl` |

---

## 2. Qué herramienta para qué

| Herramienta | Para qué se usa | Dónde |
|---|---|---|
| **Trello** | Quién hace qué y en qué estado va cada tarea | https://trello.com/b/8FNkEY97/nierika-studio-recorrido-mazunte |
| **GitHub** | Código y documentos; revisión por *pull request* (PR) | https://github.com/hugo12341234/nierika-recorrido-mazunte |
| **Claude Code** | Modificar archivos del repo trabajando en tu PC (código, shaders, `.mtl`, docs) | En la carpeta del repo (`claude`) |
| **Proyecto de Claude (web)** | Planear, teoría, matemáticas, guías de Blender/GIMP, manuales. **También puede modificar el repo**: el proyecto tiene el repositorio con permiso de escritura y cada hilo lo clona al momento | claude.ai → proyecto "Nierika – Recorrido Mazunte" |
| **Visual Studio 2022** | Compilar (`NierikaStudio.sln`, Release \| x64) y ver la escena | PC de cada integrante |

---

## 3. Cómo se nombran las tareas

- Cada tarea del *Plan vigente* (ESTADO, sección 2) tiene un **código**: letra del bloque + guion + número
  (`A-1` … `A-12`, `B-1` …, `E-7`; `X-1`, `X-2` para las sueltas).
- El guion es a propósito: `D-1` es una tarea del Bloque D; **D1** es el integrante 319266894.
- La columna **Act.** del ESTADO dice qué actividades originales (1–60) cubre cada tarea.
- El mismo código se usa en todos lados:

| Dónde | Formato | Ejemplo |
|---|---|---|
| Tarjeta de Trello | `<código> · <tarea>` | `A-10 · Arco con dovelas` |
| Rama de Git | `feature/<código en minúsculas>-<nombre-corto>` | `feature/a-10-dovelas` |
| Título del PR | `<código> · <tarea> (act. N)` | `A-10 · Arco con dovelas (act. 17)` |
| Claude Code | `/actividad <código>` y `/verificar <código>` | `/actividad A-10` |
| Hilo del Proyecto de Claude | `<código> — <tema>` | `A-10 — Dovelas en Blender` |

---

## 4. Ciclo de cada tarea

### Antes de empezar
1. **Trello:** tomar una tarjeta de *Pendiente* (primero las rojas = Alta) → moverla a *En proceso* → asignarse en **Members**. Máximo 2 tarjetas en proceso por persona.
2. **Actualizar la copia local** (PowerShell, en la carpeta del repo):
   ```
   git checkout develop
   git pull
   ```
3. **Crear la rama de la tarea:**
   ```
   git checkout -b feature/<código>-<nombre-corto>
   ```
   Ejemplo: `git checkout -b feature/a-10-dovelas`.

### Mientras se trabaja
4. Abrir Claude Code (`claude`) y escribir **`/actividad <código>`**: lee el ESTADO, el criterio de terminado y las dependencias, y propone los pasos. **No edita nada hasta que confirmes.**
5. Claude edita los archivos; **quien trabaja compila en Visual Studio (F5)** y le dice qué se ve o qué error sale (Claude no ve la pantalla ni compila en Windows).
6. Tareas de **Blender o GIMP**: abrir un hilo en el Proyecto de Claude (p. ej. "A-10 — Dovelas en Blender"), seguir la guía, exportar el `.obj` a `Models/<objeto>/` y volver a Claude Code para integrarlo.

### Al terminar
7. Comprobar el **criterio de terminado** de la tarea y que compile en **Release | x64** sin errores.
8. En Claude Code: **`/cierre`** → actualiza el ESTADO (estado de la tarea y de sus actividades, parámetros con valores exactos, decisiones, problemas abiertos). **Leer el resumen antes de aprobarlo.**
9. Pedir a Claude Code **"haz commit y push de la rama"** (no lo hace sin que se le pida). Los commits salen **a tu nombre**, sin co-autoría de Claude.
10. **Abrir el PR** en GitHub: base `develop` ← la rama; título `<código> · <tarea> (act. N)`. (O pedir a Claude Code "abre el PR".)
11. **Trello:** tarjeta a *En revisión* y avisar al compañero.

### Revisión (la hace el otro integrante)
12. ```
    git fetch
    git checkout feature/<código>-<nombre-corto>
    ```
    Compilar, probar y en Claude Code escribir **`/verificar <código>`**. Aprobar o comentar en el PR.
13. Si se aprueba: **Merge** en GitHub y borrar la rama. Tarjeta a *Terminado*. Si sirve para el manual técnico, guardar una captura en ese momento.
14. **Los dos** actualizan:
    ```
    git checkout develop
    git pull
    ```

---

## 5. Cuándo usar cada Claude

- **Los dos pueden cambiar archivos del repo**, con las mismas reglas: rama `feature/…`, `/cierre`, PR, revisión del otro integrante y commits a nombre del integrante.
- **Claude Code** → cuando trabajas en tu PC y quieres compilar enseguida (F5) lo que Claude cambió.
- **Proyecto de Claude (web)** → para entender o planear ("¿qué sigue?", fórmulas, Blender, textos de fichas, manuales) y también para cambios en el repo cuando no estás en tu PC. **Un hilo por tarea.**
- **De dónde lee el Proyecto web:** del repositorio, que cada hilo clona al momento; por eso siempre ve el `develop` actual. **No subas copias de `docs/` como archivos del proyecto**: serían una segunda versión que se queda vieja y contradice al repo.
- **Autoría en los hilos web:** Git empieza configurado como `Claude`. Antes del primer commit, Claude debe preguntarte quién eres (D1 o D2) y configurar tu nombre y correo (tabla en `CLAUDE.md`, sección 5).
- **Borrar ramas:** si Claude no puede borrar la rama en GitHub, después del *merge* pulsa **Delete branch** en el PR.
- **Sus instrucciones** son las de `docs/INSTRUCCIONES_PROYECTO_CLAUDE.md`: si cambian ahí, cópialas de nuevo al proyecto.
- **Cierre:** donde se haya hecho la tarea, ahí se corre `/cierre` antes del commit (en Claude Code o en el hilo web con el repo). Escribe `CIERRE` en un hilo web solo si ese hilo no tocó el repo: te dará los cambios para el ESTADO y luego se aplican en una rama.
- **Modelo:** Sonnet para el día a día; Opus para lo difícil (KeyFrames, shaders, errores raros); Haiku para cosas simples. En Claude Code se cambia con `/model`.

---

## 6. Reglas para no rehacer trabajo

1. **Nunca trabajar directo en `develop` ni en `main`**; siempre en una rama `feature/...`.
2. **Siempre `git pull` en `develop` antes de crear una rama.**
3. **Un PR = una tarea**, pequeño, para revisarlo rápido y evitar conflictos.
4. **Antes de cada commit, revisar `git status`**: no deben aparecer `build/`, `.vs/`, `.exe` ni `.dll` (el `.gitignore` los bloquea). Si aparece algo raro, preguntar.
5. **Autoría:** commits y PR solo a nombre del integrante (sin `Co-Authored-By` ni marcas de Claude). El historial es evidencia del manual técnico. Antes del primer commit de cada sesión, comprobar `git config user.name` (en los hilos web viene como `Claude`).
6. **Todo valor que cambie** (escala, posición, color de luz) queda en la **sección 5 del ESTADO**.
7. **Toda decisión nueva** (techo plano, quitar un objeto, cambiar una librería) va al **Registro de decisiones** (sección 4 del ESTADO) y, en el mismo commit, se corrige cualquier otro documento que quede desactualizado.
8. **Conflictos** (casi siempre en `main.cpp`): pedir a Claude Code "actualiza mi rama con develop y resuelve el conflicto".
9. **Viernes, 30 min juntos:** revisar Trello y ESTADO, ver atraso y qué recortar (orden de recorte en la sección 2 del ESTADO).
10. **Al cerrar cada bloque:** capturas y parámetros en `docs/manual_tecnico/`, para no juntarlo todo al final.

---

## 7. Ejemplo: primera tarea del Bloque A

```
Trello: "A-1 · Corregir material del altar" → En proceso
git checkout develop && git pull
git checkout -b feature/a-1-material-altar
claude  →  /actividad A-1
   (Claude corrige altar.obj y altar.mtl; F5 en Visual Studio: el altar se ve con su textura)
/cierre  →  revisar el resumen
"haz commit, push y abre el PR"   →  PR "A-1 · Corregir material del altar"
Trello → En revisión; avisar al compañero
Compañero: git fetch + checkout + F5 + /verificar A-1 → aprueba → Merge
Trello → Terminado; los dos: git checkout develop && git pull
```

---

## 8. Lista rápida

- [ ] Tarjeta en *En proceso* y asignada
- [ ] `git pull` en `develop` y rama nueva `feature/<código>-<nombre>`
- [ ] `/actividad <código>`
- [ ] Compila en Release x64 y cumple el criterio de terminado
- [ ] `/cierre` revisado
- [ ] Commit + push + PR (a tu nombre)
- [ ] Tarjeta en *En revisión* y aviso al compañero
- [ ] `/verificar <código>` y merge
- [ ] Tarjeta en *Terminado* y `git pull` de los dos
