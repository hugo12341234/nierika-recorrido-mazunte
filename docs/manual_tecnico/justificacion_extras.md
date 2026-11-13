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
