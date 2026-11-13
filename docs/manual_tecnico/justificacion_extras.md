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
- La vertical de la tabla se alinea con la normal de la superficie n = normalize(−∂h/∂x, 1, −∂h/∂z) (`Olas::Normal`): se gira un ángulo α = arccos(nᵧ) alrededor del eje u = (0,1,0) × n. Como la pendiente máxima es ≈ Σ A·k ≈ 0.08, α ≤ ≈ 5°: la tabla cabecea y alabea poco, como una tabla larga sobre olas pequeñas.

| Parámetro | Valor | Por qué |
|---|---|---|
| R | 3 o 4 m | la tabla (3.2 m) se mantiene cerca de la orilla visible desde la playa |
| PERIODO_TABLA | 70 s | vuelta lenta y calmada |
| φ | 0, 1.2, 2.4, 3.6, 4.8 rad | las 5 tablas no van en fila |

**Comprobación:** `tools/x19_pruebas_extras.cpp` verifica que la posición recorre el círculo (distancia al centro = R), que la tangente es unitaria y perpendicular al radio, y que α ≤ 6° en el rango de las olas.
