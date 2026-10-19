# CONTEXTO DEL PROYECTO — Nierika Studio (qué prometimos)

> **Qué es este archivo:** el resumen de la **Propuesta técnica v2.0 (4 oct 2026)** y del **Plan de trabajo**
> (PDFs en `docs/propuesta/`), con los cambios ya aprobados por el equipo. Dice **qué** se entrega y **cómo** debe ser;
> el avance real está en `ESTADO_PROYECTO.md`.
> **Orden de autoridad:** (1) Registro de decisiones de `ESTADO_PROYECTO.md` → (2) este archivo →
> (3) `PLAN_ACTIVIDADES.md` y los PDFs. Si una conversación o un documento contradice este archivo, gana este archivo,
> salvo que la diferencia esté registrada como decisión.

---

## 1. Ficha

| Campo | Valor |
|---|---|
| Materia | Computación Gráfica e Interacción Humano-Computadora (CGeIHC), FI UNAM, semestre 2027-1, Grupo 01 |
| Profesor (cliente) | Arturo Pérez |
| Equipo | Equipo 15 → se presenta como empresa **Nierika Studio** («Ver más allá de lo visible») |
| Integrantes | **D1 = 319266894**, **D2 = 321332086** (solo 2 integrantes) |
| Proyecto | Recorrido virtual interactivo 3D: **Día de Muertos en Mazunte, Oaxaca** |
| Momento representado | Del atardecer a la noche del 1 de noviembre |
| Periodo | Lun 28 sep 2026 → **Mié 18 nov 2026 (entrega final)** |
| Esfuerzo | 60 actividades · 173 h ejecución + 30 h verificación = 203 h |
| Precio propuesto | $31,228.27 MXN con IVA |

**Reglas de formato del profesor para documentos:** formato profesional/empresarial, **sin logos de la escuela ni datos personales** (solo números de cuenta), costos justificados con fuentes reales, 3 alternativas técnicas por necesidad, plan por semana con responsable, referencias APA 7 de todo.

---

## 2. Alcance

### Incluye
- Escenario continuo con **3 zonas**: (1) playa y orilla, (2) panteón con barda de piedra, (3) cuarto del cementerio con altar.
- Navegación **primera persona** (teclado + ratón) **con colisiones**.
- **5 animaciones**: 2 complejas por KeyFrames + 3 simples.
- **5 puntos de información cultural** con su fuente.
- Iluminación: **luz ambiental cálida + 3 luces clave**, modelo **Blinn-Phong**.
- Audio ambiental (un solo ciclo: oleaje + música lejana).
- Proyecto de Visual Studio, repositorio GitHub, **3 videos**, **2 manuales** (técnico y usuario, español e inglés).

### NO incluye (no proponer, no agregar)
- Calles/edificios del pueblo, colina y mirador de Punta Cometa, tapete de arena.
- Versión móvil, web o VR. Multijugador o guardado de partida.
- Reproducción exacta del panteón real (es adaptación del estudio).
- Personajes con diálogo o IA. **Humo de copal con partículas** (el copalero se modela, pero sin humo).

---

## 3. Requerimientos técnicos y criterios de aceptación

| Categoría | Requerimiento | Criterio de aceptación |
|---|---|---|
| Modelado geométrico | Playa, mar, tumbas, barda, cuarto (fachada, arco, piso, paredes, techo), cruz, altar y 7 objetos de ofrenda | **≥ 18 modelos propios** (qué cuenta como modelo: decisión del 19/10 e inventario en `ESTADO_PROYECTO.md`); externos solo secundarios y con crédito |
| Modelado jerárquico | Altar de 3 niveles; silueta con hombro, codo, cadera, rodilla por lado | Al mover el padre, los hijos lo siguen |
| Texturizado | Arena, piedra rústica, madera, mantel bordado, barro, vidrio, papel picado con transparencia | Texturas **1K**, sin estiramientos visibles |
| Iluminación | Ambiental cálida + 3 luces clave, Blinn-Phong | El interior del cuarto se distingue sin perder el ambiente nocturno |
| Animación | 5 animaciones (2 KeyFrames + 3 simples) | **Ninguna proviene de las prácticas del curso**; cada una con justificación matemática documentada |
| Interacción | Fichas al acercarse a 5 objetos | La ficha aparece a **< 2 m** y **cita su fuente** |
| Recorrido | Cámara 1ª persona con colisiones | No se atraviesa el terreno ni las bardas |
| Rendimiento | Laptop con gráficos integrados | **≥ 30 FPS a 1280×720** |

---

## 4. Animaciones (restricción crítica del profesor)

El profesor **NO acepta como KeyFrames**: velas flotando, fuego, papel picado ni humo (el humo solo cuenta como extra).

| # | Animación | Tipo | Técnica |
|---|---|---|---|
| 1 | Caminata de silueta (orilla → altar) | Compleja, **KeyFrames** | Rotaciones por articulación interpoladas con **slerp**; avance suavizado con **smoothstep**; piernas desfasadas medio ciclo; brazos en oposición |
| 2 | Silueta dejando flores en el altar | Compleja, **KeyFrames** | Brazo se extiende entre cuadros clave; la flor sigue **Bézier cuadrática** mano → nivel del altar, con caída final |
| 3 | Olas del mar | Simple | **Vertex shader**: altura = suma de senoidales (frecuencias y amplitudes definidas) |
| 4 | Velas flotando | Simple | Trayectoria **Bézier** mar adentro + oscilación senoidal vertical; desfase por vela; llama con **fragment shader con ruido** (Perlin o similar) |
| 5 | Flores de cempasúchil flotando | Simple | Movimiento circular de radio pequeño + rotación continua; velocidad variable con pausas |

**Silueta (jerarquía):** Torso (raíz) → Cabeza, Brazo izq/der → Antebrazo izq/der, Muslo izq/der → Pierna izq/der. Cada cuadro clave guarda posición del torso + rotación de cada articulación.
**Línea de tiempo:** Caminata 0 s → Llega al altar 6 s → Extiende brazo 7 s → Suelta la flor 8 s → Retrocede 10 s.

---

## 5. Recorrido (paradas y fichas)

| # | Parada | Qué ve | Animación / interacción |
|---|---|---|---|
| 1 | Llegada a la playa | Mar al atardecer | Olas |
| 2 | Orilla | Velas y cempasúchil sobre el agua | Velas y flores flotando; **fichas: velas, cempasúchil** |
| 3 | Sendero al panteón | Camino orilla → panteón | Caminata de la silueta |
| 4 | Panteón | Tumbas, barda, cruz, arco con papel picado | **Ficha: papel picado** |
| 5 | Cuarto del cementerio | Altar de 3 niveles con 7 objetos | Silueta dejando flores; **fichas: altar, pan de muerto** |

**Objetos de la ofrenda (7):** vela, botella de refresco de vidrio, pan de muerto, flor de cempasúchil, marco con fotografía, papel picado, copalero de barro (sin humo; obligatorio para el conteo de modelos, decisión del 19/10) — sobre altar de 3 niveles.

**Fuentes culturales por ficha** (nivel de evidencia): flores y velas en el mar → Baños (2025), *local documentado*; velas, cempasúchil, copal, pan, papel picado, agua y sal → SEP (2026), *tradición general*; cuarto del cementerio → UNESCO (s. f.), *adaptación del estudio*. Lo que sea adaptación debe decirlo en la ficha.

---

## 6. Stack tecnológico (no cambiar sin registrar decisión)

- **C++17**, **OpenGL 3.3 Core Profile**, **GLSL 330**, **Visual Studio 2022**, compilación **x64** Debug/Release.
- **GLFW** (ventana/entrada) · **GLEW** (carga de funciones; la propuesta decía GLAD, se cambió el 18/10) · **GLM** (matemáticas) · **Assimp** (.obj) · **SOIL2 / stb_image** (texturas) · **Dear ImGui** (fichas) · **miniaudio** (audio).
- Assimp, GLFW, GLEW y GLM se instalan con **vcpkg** (`vcpkg.json`, versiones fijas por `builtin-baseline`); SOIL2/stb en `vendor/SOIL2` (decisiones del 19/10).
- Base de código: proyecto de la **Práctica 6** del laboratorio (clases del curso `Shader`, `Camera`, `Model`, `Mesh`).
- **Blender** (modelado, UV, export .obj) · **GIMP** (texturas, canales alfa) · **OBS** (videos).
- Texturas **Poly Haven / ambientCG (CC0)**, 1K, albedo + normal.
- **Git/GitHub** (ramas por funcionalidad, PR) · **Trello** (Kanban).
- Referencia técnica principal: **LearnOpenGL** (de Vries, 2020).

**Arquitectura por capas:** App (C++17) → módulos [Cámara y colisiones (GLFW·GLM) | Carga de modelos (Assimp·SOIL2/stb_image) | Shaders e iluminación (GLSL) | Animación (KeyFrames·slerp) | Fichas culturales (Dear ImGui) | Audio (miniaudio)] → OpenGL 3.3 Core (GLEW) → GPU / Recursos (.obj, texturas, audio, keyframes).
Todo movimiento se calcula en la aplicación (keyframes en C++; oleaje en vertex shader).

---

## 7. Convenciones

- **Escala:** 1 unidad = 1 metro (escalar en Blender antes de exportar, no con factores grandes en `main.cpp`). Playa 100×50 m; mar 100×100 m (malla subdividida).
- **Cámara:** pitch bloqueado a ±89°; colisiones **AABB**; ciclo con **deltaTime**.
- **Modelos:** exportar .obj desde Blender **aplicando modificadores**; pivote correcto (vela: pivote en la base); rutas **relativas**.
- **Carpetas:** `src/` código C++; `Models/<objeto>/<objeto>.obj` (+ .mtl y **texturas del objeto en la misma carpeta**; la propuesta habla de una carpeta `textures/`, pero en el repo cada textura vive junto a su modelo); `Shaders/*.vs` y `*.frag`; `vendor/SOIL2` (dependencia en código fuente); `build/` salida de compilación (ignorada); `dist/` con el .exe autónomo + .dll.
- **Git:** ramas `main` (releases), `develop` (integración), `feature/<código>-<nombre>` (código de tarea del ESTADO, p. ej. `feature/a-10-dovelas`); todo entra por **pull request** aprobado por el otro integrante. Commits solo a nombre del integrante.
- **Definición de terminado:** el entregable existe en `develop`, **compila sin errores** y el verificador lo aprobó en el PR.
- **Kanban:** columnas Pendiente / En proceso / En revisión / Terminado; **máx. 2 tarjetas en proceso por integrante**; revisión cada viernes 30 min.

---

## 8. Plan semanal ORIGINAL de la propuesta (ya no rige: ver *Plan vigente* en `ESTADO_PROYECTO.md`)

| Sem | Fechas | Fase | Hito |
|---|---|---|---|
| 1 | 28/09–02/10 | Arranque e infraestructura | Repo, proyecto base compilando, cuarto documentado |
| 2 | 05/10–09/10 | Escenario base y navegación | Playa y mar navegables en 1ª persona |
| 3 | 12/10–16/10 | Cuarto del cementerio y panteón | Fachada, arco, interior y barda modelados y texturizados |
| 4 | 19/10–23/10 | Objetos de la ofrenda | Altar de 3 niveles y 7 objetos modelados |
| 5 | 26/10–30/10 | Texturizado, integración, iluminación | Escena completa cargada, texturizada e iluminada |
| 6 | 02/11–06/11 | Animaciones simples | Olas, velas y flores integradas |
| 7 | 09/11–13/11 | Animaciones complejas, interacción, audio | 2 animaciones KeyFrames, fichas y audio |
| 8 | 16/11–18/11 | Pruebas, empaquetado, entrega | Ejecutable, videos y manuales entregados |

---

## 9. Entregables finales (18 nov 2026)

1. Proyecto de Visual Studio + repositorio GitHub con **Release** publicado.
2. `dist/` con **.exe autónomo** (Release x64) y sus .dll.
3. **3 videos MP4**: (a) entorno, (b) recorrido, (c) animaciones e interactividad.
4. **Manual técnico** (ES + EN): arquitectura, cronograma, RACI, justificación matemática de las 5 animaciones, conclusiones, evidencias (capturas del tablero, commits, minutas).
5. **Manual de usuario** (ES + EN): capturas y explicación de controles e interacciones.
