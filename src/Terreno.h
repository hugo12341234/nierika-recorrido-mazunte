#pragma once

// X-33: terreno caminable. Une la playa (A-5) con las colinas (X-18) en una sola funcion de altura h(x, z), para que el visitante
// pueda subir a las colinas. La funcion de las colinas es la MISMA de tools/x18_colinas.py (alturaColinas): misma semilla, mismo hash
// entero y mismos coeficientes, asi la altura de los pies coincide con la malla de Models/colinas/colinas.obj (que solo la muestrea cada 4 m).
// Unidades: metros (1 u = 1 m). Todo en coordenadas del mundo. Solo funciones inline (cabecera sin .cpp, como pide CLAUDE.md).

#include <cmath>
#include <cstdint>
#include <algorithm>

namespace Terreno
{
    // --- Parametros de las colinas (ESTADO, seccion 5, X-18) ---
    constexpr double PLAYA_X = 50.0, PLAYA_Z0 = -20.0;   // la playa llega a |x| = 50 y a z = -20: ahi h = 0
    constexpr double ALTURA = 34.0;                      // altura maxima aproximada (m)

    // --- Zona caminable (X-33) ---
    constexpr float LIM_X = 140.0f;       // la malla de colinas llega a |x| = 150; se deja 10 m de margen
    constexpr float LIM_Z_NORTE = -160.0f;  // la malla llega a z = -170
    constexpr float LIM_Z_SUR_COLINA = 40.0f;  // junto a la bahia (|x| > 50) la malla llega a z = +45
    constexpr float LIM_Z_ORILLA = 24.0f;      // frente a la playa (|x| <= 50): la orilla del mar (A-9)
    constexpr float PENDIENTE_MAX = 0.70f;     // m de subida por m recorrido (35 grados); el 97 % de las colinas es mas suave que 0.6

    // Ruido de valor suave: hash entero de 32 bits + interpolacion smoothstep (identico al de x18_colinas.py)
    inline double ruido(double x, double z, int semilla)
    {
        auto h = [semilla](int i, int j) -> double
        {
            uint32_t n = (uint32_t)i * 374761393u + (uint32_t)j * 668265263u + (uint32_t)semilla * 1442695041u;
            n = (n ^ (n >> 13)) * 1274126177u;
            return (double)((n ^ (n >> 16)) & 0xFFFFu) / 65535.0;
        };
        const double fi = std::floor(x), fj = std::floor(z);
        const int i = (int)fi, j = (int)fj;
        const double fx = x - fi, fz = z - fj;
        const double ux = fx * fx * (3.0 - 2.0 * fx), uz = fz * fz * (3.0 - 2.0 * fz);
        const double a = h(i, j), b = h(i + 1, j), c = h(i, j + 1), d = h(i + 1, j + 1);
        return (a * (1.0 - ux) + b * ux) * (1.0 - uz) + (c * (1.0 - ux) + d * ux) * uz;
    }

    // Suma de 4 octavas (cada una al doble de frecuencia y la mitad de amplitud), normalizada a [0, 1]
    inline double fbm(double x, double z, int semilla)
    {
        double t = 0.0, amp = 1.0, f = 1.0, s = 0.0;
        for (int k = 0; k < 4; k++)
        {
            t += amp * ruido(x * f, z * f, semilla + k);
            s += amp; amp *= 0.5; f *= 2.0;
        }
        return t / s;
    }

    // h(x, z) = ALTURA * m(d) * (0.35 + 0.65 * fbm),  m(d) = smoothstep(0, 70, d),  d = distancia al borde de la playa
    inline double alturaColinas(double x, double z)
    {
        const double dx = std::max(std::fabs(x) - PLAYA_X, 0.0);
        const double dz = std::max(PLAYA_Z0 - z, 0.0);
        const double d = (dx > 0.0 || dz > 0.0) ? std::hypot(dx, dz) : 0.0;
        const double u = std::min(d / 70.0, 1.0);
        const double m = u * u * (3.0 - 2.0 * u);
        return ALTURA * m * (0.35 + 0.65 * fbm(x / 55.0, z / 55.0, 7));
    }

    // Perfil de la playa (A-5): y = 0 hasta z = +20 y baja 0.05 m por metro hacia el mar
    inline float alturaPlaya(float z) { return (z <= 20.0f) ? 0.0f : -0.05f * (z - 20.0f); }

    // Altura del suelo en (x, z): playa dentro de |x| <= 50 y z >= -20; colinas en el resto
    inline float altura(float x, float z)
    {
        if (std::fabs(x) <= (float)PLAYA_X && z >= (float)PLAYA_Z0) return alturaPlaya(z);
        return (float)alturaColinas((double)x, (double)z);
    }

    // Mayor pendiente (m/m) alrededor de (x, z), con diferencias centrales a +-0.5 m
    inline float pendiente(float x, float z)
    {
        const float dx = std::fabs(altura(x + 0.5f, z) - altura(x - 0.5f, z));
        const float dz = std::fabs(altura(x, z + 0.5f) - altura(x, z - 0.5f));
        return std::max(dx, dz);
    }

    // true si el cuerpo (con 'r' de radio) puede estar en (x, z): dentro de la malla, fuera del mar y en una pendiente transitable
    inline bool caminable(float x, float z, float r)
    {
        if (std::fabs(x) > LIM_X - r) return false;
        if (z < LIM_Z_NORTE + r) return false;
        if (std::fabs(x) <= (float)PLAYA_X)
        {
            if (z > LIM_Z_ORILLA - r) return false;                  // el mar
        }
        else if (z > LIM_Z_SUR_COLINA - r) return false;             // borde de la malla junto a la bahia
        return pendiente(x, z) <= PENDIENTE_MAX;
    }
}
