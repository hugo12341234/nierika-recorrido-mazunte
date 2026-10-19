# ESTADO DEL PROYECTO — documento vivo

> **Este es el archivo que evita que Claude "olvide".** Se actualiza al cerrar cada sesión
> (`CIERRE` en el Proyecto o `/cierre` en Claude Code) y reemplaza la versión anterior en el
> Proyecto de Claude y en `docs/ESTADO_PROYECTO.md` del repositorio.

**Última actualización:** 18/10/2026 · **Hoy:** fin de semana 3 · **Días a la entrega:** 31 (18/11/2026)
**Integrantes:** D1 = 319266894 · D2 = 321332086
**Repositorio:** https://github.com/hugo12341234/nierika-recorrido-mazunte (ramas `main` y `develop`) · **Hoja del plan:** https://docs.google.com/spreadsheets/d/13i1yXf3dMuiBWyxka-R17Zw0EG2F-fMPPWCSC70_qXA/edit?usp=sharing

**Forma de trabajo acordada (18/10):** las columnas Resp./Verif. de la propuesta ya **no se siguen**.
Quien esté libre toma la siguiente tarea del *Plan vigente* (sección 2); **el otro revisa** antes de
integrarla a `develop`. Los números de actividad (1–60) se siguen usando solo como referencia.

---

## 1. Lo que ya existe (revisado el 18/10/2026)

**Código** (base: proyecto de la Práctica 6 del laboratorio, `319266894_REPORTE6_GPO8`):
- `main.cpp`: ventana GLFW 800×600 no redimensionable, OpenGL 3.3 Core, **GLEW** (no GLAD), **SOIL2 + stb_image**.
- Clases del curso: `Shader`, `Camera` (WASD/flechas + ratón), `Model` (Assimp).
- Ciclo con `deltaTime`. Shader `Shaders/modelLoading.vs/.frag`: **solo textura, sin iluminación**.
- Carga y dibuja 3 modelos: fachada, "altar" y calavera.

**Modelos** (`Models/`):

| Modelo | Origen | Geometría | Texturas | Observaciones |
|---|---|---|---|---|
| `fachada/fachada.obj` | Blender 4.4.3 (propio) | 204 caras; 3.2 × 2.5 × 2.6 u; muros con **apertura de arco de medio punto** + **techo a dos aguas** | `concreto.jpg`, `laminado1.jpg` (512×512) | Arco sin dovelas visibles. El material `Material` no tiene textura. |
| `altar/altar.obj` | Exportado desde Maya; estructura de texturas tipo herramienta externa (**confirmar origen**) | 2 148 caras; 1.3 × 1.9 × 0.9 u; **es una capilla/tumba con domo y cruz, no un altar de 3 niveles** | 2048×2048 (diffuse, normal, roughness, metallic, pbr, shaded) | **Bug:** el .obj dice `mtllib base.mtl` pero el archivo es `altar.mtl` → no carga su material. `altar.mtl` usa `shaded.png` (con luz "horneada") en vez de `texture_diffuse.png`. |
| `calavera/base.obj` | Igual que el anterior (**confirmar origen**) | 13 518 caras; 1.5 × 1.9 × 1.7 u | 2048×2048 | Calavera de azúcar; **no está en la lista de objetos de la propuesta**. |

**Transformaciones actuales en `main.cpp`** (rompen la regla 1 u = 1 m):
fachada esc. 3.0 → ~9.6 m de ancho y 7.6 m de alto · "altar" esc. 3.5 → **6.6 m de alto** · calavera esc. 0.4 en y = 2.1 m.

## 2. Plan vigente (replaneación del 18/10 — sustituye las fechas de la propuesta)

> Orden pensado para que **siempre haya algo funcionando** y para cubrir primero lo que se evalúa.
> Los números entre paréntesis son las actividades originales.

| Bloque | Fechas | Objetivo del bloque (hito) | Tareas |
|---|---|---|---|
| **A** | 19–25 oct | **Escena base correcta y navegable con luz** | Corregir escalas a 1 u = 1 m · arreglar `mtllib` del altar · ventana 1280×720 · *far plane* 300 · playa (8) + textura arena 1K (13) · mar como malla subdividida (10) · shader Blinn-Phong: ambiental cálida + 3 luces (37) · colisiones AABB suelo/muros (12) · arco con dovelas (17) · piso interior (18) · texturas de piedra rústica 1K (21) |
| **B** | 26 oct–1 nov | **Panteón + altar con ofrenda** | Altar de 3 niveles **propio** (24) · vela con pivote en la base (23) · cempasúchil de planos cruzados (28) · pan de muerto (27) · botella (26) · marco con foto (29) · papel picado con alfa (25, 33) + soportes (22) · tumbas propias (14) · barda (20) · cruz (19) · texturizar (31–34) · colocar y ajustar (36, 38) |
| **C** | 2–8 nov | **3 animaciones simples + fichas** | Olas en vertex shader (39) · llama con ruido (40) · velas flotando Bézier + seno (41) · flores flotando (42) · descarte por alfa (44) · Dear ImGui con 5 fichas por proximidad (51) · justificación matemática de las simples (45) |
| **D** | 9–15 nov | **2 animaciones KeyFrames + audio** | Silueta low-poly jerárquica (46) · caminata con slerp + smoothstep (47, 48) · dejar flores con Bézier cuadrática (49, 50) · audio miniaudio (52) · justificación matemática de las complejas (53) |
| **E** | 16–18 nov | **Entrega** | Pruebas y fugas (54, 55) · Release x64 en `dist/` (56) · 3 videos (57) · manuales ES/EN (58, 59) · Release GitHub (60) |

**Para no ahogarse al final:** cada bloque deja escritas sus secciones del manual técnico (capturas + parámetros) en `docs/manual_tecnico/`.

**Qué recortar primero si el tiempo no alcanza** (en este orden): copalero (30) → detalle del marco (29) → variantes de rocas en la barda (20) → pausas en la velocidad de las flores (42). **No se recorta:** colisiones, iluminación, 5 fichas, 2 KeyFrames, 3 simples, ≥ 18 modelos propios, 30 FPS, manuales y videos.

## 3. Estado de actividades

Leyenda: ✅ Hecho · 🔄 En proceso / parcial · 👀 En revisión · ⏳ Pendiente · ❓ Sin confirmar · ✂️ Recortado

| N.º | Actividad (corta) | Estado | Notas / evidencia |
|---|---|---|---|
| 1 | Fuentes Mazunte (APA 7) | ✅ | Hoja del plan |
| 2 | Propuesta técnica | ✅ | Propuesta v2.0 (04/10/2026) |
| 3 | Repo + .gitignore | ✅ | 18/10: `hugo12341234/nierika-recorrido-mazunte` (privado), ramas `main` y `develop`, README, .gitignore VS corregido, D1 invitado como colaborador (pendiente de aceptar) |
| 4 | VS + dependencias | ✅ | Base de la Práctica 6 (GLEW, SOIL2, Assimp) |
| 5 | Trello | ❓ | |
| 6 | Blender y GIMP | 🔄 | Blender 4.4.3 en al menos un equipo; verificar versión en ambos |
| 7 | Bocetos del cuarto | ❓ | |
| 8 | Playa 100×50 | ⏳ | Bloque A |
| 9 | Clase Camera | ✅ | Clase del curso; verificar límite de pitch ±89° |
| 10 | Mar 100×100 | ⏳ | Bloque A |
| 11 | Callbacks WASD/ratón | ✅ | Del curso |
| 12 | Colisiones AABB | ⏳ | Bloque A |
| 13 | Textura arena | ⏳ | Bloque A |
| 14 | Tumbas | 🔄 | El modelo "altar" actual sirve como tumba/capilla (si es externo, solo secundario y con crédito) |
| 15 | Ciclo deltaTime + render | 🔄 | Ciclo ✅; falta la playa |
| 16 | Fachada con apertura | 🔄 | `fachada.obj`; falta escala real y textura de piedra |
| 17 | Arco con dovelas | 🔄 | Arco existe; faltan dovelas visibles (criterio "obligatorio") |
| 18 | Piso, paredes, techo | 🔄 | Paredes y techo (a dos aguas) existen; confirmar piso interior |
| 19 | Cruz | ⏳ | |
| 20 | Barda perimetral | ⏳ | |
| 21 | Textura piedra rústica | 🔄 | Hoy: concreto/laminado 512 px → cambiar a piedra 1K |
| 22 | Soportes papel picado | ⏳ | |
| 23–60 | — | ⏳ | Ver plan vigente (sección 2) |

## 4. Registro de decisiones

| Fecha | Decisión | Motivo | Afecta a |
|---|---|---|---|
| 18/10 | Se trabaja por disponibilidad, no por las columnas Resp./Verif. | Falta de tiempo | Plan, RACI del manual técnico |
| 18/10 | Se usa la base de la Práctica 6: **GLEW en lugar de GLAD**; **SOIL2** además de stb_image | Ya compila y el equipo la domina | Stack, manual técnico (actualizar tabla de herramientas) |
| 18/10 | Rutas: `Models/<objeto>/<objeto>.obj`, `Shaders/*.vs` / `*.frag` | Estructura heredada de la práctica | Convenciones |
| 18/10 | `.gitignore` de VS con excepción `!**/Models/**/*.obj` y `.mtl` | La plantilla ignora todos los `.obj` y se perderían los modelos 3D | Repositorio |
| 18/10 | README del repositorio en inglés con resumen en español | Portafolio | Repositorio |
| 18/10 | Rama por defecto del repo: `develop` (`main` solo para entregas). Los hilos del Proyecto de Claude leen `docs/` directo del repo | Que Claude siempre vea el estado actualizado | Flujo de trabajo |
| 18/10 | Propuesta v2.0 y plan de trabajo en PDF guardados en `docs/propuesta/` | Consulta desde cualquier hilo | Documentación |
| *pendiente* | ¿Techo a dos aguas (como está modelado) o plano (como dice la propuesta)? | | Act. 18 |
| *pendiente* | ¿Se agrega la calavera de azúcar a la ofrenda? (no está en la propuesta) | | Alcance, fichas |
| *pendiente* | Origen del "altar" y la calavera: si son externos, se documentan con crédito y no cuentan para los 18 propios | Criterio de modelado de la propuesta | Act. 14 y 24 |

## 5. Parámetros fijados

| Elemento | Parámetro | Valor | Dónde vive |
|---|---|---|---|
| Ventana | tamaño | 800×600 → **cambiar a 1280×720** | `main.cpp` |
| Proyección | near / far | 0.1 / 100 → **cambiar far a 300** | `main.cpp` |
| Cámara | posición inicial | (0, 2, 6) | `main.cpp` |
| Fachada | trans / rot Y / esc | (0,0,0) / 180° / 3.0 → **revisar** | `main.cpp` |
| "Altar" (tumba) | trans / esc | (0,0,2) / 3.5 → **revisar** | `main.cpp` |
| Calavera | trans / esc | (0,2.1,1.8) / 0.4 → **revisar** | `main.cpp` |
| Fichas | radio de proximidad | 2 m | — |

## 6. Problemas abiertos

1. `altar.obj` línea 3: `mtllib base.mtl` → debe decir `mtllib altar.mtl` (si no, el modelo sale sin textura).
2. `altar.mtl`: `map_Kd shaded.png` → usar `texture_diffuse.png`; `Kd 0 0 0` → `Kd 1 1 1`.
3. Texturas del altar y la calavera de 2048 px (la propuesta pide 1K): reducir a 1024 en GIMP.
4. Escalas fuera de 1 u = 1 m (ver sección 5).
5. Falta iluminación: el shader actual solo muestra la textura.
6. No se ha revisado aún el resto del código (`Camera.h`, `Shader.h`, `Model.h`, shaders): conectar la carpeta completa de la solución.

## 7. Siguiente paso concreto

Bloque A, en este orden: (1) corregir problemas 1–2 y las escalas; (2) ventana 1280×720 y far 300;
(3) playa + mar; (4) shader de iluminación; (5) colisiones.
