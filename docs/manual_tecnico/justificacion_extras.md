# Justificación matemática de los extras de ambiente

> Los extras (fase «mejorar y producir», `docs/EXTRAS_Y_MEJORAS.md`) **no son KeyFrames** y **no sustituyen** a las 5 animaciones comprometidas.
> Cada uno usa una fórmula simple, con valores tomados del código. Las pruebas numéricas están en `tools/x19_pruebas_extras.cpp`.

## 1. Balanceo senoidal de las figuras de la ceremonia (X-19)

**Qué es:** las figuras humanas son mallas estáticas con una pose fija. Para que no se vean congeladas se inclinan muy poco alrededor de su pivote.

**Fórmula:** θ(t) = A · sen(2π t / T + φ), en grados (`Extras::Balanceo`, `src/Extras.h`).

| Símbolo | Significado | Valores en el código |
|---|---|---|
| A | amplitud máxima del giro | 1.2° de pie (ladeo, eje Z); 2.0° de rodillas (cabeceo al respirar, eje X) |
| T | periodo | 4.5 s de pie; 5.0 s de rodillas |
| φ | fase, distinta por figura | de 0 a 5.0 rad (`Extras::FIGURAS[i].fase`): las figuras no se mueven al unísono |
| t | tiempo del programa (`Reloj()`) | s |

**Por qué un seno:** es suave (su derivada es continua), periódico y no necesita guardar poses. A ≤ 2° da un desplazamiento máximo de la cabeza de 1.7 m · sen(1.2°) ≈ 3.6 cm: se nota como «respiración», no como caminata.

**Cómo se comprueba:** `tools/x19_pruebas_extras.cpp` verifica que |θ| ≤ A, que el periodo es T y que dos fases distintas dan valores distintos.

## 2. Tiras de papel picado como péndulos de pequeña amplitud (X-20)

**Qué es:** las 17 tiras de los 2 tendederos de la playa (9 + 8 bastidores) se mecen con la brisa.

**Fórmula:** el mismo `Extras::Balanceo`, θ(t) = A · sen(2π t / T + φ), aplicado como giro alrededor del **borde superior** de la tira (eje X, pivote del `.obj` en su borde de arriba).

| Símbolo | Valor | Por qué |
|---|---|---|
| A | 6° | una tira de papel de 0.5 m se desplaza ≈ 0.5 · sen(6°) ≈ 5 cm en su punto más bajo: se nota sin parecer una bandera de viento fuerte |
| T | 3.6 s | para un péndulo de largo L el periodo natural es T = 2π√(L/g); con L = 0.5 m da ≈ 1.4 s, pero el papel está amortiguado por el aire y se mueve con ráfagas más lentas: se eligió 3.6 s para un vaivén calmado |
| φ | 0.8 · i + 2.1 · (tendedero) | las tiras vecinas no oscilan al unísono (evita el efecto «ola uniforme») |

**Comprobación:** `tools/x19_pruebas_extras.cpp` (|θ| ≤ 6° y periodo 3.6 s).

## 3. Personas sobre tablas de remo que siguen la ola (X-21)

**Qué es:** 5 tablas con una persona de pie cada una (pose fija, sin IA) navegan despacio por la bahía y suben y bajan con el oleaje. **No hay KeyFrames.**

**Trayectoria (círculo lento):** (x, z)(t) = (cₓ + R·cos(ωt + φ), c_z + R·sen(ωt + φ)), con ω = 2π/70 s. Con R = 3 m la tabla avanza 2π·3/70 ≈ 0.27 m/s (paso de paseo tranquilo en el agua). La dirección de la proa es la tangente (−sen a, cos a) y el giro de la tabla es `yaw = atan2(−dx, −dz)`.

**Altura y pendiente (de las olas ya justificadas en C-1):**
- y(t) = `Olas::Altura(x, z, t)` + 0.03 m: la altura de la suma de tres senos de `src/Olas.h`.
- La vertical de la tabla se alinea con la normal de la superficie n = normalize(−∂h/∂x, 1, −∂h/∂z) (`Olas::Normal`): se gira un ángulo α = arccos(nᵧ) alrededor del eje u = (0,1,0) × n. En las olas de `Olas.h` el α máximo **medido** (`tools/x19_pruebas_extras.cpp`) es **3.4°**: la tabla cabecea y alabea poco, como una tabla larga sobre olas pequeñas.

| Parámetro | Valor | Por qué |
|---|---|---|
| R | 3 o 4 m | la tabla (3.2 m) se mantiene cerca de la orilla visible desde la playa |
| PERIODO_TABLA | 70 s | vuelta lenta y calmada |
| φ | 0, 1.2, 2.4, 3.6, 4.8 rad | las 5 tablas no van en fila |

**Comprobación:** `tools/x19_pruebas_extras.cpp` verifica que la posición recorre el círculo (distancia al centro = R), que la tangente es unitaria y perpendicular al radio, y que α ≤ 6° en el rango de las olas (medido: 3.39°).

## 4. Luciérnagas con trayectorias de Lissajous y parpadeo senoidal (X-22)

**Qué es:** 48 puntos de luz verdosa que flotan sobre la playa y el panteón. **No** son un sistema de partículas con física ni humo: cada punto se calcula en forma cerrada.

**Posición:** p(t) = c + (Aₓ·sen(aₓt + φ), A_y·sen(a_yt + 2φ), A_z·sen(a_zt + 3φ)), con Aₓ = A_z = 1.6 m, A_y = 0.45 m y frecuencias aₓ = 0.41k, a_y = 0.67k, a_z = 0.53k (k = 1 + 0.07·(i mod 5)).
Los cocientes 0.41 : 0.67 : 0.53 son casi irracionales, así que la curva de Lissajous tarda mucho en cerrarse y no se ve un movimiento repetitivo. El centro c y la fase φ de cada luciérnaga salen de una función hash fija (`Extras::LuciernagaN`), por lo que siempre aparecen en los mismos lugares.

**Parpadeo:** brillo(t) = máx(0, sen(2πt/2.6 + 3φ))³. El cubo hace los destellos cortos y deja la luciérnaga apagada la mitad del tiempo; el brillo siempre está en [0, 1] y se envía al shader como intensidad (`llama = 2` en `lighting.frag`: color emisivo sin luz).

**Comprobación:** `tools/x19_pruebas_extras.cpp`: brillo en [0, 1], desplazamientos ≤ 1.6 m y ≤ 0.45 m, y ninguna luciérnaga queda apagada para siempre.

## 5. Espuma en la orilla y resplandor del ocaso (X-22)

**Espuma:** en el fragment shader del mar, f = smoothstep(26.2, 24.3, z) · smoothstep(0.02, 0.30, n + 0.18 + 0.18·sen(0.8t − 1.6z)), con n = ruido de Perlin fBm(0.7x, 0.9z − 0.45t) (el mismo ruido de la llama, C-2).
El primer factor limita la espuma a la franja z ∈ [24.3, 26.2] (donde el mar toca la arena); el ruido que avanza con −0.45t y el seno que viaja con −1.6z hacen que las manchas se muevan hacia la playa como las olas. Se mezcla el color del mar con blanco azulado (0.80, 0.88, 0.95), tope 0.85.

**Ocaso:** en `sky.frag`, ocaso = e^(−9h) · (0.35 + 0.65·smoothstep(−0.2, 0.9, d_z)), con h la altura del rayo sobre el horizonte y d_z su componente hacia el mar; se suma (0.55, 0.26, 0.10) · ocaso · 0.55. La exponencial concentra el color en el horizonte y representa el «del atardecer a la noche» de la propuesta.

## 6. Relieve a partir de la luminancia de la textura (X-23)

**Qué es:** la arena y la piedra (barda, tumbas, cruz) reciben relieve **sin mapa de normales aparte**: el mapa de altura es la luminancia de la propia textura, h = 0.30 R + 0.59 G + 0.11 B (pesos de luminancia habituales), multiplicada por 1.2 cm (arena) o 3.5 cm (piedra).
La normal se perturba con el método de Mikkelsen (2010), que usa las derivadas de pantalla dFdx/dFdy de la posición y de la altura (ya usado para el pan de muerto en B-12): N' = normalize(|det|·N − sign(det)·(∂h/∂x·(∂p/∂y × N) + ∂h/∂y·(N × ∂p/∂x))). No necesita tangentes ni UV especiales.

**Limitación honesta:** una textura clara no siempre es «alta»; el resultado es un relieve plausible, no la geometría real. Para una arena o piedra con normal maps reales hay que descargar texturas PBR (Poly Haven) y asignarlas por material.

## 7. Narrativa de la ruta: sendero, faroles y figuras (X-24)

**Sendero de cempasúchil.** El eje del camino serpentea: x(z) = 0.9·sen(0.35·z). Entre la orilla (z = 22.4) y la puerta del cuarto (z = 2.3) se coloca una flor cada 0.28 m de z, con una dispersión determinista de ±0.45 m en x y ±0.12 m en z (hash de seno: la misma flor siempre cae en el mismo lugar). La amplitud 0.9 m deja el eje dentro de las dos puertas de la barda (|x| < 1 en z = 3 y z = 12) y las flores que cayeran sobre un muro no se dibujan (`Extras::FlorSenderoEn`). El sendero **no es una trayectoria de la silueta**: ella sigue en x = 0.

**Parpadeo de los faroles.** brillo(t) = 0.55 + 0.30·sen(5.1 t + 1.7 i) + 0.15·sen(11.3 t + 0.9 i), con i el índice del farol: suma de dos senos de frecuencias no múltiplos, que oscila entre 0.10 y 1.00 y no se repite al unísono. El color emisivo es (1.0, 0.62, 0.22)·(0.55 + 1.1·brillo).

**Músico.** La guitarra se mece alrededor de su ángulo base de 58° con θ = 4°·sen(2πt/1.3 + φ) (≈ 46 pulsaciones por minuto: un compás tranquilo). Es un balanceo del objeto, no una animación de manos.

**Murciélagos.** Vuelan en círculos de radio R (10 a 26 m) a rapidez lineal v = 4.5 m/s, así que ω = v/R y (x, z) = (cₓ + R·cos(ωt + φ), c_z + R·sen(ωt + φ)); la altura oscila 0.8·sen(0.7t + 2φ) alrededor de 6.5–14 m. Las alas baten θ = 40°·sen(2π·3.2·t + 1.7 i) alrededor del eje del cuerpo (el ala izquierda es el espejo de la derecha). Un murciélago real bate a 10–20 Hz; se usa 3.2 Hz para que el movimiento se perciba a 60 cuadros por segundo.

**Sonido por zona** (`Audio::ActualizarZona`, `src/Audio.cpp`): oleaje = 0.20 + 0.45·clamp((z − 2)/22, 0, 1) (más fuerte hacia el mar); música = 0.10 + 0.40·clamp(1 − d_altar/10, 0, 1); voces = 0.55·clamp(1 − d/14, 0, 1) con d la menor distancia a la ceremonia (11, 19.5) o al panteón (0, 7). Son rampas lineales de la distancia, sin espacialización 3D.

**Comprobación:** `tools/x19_pruebas_extras.cpp` (sendero, murciélagos y las anteriores): ver el resultado en el ESTADO.

## 8. Los lados de la escena (X-25 a X-28)

Todas son fórmulas simples, **sin KeyFrames**; las posiciones y parámetros están en `src/Extras.h`.

| Extra | Fórmula | Valores |
|---|---|---|
| Lanchas ancladas | altura y = `Olas::Altura(x, z, t)`, vertical = normal de la ola (como las tablas de remo, §3); giro = base + 8°·sen(2πt/12 + φ) | 6 lanchas |
| Redes de pesca | péndulo θ = 3°·sen(2πt/5 + 1.3 i) | 3 redes |
| Hamacas | balanceo θ = 2°·sen(2πt/6 + 2 i) | 2 hamacas |
| Aves | círculo (x, z) = (cₓ + R·cos(ωt + φ), c_z + R·sen(ωt + φ)), ω = v/R, v = 2.6 a 3.8 m/s; altura 9–20 m + 1.2·sen(0.4t + φ); alas θ = 25°·sen(2π·0.9·t + 1.3 i) | 7 aves; 0.9 Hz de aleteo (un pelícano real bate más lento que un murciélago) |
| Tortugas marinas | círculo igual de radio 4–6 m a v = 0.4 m/s en la superficie (y = ola + 0.02); cabeceo de aletas θ = 10°·sen(2πt/1.8 + 1.1 i) | 4 tortugas |
| Crías de tortuga | z(t) = Z₀ + ((v·t + 0.53·i·L) mod L), Z₀ = 20.2 m, L = 4.2 m, v = 0.18 m/s; bamboleo 8°·sen(2πt/0.9 + 1.7 i) | 8 crías; cruzan 4.2 m de arena en 23 s y reinician |
| Linternas flotantes | y = `Olas::Altura` + 0.06; brillo = 0.55 + 0.30·sen(3.1t + 1.9 i) + 0.15·sen(7.7t + 0.7 i) | 40 linternas, posiciones por hash fijo en x ∈ [−46, 46], z ∈ [27, 59] |
| Fogatas | llama = ruido de Perlin del shader de las velas (C-2), **sin humo ni partículas** | 2 fogatas |

**Comprobación:** `tools/x19_pruebas_extras.cpp` (círculos, rapideces, rango de las crías y de las linternas).

## 9. Comparsa, música en vivo y alfombras de flores (X-29, fuera del alcance original)

| Extra | Fórmula | Valores |
|---|---|---|
| Comparsa | x(t) = −48 + ((v·t + 48 + s_i) mod 96); paso y = 0.05·\|sen(2π f t + 0.6 i)\|; balanceo 3°·sen(2πt/2 + 0.6 i) | 12 figuras en 2 columnas, v = 1 m/s, f = 1 Hz (un paso por segundo a ritmo de marcha lenta), s_i = 1.3 m·(i div 2) |
| Músicos | balanceo 2°·sen(2πt/1.33 + φ); guitarra 58° + 4°·sen(ωt + φ); tambor sube 1.5 cm·\|sen(ωt/2)\|; trompeta 14° + 3°·sen(ωt/2 + φ), con ω = 2π·90/60 rad/s | 5 músicos (2 guitarras, 1 tambor, 2 trompetas) |
| Música | **Audio/banda.wav sintetizada** (`tools/x29_banda.py`): guitarra con el algoritmo de Karplus-Strong, tambor grave con ruido y trompeta de diente de sierra con vibrato; progresión Am–G–F–E a 90 bpm. **No son grabaciones ni músicos reales**; el volumen = 0.80·clamp(1 − d/24) con d la distancia a (−10, 19.5) | 42 s en bucle |
| Alfombras de flores | Plano texturizado con miles de **pétalos de cempasúchil** (naranja, amarillo y magenta) y hojas verdes con canal alfa; **no es arena de colores** (no es el tapete de arena que sigue fuera de alcance) | 3 alfombras: cruz de 4 × 4 m en el panteón y 2 rectángulos en la orilla |

**Comprobación:** `tools/x19_pruebas_extras.cpp` (rangos y rapidez de la comparsa).
