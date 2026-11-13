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
