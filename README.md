# Nierika Studio — Recorrido virtual del Día de Muertos en Mazunte, Oaxaca

*«Ver más allá de lo visible»*

Recorrido 3D interactivo, en primera persona y en tiempo real, que recrea el Día de Muertos en **Mazunte, Oaxaca (México)**:
la playa donde se dejan velas y cempasúchil en el mar, el panteón con su camino de ánimas y la capilla con un altar de ofrenda
de tres niveles. El visitante camina libremente, escucha un recorrido guiado con voz y abre fichas culturales cortas que
explican el significado de cada elemento y citan su fuente.

> Proyecto final de **Computación Gráfica e Interacción Humano-Computadora** — Facultad de Ingeniería, UNAM (semestre 2027-1).
> Equipo 15 · **Nierika Studio** · Entrega: **18 de noviembre de 2026**.

## Si es la primera vez que abres este repositorio

| Quiero… | Ve a… |
|---|---|
| Ejecutar el juego | [Compilar y ejecutar](#compilar-y-ejecutar) |
| Saber cómo se maneja | [Controles](#controles) |
| Leer los manuales entregados (PDF) | [`docs/entrega/`](docs/entrega/) |
| Ver los guiones de los 3 videos | [`docs/videos/`](docs/videos/README.md) |
| Saber qué está terminado y qué no | [Estado de la entrega](#estado-de-la-entrega-18-de-noviembre-de-2026) |
| Ver créditos, licencias y referencias | [Licencia y créditos](#licencia-y-créditos) |

## Qué incluye

- **Una escena continua con tres zonas:** playa y orilla del mar, panteón con tumbas y barda de piedra, y capilla con altar.
- **Navegación en primera persona** (teclado y ratón) con colisiones AABB, menú de inicio, menú de pausa y ayuda en pantalla (**F1**).
- **Recorrido guiado** (tecla **T**): 12 estaciones con subtítulos y voz (`Audio/narracion/`); cualquier tecla de movimiento lo cancela.
  El guion está en [`docs/RECORRIDO_GUIADO.md`](docs/RECORRIDO_GUIADO.md).
- **Iluminación** cálida con luces puntuales y direccionales (modelo Blinn-Phong); las velas del altar se encienden y apagan con **V**.
- **10 animaciones** (5 por cuadros clave y 5 simples; detalle y justificación matemática en
  [`docs/manual_tecnico/09_i_animaciones.md`](docs/manual_tecnico/09_i_animaciones.md)):
  - Por cuadros clave: caminata de la silueta, «dejar flores» en el altar, baile familiar, juego con el perro y sahumerio con copal.
  - Simples: olas del mar, velas flotantes con llama, flores flotantes, ritmo de la banda y la anfitriona.
  - **Aclaración honesta:** la propuesta aprobada pedía 5 animaciones (2 por cuadros clave y 3 simples); el resto se agregó después
    (decisiones registradas en `docs/ESTADO_PROYECTO.md`, sección 4).
- **13 fichas culturales** (Dear ImGui) que aparecen solas a menos de 2 m de un objeto, con su nivel de evidencia y sus referencias en APA 7
  (fuentes en [`docs/fichas_culturales.md`](docs/fichas_culturales.md)). Lo que es adaptación del estudio lo dice la propia ficha.
- **Audio:** oleaje y música de banda por zona, más la narración del recorrido. **M** cambia la mezcla de sonido.

## Controles

| Tecla | Acción |
|---|---|
| **W A S D** o flechas + ratón | Caminar y mirar alrededor |
| **T** | Recorrido guiado (**T**, **Esc** o **W A S D** lo cancelan) |
| **V** | Encender/apagar las velas del altar (a menos de 3 m de él) |
| **M** | Sonido: mar y música / solo mar / solo música / silencio |
| **R** | Reiniciar la silueta |
| **F1** | Mostrar/ocultar la ayuda |
| **Esc** | Menú de pausa |

Más detalle: [`docs/manual_usuario/`](docs/manual_usuario/README.md).

## Compilar y ejecutar

### Requisitos

- Windows 10/11 de 64 bits y una tarjeta gráfica con **OpenGL 3.3** (basta una integrada).
- **Visual Studio 2022** con la carga de trabajo *Desarrollo de escritorio con C++*.
- **Git**, y conexión a internet la primera vez (vcpkg descarga las dependencias).

### Pasos

1. **Clona el repositorio** y entra a la rama `develop` (la versión completa; ver [Estado](#estado-de-la-entrega-18-de-noviembre-de-2026)).
2. **Instala vcpkg una sola vez** (así lo usa el equipo):
   ```
   git clone https://github.com/microsoft/vcpkg C:\vcpkg
   C:\vcpkg\bootstrap-vcpkg.bat
   C:\vcpkg\vcpkg integrate install
   ```
   Si ya tenías vcpkg, actualízalo con `git -C C:\vcpkg pull`: `vcpkg.json` fija las versiones con un `builtin-baseline`
   y ese *commit* debe existir en tu copia.
3. **Abre `NierikaStudio.sln`**, elige **Release | x64** (o Debug | x64) y presiona **F5**.
   - La primera compilación descarga y compila Assimp, GLFW, GLEW y GLM (unos 10–20 minutos); las siguientes son rápidas.
   - F5 ya fija el directorio de trabajo en la raíz del repositorio, así que las rutas relativas a `Models/`, `Shaders/` y `Audio/` funcionan sin configurar nada.
4. Desde terminal (Developer PowerShell) también se puede compilar con
   `msbuild NierikaStudio.sln /p:Configuration=Release /p:Platform=x64`; el ejecutable queda en `build/x64/<Configuración>/`.

**Si algo falla:** consulta [`docs/manual_usuario/05_problemas_frecuentes.md`](docs/manual_usuario/05_problemas_frecuentes.md).
Un error del *baseline* de vcpkg casi siempre se arregla con el `git pull` del paso 2.

## Rendimiento

Medido por D2 en un procesador Intel Core i5 de 11.ª generación con **gráficos integrados**, a 1280×720: **mínimo 45 FPS y promedio
cercano a 60 FPS** (antes de la optimización por *frustum culling* eran unos 15 FPS). El requisito era ≥ 30 FPS.
**Límite:** se midió en una sola máquina. Método y datos: [`docs/manual_tecnico/15_anexo_pruebas.md`](docs/manual_tecnico/15_anexo_pruebas.md).

## Tecnologías

| Área | Herramientas |
|---|---|
| Lenguaje y API gráfica | C++17 · OpenGL 3.3 Core · GLSL 330 (sin OpenGL legacy) |
| Bibliotecas | GLFW · GLEW · GLM · Assimp · SOIL2 / stb_image · Dear ImGui · miniaudio |
| Entorno | Visual Studio 2022 (solo x64) · vcpkg |
| Contenido | Blender · GIMP · texturas CC0 (Poly Haven, ambientCG) |
| Gestión | Git / GitHub (ramas `feature/` + pull requests) · tablero Kanban |

## Estructura del repositorio

**Aplicación**

```
NierikaStudio.sln / .vcxproj   Solución de Visual Studio 2022 (solo x64)
vcpkg.json                     Dependencias que instala vcpkg (assimp, glfw3, glew, glm), fijadas con builtin-baseline
src/                           Código C++ (main.cpp y un módulo por función: Camera, Shader, Model, Mesh, Fichas, Audio, Menu, Recorrido, ...)
vendor/                        SOIL2 + stb_image, Dear ImGui y miniaudio, compilados con el proyecto
Models/<objeto>/               Modelo 3D: <objeto>.obj + .mtl + sus texturas (y el .blend de los modelos propios)
Shaders/                       Shaders GLSL 330 core (.vs / .frag)
Audio/                         Pistas de mar y música (Audio/README.md) y los 12 archivos de narración (Audio/narracion/)
tools/                         Scripts auxiliares (generadores Blender/bpy, pruebas, empaquetado); ver tools/README.md
LICENSE                        Licencia MIT del código
```

**Documentación** (en español; empieza por [`docs/README.md`](docs/README.md), que indexa todo)

```
docs/
  README.md                         Índice de documentos y su estado
  ESTADO_PROYECTO.md                Dónde vamos: plan con códigos de tarea, avance, decisiones, parámetros y problemas abiertos
  CONTEXTO_PROYECTO.md              Qué se prometió: alcance, criterios de aceptación, stack y convenciones
  PLAN_ACTIVIDADES.md               Las 60 actividades originales (solo referencia)
  FLUJO_DE_TRABAJO.md               Cómo trabaja el equipo: mapa del repositorio, ciclo de tareas y reglas de Git
  REFERENCIAS.md                    Referencias APA 7 de todo el proyecto
  fichas_culturales.md              Texto y fuentes de las fichas culturales
  RECORRIDO_GUIADO.md               Guion del recorrido guiado
  entrega/                          Entrega: los dos manuales en PDF, notas de versión y lista de verificación
  manual_tecnico/, manual_usuario/  Fuente (Markdown) de los dos manuales
  videos/                           Guiones y notas de los 3 videos de demostración (los MP4 se entregan aparte, no están en Git)
  propuesta/                        Propuesta y plan entregados (PDF)
  archivo/                          Material reemplazado, conservado por historia
CLAUDE.md                           Reglas del proyecto para el asistente de IA (Claude Code)
.claude/                            Configuración del asistente de IA (ajustes, comandos y agentes)
```

## Estado de la entrega (18 de noviembre de 2026)

Lo verificado en el repositorio se distingue de lo que solo declara el equipo.

| Elemento | Dónde | Estado |
|---|---|---|
| Código y proyecto de Visual Studio | este repositorio, rama `develop` | Compila y corre en Visual Studio 2022 en la PC de D2 (**lo declara D2**; no se pudo comprobar desde la nube) |
| Manual de usuario y manual técnico (PDF) | [`docs/entrega/`](docs/entrega/) | Generados el 18/11 desde las fuentes Markdown; al manual técnico aún le falta el texto individual de D1 (secciones g y j) |
| 3 videos de demostración (MP4) | fuera de Git (≈ 450 MB) | Grabados el 18/11 según D2; se entregan aparte ([`docs/videos/README.md`](docs/videos/README.md)) |
| Rendimiento | [`15_anexo_pruebas.md`](docs/manual_tecnico/15_anexo_pruebas.md) | 45 FPS mínimo y ≈ 60 de promedio en una sola máquina con gráficos integrados |
| Release `.zip` autónomo | `tools/empaquetar_release.ps1` | **Aún no construido ni publicado**; no se probó en una PC sin Visual Studio |
| Rama `main` | — | **Todavía no contiene la versión final** (sigue en la estructura inicial); todo el trabajo está en `develop` |
| Participación de D1 | [`05_e_roles_raci.md`](docs/manual_tecnico/05_e_roles_raci.md) | **Sin evidencia en el repositorio** (ningún commit ni pull request a su nombre); sus textos individuales están pendientes y nadie más los escribe |

Los pendientes y las decisiones se llevan en [`docs/ESTADO_PROYECTO.md`](docs/ESTADO_PROYECTO.md) (secciones 4, 6 y 7).

## Licencia y créditos

- El **código fuente** (`src/`, `Shaders/`, `tools/`) se publica bajo la **licencia MIT** (ver [`LICENSE`](LICENSE)); D2 la confirmó el 18/11.
- **Los modelos, texturas y audio no están cubiertos por esa licencia.** Las pistas de mar y música son grabaciones de terceros usadas con
  fines educativos no comerciales, sin licencia libre confirmada; los 12 archivos de narración se sintetizaron con ElevenLabs (voz y términos en
  [`docs/manual_tecnico/13_m_creditos_licencias.md`](docs/manual_tecnico/13_m_creditos_licencias.md)); algunas texturas son CC0 (Poly Haven).
  No los redistribuyas con fines comerciales.
- Créditos y referencias APA 7: [`docs/REFERENCIAS.md`](docs/REFERENCIAS.md), [`Audio/README.md`](Audio/README.md) y la pantalla de Créditos dentro del juego.
  Algunas referencias de audio aún están pendientes de completar; el inventario marca cuáles.
- Las bibliotecas de `vendor/` y las que instala vcpkg conservan sus propias licencias.

## Ramas

- `main` — solo versiones probadas y entregables.
- `develop` — integración diaria.
- `feature/<código>-<nombre>` (p. ej. `feature/a-10-dovelas`) — una rama por tarea; entra a `develop` por pull request.

## Equipo — Nierika Studio (Equipo 15)

| Integrante | Número de cuenta | Cuenta de GitHub |
|---|---|---|
| D1 | 319266894 | `thecurlyguy` — **sin confirmar**: D2 indica que es la cuenta de D1, pero no hay commits ni pull requests suyos que lo respalden |
| D2 | 321332086 | `hugo12341234` |
