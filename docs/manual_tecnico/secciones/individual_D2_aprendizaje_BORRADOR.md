# Aprendizaje autónomo — D2 (321332086) · **BORRADOR PARA REVISIÓN DE HUGO**

> **No es texto final.** Lo armó un asistente a partir de lo que el repositorio muestra; **no sabe** qué aprendiste tú ni cuánto. Hugo debe: (1) elegir **una** herramienta que de verdad aprendió, (2) corregir cada frase marcada con ❓, (3) borrar lo que no sea cierto, (4) escribirlo con sus palabras y firmarlo. Nada de esto es evidencia por sí mismo: la evidencia son los archivos citados.

## 0. Candidatas que el repositorio respalda (elige una)

| Herramienta | Qué muestra el repo | Qué solo tú puedes confirmar |
|---|---|---|
| **Blender 5.2 (Python `bpy`)** | Playa y mar modelados en 5.2.2 (A-5, A-7); fachada reexportada desde 5.2.2; scripts de modelado en `tools/` (`b3_…`, `b4_…`, `b9_…`, `x7_silueta.py`…); decisión del 11/11 de usar `bpy` como módulo (ESTADO §4); `docs/manual_tecnico/evidencias/` con capturas | ❓ Si aprendiste a modelar tú, o si los scripts los generó Claude y tú los revisaste. El manual ya lo admite: «trabajo del equipo con ayuda de Claude» (`03_herramientas.md` §3.1) |
| **OpenGL 3.3 Core + GLSL** | `Shaders/lighting.frag`, `sky.frag`, olas con suma de senos, llama con ruido, KeyFrames con slerp (`docs/manual_tecnico/07_…`); `Shaders/` completos | ❓ Qué parte entendiste y puedes explicar; qué parte escribió Claude. La iluminación base es de D1 |
| **vcpkg + Visual Studio 2022 x64** | `vcpkg.json` con `builtin-baseline` (decisión 19/10); instalación en `C:\vcpkg`; `NierikaStudio.vcxproj`; resolución de errores de *baseline* (README) | ❓ Cuántos intentos/errores tuviste y cómo los resolviste |
| **miniaudio** | `src/Audio.cpp`, `Audio/README.md`, música con atenuación por distancia (D-6, D-6m) | ❓ Si lo probaste tú oyéndolo; el ESTADO dice «audio escuchado por Hugo» |
| **ElevenLabs (voz de la narración)** | 12 mp3 en `Audio/narracion/`, voz «El Faraón», Eleven v4 (REFERENCIAS.md; confirmado por D2 el 18/11) | ❓ Cómo generaste cada audio, ajustes, costo |
| **Git/GitHub con ramas y PR** | ~150 PR; reglas en `docs/FLUJO_DE_TRABAJO.md` | ❓ Si ya lo conocías antes |
| **Claude Code / Proyecto de Claude como herramienta de trabajo** | Decisiones del ESTADO, CLAUDE.md, subagentes `verificador`, `revisor-opengl` | ❓ Cómo aprendiste a dirigirlo y a verificar su trabajo (es una competencia real, y declararla es honesto) |

## 1. Herramienta o tecnología nueva (borrador)

❓ **[Elegida por Hugo]**. Ejemplo si eliges Blender 5.2 con Python: «Aprendí a modelar y exportar `.obj` con Blender 5.2 y a automatizar el modelado con scripts de Python (`bpy`) para generar de forma reproducible los 18+ modelos propios del proyecto.»

## 2. Para qué la necesité (verificable en el repo)

- Modelos propios de la ofrenda, tumbas, barda, silueta, personas, etc. (inventario del ESTADO §2).
- Todo `.obj` con escala real 1 u = 1 m, pivote correcto y texturas 1K (CLAUDE.md §4).
- ❓ [Hugo: añade lo que tú hiciste a mano y qué partes dirigiste con prompts.]

## 3. Cómo la aprendí

❓ **Solo Hugo puede escribir esta sección.** El repo no registra tutoriales, horas ni cursos. Sí muestra: uso de Blender 5.2 en español desde 12/10 (capturas `ref_12-10_…` del 12–13/10), guías paso a paso preparadas para ti en `flujo/` (A-5 y A-6, con menús y valores) y errores corregidos en PR sucesivos. ❓ ¿Qué tutoriales, documentación o videos viste? ¿Cuántas horas?

## 4. Evidencia

| Evidencia | Ruta |
|---|---|
| Capturas de Blender 5.2 | `docs/manual_tecnico/evidencias/`, `plan/evidencias_2026-11-04/ref_12-10_…`, `X-1_blender_522_ui.png` (carpeta de proyecto) |
| Scripts de modelado | `tools/b3_cempasuchil.py`, `b4_pan_muerto.py`, `b9_crear_tumbas.py`, `x7_silueta.py`… |
| Modelos exportados | `Models/<objeto>/<objeto>.obj` |
| PR y commits propios | historial en GitHub (≈460 commits de `Hugo Chacón`) |

## 5. Dificultades (hechos del repo; ❓ completa con tu vivencia)

- Escalas y pivotes: capilla rehecha y escalada varias veces (A-2, X-44, X-46).
- Errores de iluminación y transparencias (alfa) corregidos en A-8.
- Rendimiento: 15 FPS en gráficos integrados que subieron a ≥45 FPS tras el descarte por frustum (#148, medido por Hugo el 18/11).
- ❓ Qué te costó entender y cómo lo resolviste.

## 6. Declaración (firmar solo cuando sea cierto)

Declaro que este texto lo revisé y reescribí yo y que describe mi trabajo real.

Fecha: [dd/mm/2026]  ·  Cuenta: 321332086
