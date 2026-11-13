# Video 3 — Animaciones e interactividad (≈ 3:00–4:00)

> **Qué muestra:** las 5 animaciones, una por una, con su idea matemática, y la interactividad (cámara, colisiones, luces, fichas).
> **Verificado (de `justificacion_simples.md` y `justificacion_complejas.md`):** valores de abajo con su fuente. **Inferido:** tiempos y qué tan visibles se ven
> en pantalla (hay que probarlo). Regla del cliente: **solo** la caminata y «dejar flores» son KeyFrames.

## Las 5 animaciones (conteo oficial: CONTEXTO §2 y §6 «olas, velas y flores» + 2 KeyFrames)
| # | Animación | Tipo | Idea matemática (para decirla en una frase) |
|---|---|---|---|
| 1 | Olas | Simple | Suma de **3 senoidales** en el *vertex shader*: amplitudes 0.040 / 0.025 / 0.015 m, longitudes de onda 12 / 7 / 4.5 m |
| 2 | Velas flotando | Simple | Trayectoria de **Bézier** mar adentro + vaivén **senoidal** (0.012 m, periodo 2.4 s), 4 velas con desfase 2πj/4. Su **llama** (ruido de Perlin, 3 octavas, en el *fragment shader*) es parte de esta animación, no una sexta |
| 3 | Flores flotando | Simple | **Movimiento circular** de radio pequeño + rotación sobre su eje, con pausas |
| 4 | Caminata de la silueta | KeyFrames | **Slerp** de cuaterniones + **smoothstep**; jerarquía cadera–rodilla, hombro–codo; 18.33 s |
| 5 | Dejar la flor | KeyFrames | **Bézier cuadrática** que coincide con una caída libre (0.38 m en 0.429 s) |

> **Nota de conteo (inferido, confirmar):** el ESTADO (C-5/C-8) nombra la llama aparte (C-2), pero el CONTEXTO cuenta solo 3 simples (olas, velas y flores).
> Este guion trata la llama como parte de las velas para que el video diga «5 animaciones». Si D2 prefiere otro conteo, se registra en el ESTADO y se ajusta el video.

## Tomas a grabar
| Toma | Qué hacer | Duración |
|---|---|---|
| A | Orilla, mirando el mar: dejar correr las olas 10 s | 15 s |
| B | Velas flotando; acercarte a ver el vaivén | 20 s |
| C | Llama de una vela del altar, de cerca (≈ 1 m) | 15 s |
| D | Flores flotando en la orilla | 20 s |
| E | **R** en la orilla; la silueta camina completa (≈ 18 s) vista de lado | 30 s |
| F | La silueta llega al cuarto y suelta la flor (≈ 4 s); repetir con **R** y verla otra vez | 25 s |
| G | Interactividad: caminar hacia un muro (colisión), **barra espaciadora** (luces de colores), acercarse y alejarse de una ficha | 40 s |

## Guion
| Tiempo | Imagen | Narración |
|---|---|---|
| 0:00 | Título «Animaciones e interactividad» | «Este video muestra las animaciones del proyecto y cómo interactúa el visitante.» |
| 0:10 | Toma A | «Primera animación simple: las olas. Se calculan en el *vertex shader* sumando tres senoidales de distintas amplitudes y longitudes de onda; la luz reacciona porque también se calcula su normal.» |
| 0:30 | Toma B | «Segunda: las velas flotan siguiendo una curva de Bézier mar adentro, con un vaivén senoidal desfasado para que cada vela se mueva distinto.» |
| 0:55 | Toma C | «La llama de esas velas parpadea con ruido de Perlin calculado en el *fragment shader*; forma parte de la animación de las velas.» |
| 1:10 | Toma D | «Las flores flotan con un movimiento circular y una rotación continua, con pausas.» |
| 1:35 | Toma E | «Ahora las animaciones por KeyFrames. La silueta camina con una jerarquía de articulaciones; entre posturas interpolamos con *slerp* de cuaterniones y suavizamos con *smoothstep*.» |
| 2:10 | Toma F | «Al llegar al altar extiende el brazo y suelta una flor. Su trayectoria es una Bézier cuadrática que reproduce una caída libre.» |
| 2:40 | Toma G (colisión) | «La cámara en primera persona tiene colisiones: no atravesamos muros ni el altar.» |
| 3:00 | Toma G (espacio) | «Con la barra espaciadora se encienden luces de colores.» |
| 3:15 | Toma G (ficha) | «Y al acercarnos a menos de dos metros aparece la ficha cultural; al alejarnos desaparece.» |
| 3:35 | Cierre | «La justificación matemática de cada animación está en el manual técnico.» |

## Confirmar antes de publicar
- [ ] Confirmar el conteo (la llama dentro de «velas»; ver la nota de arriba).
- [ ] Que cada frase matemática coincida con los valores reales del código (`src/Olas.h`, `src/main.cpp`, `Shaders/lighting.frag`, `src/Animacion.cpp`, `src/DejarFlor.h`); si cambió algo, usar el valor del código.
- [ ] Que «velas, llama, olas y flores flotantes no son KeyFrames» se diga igual que en `justificacion_*.md`.
- [ ] No afirmar que «ninguna animación es del curso» sin haber revisado el argumento de D-7 §5 con D2 (el ESTADO lo deja pendiente).
