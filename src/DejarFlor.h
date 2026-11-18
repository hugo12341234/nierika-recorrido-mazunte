// D-4 (act. 49): la flor que la silueta deja en el altar. Solo matematica (<cmath>, sin GLM ni GPU).
// Trayectoria: Bezier cuadratica mano -> nivel del altar con "caida libre final" (criterio de D-4 y CONTEXTO seccion 4).
//   B(u) = (1-u)^2 P0 + 2 (1-u) u P1 + u^2 P2
// Se elige P1 = (media de x, y de la mano, media de z): el punto de control a la altura de la mano y a medio camino
// en el plano horizontal. Entonces, con u = tau / Tb:
//   x(u), z(u) son lineales en u  (velocidad horizontal constante)
//   y(u) = y0 - u^2 (y0 - y2)     (parabola)
// y si Tb = sqrt(2 h / g), con h = y0 - y2, resulta y = y0 - g tau^2 / 2: EXACTAMENTE un tiro horizontal en caida libre.
// Es funcion cerrada del tiempo desde que se suelta (tau), asi que no depende de los FPS.
#pragma once

#include <cmath>

namespace dejarflor
{
    const float G = 9.81f;   // m/s^2

    // Tiempo de caida libre desde la altura h (m): tau_c = sqrt(2 h / g). 0 si no hay caida.
    inline float TiempoCaida(float h)
    {
        return h > 0.0f ? std::sqrt(2.0f * h / G) : 0.0f;
    }

    // Posicion de la flor tau segundos despues de soltarla. p0 = mano al soltar, p2 = destino; salida en out[3] (x, y, z).
    // Antes de soltar (tau <= 0) esta en p0; despues de Tb se queda en p2.
    inline void Posicion(const float p0[3], const float p2[3], float tau, float out[3])
    {
        const float Tb = TiempoCaida(p0[1] - p2[1]);
        float u = (Tb > 0.0f) ? tau / Tb : 1.0f;
        u = u < 0.0f ? 0.0f : (u > 1.0f ? 1.0f : u);
        const float p1[3] = { 0.5f * (p0[0] + p2[0]), p0[1], 0.5f * (p0[2] + p2[2]) };
        const float a = (1.0f - u) * (1.0f - u), b = 2.0f * (1.0f - u) * u, c = u * u;
        for (int i = 0; i < 3; i++)
            out[i] = a * p0[i] + b * p1[i] + c * p2[i];
    }
}
