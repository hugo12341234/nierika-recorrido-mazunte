# Roles ejecutados frente a la Matriz RACI inicial

> **Pendientes al 18/11/2026 (D2):** (1) justificación del trabajo de D1: **pendiente, sin inventar**. Los textos individuales de D1 y D2 (g.4, g.5, j.4, j.5) ya están integrados desde el 18/11.

> Estado al **18/11/2026**. Todo lo que aparece aquí sale del repositorio, de `docs/ESTADO_PROYECTO.md` o de la propuesta v2.0.
> Cada afirmación lleva su fuente o está marcada **«sin evidencia»**. No se corrige ni reescribe el historial de Git.
> Marcas: **[git]** = verificado con `git log` en todas las ramas · **[PR]** = verificado con la API de GitHub · **[ESTADO]** = lo dice el ESTADO (puede ser palabra de D2) · **[inferido]** = deducción, no confirmada.

D1 = 319266894 · D2 = 321332086.

## 1. Qué se planeó (propuesta v2.0, Tabla 8 y `PLAN_ACTIVIDADES.md`)

El plan original repartía las 60 actividades casi por mitades, con un integrante **responsable (R)** y el otro **verificador (A)** de cada una:

| | Actividades como R | Horas estimadas como R |
|---|---|---|
| D1 | 29 | 88 h |
| D2 | 31 | 85 h |

Cálculo sobre la tabla de `docs/PLAN_ACTIVIDADES.md` (columnas *Resp.* y *Horas*). Las horas son **estimaciones de la propuesta**, no horas reales; no existe registro de horas reales de ninguno de los dos.

Por área (Tabla 8, ya transcrita en la sección e)): D1 era R en investigación (1), configuración de VS (4), bocetos del cuarto (7), cámara (9), colisiones (12), arena (13), ciclo de render (15), fachada (16), cruz (19), texturas de piedra (21), vela (23), botella (26), pan (27), copalero (30), texturas PBR (31, 34), clase `Model` (35), ajuste del cuarto (38), olas (39), flores flotando (42, 43), caminata (47), dejar flores (49), fichas ImGui (51), justificación de complejas (53), correcciones de cámara (55), Release x64 (56), manual técnico (58) y Release en GitHub (60).

## 2. Qué muestra realmente el repositorio

### 2.1 Autoría en Git (corte de la mañana del 18/11/2026, todas las ramas) **[git]**

| Dato | Valor |
|---|---|
| Commits distintos en todas las ramas | 469 |
| Autor «Hugo Chacón» (`152136307+hugo12341234@users.noreply.github.com`) | 460 (148 de ellos con *committer* «GitHub»: merges por la web) |
| Autor «Hugo Chacón» con correo genérico `noreply@users.noreply.github.com` | 8 |
| Autor «Claude» (commit de plataforma) | 1 |
| Commits con otro autor | **0** |
| Commits con `Co-Authored-By` de un integrante | **0** (los 15 con `Co-Authored-By` son de Claude) |
| Commits a nombre de D1 | **0** |
| PR abiertos en el repositorio | 150 (todos por `hugo12341234`) **[PR]** |
| PR con revisión, comentario o mención de `thecurlyguy` | **0** (búsqueda en GitHub: `involves`, `commenter`, `reviewed-by`) **[PR]** |
| Comentarios en PR e *issues* por otra cuenta | **0** **[PR]** |

*Esta tabla es un corte del 18/11 por la mañana. Al cierre del 18/11, la API de GitHub da para la rama `develop` 491 commits (489 de «Hugo Chacón» y 2 de «Claude»), 157 PR fusionados a `develop` y PR numerados hasta el #159; la conclusión no cambia: ningún commit, PR ni revisión de D1.*

Los commits se concentran en 15 días con actividad: 18–20/10 (61), 25–26/10 (3), 03–04/11 (105), 11–18/11 (300). **No hubo commits** el 21–24/10, 27/10–02/11 ni 05–10/11 **[git]**. La participación de D2 es continua pero **irregular**; la de D1 **no se puede mostrar con el historial**.

> **Aviso sobre la lectura del historial:** una parte importante de los commits de D2 fue producida con ayuda de Claude (Claude Code y los hilos del Proyecto web), bajo instrucciones y revisión de D2, y sin `Co-Authored-By` en la mayoría (por regla del repositorio, CLAUDE.md §5). El número de commits **mide actividad en el repositorio, no horas de trabajo humano** de nadie.

### 2.2 Lo que consta de D1 (el aporte fue entregado a D2 fuera de Git)

| Aporte | Evidencia en el repo | Qué se puede afirmar | Qué **no** |
|---|---|---|---|
| **Iluminación** (Práctica 8 adaptada: 1 direccional, 4 puntuales animadas con seno de 2t, 1 reflector en la cruz, 6 en los escalones) | ESTADO §4 del 04/11 («luces de D1»); PR #16 (A-8), fusionado el 03/11 (hora de México); `Shaders/lighting.vs/.frag`; análisis en `PLAN_INTEGRACION_D1_2026-11-04.md` (archivos `lighting.*`, `lamp.*`, `REPORTE8_GPO8.cpp`) **[ESTADO]** | D2 decidió usar la base de D1; D2 la corrigió para que funcionara (Blinn-Phong, samplers, alfa, `escalonSpots[]`, posiciones) | Que D1 fuera el autor de cada línea: los commits son de D2; el problema #13 (¿animación copiada de la práctica?) se cerró **solo por palabra de D2** |
| **Pan, flor y calavera** (Maya) | PR #17 (X-3), `Models/pan`, `Models/flor`, `Models/calavera` **[ESTADO]** | Existen en el repo como «extras de D1»; **no cuentan** entre los 18 modelos propios (modelados en Maya, no en Blender) | Fechas y archivos fuente de D1 (los originales de 2048 px quedaron con D1/D2, fuera del repo) |
| **Modelo de la silueta** (10 piezas `.obj` + `silueta.blend`) | PR #39 (D-1), 04/11 **[ESTADO]** | Estuvo en `Models/silueta/` desde el 04/11; el 12/11 D2 lo sustituyó por piezas orgánicas generadas con `tools/x7_silueta.py` (X-7) | Que quede algo de ese modelo en la versión final |
| **Modelos de Maya del altar** (cuadro, fuego, humo, mariposa, maceta, refresco, papeles) y **escena de referencia en video** | X-43, X-44, X-47, X-49; PR #111, #112, #115 y commits `6f80dc1`, `d4409f8` **[ESTADO]** | D1 se los envió a D2, que los redujo a 1K y los integró; hoy se usan en la escena (fuego, humo, papeles, macetas, cuadro) | La herramienta con que se hicieron (el ESTADO la deja «sin confirmar») |
| **Base del código** (Práctica 6, `319266894_REPORTE6_GPO8`) | ESTADO, sección de código, y PR #1 (act. 4) | El nombre del proyecto base lleva el número de cuenta de D1 **[inferido: la base proviene de un reporte de D1]**; el PR #1 es de D2 | Quién configuró VS y vcpkg (el commit es de D2) |

### 2.3 Lo que **no** consta de D1

- **Ningún commit, PR, revisión ni comentario** de D1 en GitHub **[git][PR]**.
- **Ninguna verificación `/verificar`** hecha por D1: las 9 tareas con APROBAR (A-1, A-2, A-5, A-6, A-7, A-8, X-3, B-9, B-10) las registró D2 (en A-8 y X-3 «según D2») **[ESTADO §2]**.
- **Sin evidencia** de que D1 haya ejecutado ninguna de las otras 25 de sus 29 actividades como R (cámara, colisiones, `Model`, olas, flores flotando, KeyFrames, ImGui, justificaciones, Release…). En el repo esas tareas figuran hechas desde cuentas y sesiones de D2.
- **Sin evidencia** del uso de Trello por D1: no se puede consultar el tablero desde la nube. El ESTADO pendiente 3 (19/10) dice «invitar a D1, asignar responsable y fecha» y no hay registro de que se haya cerrado **[ESTADO]**.
- Trello, Google Sheets y Maya son fuentes **fuera del repositorio** que D2 puede consultar y que habría que revisar antes de afirmar algo más.

### 2.4 ¿`thecurlyguy` es D1?

- **Dato objetivo:** `thecurlyguy` (id 125844014) es colaborador del repositorio con permiso **write**; el único otro es `hugo12341234` (admin) **[API de GitHub, 18/11]**. El 21/10 la lista de colaboradores solo mostraba a `hugo12341234` (nota del hilo de esa fecha); por tanto la cuenta se unió **después del 21/10** (por aceptar una invitación o por una nueva) **[inferido]**.
- **A favor de que sea D1 [inferido]:** el ESTADO dice que D1 fue invitado como colaborador (18/10), «pendiente de aceptar», y no se registra ninguna otra persona invitada.
- **En contra o sin soporte:** no tiene commits, PR, comentarios ni revisiones; el ESTADO nunca registró un nombre de usuario de D1 (la tabla de `CLAUDE.md` §5 dice «pendiente»); no se pudo consultar su perfil (el entorno bloquea ese endpoint).
- **Confirmación (D2, 18/11/2026):** Hugo indicó que `thecurlyguy` es la cuenta de D1 (palabra de D2; no verificable desde el repositorio). No se cambió ningún permiso: la cuenta ya puede subir por PR.
- **Estado de la justificación del trabajo de D1: PENDIENTE.** D2 declara que no puede justificar con evidencia el trabajo de D1 más allá de lo listado en §2.2 (palabra de D2 y ESTADO). Hasta que D1 aporte su propia evidencia, **no se afirma nada más**.

## 3. Matriz RACI: planeada frente a ejecutada

Leyenda: «R real» = quién ejecutó según el repo/ESTADO. Cuando una tarea del repo se hizo desde las cuentas y sesiones de D2, se escribe **D2** (con ayuda de Claude cuando el ESTADO lo registra: B-3, B-4, B-7, B-9, B-10, B-12, E-2, D-6, y desde el 04/11 casi todo el código).

| Área (actividades) | Plan (propuesta) | R real según evidencia | Lectura | Evidencia / observaciones |
|---|---|---|---|---|
| Investigación y propuesta (1–2) | **D1** R 1 · A 1<br>**D2** R 1 · A 1 | D2 (propuesta v2.0 en el repo); investigación (act. 1): **sin evidencia de autoría de D1** | Sin evidencia del aporte de D1 | `docs/propuesta/`; `docs/investigacion_mazunte.md` y `REFERENCIAS.md` están a nombre de D2 |
| Infraestructura (3–6) | **D1** R 1 · A 3<br>**D2** R 3 · A 1 | **D2** (repo, Trello, vcpkg, VS); base de código de un reporte de D1 **[inferido]** | Concentrado en D2 | PR #1; ESTADO act. 3–6; X-1 (versiones de Blender/GIMP en ambas PC) sigue sin captura |
| Cuarto del cementerio, diseño (7) | **D1** R 1 · A 0<br>**D2** R 0 · A 1 | Sin evidencia de bocetos de D1 | Sin evidencia del aporte de D1 | ESTADO X-2 (bocetos) pendiente |
| Playa, mar y panteón (8, 10, 13, 14) | **D1** R 1 · A 3<br>**D2** R 3 · A 1 | **D2** (A-5, A-6, A-7, B-9) | Concentrado en D2 | ESTADO; arena Poly Haven; Blender 5.2.2 |
| Cámara, render y colisiones (9, 11, 12, 15) | **D1** R 3 · A 1<br>**D2** R 1 · A 3 | **D2** (A-9, A-3/A-4, E-2) | Concentrado en D2 | ESTADO A-9; **ninguna** ejecutada por D1 |
| Cuarto del cementerio (16–22) | **D1** R 3 · A 4<br>**D2** R 4 · A 3 | **D2** (A-2, A-10, A-11, A-12, B-8, B-10, B-11) | Concentrado en D2 | PR #11 y siguientes |
| Objetos de la ofrenda (23–30) | **D1** R 4 · A 4<br>**D2** R 4 · A 4 | **D2** con scripts `bpy` y ayuda de Claude (B-1…B-8, B-14). D1: pan, flor y calavera de Maya como **extras** (X-3), que no sustituyen a B-3/B-4 | Parcial: aporte de D1 (extras de Maya) | PR #17; inventario del ESTADO §2 |
| Texturizado (31–34) | **D1** R 2 · A 2<br>**D2** R 2 · A 2 | **D2** (B-12, X-6, texturas por script) | Concentrado en D2 | `tools/b12_texturas.py`, `x6_texturas.py` |
| Integración e iluminación (35–38) | **D1** R 2 · A 2<br>**D2** R 2 · A 2 | **D2** integra; **D1 aporta la base de la iluminación (A-8)** | Parcial: aporte de D1 (base de la iluminación) | PR #16; ESTADO §4 04/11 |
| Animaciones simples (39–45) | **D1** R 3 · A 4<br>**D2** R 4 · A 3 | **D2** (C-1…C-8) | Concentrado en D2 | ESTADO; animación de las 4 luces de D1 (seno de 2t) se acepta como propia por palabra de D2 |
| Animaciones complejas (46–50, 53) | **D1** R 3 · A 3<br>**D2** R 3 · A 3 | **D2** (D-2…D-7, X-7); D1 aportó el primer modelo de la silueta (D-1, PR #39), luego sustituido | Parcial: aporte de D1 (primer modelo de la silueta, sustituido) | `Silueta.cpp`, `Keyframes.*` en `src/` |
| Fichas y audio (51–52) | **D1** R 1 · A 1<br>**D2** R 1 · A 1 | **D2** (C-7, D-6; voz de narración con ElevenLabs, confirmado por D2) | Concentrado en D2 | `docs/REFERENCIAS.md` |
| Pruebas, videos, manuales y entrega (54–60) | **D1** R 4 · A 3<br>**D2** R 3 · A 4 | **D2**; los videos MP4, el Release y el manual en inglés siguen pendientes al 18/11 | Concentrado en D2 | `docs/entrega/LISTA_VERIFICACION.md` |

**Columna A (quien aprueba):** el plan asignaba al otro integrante revisar cada actividad. En la práctica D2 aprobó todo (`/verificar` en sesiones de Claude, visto bueno en pantalla) y, por decisiones escritas del ESTADO (19/10, 11/11, 12/11, 14/11, 17/11, 18/11), los PR se fusionaron **sin revisión de otra persona**. Sin evidencia de revisión humana independiente en ningún PR.

**Cambio formal del reparto (ESTADO §4, 18/10):** las columnas Resp./Verif. dejaron de seguirse; «quien está libre toma la siguiente tarea y el otro revisa». En los hechos no hubo segundo integrante que revisara.

### Lectura de la diferencia

- **Resumen por área (13 áreas):** el trabajo real quedó **concentrado en D2 en 8 áreas**, hay **aporte parcial de D1 en 3** (extras de Maya en objetos de la ofrenda, base de la iluminación y primer modelo de la silueta) y **sin evidencia del aporte de D1 en 2** (investigación y bocetos del cuarto).
- **Responsable (R):** el plan repartía 29 actividades a D1 y 31 a D2; en el repositorio constan como hechas por D2 casi todas, con las excepciones de la lista anterior. No hay registro de horas reales de ninguno de los dos.
- **Aprobador (A):** el plan asignaba cada actividad a un aprobador distinto del responsable. Según el registro, esa revisión por una segunda persona **no se produjo**.
- **Qué no se afirma:** que el plan se cumplió, ni que D1 hizo lo que el repositorio no muestra. La causa del reparto real queda para que la redacten D1 y D2 con sus propias palabras.

## 4. Redacción propuesta (para que D1 y D2 la ajusten; no la cierra este hilo)

> **Participación y reparto real del trabajo.** La matriz RACI de la propuesta repartía las 60 actividades casi por mitades (29 y 31 como responsables). La ejecución real fue distinta: el desarrollo, el modelado, la integración y la documentación se concentraron en el integrante D2 (321332086), con apoyo de herramientas de asistencia (Claude), mientras que el integrante D1 (319266894) aportó, fuera del repositorio, la base de la iluminación (Práctica 8 adaptada), los modelos de Maya de pan, flor, calavera, fuego, humo, cuadro y otros objetos del altar, y un primer modelo de la silueta. Esas aportaciones se integraron en el repositorio mediante commits de D2 (PR #16, #17, #39, #111, #115), por lo que el historial de Git no muestra la autoría de D1. [**D1 y D2: completar la causa real con sus propias palabras, p. ej. carga académica, coordinación de horarios o acceso a la herramienta; este hilo no la conoce y no la supone.**]
>
> **Impacto en el cronograma.** Al no cumplirse el reparto por mitades, la carga prevista de ≈173 h de la propuesta quedó prácticamente en una persona; el 18/10 se replanteó el trabajo (quien está libre toma la siguiente tarea). Hubo semanas sin commits (21–24/10, 27/10–02/11 y 05–10/11) y se acumuló trabajo para la última semana (300 de 469 commits entre el 11 y el 18/11), lo que redujo el tiempo de pruebas independientes: la mayoría de los PR se fusionaron sin revisión de un segundo integrante y varias tareas se dieron por buenas con la verificación visual de D2, sin otra persona que las revisara.
>
> **Medidas tomadas.** Se documentaron las excepciones a la regla de revisión en el ESTADO, se redujo el alcance de los entregables a lo verificable y, el 18/11, D1 aportó por primera vez desde su propia cuenta de GitHub [**solo si ocurre: sus PR y archivos**].

**Lo que no debe escribirse:** que el RACI planeado se cumplió; horas reales (no se registraron); que D1 haya escrito o revisado algo que el repo no muestra.

## 5. Qué hacer hoy para que la sección sea verdadera y completa

1. ~~Preguntar a D1 su usuario de GitHub~~: confirmado por D2, es `thecurlyguy` (§2.4).
2. **D1:** leer y corregir §2.2 (¿qué hizo realmente, con qué herramienta, cuándo?) y aportar la fuente (archivos `.ma`/`.blend`, capturas, fechas). Todo eso entra por PR **desde su cuenta** (guía `GUIA_D1_HOY.md`).
3. **Hugo y D1:** completar la causa en §4 y decidir si se publica tal cual.
4. **Hugo:** consultar el tablero de Trello (tarjetas, miembros asignados, actividad) y añadir aquí lo que muestre; esta sección no pudo acceder a él.
5. Registrar en el ESTADO §4 la decisión de qué RACI real se publica (CLAUDE.md §1).
