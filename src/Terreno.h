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
    constexpr float LIM_Z_SUR_COLINA = 40.0f;  // junto a la bahia (|x| > 50) la malla llega a z = +42 (la rejilla de 4 m termina ahi; X-53)
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

    // X-53: la malla de Models/colinas/colinas.obj solo tiene un vertice cada 4 m y entre vertices es PLANA por triangulos, asi que la formula lisa
    // alturaColinas difiere de lo que se ve hasta +-0.5 m. Todo lo que pisa o se apoya en el suelo (pies, objetos) debe usar la superficie VISIBLE:
    // esta funcion la replica (misma rejilla y misma diagonal que tools/x53_terreno.py y tools/x53_colinas_obj.py). Rejilla: x -150..150 y
    // z -170..42 (la ultima fila de vertices cae en 42, no en 45). Cada celda (i,j) se triangula (i,j)-(i,j+1)-(i+1,j+1) y (i,j)-(i+1,j+1)-(i+1,j).
    // La celda de z -22..-20 dentro de la playa (X-40) llega solo hasta z = -20, donde h = 0 (la playa).
    constexpr double REJ_X0 = -150.0, REJ_Z0 = -170.0, REJ_PASO = 4.0;
    constexpr int REJ_CELDAS_X = 75, REJ_CELDAS_Z = 53;

    // Altura de un VERTICE de la malla: la formula lisa, salvo en el borde de la playa (|x| = 50, z > 20), donde baja con la playa (tope -0.5 en z = 30)
    inline double alturaVertice(double x, double z)
    {
        if (std::fabs(std::fabs(x) - PLAYA_X) < 1e-9 && z > 20.0) return std::max(-0.05 * (z - 20.0), -0.5);
        return alturaColinas(x, z);
    }

    inline float alturaMalla(float x, float z)
    {
        if (x < (float)REJ_X0 || x > (float)(REJ_X0 + REJ_CELDAS_X * REJ_PASO) || z < (float)REJ_Z0 || z > (float)(REJ_Z0 + REJ_CELDAS_Z * REJ_PASO))
            return (float)alturaColinas((double)x, (double)z);              // fuera de la malla no hay suelo dibujado; se conserva la formula
        const int i = std::min((int)((x - REJ_X0) / REJ_PASO), REJ_CELDAS_X - 1);
        const int j = std::min((int)((z - REJ_Z0) / REJ_PASO), REJ_CELDAS_Z - 1);
        const double x0 = REJ_X0 + i * REJ_PASO, z0 = REJ_Z0 + j * REJ_PASO;
        double fx = (x - x0) / REJ_PASO, fz = (z - z0) / REJ_PASO;
        const double y00 = alturaVertice(x0, z0), y10 = alturaVertice(x0 + REJ_PASO, z0);
        double y01 = alturaVertice(x0, z0 + REJ_PASO), y11 = alturaVertice(x0 + REJ_PASO, z0 + REJ_PASO);
        if (x0 >= -PLAYA_X - 1e-9 && x0 + REJ_PASO <= PLAYA_X + 1e-9 && z0 < PLAYA_Z0 && PLAYA_Z0 < z0 + REJ_PASO)
        {
            fz = std::min(std::max((z - z0) / (PLAYA_Z0 - z0), 0.0), 1.0);   // celda recortada (X-40)
            y01 = y11 = 0.0;
        }
        const double y = (fz >= fx) ? y00 + fx * (y11 - y01) + fz * (y01 - y00)
                                    : y00 + fx * (y10 - y00) + fz * (y11 - y10);
        return (float)y;
    }

    // Altura del suelo VISIBLE en (x, z): playa dentro de |x| <= 50 y z >= -20; malla de colinas en el resto
    inline float altura(float x, float z)
    {
        if (std::fabs(x) <= (float)PLAYA_X && z >= (float)PLAYA_Z0) return alturaPlaya(z);
        return alturaMalla(x, z);
    }

    // X-53: inclinacion del suelo para apoyar rocas sobre una ladera. Pendiente (gx = dh/dx, gz = dh/dz) medida con diferencias centrales a +-w,
    // w = max(1, 0.8 * esc) (la huella de la roca), y limitada a 0.5 m/m (26.6 grados). La misma formula esta en tools/x53_terreno.py.
    inline void inclinacion(float x, float z, float esc, float& gx, float& gz)
    {
        const float w = std::max(1.0f, 0.8f * esc);
        gx = (altura(x + w, z) - altura(x - w, z)) / (2.0f * w);
        gz = (altura(x, z + w) - altura(x, z - w)) / (2.0f * w);
        const float m = std::sqrt(gx * gx + gz * gz);
        if (m > 0.5f) { gx *= 0.5f / m; gz *= 0.5f / m; }
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
