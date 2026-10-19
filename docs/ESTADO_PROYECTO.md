# ESTADO DEL PROYECTO — documento vivo

> **Qué es este archivo:** la memoria del proyecto. Dice **dónde vamos**: qué está hecho, qué sigue,
> qué decidimos y con qué valores. Lo que no esté aquí, Claude no lo sabrá en la siguiente sesión.
> **Quién lo actualiza:** Claude Code con `/cierre` al terminar cada sesión (se revisa antes de hacer commit).
> **Dónde vale:** la versión de la rama `develop`.

**Última actualización:** 19/10/2026 · **Hoy:** inicio de la semana 4 (Bloque A) · **Días a la entrega:** 30 (mié 18/11/2026)
**Integrantes:** D1 = 319266894 · D2 = 321332086
**Repositorio:** https://github.com/hugo12341234/nierika-recorrido-mazunte (**público**; rama por defecto `develop`) · **Hoja del plan:** https://docs.google.com/spreadsheets/d/13i1yXf3dMuiBWyxka-R17Zw0EG2F-fMPPWCSC70_qXA/edit?usp=sharing · **Tablero Trello:** https://trello.com/b/8FNkEY97/nierika-studio-recorrido-mazunte

**Forma de trabajo acordada (18/10):** las columnas Resp./Verif. de la propuesta ya **no se siguen**.
Quien esté libre toma la siguiente tarea del *Plan vigente* (sección 2); **el otro revisa** antes de
integrarla a `develop`.

**Contenido:** 1. Lo que ya existe · 2. Plan vigente (tareas con código) · 3. Estado por actividad (1–60) ·
4. Registro de decisiones · 5. Parámetros fijados · 6. Problemas abiertos · 7. Siguiente paso

---

## 1. Lo que ya existe (revisado el 19/10/2026)

**Código** (base: proyecto de la Práctica 6 del laboratorio, `319266894_REPORTE6_GPO8`; act. 4, PR #1):
- `src/main.cpp`: ventana GLFW 800×600 no redimensionable, OpenGL 3.3 Core, **GLEW** (no GLAD), **SOIL2 + stb_image**, cursor capturado.
- Clases del curso en `src/`: `Shader.h`, `Camera.h` (WASD/flechas + ratón, pitch ±89°), `Model.h` + `Mesh.h` (Assimp, texturas RGBA).
- Ciclo con `deltaTime`. Shader `Shaders/modelLoading.vs/.frag`: **solo textura, sin iluminación** (descarta píxeles con alfa < 0.1).
- Carga y dibuja 3 modelos: fachada, "altar" y calavera.
- Dependencias: Assimp, GLFW, GLEW y GLM por **vcpkg** (`vcpkg.json`); SOIL2/stb en `vendor/SOIL2`.

**Modelos** (`Models/`):

| Modelo | Origen | Geometría | Texturas | Observaciones |
|---|---|---|---|---|
| `fachada/fachada.obj` | Blender 4.4.3 (propio) | 204 caras; 3.2 × 2.5 × 2.6 u; muros con **apertura de arco de medio punto** + **techo a dos aguas** (decisión 19/10: se queda así) | `concreto.jpg`, `laminado1.jpg` (512×512) | Arco sin dovelas visibles. El material `Material` no tiene textura. |
| `altar/altar.obj` | **Propio** (confirmado de palabra por D2 el 19/10); exportado desde Maya. **Se retira**: se rehace en Blender como una de las tumbas de B-9 (decisión 19/10, solo Blender) | 2 148 caras; 1.3 × 1.9 × 0.9 u; **es una capilla/tumba con domo y cruz, no un altar de 3 niveles** | `texture_diffuse.png`, `texture_normal.png`, `shaded.png` (2048×2048) | El .obj dice `mtllib base.mtl` pero el archivo es `altar.mtl`; `altar.mtl` usa `shaded.png` (con luz "horneada") y `Kd 0 0 0` → tarea **A-1**. |
| `calavera/base.obj` | **Propio**, exportado desde Maya. **Se retira de la escena** en A-1 (decisión 19/10: no está en la propuesta) | 13 518 caras; 1.5 × 1.9 × 1.7 u | `texture_diffuse.png`, `texture_normal.png` (2048×2048) | Calavera de azúcar; no está en la propuesta: se retira (decisión del 19/10). |

**Transformaciones actuales en `main.cpp`** (rompen la regla 1 u = 1 m → tarea **A-2**):
fachada esc. 3.0 → ~9.6 m de ancho y 7.6 m de alto · "altar" esc. 3.5 → **6.6 m de alto** · calavera esc. 0.4 en y = 2.1 m.

---

## 2. Plan vigente (replaneación del 18/10 — sustituye las fechas de la propuesta)

> Orden pensado para que **siempre haya algo funcionando** y para cubrir primero lo que se evalúa.
> **Cada tarea tiene un código** (`A-1`, `B-3`…): letra del bloque + número. Ese código se usa en Trello,
> en el nombre de la rama, en el PR y con `/actividad`. La columna **Act.** dice qué actividades del
> plan original (1–60) cubre; "—" = corrección de la base que no estaba en el plan.
> El guion evita confundir la tarea `D-1` (Bloque D) con el integrante **D1**.

**Leyenda de estado:** ✅ Hecho · 🔄 En proceso / parcial · 👀 En revisión (PR abierto, solo mientras se espera la revisión) · ⏳ Pendiente · ⚠️ Atrasado · ❓ Sin confirmar · ✂️ Recortado

### Bloque A · 19–25 oct · Escena base correcta y navegable con luz

| Código | Tarea | Act. | Criterio de terminado | Estado |
|---|---|---|---|---|
| A-1 | Quitar la calavera de la escena y del repo; dejar la capilla de Maya solo como **provisional** hasta B-9 (corregir `mtllib`, `map_Kd`, `Kd`) | — | La calavera ya no se carga y `Models/calavera/` se borra; la capilla carga `altar.mtl` con `texture_diffuse.png`; sin errores en consola. No se reducen sus texturas, porque B-9 la reemplaza | ⏳ |
| A-2 | Escalas reales 1 u = 1 m (fachada y capilla provisional) | — | Medidas en metros registradas en la sección 5; escalas en `main.cpp` cercanas a 1 | ⏳ |
| A-3 | Ventana 1280×720 | — | La ventana abre a 1280×720 | ⏳ |
| A-4 | *Far plane* a 300 m | — | Nada visible se recorta a menos de 300 m | ⏳ |
| A-5 | Playa: plano 100×50 m subdividido | 8, 15 | Escala 1 u = 1 m; se dibuja en el ciclo de render | ⏳ |
| A-6 | Textura de arena 1K con mapeo UV | 13 | Textura 1K sin estiramientos visibles | ⏳ |
| A-7 | Mar: malla 100×100 m subdividida | 10 | Malla preparada para el vertex shader de olas | ⏳ |
| A-8 | Shader Blinn-Phong: luz ambiental cálida + 3 luces clave | 37 | El interior del cuarto se distingue sin perder el ambiente nocturno; sin luces puntuales extra | ⏳ |
| A-9 | Colisiones AABB con suelo y muros | 12 | La cámara no atraviesa el suelo ni los muros | ⏳ |
| A-10 | Arco de medio punto con dovelas visibles | 17 | Dovelas visibles (detalle obligatorio) | ⏳ |
| A-11 | Piso interior del cuarto y techo a dos aguas en su propio `.obj` | 18 | Piso, paredes y techo sin detalles innecesarios; techo separado de `fachada.obj` (decisión del techo: a dos aguas, 19/10) | ⏳ |
| A-12 | Texturas de piedra rústica 1K en muros y fachada | 16, 21 | Piedra rústica coherente con materiales reales, 1K | ⏳ |

### Bloque B · 26 oct–1 nov · Panteón + altar con ofrenda

| Código | Tarea | Act. | Criterio de terminado | Estado |
|---|---|---|---|---|
| B-1 | Altar de 3 niveles escalonados (**propio**) | 24 | Cabe dentro del cuarto | ⏳ |
| B-2 | Vela cilíndrica con pivote en la base | 23 | Pivote correcto para animación | ⏳ |
| B-3 | Flor de cempasúchil con planos cruzados | 28 | Optimizada para instancing | ⏳ |
| B-4 | Pan de muerto con detalles de huesos | 27 | Decimate activo (pocos polígonos) | ⏳ |
| B-5 | Botella de vidrio (modificador Screw) | 26 | Exportada aplicando modificadores | ⏳ |
| B-6 | Marco de madera con plano para la foto | 29 | UV exactas para la foto | ⏳ |
| B-7 | Papel picado: malla plana con UV y textura con alfa | 25, 33 | Subdividida; descarte por alfa funcionando | ⏳ |
| B-8 | Soportes de madera para el papel picado | 22 | Postes simples | ⏳ |
| B-9 | Tumbas propias: **4** con geometría distinta, en Blender; una es la capilla con domo y cruz (rehecha usando la de Maya como referencia) | 14 | Lápida y base; 4 tumbas distintas, cada una en su `.obj` (inventario #7–#10); la capilla de Maya se retira de `Models/altar/` | ⏳ |
| B-10 | Barda perimetral del panteón | 20 | Muros bajos; variantes de rocas; **una roca en su propio `.obj`** (inventario #12, decisión del 19/10) | ⏳ |
| B-11 | Cruz de piedra de la entrada | 19 | Una sola, sin variantes | ⏳ |
| B-12 | Texturizar altar, vela, botella, pan, cempasúchil y marco (PBR 1K) | 31, 32, 33, 34 | Materiales coherentes; mapa de relieve para el pan | ⏳ |
| B-13 | Cargar todos los .obj, colocar y ajustar la ofrenda en el cuarto | 35, 36, 38 | Rutas relativas; los 7 objetos caben y se ven | ⏳ |
| B-14 | Copalero de barro (sin humo) — **obligatorio para el conteo de modelos** | 30 | Hueco interno; sin partículas | ⏳ |

### Bloque C · 2–8 nov · 3 animaciones simples + fichas

| Código | Tarea | Act. | Criterio de terminado | Estado |
|---|---|---|---|---|
| C-1 | Olas: vertex shader con suma de senos | 39 | Frecuencias y amplitudes definidas | ⏳ |
| C-2 | Llama de las velas: fragment shader con ruido | 40 | Ruido Perlin o similar | ⏳ |
| C-3 | Velas flotando: trayectoria Bézier + oscilación senoidal | 41 | Desfase entre velas | ⏳ |
| C-4 | Flores flotando: movimiento circular + rotación | 42 | Velocidad variable con pausas | ⏳ |
| C-5 | Integrar las 3 animaciones simples en el ciclo de render | 43 | Olas, velas y flores a la vez, ≥ 30 FPS | ⏳ |
| C-6 | Descarte por alfa en flores y velas | 44 | Sin artefactos de transparencias superpuestas | ⏳ |
| C-7 | Dear ImGui: 5 fichas culturales por proximidad | 51 | La ficha aparece a < 2 m y cita su fuente | ⏳ |
| C-8 | Justificación matemática de las 3 simples | 45 | Fórmulas y parámetros reales en `docs/manual_tecnico/` | ⏳ |

### Bloque D · 9–15 nov · 2 animaciones por KeyFrames + audio

| Código | Tarea | Act. | Criterio de terminado | Estado |
|---|---|---|---|---|
| D-1 | Silueta low-poly jerárquica (hombro, codo, cadera, rodilla) | 46 | Al mover el padre, los hijos lo siguen | ⏳ |
| D-2 | Caminata por KeyFrames con slerp | 47 | Curvas de aceleración no lineales | ⏳ |
| D-3 | Smoothstep y desfase de piernas | 48 | Justificación documentada | ⏳ |
| D-4 | Silueta dejando flores (Bézier cuadrática) | 49 | Caída libre de la flor | ⏳ |
| D-5 | Integrar las 2 animaciones complejas | 50 | Caminata y dejar flores en la escena | ⏳ |
| D-6 | Audio ambiental con miniaudio | 52 | Un solo ciclo: mar y música lejana | ⏳ |
| D-7 | Justificación matemática de las 2 complejas | 53 | Slerp, Bézier y smoothstep en `docs/manual_tecnico/` | ⏳ |

### Bloque E · 16–18 nov · Entrega

| Código | Tarea | Act. | Criterio de terminado | Estado |
|---|---|---|---|---|
| E-1 | Pruebas de integración y fugas de memoria (VAO, VBO, texturas) | 54 | Lista de errores priorizada | ⏳ |
| E-2 | Corrección de errores de cámara y colisiones | 55 | Sin atascos en bardas | ⏳ |
| E-3 | Release x64 autónomo en `dist/` con sus .dll | 56 | El .exe corre en otra PC | ⏳ |
| E-4 | 3 videos MP4 | 57 | Entorno; recorrido; animaciones e interactividad | ⏳ |
| E-5 | Manual técnico (ES + EN) | 58 | Arquitectura, cronograma, RACI, justificación, conclusiones | ⏳ |
| E-6 | Manual de usuario (ES + EN) | 59 | Capturas y explicación de controles | ⏳ |
| E-7 | Release en GitHub y entrega | 60 | Release final y enlaces | ⏳ |

### Tareas sueltas (sin bloque)

| Código | Tarea | Act. | Criterio de terminado | Estado |
|---|---|---|---|---|
| X-1 | Verificar Blender y GIMP con la misma versión en las dos PCs | 6 | Captura de la versión en cada equipo | 🔄 |
| X-2 | Bocetos del cuarto del cementerio | 7 | Fachada, interior y distribución de la ofrenda, para el manual técnico | ❓ |

**Para no ahogarse al final:** cada bloque deja escritas sus secciones del manual técnico (capturas + parámetros) en `docs/manual_tecnico/`.

**Qué recortar primero si el tiempo no alcanza** (en este orden): detalle del marco (B-6) → variantes de rocas en la barda (B-10), salvo la roca del inventario → pausas en la velocidad de las flores (C-4). **No se recorta:** colisiones, iluminación, 5 fichas, 2 KeyFrames, 3 simples, ≥ 18 modelos propios (por eso no se recortan la 4.ª tumba, la roca de la barda ni el copalero), 30 FPS, manuales y videos.
Ningún recorte puede dejar el conteo de modelos propios por debajo de 18 (ver inventario).

### Inventario de modelos propios (meta ≥ 18)

> Definición en el Registro de decisiones (19/10). Cuenta cada **objeto distinto** modelado por el equipo y exportado a
> su propio `.obj` en `Models/<objeto>/`. Las instancias o copias del mismo `.obj` cuentan **una vez**; las variantes
> con geometría distinta (cada tumba) cuentan por separado. **Evidencia:** el archivo fuente `.blend` (todo se modela en **Blender**, como dice la propuesta; el `.ma`/`.mb` solo si
> se hubiera hecho en otra herramienta) se guarda para el manual técnico. **Plano** = malla plana subdividida (riesgo de que el profesor no la
> considere un modelo; ver la nota del conteo).

| # | Modelo | Archivo (`Models/…`) | Tarea | Tipo | Estado | Evidencia fuente |
|---|---|---|---|---|---|---|
| 1 | Fachada y muros del cuarto (con apertura del arco) | `fachada/fachada.obj` | A-12 (acts. 16, 21) | Volumen | 🔄 existe; hoy incluye también el techo | Blender 4.4.3 (encabezado del `.obj`); falta guardar el `.blend` |
| 2 | Techo a dos aguas del cuarto | `techo/techo.obj` (separar de `fachada.obj`) | A-11 (act. 18) | Volumen (con grosor: tiene caras inferiores y tapas) | 🔄 existe dentro de `fachada.obj`; a dos aguas (decisión 19/10) | — |
| 3 | Arco de medio punto con dovelas | `arco/arco.obj` | A-10 (act. 17) | Volumen | ⏳ | — |
| 4 | Piso interior del cuarto | `piso/piso.obj` | A-11 (act. 18) | Plano | ⏳ (no existe en `fachada.obj`) | — |
| 5 | Playa 100×50 m | `playa/playa.obj` | A-5 (act. 8) | Plano | ⏳ | — |
| 6 | Mar 100×100 m | `mar/mar.obj` | A-7 (act. 10) | Plano | ⏳ | — |
| 7 | Tumba 1: capilla con domo y cruz (rehecha en Blender) | `tumba1/tumba1.obj` | B-9 (act. 14) | Volumen | ⏳ (sustituye a `altar/altar.obj`) | — |
| 8 | Tumba 2 | `tumba2/tumba2.obj` | B-9 (act. 14) | Volumen | ⏳ | — |
| 9 | Tumba 3 | `tumba3/tumba3.obj` | B-9 (act. 14) | Volumen | ⏳ | — |
| 10 | Tumba 4 | `tumba4/tumba4.obj` | B-9 (act. 14) | Volumen | ⏳ obligatoria: sin ella el estricto baja de 18 | — |
| 11 | Barda perimetral | `barda/barda.obj` | B-10 (act. 20) | Volumen | ⏳ | — |
| 12 | Roca de la barda (variante) | `roca/roca.obj` | B-10 (act. 20) | Volumen | ⏳ obligatoria: sin ella el estricto baja de 18 (decisión 19/10) | — |
| 13 | Cruz de piedra | `cruz/cruz.obj` | B-11 (act. 19) | Volumen | ⏳ | — |
| 14 | Soportes de madera del papel picado | `soportes/soportes.obj` | B-8 (act. 22) | Volumen | ⏳ | — |
| 15 | Papel picado | `papel_picado/papel_picado.obj` | B-7 (act. 25) | Plano | ⏳ | — |
| 16 | Altar de 3 niveles | `altar3/altar3.obj` (nombre a decidir en B-1) | B-1 (act. 24) | Volumen | ⏳ | — |
| 17 | Vela | `vela/vela.obj` | B-2 (act. 23) | Volumen | ⏳ | — |
| 18 | Flor de cempasúchil | `cempasuchil/cempasuchil.obj` | B-3 (act. 28) | Plano (planos cruzados) | ⏳ | — |
| 19 | Pan de muerto | `pan/pan.obj` | B-4 (act. 27) | Volumen | ⏳ | — |
| 20 | Botella de vidrio | `botella/botella.obj` | B-5 (act. 26) | Volumen | ⏳ | — |
| 21 | Marco con fotografía | `marco/marco.obj` | B-6 (act. 29) | Volumen | ⏳ | — |
| 22 | Silueta jerárquica (sus piezas cuentan como **un** modelo) | `silueta/silueta.obj` (o una pieza por `.obj` en `silueta/`) | D-1 (act. 46) | Volumen | ⏳ | — |
| 23 | Copalero de barro (sin humo) | `copalero/copalero.obj` | B-14 (act. 30) | Volumen | ⏳ **obligatorio** (conteo estricto) | — |
| — | ~~Capilla/tumba con domo y cruz (hoy "altar")~~ | `altar/altar.obj` | A-1 (provisional) → B-9 | Volumen | ✂️ **no cuenta**: hecha en Maya; se retira cuando B-9 la reemplace (#7) | Propia según D2 (19/10) |
| — | ~~Calavera de azúcar~~ | `calavera/base.obj` | A-1 | Volumen | ✂️ **no cuenta**: hecha en Maya y fuera de la propuesta; se retira en A-1 | Propia según D2 (19/10) |

**Conteo (19/10, corregido tras revisar el PR #4):** existe 1 que cuenta (la fachada). Planeados: **23**, todos de la propuesta y hechos en Blender
(21 con 4 tumbas y la roca de la barda, + silueta y copalero). **Escenario estricto**, sin las 5 mallas planas (piso, playa, mar, papel picado y
cempasúchil de planos cruzados): **18**, justo la meta. Sin la roca quedaba en **17** (error del conteo anterior, que no contaba el cempasúchil como
plano). Por eso **no se recortan la 4.ª tumba, la roca ni el copalero**: sin cualquiera de ellos, el estricto baja a 17. El techo a dos aguas cuenta
como volumen porque tiene grosor (V: `fachada.obj`, objeto `roof`).


---

## 3. Estado por actividad del plan original (1–60)

> Sirve para el **cronograma real** del manual técnico. Cuando una tarea se termina, `/cierre` marca
> también aquí las actividades que cubre. Fechas, responsables y criterios originales: `PLAN_ACTIVIDADES.md`.

| N.º | Actividad (corta) | Tarea | Estado | Notas / evidencia |
|---|---|---|---|---|
| 1 | Fuentes Mazunte (APA 7) | — | ✅ | Hoja del plan |
| 2 | Propuesta técnica | — | ✅ | Propuesta v2.0 (04/10/2026), `docs/propuesta/` |
| 3 | Repo + .gitignore | — | ✅ | 18/10: `hugo12341234/nierika-recorrido-mazunte` (público), ramas `main` y `develop`, README, .gitignore VS corregido, D1 invitado como colaborador (pendiente de aceptar) |
| 4 | VS + dependencias | — | ✅ | Rama `feature/act-4-base-solucion` (PR #1): `NierikaStudio.sln` x64 + `vcpkg.json` con `builtin-baseline` (Assimp 6.0.5, GLFW 3.5.1, GLEW 2.3.1, GLM 1.0.3) + `vendor/SOIL2` + `src/` + 3 modelos. **19/10: con las correcciones del código base, compila y corre en la PC de D2** (Release x64, VS 2022): se ven fachada, altar y calavera con textura, FOV 45°, cursor capturado. Fusionada en `develop` el 19/10 (PR #1) **sin revisión previa de D1** (excepción, ver decisiones); D1 la revisa después con `/verificar 4` |
| 5 | Trello | — | ✅ | 18/10: 4 listas (Pendiente / En proceso máx. 2 por persona / En revisión / Terminado), 6 tarjetas en Terminado y 12 del Bloque A en Pendiente. Etiquetas: Bloque A (azul), prioridad Alta (roja) / Media (amarilla) / Baja (verde). Falta: invitar a D1 y poner el código (A-1…A-12) en el título de cada tarjeta |
| 6 | Blender y GIMP | X-1 | 🔄 | Blender 4.4.3 en al menos un equipo |
| 7 | Bocetos del cuarto | X-2 | ❓ | |
| 8 | Playa 100×50 | A-5 | ⏳ | |
| 9 | Clase Camera | — | ✅ | Clase del curso; 19/10: pitch limitado a ±89° verificado en `Camera.h` |
| 10 | Mar 100×100 | A-7 | ⏳ | |
| 11 | Callbacks WASD/ratón | — | ✅ | Del curso; 19/10: cursor capturado en `main.cpp` |
| 12 | Colisiones AABB | A-9 | ⏳ | |
| 13 | Textura arena | A-6 | ⏳ | |
| 14 | Tumbas | B-9 | ⏳ | Faltan las 4 tumbas propias en Blender (decisión del 19/10); una sustituye a la capilla de Maya (`altar/altar.obj`) |
| 15 | Ciclo deltaTime + render | A-5 | 🔄 | Ciclo ✅; falta la playa |
| 16 | Fachada con apertura | A-12 | 🔄 | `fachada.obj` con apertura de arco; falta escala real (A-2) y textura de piedra (A-12) |
| 17 | Arco con dovelas | A-10 | 🔄 | Arco existe; faltan dovelas visibles |
| 18 | Piso, paredes, techo | A-11 | 🔄 | Paredes y techo (a dos aguas) existen; confirmar piso interior |
| 19 | Cruz | B-11 | ⏳ | |
| 20 | Barda perimetral | B-10 | ⏳ | |
| 21 | Textura piedra rústica | A-12 | 🔄 | Hoy: concreto/laminado 512 px → cambiar a piedra 1K |
| 22 | Soportes papel picado | B-8 | ⏳ | |
| 23 | Vela con pivote | B-2 | ⏳ | |
| 24 | Altar de 3 niveles | B-1 | ⏳ | |
| 25 | Malla del papel picado | B-7 | ⏳ | |
| 26 | Botella | B-5 | ⏳ | |
| 27 | Pan de muerto | B-4 | ⏳ | |
| 28 | Cempasúchil | B-3 | ⏳ | |
| 29 | Marco con foto | B-6 | ⏳ | |
| 30 | Copalero | B-14 | ⏳ | Obligatorio para el conteo de modelos (decisión del 19/10); ya no es el primer recorte |
| 31 | Descargar texturas PBR 1K | B-12 | ⏳ | |
| 32 | Texturizar altar | B-12 | ⏳ | |
| 33 | Texturizar vela, papel, botella | B-7, B-12 | ⏳ | |
| 34 | Texturizar pan, flor, marco | B-12 | ⏳ | El mapa de relieve del pan requiere ampliar `Model.h` (hoy no carga mapas normales) |
| 35 | Clase Model con Assimp | B-13 | 🔄 | `Model.h` del curso ya carga .obj con rutas relativas (texturas RGBA desde el 19/10); se cierra al cargar todos los .obj |
| 36 | Colocar los 7 objetos | B-13 | ⏳ | |
| 37 | Iluminación | A-8 | ⏳ | |
| 38 | Ajustar posiciones/escalas | B-13 | ⏳ | |
| 39 | Olas | C-1 | ⏳ | |
| 40 | Llama con ruido | C-2 | ⏳ | |
| 41 | Velas flotando | C-3 | ⏳ | |
| 42 | Flores flotando | C-4 | ⏳ | |
| 43 | Integrar simples | C-5 | ⏳ | |
| 44 | Descarte por alfa | C-6 | ⏳ | |
| 45 | Justificación simples | C-8 | ⏳ | |
| 46 | Silueta jerárquica | D-1 | ⏳ | |
| 47 | Caminata slerp | D-2 | ⏳ | |
| 48 | Smoothstep y desfases | D-3 | ⏳ | |
| 49 | Dejar flores (Bézier) | D-4 | ⏳ | |
| 50 | Integrar complejas | D-5 | ⏳ | |
| 51 | Fichas Dear ImGui | C-7 | ⏳ | |
| 52 | Audio | D-6 | ⏳ | |
| 53 | Justificación complejas | D-7 | ⏳ | |
| 54 | Pruebas y fugas | E-1 | ⏳ | |
| 55 | Corrección cámara/colisiones | E-2 | ⏳ | |
| 56 | Release x64 en dist/ | E-3 | ⏳ | |
| 57 | Videos | E-4 | ⏳ | |
| 58 | Manual técnico | E-5 | ⏳ | |
| 59 | Manual de usuario | E-6 | ⏳ | |
| 60 | Release GitHub | E-7 | ⏳ | |

---

## 4. Registro de decisiones

> Todo cambio respecto a la propuesta o a una regla del proyecto se anota aquí **antes** de aplicarlo
> en el resto de los documentos. Una decisión registrada aquí manda sobre `CONTEXTO_PROYECTO.md`.

| Fecha | Decisión | Motivo | Afecta a |
|---|---|---|---|
| 18/10 | Se trabaja por disponibilidad, no por las columnas Resp./Verif. | Falta de tiempo | Plan, RACI del manual técnico |
| 18/10 | Se usa la base de la Práctica 6: **GLEW en lugar de GLAD**; **SOIL2** además de stb_image | Ya compila y el equipo la domina | Stack, manual técnico (actualizar tabla de herramientas) |
| 18/10 | Rutas: `Models/<objeto>/<objeto>.obj`, `Shaders/*.vs` / `*.frag` | Estructura heredada de la práctica | Convenciones |
| 18/10 | `.gitignore` de VS con excepción `!**/Models/**/*.obj` y `.mtl` | La plantilla ignora todos los `.obj` y se perderían los modelos 3D | Repositorio |
| 18/10 | README del repositorio en inglés con resumen en español | Portafolio | Repositorio |
| 18/10 | Rama por defecto del repo: `develop` (`main` solo para entregas). Los hilos del Proyecto de Claude leen `docs/` del repo | Que Claude siempre vea el estado actualizado | Flujo de trabajo |
| 18/10 | Propuesta v2.0 y plan de trabajo en PDF guardados en `docs/propuesta/` | Consulta desde cualquier hilo | Documentación |
| 18/10 | Se mantiene **Trello** como tablero Kanban (el de la propuesta); no se usa GitHub Projects | Coherencia con la propuesta | Act. 5, evidencias del manual técnico |
| 18/10 | En Trello las tarjetas no llevan "#" al inicio (activa el selector de etiquetas) | Evitar errores al capturar | Trello |
| 19/10 | La solución recibida (P5.rar) estaba compilada en **32 bits** (libs, DLL y .exe x86; configs x64 rotas). Se reestructura a **x64 solo**: dependencias compiladas (Assimp, GLFW, GLEW, GLM) por **vcpkg** (`vcpkg.json`); SOIL2/stb en `vendor/` compilados desde fuente | La propuesta exige x64 y `vendor/`; no hay binarios x64 en la solución; vcpkg es la vía oficial de VS 2022. Assimp pasa de 4.x del curso a la versión de vcpkg | Act. 4, 56 (dist/ con .exe + DLL), CLAUDE.md |
| 19/10 | Estructura del repo: `src/`, `vendor/`, `Models/`, `Shaders/`; salida en `build/` (ignorada). No se suben `.pdb`/`.ilk`/`.vs`/`Debug`, los `.rar` ni los modelos de prácticas anteriores (peces, pokebolas) | Repo ligero (la solución pesaba 195 MB) | Repositorio |
| 19/10 | `vcpkg.json` con `builtin-baseline` `664933b8…` (Assimp 6.0.5, GLFW 3.5.1, GLEW 2.3.1, GLM 1.0.3) | Sin baseline cada PC toma la versión que tenga vcpkg en ese momento; con él D1, D2 y el Release final usan exactamente las mismas versiones (las mismas con que ya compiló D2) | Act. 4, 56 |
| 19/10 | Correcciones al código base de la práctica: FOV con `glm::radians` (antes 45 se interpretaba como radianes → ~58° por casualidad); cursor capturado (1ª persona); texturas **RGBA** (necesario para el alfa del papel picado y las flores); `TextureFromFile` `inline`; textura faltante → aviso en consola + color magenta; `aiProcess_GenNormals`; se quita `GLEW_STATIC` | Errores latentes que iban a aparecer en las actividades 25, 33, 37 y 44 o al agregar más `.cpp` | `src/main.cpp`, `src/Model.h`, `src/Camera.h` |
| 19/10 | Commits y PR **solo a nombre del integrante**, sin `Co-Authored-By` ni marcas de Claude (`.claude/settings.json` + regla en `CLAUDE.md`); se reescribió el historial para que todos los commits sean del integrante | El historial de commits es evidencia del manual técnico | Repositorio, manual técnico |
| 19/10 | **Códigos de tarea** `A-1`…`E-7` (+ `X-1`, `X-2`) en el *Plan vigente*; se usan en Trello, ramas (`feature/a-1-<nombre>`), PR y `/actividad`. Tarjetas de Trello: `<código> · <tarea>` (sustituye el formato "Act. N …") | Las tareas del Bloque A sin número del plan (escalas, ventana…) no se podían referenciar; el guion evita confundir `D-1` con el integrante D1 | Trello, Git, skills `/actividad` y `/verificar` |
| 19/10 | Se completa el plan vigente: act. 30 (copalero) → B-14; act. 43 (integrar simples) → C-5; acts. 6 y 7 → tareas sueltas X-1 y X-2; act. 35 → B-13 | Esas actividades no estaban en ningún bloque | Plan vigente |
| 19/10 | **Orden de autoridad** de los documentos: (1) Registro de decisiones del ESTADO → (2) `CONTEXTO_PROYECTO.md` → (3) `PLAN_ACTIVIDADES.md` y PDFs de `docs/propuesta/`. Si dos documentos se contradicen, se señala y se registra una decisión; no se elige en silencio | Integridad y coherencia del proyecto | Todos los documentos, CLAUDE.md, instrucciones del Proyecto de Claude |
| 19/10 | Las instrucciones del Proyecto de Claude (web) se guardan en `docs/INSTRUCCIONES_PROYECTO_CLAUDE.md` | Que los dos integrantes usen las mismas y queden versionadas | Proyecto de Claude |
| 19/10 | **Excepción única:** los PR #1 (act. 4) y #2 (documentación) se fusionan sin la revisión previa de D1. D1 los revisa **después**, ya en `develop`, con `/verificar 4`; si pide cambios, entran por un PR nuevo. Desde la siguiente tarea vuelve la regla normal: nada entra a `develop` sin la revisión del otro integrante | D1 aún no acepta la invitación al repositorio y el Bloque A empieza hoy; la act. 4 ya compila y corre en la PC de D2 | Act. 4, flujo de trabajo |
| 19/10 | El Proyecto de Claude (web) tiene el repositorio **con permiso de escritura**: sus hilos leen `develop` directo del repo (sin copias de `docs/` como archivos del proyecto) y pueden hacer cambios con el mismo ciclo (rama → `/cierre` → PR → revisión). Antes del primer commit de cada sesión se comprueba `git config user.name` y se configura la identidad del integrante (tabla en `CLAUDE.md`, sección 5) | En los hilos web Git empieza como `Claude`, lo que rompería la regla de autoría; dos copias del ESTADO se contradirían | CLAUDE.md, FLUJO_DE_TRABAJO, instrucciones del Proyecto, `/cierre` |
| 19/10 | **Definición de modelo propio** (criterio "≥ 18 modelos propios"): cada objeto distinto modelado por el equipo y exportado a su propio `.obj` en `Models/<objeto>/`. Las copias o instancias cuentan una vez; las variantes con geometría distinta cuentan por separado. Se guarda el archivo fuente como evidencia. El conteo vive en el *Inventario de modelos propios* (sección 2), y ningún recorte puede dejarlo por debajo de 18. El techo se separa de `fachada.obj` en su propio `.obj` (A-11) | La propuesta no definía qué cuenta como modelo, y con el cuarto en un solo archivo el conteo podía bajar de 18 (auditoría del 19/10) | Inventario, A-10, A-11, B-9, orden de recortes, manual técnico |
| 19/10 | **B-9 se hace con 4 tumbas distintas** (la propuesta dice "3 o 4") | Con 3 tumbas no se llega a 18 en el conteo estricto (sin planos) | B-9 (act. 14), inventario |
| 19/10 | El "altar" (capilla/tumba con domo y cruz) y la calavera son **propios**, según confirmó D2 de palabra. Cierra la decisión pendiente sobre su origen. Por la decisión siguiente (solo Blender) **no cuentan** y se retiran | Confirmación de D2 | Inventario (filas tachadas), act. 14 |
| 19/10 | **Solo Blender, como dice la propuesta.** No se registra Maya. La capilla y la calavera, modeladas en Maya, **no se reexportan**: pasar un `.obj` de Maya por Blender solo cambiaría el encabezado, no la herramienta con que se modeló. La capilla se **rehace en Blender** como una de las 4 tumbas de B-9 (usando la de Maya como referencia) y la calavera **sale de la escena** (cierra la pendiente sobre la ofrenda). Mientras tanto, la capilla queda como provisional (A-1, A-2). **Impacto:** B-9 pasa de 3 h a unas 4 h (+1 h, estimado, por el domo y la cruz); la calavera, 0 h; A-1 se reduce (ya no se bajan texturas); el copalero (2.5 h) deja de poder recortarse | La propuesta elige Blender y descarta Maya (tabla 15); D2 pidió seguirla "al pie de la letra". Sin la capilla y la calavera, el conteo estricto solo llega a 18 con la 4.ª tumba y el copalero | A-1, A-2, B-9, B-14, orden de recortes, inventario, Trello (títulos de A-1, A-2 y B-14) |
| 19/10 | **Techo a dos aguas**, como ya está modelado en `fachada.obj`. Es una **desviación de la propuesta** (act. 18: "techo plano"); se separa a su propio `.obj` en A-11 y cuenta como volumen | Decisión de D2: el techo ya existe así y rehacerlo plano cuesta horas sin mejorar el criterio de aceptación ("sin vigas ni detalles innecesarios" se cumple igual) | A-11 (act. 18), inventario #2, manual técnico |
| 19/10 | **Corrección del conteo:** el cempasúchil (planos cruzados) se cuenta como malla plana en el escenario estricto, que así bajaba a 17. Se agrega **una roca de la barda en su propio `.obj`** (la propuesta pide "variantes de rocas", act. 20) y queda obligatoria | Revisión independiente del PR #4 (`/mnt/project-files/revisiones/REVISION_PR4_2026-10-19.md`) | Inventario, B-10, orden de recortes |
| 19/10 | **Excepción de revisión:** el PR #4 se fusionó sin la revisión de D1. D1 lo revisa **después**, en GitHub (*Files changed*) o en Claude Code sobre el commit `09c39d8`; si pide cambios, entran por un PR nuevo. Vuelve la regla normal: nada entra a `develop` sin la revisión del otro integrante, y este PR de seguimiento lo fusiona D1 | D2 lo fusionó antes de la revisión | Flujo de trabajo, PR #4 |
| *pendiente* | **Bloque de atribución en los PR abiertos desde hilos web:** la plataforma exige que empiecen con un comentario HTML y la línea "Requested by … · project thread", y eso choca con `CLAUDE.md` §5 ("sin marcas de Claude"). Opciones: (a) aceptarlo solo en la descripción del PR (los commits siguen limpios); (b) que Claude lo ponga y el integrante lo borre antes del merge; (c) que los PR los abra el integrante a mano | | `CLAUDE.md` §5, FLUJO_DE_TRABAJO, manual técnico |
| 19/10 | La calavera de azúcar **no** entra a la ofrenda: se quita de la escena y del repo en A-1 | No está en la propuesta y se modeló en Maya (decisión anterior) | A-1, alcance, fichas |
| *pendiente* | ¿El repositorio queda **público** (portafolio) o **privado**? Hoy es público | Expone los números de cuenta y la configuración de Claude | Repositorio |

---

## 5. Parámetros fijados

| Elemento | Parámetro | Valor | Dónde vive |
|---|---|---|---|
| Ventana | tamaño | 800×600 → **cambiar a 1280×720** (A-3) | `src/main.cpp` |
| Proyección | near / far | 0.1 / 100 → **cambiar far a 300** (A-4) | `src/main.cpp` |
| Cámara | posición inicial | (0, 2, 6) | `src/main.cpp` |
| Cámara | campo de visión vertical (FOV) | 45° (`ZOOM` en grados → `glm::radians`) | `src/Camera.h` (`ZOOM`), `src/main.cpp` (`glm::perspective`) |
| Cámara | velocidad / sensibilidad / pitch | 6 m/s · 0.25 · ±89° | `src/Camera.h` |
| Fachada | trans / rot Y / esc | (0,0,0) / 180° / 3.0 → **revisar** (A-2) | `src/main.cpp` |
| "Altar" (tumba) | trans / esc | (0,0,2) / 3.5 → **revisar** (A-2) | `src/main.cpp` |
| Calavera | trans / esc | (0,2.1,1.8) / 0.4 → **se quita** (A-1) | `src/main.cpp` |
| Texturas | descarte por alfa | alfa < 0.1 | `Shaders/modelLoading.frag` |
| Fichas | radio de proximidad | 2 m | — (C-7) |
| Modelos propios | meta / planeados / estricto (sin planos) | ≥ 18 / 23 / 18 (19/10, con 4 tumbas, roca y copalero) | ESTADO, sección 2 (inventario) |

---

## 6. Problemas abiertos

| # | Problema | Se resuelve en |
|---|---|---|
| 1 | `altar.obj` línea 3: `mtllib base.mtl` → debe decir `mtllib altar.mtl` *(con Assimp 6.0.5 el altar ya se ve con textura; confirmar al corregirlo)* | A-1 |
| 2 | `altar.mtl`: `map_Kd shaded.png` → `texture_diffuse.png`; `Kd 0 0 0` → `Kd 1 1 1` | A-1 |
| 3 | Texturas del altar y la calavera de 2048 px (la propuesta pide 1K). Ya no se reducen: la calavera se borra en A-1 y la capilla se reemplaza en B-9 | A-1 (calavera), B-9 (capilla) |
| 4 | Escalas fuera de 1 u = 1 m (sección 5) | A-2 |
| 5 | Falta iluminación: el shader actual solo muestra la textura | A-8 |
| 6 | El material `Material` de la fachada no tiene textura (se ve negro) | A-12 |
| 7 | No se libera memoria de GPU (VAO/VBO/EBO, texturas, programa de shader) | E-1 (o antes, al tocar esas clases) |
| 8 | `Model.h` no carga mapas normales/de relieve (necesario para el pan) | B-12 |
| 9 | `fachada.obj` incluye el techo (objeto `roof`); según la definición de modelo propio, el techo va en su propio `.obj` | A-11 |
| 10 | **Prueba de autoría:** falta guardar el `.blend` de la fachada, y cada modelo nuevo debe guardar el suyo. Si el manual técnico menciona la capilla y la calavera de Maya como trabajo previo, también hay que guardar sus `.ma`/`.mb`. La capilla provisional no debe salir en videos ni capturas finales (no cuenta y no tiene crédito) | B-13 (antes del manual técnico) |

---

## 7. Siguiente paso concreto

1. **D2:** volver a pegar las instrucciones de `docs/INSTRUCCIONES_PROYECTO_CLAUDE.md` en el Proyecto de Claude (cambiaron el 19/10) y quitar del proyecto cualquier copia suelta de `docs/`.
2. **D1:** dar su nombre y correo de GitHub para la tabla de autoría de `CLAUDE.md`; aceptar la invitación al repositorio, instalar vcpkg (ver README), compilar `develop` (Release x64) y revisar la act. 4 y la documentación con `/verificar 4` (revisión posterior, ver decisiones del 19/10).
3. Trello: poner el código en el título de las 12 tarjetas del Bloque A (`A-1 · …`) e invitar a D1.
4. Trello: actualizar los títulos de A-1, A-2, A-11, B-10 y B-14 (cambiaron el 19/10).
5. **D2:** guardar el `.blend` de la fachada (problema #10).
6. **D1:** revisar el PR #4 (excepción del 19/10) y fusionar el PR de seguimiento.
7. Bloque A en este orden: A-1 y A-2 → A-3 y A-4 → A-5, A-6 y A-7 → A-8 → A-9 → A-10, A-11 y A-12.
