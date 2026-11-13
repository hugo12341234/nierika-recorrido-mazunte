# 7. Justificación matemática de las 5 animaciones

> **Estado (12/11/2026):** las 5 animaciones tienen código y justificación con **los valores del código** y su `archivo:línea`. La deducción completa vive en dos documentos que forman parte de este manual:
> - [`justificacion_simples.md`](justificacion_simples.md) — olas, velas, llama y flores (C-8, 11/11).
> - [`justificacion_complejas.md`](justificacion_complejas.md) — slerp, smoothstep, jerarquía y Bézier (D-3, D-7, 12/11; 23 comprobaciones numéricas con `tools/d7_pruebas_matematicas.cpp`).
>
> **Lo que NO está comprobado:** que ninguna animación se vea bien con MSVC y el modelo real (nadie lo ha compilado desde el cambio) y la comparación «no es del curso», porque **las prácticas no están en el repositorio**: la revisa D2. El documento de simples conserva el encabezado de borrador del 11/11 (dice «sin compilar»); sigue siendo cierto.

## 7.1 Resumen de las 5 animaciones ✅ **[repo, ESTADO §5]**

| # | Animación | Tipo | Fórmula / técnica | Valores del código | Tarea |
|---|---|---|---|---|---|
| 1 | Caminata de la silueta (orilla → altar) | KeyFrames | Cuaterniones con **slerp** propio; avance con **smoothstep** (SmoothIn/SmoothOut al arrancar y frenar); piernas desfasadas medio ciclo, brazos en oposición | z = +24 → +0.2 (23.8 m = 17 zancadas de 1.4 m), crucero 1.4 m/s, arranque y frenado 2 s, **18.33 s** en total, ciclo de zancada 1 s; termina en x = +0.45 (D-5, 12/11) | D-2, D-3 |
| 2 | Silueta dejando una flor | KeyFrames | Brazo derecho por KeyFrames con slerp; flor en **Bézier cuadrática** con P1 a la altura de la mano y a medio camino horizontal, de modo que la curva **es** un tiro horizontal en caída libre | tras la caminata: extiende 1 s, suelta 2 s, regresa 3–4 s; hombro +75°, antebrazo +10°; P2 = (1.05, 0.435, z de la mano); T_b = √(2h/g) ≈ 0.429 s con h ≈ 0.901 m | D-4 |
| 3 | Olas | Simple | *Vertex shader*: y = −0.2 + Σ A·sen(k·(d·x) − ωt + φ), ω = √(9.81·k); normal analítica | 3 ondas: A 0.040 / 0.025 / 0.015 m · λ 12 / 7 / 4.5 m · θ 0° / 25° / −30° · φ 0 / 1.7 / 4.1 rad; altura entre −0.28 y −0.12 m | C-1 |
| 4 | Velas flotando | Simple | **Bézier cuadrática** mar adentro + oscilación senoidal vertical con desfase por vela; llama: ruido de Perlin 2D en el *fragment shader* | 4 velas: P0 (x0, 25), P1 (x0 ± 8, 40), P2 (x0 ± 3, 60), x0 = −2.5, 0.5, 2.5, 5.0; T 100–130 s; vaivén 0.012 m con periodo 2.4 s; llama: 3 octavas, ruido sube a 2 unidades/s | C-3, C-2 |
| 5 | Flores de cempasúchil flotando | Simple | Movimiento circular de radio pequeño + rotación continua; velocidad variable con pausas | radio 0.45–0.75 m, ciclo de movimiento 4–5.5 s + pausa 1.5–3 s, giro 0.6–1.0 rad/s, altura = ola de C-1 + 0.02 m | C-4 |

Las animaciones simples dependen del tiempo absoluto (`glfwGetTime()`), no acumulan `deltaTime`; las dos de KeyFrames comparten un solo reloj `tSilueta` (D-5). ✅ **[repo, justificaciones §0 y §4]**

## 7.2 Resolución de la advertencia de los 6 s

La propuesta fijaba 6 s hasta el altar; la distancia real (23.8 m) lo haría imposible (≈ 4 m/s). La **decisión del 04/11** del ESTADO fija **18.33 s** a 1.4 m/s de crucero; manda sobre el CONTEXTO §4. ✅ **[repo]**

## 7.3 Dónde está cada derivación

| Tema | Documento y sección |
|---|---|
| Suma de senoidales, elección de ω, normal analítica | `justificacion_simples.md` §1 |
| Bézier de las velas, vaivén y desfase, llama con ruido | `justificacion_simples.md` §2 |
| Círculo, rotación y pausas de las flores | `justificacion_simples.md` §3 |
| Slerp, jerarquía de la silueta, cuadros clave | `justificacion_complejas.md` §1 |
| Smoothstep y desfase de piernas | `justificacion_complejas.md` §2 |
| Bézier de la flor y caída libre | `justificacion_complejas.md` §3 |
| Integración con `deltaTime` y por qué no son del curso | `justificacion_complejas.md` §4–5 |

## 7.4 Referencia técnica principal

de Vries (2020), *Learn OpenGL* (ver [§11](11_referencias.md)). Slerp, Bézier y ruido de Perlin se citan en cada documento de justificación; ❓ falta añadir sus referencias APA 7 a §11 (Shoemake, 1985; Perlin, 2002 u otras) **solo si D2 confirma que son las fuentes usadas**: este manual no las agrega sin verificarlas.
