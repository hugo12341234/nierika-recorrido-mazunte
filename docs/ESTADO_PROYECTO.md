# ESTADO DEL PROYECTO — documento vivo

> **Qué es este archivo:** la memoria del proyecto. Dice **dónde vamos**: qué está hecho, qué sigue,
> qué decidimos y con qué valores. Lo que no esté aquí, Claude no lo sabrá en la siguiente sesión.
> **Quién lo actualiza:** Claude Code con `/cierre` al terminar cada sesión (se revisa antes de hacer commit).
> **Dónde vale:** la versión de la rama `develop`.

**Última actualización:** 25/10/2026 (A-7) · **Hoy:** semana 4 (Bloque A) · **Días a la entrega:** 24 (mié 18/11/2026)
**Integrantes:** D1 = 319266894 · D2 = 321332086
**Repositorio:** https://github.com/hugo12341234/nierika-recorrido-mazunte (**público**; rama por defecto `develop`) · **Hoja del plan:** https://docs.google.com/spreadsheets/d/13i1yXf3dMuiBWyxka-R17Zw0EG2F-fMPPWCSC70_qXA/edit?usp=sharing · **Tablero Trello:** https://trello.com/b/8FNkEY97/nierika-studio-recorrido-mazunte

**Forma de trabajo acordada (18/10):** las columnas Resp./Verif. de la propuesta ya **no se siguen**.
Quien esté libre toma la siguiente tarea del *Plan vigente* (sección 2); **el otro revisa** antes de
integrarla a `develop`.

**Contenido:** 1. Lo que ya existe · 2. Plan vigente (tareas con código) · 3. Estado por actividad (1–60) ·
4. Registro de decisiones · 5. Parámetros fijados · 6. Problemas abiertos · 7. Siguiente paso

---

## 1. Lo que ya existe (revisado el 19/10/2026; fachada, transformaciones, ventana, *far plane*, playa y arena, el 20/10)

**Código** (base: proyecto de la Práctica 6 del laboratorio, `319266894_REPORTE6_GPO8`; act. 4, PR #1):
- `src/main.cpp`: ventana GLFW **1280×720** (A-3, 20/10) no redimensionable, proyección con FOV 45° y near/far **0.1 / 300 m** (A-4, 20/10), OpenGL 3.3 Core, **GLEW** (no GLAD), **SOIL2 + stb_image**, cursor capturado.
- Clases del curso en `src/`: `Shader.h`, `Camera.h` (WASD/flechas + ratón, pitch ±89°), `Model.h` + `Mesh.h` (Assimp, texturas RGBA).
- Ciclo con `deltaTime`. Shader `Shaders/modelLoading.vs/.frag`: **solo textura, sin iluminación** (descarta píxeles con alfa < 0.1).
- Carga y dibuja 3 modelos: playa (A-5, 20/10), fachada y "altar" (capilla provisional). La calavera se quitó en A-1 (19/10, rama `feature/a-1-quitar-calavera`).
- Dependencias: Assimp, GLFW, GLEW y GLM por **vcpkg** (`vcpkg.json`); SOIL2/stb en `vendor/SOIL2`.

**Modelos** (`Models/`):

| Modelo | Origen | Geometría | Texturas | Observaciones |
|---|---|---|---|---|
| `fachada/fachada.obj` | Blender 4.4.3 (propio); **reexportado a escala real en A-2 (20/10)** desde Blender 5.2.2, con `fachada.blend` reconstruido desde el `.obj` | 204 caras; muros 3.93 × 2.62 m y 3.28 m de alto, techo 4.18 × 3.43 m hasta 3.32 m (1 u = 1 m, pivote en el centro del piso); muros con **apertura de arco** (no es de medio punto: 1.18 m de ancho, arranque a 1.14 m y clave a 2.11 m, casi semielíptica; problema #12) + **techo a dos aguas** (decisión 19/10: se queda así) | `concreto.jpg`, `laminado1.jpg` (512×512) | Arco sin dovelas visibles. Al reexportar (A-2) desapareció el material sobrante `Material`. |
| `altar/altar.obj` | **Propio** (confirmado de palabra por D2 el 19/10); exportado desde Maya. **Se retira**: se rehace en Blender como una de las tumbas de B-9 (decisión 19/10, solo Blender) | 2 148 caras; 1.3 × 1.9 × 0.9 u; **es una capilla/tumba con domo y cruz, no un altar de 3 niveles** | `texture_diffuse.png`, `texture_normal.png`, `shaded.png` (2048×2048) | A-1 (19/10): el .obj ya dice `mtllib altar.mtl`; `altar.mtl` usa `texture_diffuse.png` y `Kd 1 1 1`. `shaded.png` y `texture_normal.png` siguen en la carpeta sin usarse (se van con la capilla en B-9). |
| `playa/playa.obj` | **Propio**, Blender 5.2.2 (A-5, 20/10), con `playa.blend` | 1 250 caras (50 × 25 cuadros de 2 m), 1 326 vértices; x −50…+50, z −20…+30; y = 0 hasta z = +20 y pendiente hasta y = −0.5 en z = +30 (vértices en coordenadas del mundo) | `arena.jpg` (1024×1024, mapa *Diffuse* de **Sandy Gravel**, Poly Haven, CC0; abarca 2.1 m reales) desde A-6 (20/10); sustituye a `Playa_provisional.png` | UV de 0 a 50 × 0 a 25: la arena se repite cada 2 m (A-6). Textura externa (no es modelo): crédito APA 7 → Baglioni, C. (s. f.). *Sandy gravel* [Textura]. Poly Haven. Recuperado el 20 de octubre de 2026, de https://polyhaven.com/a/sandy_gravel. Sin luz (problema #5) la pendiente no se distingue en pantalla |
| `mar/mar.obj` | **Propio**, Blender 5.2.2 (A-7, 25/10), con `mar.blend` | 10 000 caras (100 × 100 cuadros de 1 m), 10 201 vértices; x −50…+50, z +22…+122, plano en y = −0.2 (vértices en coordenadas del mundo); normal (0, 1, 0) | `mar_provisional.png` (1024×1024, un solo color, hex `1E5A78`), **provisional** | UV de 0 a 1 (una sola vez: con un color liso no importa). Orilla con la playa en z = +24. El color y la textura definitivos del agua llegan con el shader de olas (C-1) |
| ~~`calavera/base.obj`~~ | Propio, exportado desde Maya | 13 518 caras; 1.5 × 1.9 × 1.7 u | `texture_diffuse.png`, `texture_normal.png` (2048×2048) | ✂️ **Retirada en A-1 (19/10):** ya no se carga y `Models/calavera/` se borró del repo (decisión del 19/10). |

**Transformaciones en `main.cpp`** (corregidas en **A-2**, 20/10): fachada esc. 1.0 (rot. Y 180°) · "altar" esc. 1.0 en (0, 0, −0.6), **1.9 m de alto**.
Antes: fachada esc. 3.0 (~7.6 m de alto) y "altar" esc. 3.5 (6.6 m de alto).

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
| A-1 | Quitar la calavera de la escena y del repo; dejar la capilla de Maya solo como **provisional** hasta B-9 (corregir `mtllib`, `map_Kd`, `Kd`) | — | La calavera ya no se carga y `Models/calavera/` se borra; la capilla carga `altar.mtl` con `texture_diffuse.png`; sin errores en consola. No se reducen sus texturas, porque B-9 la reemplaza | ✅ 19/10, PR #9 (`/verificar A-1`: APROBAR): compila en Release x64 en la PC de D2 (1 correcto, 0 fallidos; solo *warnings* C4244/C4267 que ya existían); capturas de D2: capilla con textura, sin magenta ni calavera, consola sin avisos |
| A-2 | Escalas reales 1 u = 1 m (fachada y capilla provisional) | — | Medidas en metros registradas en la sección 5; escalas en `main.cpp` cercanas a 1 | ✅ 20/10, PR #11 (rama `feature/a-2-escalas-reales`): fachada escalada ×1.31 en Blender y reexportada (arco 2.11 × 1.18 m); escalas de `main.cpp` en 1.0; capilla en (0, 0, −0.6). Compila en Release x64 en la PC de D2 (1 correcto) y se ve el cuarto con la capilla al fondo. **Verificada** con `/verificar A-2`: APROBAR (comentario de D2 en el PR #11, 20/10 a las 00:39, hora de México) |
| A-3 | Ventana 1280×720 | — | La ventana abre a 1280×720; `lastX`/`lastY` iniciales pasan de 400/300 a 640/360 (`main.cpp:37`) | ✅ 20/10, rama `feature/a-3-a-4-ventana-far-plane`: `WIDTH, HEIGHT = 1280, 720` y `lastX, lastY = 640, 360` en `src/main.cpp`. Compila en Release x64 en la PC de D2; la captura de la ventana completa mide 1280 px de ancho con 720 px de área de dibujo bajo la barra de título, y el cuarto no se ve estirado |
| A-4 | *Far plane* a 300 m | — | Nada visible se recorta a menos de 300 m | ✅ 20/10, misma rama que A-3: `glm::perspective(..., 0.1f, 300.0f)` en `src/main.cpp`. Hoy la escena solo es el cuarto (~4 m), así que el criterio se cumple sin prueba real de distancia: **se vuelve a revisar en A-5** con la playa de 100×50 m. **Revisado en A-5 (20/10):** desde una esquina de la playa se ve completa hasta la esquina opuesta (~112 m), sin cortes (captura de D2) |
| A-5 | Playa: plano 100×50 m subdividido | 8, 15 | Escala 1 u = 1 m; se dibuja en el ciclo de render | ✅ 20/10, rama `feature/a-5-playa`: `Models/playa/playa.obj` modelado por D2 en Blender 5.2.2 con los valores de la decisión "Playa y mar" (20/10). Revisado en el `.obj`: 1 250 caras, 1 326 vértices, x −50…+50, z −20…+30, filas z = +22…+30 en y = −0.1…−0.5. `main.cpp` lo dibuja con matriz identidad. Compila y corre (F5) en la PC de D2: arena alrededor y dentro del cuarto (sin piso hasta A-11); *far plane* revisado desde una esquina (sin cortes); z-fighting: D2 no vio parpadeo en la base de muros y capilla (revisión rápida); consola sin avisos. La orilla con el mar se revisa en A-7. PR #13; compilado en **Release x64** (confirmado por D2); **verificada** con `/verificar A-5`: APROBAR (comentario de D2 en el PR #13, 20/10 a las 15:15, hora de México) |
| A-6 | Textura de arena 1K con mapeo UV | 13 | Textura 1K sin estiramientos visibles | ✅ 20/10, rama `feature/a-6-arena`: `Models/playa/arena.jpg` (1024×1024, *Sandy Gravel* de Poly Haven, CC0, 2.1 m reales; se deja la repetición de 2 m acordada porque la diferencia es de 5 %). UV de la playa escalada ×50 × ×25 desde el origen en Blender (mediana de la UV en 25 / 12.5); `playa.mtl` → `map_Kd arena.jpg`; `Playa_provisional.png` borrada. Sin cambios de código (`Model.h` ya usa `GL_REPEAT` y mipmaps). Compila y corre en Release x64 en la PC de D2: de cerca los granos se ven nítidos, sin estiramiento; de lejos no se nota un patrón de repetición obvio. Se descartó *Aerial Beach 01* (30 m reales: con 1K quedaría borrosa de cerca). Solo se usa el color: el mapa normal espera al problema #8 (B-12) y a la luz (A-8). PR #14; **verificada** con `/verificar A-6`: APROBAR (comentario de D2 en el PR #14, 20/10 a las 16:06, hora de México) |
| A-7 | Mar: malla 100×100 m subdividida | 10 | Malla preparada para el vertex shader de olas | ✅ 25/10, rama `feature/a-7-mar`: `Models/mar/mar.obj` modelado por D2 en Blender 5.2.2 con los valores de la decisión "Playa y mar" (20/10). Revisado en el `.obj`: 10 000 caras y 10 201 vértices (100 × 100 cuadros de 1 m, los vértices que el *vertex shader* de olas de C-1 va a desplazar), x −50…+50, z +22…+122, todo en y = −0.2, normal (0, 1, 0). Textura provisional de un solo color (`mar_provisional.png`, decisión del 25/10). `main.cpp` lo dibuja con matriz identidad. Compila y corre (F5, Release x64) en la PC de D2: el mar se ve desde el inicio y llega hasta su borde en z = +122 visto desde una esquina de la playa (*far plane* sin cortes); la orilla en z = +24 es una línea recta y limpia, y D2 no vio parpadeo al moverse cerca (sin z-fighting); consola sin errores (confirmado por D2). Todavía no hay olas: el shader llega en C-1 |
| A-8 | Shader Blinn-Phong: luz ambiental cálida + 3 luces clave | 37 | El interior del cuarto se distingue sin perder el ambiente nocturno; sin luces puntuales extra; `Draw(Shader&)` por referencia y nada de `glDelete*` en destructores (decisión `cleanup()`, 19/10) | ⏳ |
| A-9 | Colisiones AABB con suelo y muros | 12 | La cámara no atraviesa el suelo ni los muros; camina en el plano XZ a la altura de ojos fijada en la sección 5 (valor inicial) | ⏳ |
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
| E-3 | Release x64 autónomo en `dist/` con sus .dll | 56 | El .exe corre en otra PC sin Visual Studio; `dist/` incluye `Models/`, `Shaders/` y las DLL; se publica como .zip en el Release (decisión del 19/10), no en Git | ⏳ |
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
| 1 | Fachada y muros del cuarto (con apertura del arco) | `fachada/fachada.obj` | A-12 (acts. 16, 21) | Volumen | 🔄 existe; hoy incluye también el techo | Blender 4.4.3 (encabezado del `.obj`). El `.blend` original **no apareció** (20/10). `fachada/fachada.blend` está en el repo desde A-2, pero **se reconstruyó importando el `.obj`**: sirve como fuente para editar, **no** como prueba de autoría |
| 2 | Techo a dos aguas del cuarto | `techo/techo.obj` (separar de `fachada.obj`) | A-11 (act. 18) | Volumen (con grosor: tiene caras inferiores y tapas) | 🔄 existe dentro de `fachada.obj`; a dos aguas (decisión 19/10) | — |
| 3 | Arco de medio punto con dovelas | `arco/arco.obj` | A-10 (act. 17) | Volumen | ⏳ | — |
| 4 | Piso interior del cuarto | `piso/piso.obj` | A-11 (act. 18) | Plano | ⏳ (no existe en `fachada.obj`) | — |
| 5 | Playa 100×50 m | `playa/playa.obj` | A-5 (act. 8) | Plano | ✅ 20/10 | `playa/playa.blend` (Blender 5.2.2), modelado por D2 el 20/10 |
| 6 | Mar 100×100 m | `mar/mar.obj` | A-7 (act. 10) | Plano | ✅ 25/10 | `mar/mar.blend` (Blender 5.2.2), modelado por D2 el 25/10 |
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
| — | ~~Calavera de azúcar~~ | `calavera/base.obj` | A-1 | Volumen | ✂️ **no cuenta**: hecha en Maya y fuera de la propuesta; **retirada en A-1 (19/10)** | Propia según D2 (19/10) |

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
| 5 | Trello | — | ✅ | 4 listas (Pendiente / En proceso máx. 2 por persona / En revisión / Terminado). **19/10 (export JSON):** 56 tarjetas: las 49 tareas A-1…E-7, X-1 y X-2 con código y su etiqueta de bloque (A–E y "Sueltas (X)"), más 7 en Terminado. Prioridades Alta/Media/Baja iguales a la columna *Prior.* del plan original. Falta: invitar a D1, asignar responsable y fecha a cada tarjeta, separar "A-3 + A-4" y archivar "Act. 0" |
| 6 | Blender y GIMP | X-1 | 🔄 | Blender 4.4.3 en al menos un equipo |
| 7 | Bocetos del cuarto | X-2 | ❓ | |
| 8 | Playa 100×50 | A-5 | ✅ | 20/10, rama `feature/a-5-playa` |
| 9 | Clase Camera | — | ✅ | Clase del curso; 19/10: pitch limitado a ±89° verificado en `Camera.h` |
| 10 | Mar 100×100 | A-7 | ✅ | 25/10, rama `feature/a-7-mar` |
| 11 | Callbacks WASD/ratón | — | ✅ | Del curso; 19/10: cursor capturado en `main.cpp` |
| 12 | Colisiones AABB | A-9 | ⏳ | |
| 13 | Textura arena | A-6 | ✅ | 20/10, PR #14 |
| 14 | Tumbas | B-9 | ⏳ | Faltan las 4 tumbas propias en Blender (decisión del 19/10); una sustituye a la capilla de Maya (`altar/altar.obj`) |
| 15 | Ciclo deltaTime + render | A-5 | ✅ | Ciclo ✅; la playa se dibuja en el ciclo desde A-5 (20/10) |
| 16 | Fachada con apertura | A-12 | 🔄 | `fachada.obj` con apertura de arco, a escala real desde A-2 (20/10); falta textura de piedra (A-12) |
| 17 | Arco con dovelas | A-10 | ⏳ | No hay pieza de arco: `fachada.obj` solo tiene la apertura del muro (act. 16) |
| 18 | Piso, paredes, techo | A-11 | 🔄 | Paredes y techo a dos aguas (decidido el 19/10) existen; **no hay piso interior** (las caras en y = 0 son la base de los muros y miran hacia abajo) |
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
| 19/10 | **Excepción de revisión, otra vez:** los PR #5 (seguimiento del #4, commit `71e0c09`) y #6 (cámara a pie propuesta, `cleanup()`, correcciones al ESTADO) también se fusionaron sin la revisión de D1. D1 los revisa **después**, igual que el #4; si pide cambios, entran por un PR nuevo. Ya van cuatro fusiones sin revisión (PR #1/#2, #4, #5 y #6): la regla de revisión sigue escrita, pero mientras D1 no entre al repositorio no se está cumpliendo | Decisión de D2: D1 aún no participa y el Bloque A no puede esperar | Flujo de trabajo, PR #5, PR #6 |
| 19/10 | **Problema #10 (prueba de autoría) cerrado por confirmación de palabra de D2.** D2 dice que la evidencia de autoría está resuelta. **No se verificó ningún archivo fuente:** no hay ningún `.blend` en el repositorio y Claude no vio ninguno. El manual técnico no debe afirmar más que eso (p. ej., no decir que el `.blend` de la fachada está en el repo). Siguen vigentes: cada modelo nuevo guarda su archivo fuente (definición de modelo propio) y la capilla provisional no sale en videos ni capturas finales | Decisión de D2 | Problema #10, inventario #1, manual técnico |
| 19/10 | **Bloque de atribución en los PR abiertos desde hilos web:** se acepta **solo en la descripción del PR** (el comentario HTML y la línea "Requested by … · project thread" que exige la plataforma). Los commits siguen solo a nombre del integrante, sin `Co-Authored-By`, `Claude-Session` ni otras marcas | Decisión de D2: el uso de Claude ya es visible en el repo (`CLAUDE.md`, `.claude/`) y la evidencia del manual técnico son los commits, que siguen limpios | `CLAUDE.md` §5, FLUJO_DE_TRABAJO, instrucciones del Proyecto |
| 19/10 | **Memoria de GPU con `cleanup()` explícito, no con destructores.** `Model::Draw` y `Mesh::Draw` pasan a recibir `Shader&` (hoy reciben el shader **por valor**, `src/Model.h:37`, `src/Mesh.h:60`) y cada clase tiene `cleanup()` con sus `glDelete*`, que se llama al final de `main` antes de `glfwTerminate()`. Matiza la regla de `CLAUDE.md` §4 ("en destructores o `cleanup()`"): en estas clases, solo `cleanup()` | Las clases se copian (`Draw(Shader)` por valor; `vector<Mesh>` copia y mueve al crecer, `src/Model.h:82`): un destructor con `glDeleteProgram` o `glDeleteVertexArrays` borraría recursos en uso en cada `Draw` o al crecer el vector, y se verían modelos invisibles sin ningún error | A-8, E-1, problema #7, `CLAUDE.md` §4 |
| 19/10 | **`dist/` no se versiona:** el `.exe` con sus DLL, `Models/` y `Shaders/` se publica como .zip en el Release de GitHub (E-7). `dist/` va al `.gitignore` | Binarios pesados en el historial; `FLUJO` decía que `.gitignore` bloqueaba `.exe` y `.dll`, y no era cierto (verificado con `git check-ignore`) | E-3, E-7, `.gitignore`, FLUJO_DE_TRABAJO |
| 19/10 | **Evidencias de gestión:** cada viernes se guarda una captura del Trello y una minuta corta en `docs/manual_tecnico/evidencias/` | La propuesta las promete como evidencia del manual técnico (pág. 20, "Evidencias") y no había dónde guardarlas | Manual técnico, sección 7 |
| 19/10 | **Verificación obligatoria antes de todo merge, sin excepciones (vigente desde el 20/10/2026).** Ningún PR entra a `develop` sin pasar `/verificar` (subagente `verificador`) con veredicto APROBAR. Las excepciones de los PR #1 a #7 (fusionados sin revisión el 18–19/10, incluido el #7, que no tenía excepción propia) **se dejan como historial**, tal como están escritas, y ya no se revisan después: D2 decidió ignorarlas ("vamos a ignorar lo de los pull request sin verificación anteriores, a partir de ahora todo va a tener verificación") | Cortar el ciclo de registrar una excepción por PR en otro PR que también se fusionaba sin revisión | Flujo de trabajo, FLUJO §4, INSTRUCCIONES §3, sección 7 |
| 20/10 | **Escalas de A-2.** (1) El cuarto se escala **×1.31**, no ×1.3: el arco mide 1.610 m a escala 1 y con ×1.3 quedaba en 2.09 m, debajo de los 2.1 m pedidos; con ×1.31 mide **2.11 × 1.18 m**. (2) La escala se aplica **en Blender** (pivote al centro del piso, `Ctrl+A`, reexportado con Forward −Z / Up Y) y `main.cpp` queda en 1.0, como pide CLAUDE.md §4. (3) La capilla provisional (Maya) **no se reexporta** (decisión del 19/10): ya mide 1.9 m, así que solo cambia su escala a 1.0 y se coloca al fondo del cuarto, en (0, 0, −0.6). (4) El `.blend` original de la fachada no apareció; se reconstruyó desde el `.obj` y se versiona en `Models/fachada/fachada.blend` (110 KB). No prueba autoría | (1) Criterio de la pendiente de la cámara. (2) Regla 1 u = 1 m en el `.obj`. (3) B-9 reemplaza la capilla. (4) Tener una fuente editable para A-10/A-11/A-12 | A-2, sección 5, inventario #1, problemas #4, #6 y #11 |
| 20/10 | **Playa y mar: distribución, alturas y mallas (A-5, A-6, A-7).** Valores propuestos por Claude y aceptados por D2 (hilo "A-5, A-6, A-7 — Playa", 20/10). Coordenadas de OpenGL (y hacia arriba; el arco del cuarto mira a +z): (1) **playa** de x −50…+50 y z −20…+30 (100 × 50 m), plana en y = 0 hasta z = +20 y con pendiente lineal en sus últimos 10 m hasta y = −0.5 en z = +30; (2) **mar** plano en y = −0.2, de x −50…+50 y z +22…+122 (100 × 100 m). Empieza bajo la arena (en z = +22 la arena está en y = −0.1) y la arena baja de y = −0.2 en z = +24: ahí se cruzan en una línea, **la orilla (z = +24)**, y nunca son coplanares. De z = +24 a +30 la arena queda bajo el agua; (3) subdivisión: playa **50 × 25** cuadros de 2 m, mar **100 × 100** cuadros de 1 m; (4) la arena se repite cada **2 m** (UV de 0 a 50 por 0 a 25; `Model.h` ya usa `GL_REPEAT`), textura CC0 con crédito APA 7 (A-6); (5) los vértices de cada `.obj` ya están en coordenadas del mundo (transformaciones aplicadas en Blender, pivote en el origen): en `main.cpp` su matriz `model` es la identidad; (6) mientras no exista la arena (A-6), la playa usa una textura provisional de un solo color, porque el shader actual siempre muestrea `texture_diffuse1` y sin textura tomaría la del último modelo dibujado; (7) el piso del cuarto (A-11) va a y = +0.02, no en y = 0 sobre la arena; (8) A-5, A-6 y A-7 van en **un PR por tarea** (decisión de D2). La prueba de z-fighting de la orilla se hace en A-7, cuando existe el mar | Ningún documento fijaba dónde van la playa y el mar respecto al cuarto, su altura ni su subdivisión. Recorrido playa → panteón → cuarto: el cuarto queda a 20 m del borde de tierra adentro y deja ~25 m de arena para el panteón. Playa + mar ocupan 100 × 142 m (z −20…+122): diagonal ≈ 174 m < 300 m del *far plane*. Con near 0.1 / far 300 y profundidad de 24 bits, a 100 m se distinguen ~6 mm, así que 0.2 m de separación no producen z-fighting. Cuadros de 1 m en el mar permiten olas de ≥ 4 m de longitud de onda (C-1) | A-5, A-6, A-7, A-11, C-1, sección 5, inventario #5 y #6 |
| 25/10 | **Textura provisional del mar (A-7).** El mar usa `mar_provisional.png` (1024×1024, un solo color azul, hex `1E5A78`) con UV de 0 a 1, igual que hizo la playa antes de A-6. Se cambia cuando exista el shader de olas (C-1) o una textura de agua | El shader actual siempre muestrea `texture_diffuse1`: sin textura, el mar tomaría la del último modelo dibujado (mismo motivo que el punto 6 de "Playa y mar") | A-7, C-1, sección 1 |
| *pendiente* | **Quién verifica mientras D1 no esté en el repositorio.** Propuesta de Claude, pendiente de confirmar por D2: D2 corre `/verificar <código>` en una **sesión nueva** de Claude Code (no en la que hizo la tarea, para que la revisión sea independiente), pega el veredicto como comentario en el PR y solo fusiona con APROBAR. Cuando D1 entre, vuelve la regla normal: verifica el otro integrante | D1 aún no acepta la invitación al repositorio | Flujo de trabajo, FLUJO §4 |
| 19/10 | **Cámara a pie (antes de A-9). Valores iniciales, ajustables si hace falta** (D2, chat del proyecto, 19/10: "así dejemos lo de los ojos, velocidad y escala, lo vamos modificando si hace falta"; no son valores finales): (1) movimiento solo en el plano XZ: `front` sin su componente y para avanzar (hoy avanza hacia donde mira, también hacia arriba, `src/Camera.h:60-71`); (2) altura de ojos **1.6 m** sobre el suelo; (3) velocidad al caminar **2.0 m/s** (hoy 6 m/s, `Camera.h:25`, `SPEED = 6.0f`, que es velocidad de carrera); (4) el cuarto se escala para que la apertura del arco mida **≥ 2.1 m de alto** (hoy 1.61 m a escala 1, problema #11): factor ≈ 1.3, que deja muros de ~2.6 m y una puerta de ~1.2 m de ancho. **(4) resuelto en A-2 (20/10) con factor 1.31** (ver decisión "Escalas de A-2") | (2) Altura de ojos de un adulto de pie (valor de referencia, no medido). (3) A 1.4 m/s, paso normal, cruzar la playa de 100 m toma ~71 s; a 2.0 m/s, ~50 s, mejor para el recorrido y los videos. (4) Una puerta real mide ~2.1 m (valor de referencia); con 1.61 m la cámara a 1.6 m roza el arco | A-2, A-9, sección 5, `Camera.h`, `main.cpp` |
| 19/10 | La calavera de azúcar **no** entra a la ofrenda: se quita de la escena y del repo en A-1 | No está en la propuesta y se modeló en Maya (decisión anterior) | A-1, alcance, fichas |
| *pendiente* | ¿El repositorio queda **público** (portafolio) o **privado**? Hoy es público | Expone los números de cuenta y la configuración de Claude | Repositorio |

---

## 5. Parámetros fijados

| Elemento | Parámetro | Valor | Dónde vive |
|---|---|---|---|
| Ventana | tamaño | **1280×720** (A-3, 20/10); `lastX`/`lastY` iniciales 640/360 | `src/main.cpp` |
| Proyección | near / far | **0.1 / 300** (A-4, 20/10; antes 0.1 / 100) | `src/main.cpp` |
| Cámara | posición inicial | (0, 2, 6), fuera del cuarto mirando al arco (z = +1.31) → y = 1.6 **(valor inicial, se ajusta en A-9)**; x, z se fijan con la escena (A-2, A-5) | `src/main.cpp` |
| Cámara | campo de visión vertical (FOV) | 45° (`ZOOM` en grados → `glm::radians`) | `src/Camera.h` (`ZOOM`), `src/main.cpp` (`glm::perspective`) |
| Cámara | velocidad / sensibilidad / pitch | 6 m/s · 0.25 · ±89° → velocidad 2.0 m/s **(valor inicial, se ajusta en A-9)** | `src/Camera.h` |
| Cámara | altura de ojos / plano de movimiento | 1.6 m / XZ **(valor inicial, se ajusta en A-9)**; hoy vuela | `src/Camera.h` (A-9) |
| Cuarto | apertura del arco (alto × ancho) | **2.11 × 1.18 m** (factor 1.31 aplicado en Blender, A-2, 20/10) | `Models/fachada/fachada.obj` |
| Cuarto | muros / techo | muros 3.93 × 2.62 m (planta), 2.62 m hasta el alero y 3.28 m en la cumbrera; techo 4.18 × 3.43 m, cumbre a 3.32 m; pivote en el centro del piso | `Models/fachada/fachada.obj` |
| Fachada | trans / rot Y / esc | (0,0,0) / 180° / **1.0** (A-2, 20/10); el arco queda en z = +1.31, de cara a la cámara | `src/main.cpp` |
| "Altar" (capilla provisional) | trans / esc | **(0, 0, −0.6) / 1.0** (A-2, 20/10): 1.30 × 0.90 m y 1.9 m de alto, al fondo del cuarto | `src/main.cpp` |
| Playa | extensión / alturas / subdivisión | x −50…+50, z −20…+30; y = 0 hasta z = +20, pendiente lineal hasta y = −0.5 en z = +30 / **50 × 25** cuadros de 2 m (A-5, 20/10) | `Models/playa/playa.obj` |
| Playa | repetición de la arena | cada 2 m (UV 0…50 × 0…25) (A-6) | `Models/playa/playa.obj` |
| Mar | extensión / altura / subdivisión | x −50…+50, z +22…+122, y = −0.2 (orilla en z = +24) / **100 × 100** cuadros de 1 m (A-7, 20/10) | `Models/mar/mar.obj` |
| Playa y mar | transformación en `main.cpp` | identidad (vértices en coordenadas del mundo) | `src/main.cpp` |
| Texturas | descarte por alfa | alfa < 0.1 | `Shaders/modelLoading.frag` |
| Fichas | radio de proximidad | 2 m | — (C-7) |
| Modelos propios | meta / planeados / estricto (sin planos) | ≥ 18 / 23 / 18 (19/10, con 4 tumbas, roca y copalero) | ESTADO, sección 2 (inventario) |

---

## 6. Problemas abiertos

| # | Problema | Se resuelve en |
|---|---|---|
| 3 | Texturas de la capilla provisional de 2048 px (la propuesta pide 1K). No se reducen: la capilla se reemplaza en B-9. (Los problemas #1 y #2, `mtllib` y material de la capilla, y la calavera se cerraron en A-1, 19/10) | B-9 |
| 5 | Falta iluminación: el shader actual solo muestra la textura | A-8 |
| 7 | No se libera memoria de GPU (VAO/VBO/EBO, texturas, programa de shader). Se hace con `cleanup()` explícito, no con destructores (decisión del 19/10) | E-1 (o antes, al tocar esas clases) |
| 8 | `Model.h` no carga mapas normales/de relieve (necesario para el pan) | B-12 |
| 9 | `fachada.obj` incluye el techo (objeto `roof`); según la definición de modelo propio, el techo va en su propio `.obj` | A-11 |
| 12 | La apertura de `fachada.obj` **no es de medio punto**: 1.18 m de ancho, arranca a 1.14 m y sube 0.97 m hasta la clave (2.11 m); un medio punto de ese ancho sube 0.59 m. Además está descentrada 0.03 m en X. Hay que ajustarla para que encaje el arco con dovelas (o registrar la decisión de dejarla así). Venía así desde antes; A-2 solo la escaló | A-10 / A-12 |

> **Cerrados en A-2 (20/10):** #4 escalas fuera de 1 u = 1 m, #6 material sobrante `Material` (desapareció al reexportar) y #11 tamaño del arco (ahora 2.11 × 1.18 m).

---

## 7. Siguiente paso concreto

1. **D2:** volver a pegar las instrucciones de `docs/INSTRUCCIONES_PROYECTO_CLAUDE.md` en el Proyecto de Claude (cambiaron el 19/10) y quitar del proyecto cualquier copia suelta de `docs/`.
2. **D1:** dar su nombre y correo de GitHub para la tabla de autoría de `CLAUDE.md`; aceptar la invitación al repositorio, instalar vcpkg (ver README), compilar `develop` (Release x64).
3. Trello: invitar a D1, asignar responsable y fecha a las tarjetas del Bloque A, y actualizar los títulos de A-1, A-2, A-11, B-10 y B-14 (cambiaron el 19/10).
4. **D2:** confirmar quién verifica mientras D1 no esté (pendiente del 19/10).
5. Cada viernes: captura del Trello y minuta de 3–5 líneas en `docs/manual_tecnico/evidencias/` (decisión del 19/10).
6. Bloque A en este orden: ~~A-1~~ (✅ 19/10, PR #9) → ~~A-2~~ (✅ 20/10, PR #11) → ~~A-3 y A-4~~ (✅ 20/10, PR #12) → ~~A-5~~ (✅ 20/10, PR #13) → ~~A-6~~ (✅ 20/10, PR #14) → ~~A-7~~ (✅ 25/10, rama `feature/a-7-mar`) → A-8 → A-9 → A-10, A-11 y A-12.
7. **A-7:** merge del PR solo con `/verificar A-7` APROBAR (hacer `git pull` de la rama antes de verificar). **Siguiente tarea: A-8** (shader Blinn-Phong: luz ambiental cálida + 3 luces clave; su criterio incluye `Draw(Shader&)` por referencia y nada de `glDelete*` en destructores, decisión del 19/10).
