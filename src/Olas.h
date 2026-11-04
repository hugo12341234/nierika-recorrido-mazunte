#ifndef OLAS_H
#define OLAS_H

// C-1: oleaje del mar como suma de senos.
// Esta es la UNICA fuente de los parametros: main.cpp los envia al vertex shader (Shaders/lighting.vs)
// y las funciones de abajo calculan la MISMA altura en CPU, para que flores y velas floten sobre la ola.
//
//   h(x,z,t) = NIVEL + sum_i  A_i * sin( k_i * (d_i . (x,z)) - w_i * t + fase_i )
//   k_i = 2*pi / lambda_i          (numero de onda)
//   w_i = sqrt(g * k_i)            (frecuencia angular, aguas profundas)
//   d_i = (sin(theta_i), -cos(theta_i))   (direccion de avance; theta = 0 va hacia la orilla, -z)
//
// Sin dependencias (ni GLM ni GLEW) para poder probarlo aparte.

#include <cmath>

namespace Olas
{
    constexpr int   N = 3;                 // numero de ondas
    constexpr float NIVEL = -0.2f;         // altura de reposo del mar (A-7, decision 20/10)
    constexpr float GRAVEDAD = 9.81f;      // m/s^2
    constexpr float PI = 3.14159265358979f;

    struct Onda
    {
        float amplitud;     // A_i en metros
        float longitud;     // lambda_i en metros (>= 4: la malla del mar tiene cuadros de 1 m)
        float anguloGrados; // theta_i, 0 = hacia la orilla (-z); positivo gira hacia +x
        float fase;         // phi_i en radianes (evita que las crestas coincidan)
    };

    // Amplitud total = 0.04 + 0.025 + 0.015 = 0.08 m < 0.1 m: en z = +22 la arena esta en y = -0.1
    // y el mar nunca pasa de -0.12, asi que no asoma por encima de la arena en el borde de la malla.
    inline const Onda ONDAS[N] = {
        { 0.040f, 12.0f,   0.0f, 0.0f },
        { 0.025f,  7.0f,  25.0f, 1.7f },
        { 0.015f,  4.5f, -30.0f, 4.1f },
    };

    inline float NumeroOnda(int i) { return 2.0f * PI / ONDAS[i].longitud; }
    inline float Frecuencia(int i) { return std::sqrt(GRAVEDAD * NumeroOnda(i)); }
    inline float DirX(int i) { return std::sin(ONDAS[i].anguloGrados * PI / 180.0f); }
    inline float DirZ(int i) { return -std::cos(ONDAS[i].anguloGrados * PI / 180.0f); }

    // Altura del agua en (x, z) en el instante t (segundos)
    inline float Altura(float x, float z, float t)
    {
        float h = NIVEL;
        for (int i = 0; i < N; i++)
        {
            float arg = NumeroOnda(i) * (DirX(i) * x + DirZ(i) * z) - Frecuencia(i) * t + ONDAS[i].fase;
            h += ONDAS[i].amplitud * std::sin(arg);
        }
        return h;
    }

    // Normal unitaria de la superficie: n = normalize(-dh/dx, 1, -dh/dz), con dh/dx = sum A k dx cos(arg)
    inline void Normal(float x, float z, float t, float& nx, float& ny, float& nz)
    {
        float dhdx = 0.0f, dhdz = 0.0f;
        for (int i = 0; i < N; i++)
        {
            float k = NumeroOnda(i);
            float arg = k * (DirX(i) * x + DirZ(i) * z) - Frecuencia(i) * t + ONDAS[i].fase;
            float c = ONDAS[i].amplitud * k * std::cos(arg);
            dhdx += c * DirX(i);
            dhdz += c * DirZ(i);
        }
        float inv = 1.0f / std::sqrt(dhdx * dhdx + 1.0f + dhdz * dhdz);
        nx = -dhdx * inv;
        ny = inv;
        nz = -dhdz * inv;
    }

    // Rellena los arreglos que se mandan al shader (cada uno de tamano N; dir es x,z intercalado)
    inline void ParametrosShader(float A[N], float k[N], float w[N], float dir[2 * N], float fase[N])
    {
        for (int i = 0; i < N; i++)
        {
            A[i] = ONDAS[i].amplitud;
            k[i] = NumeroOnda(i);
            w[i] = Frecuencia(i);
            dir[2 * i] = DirX(i);
            dir[2 * i + 1] = DirZ(i);
            fase[i] = ONDAS[i].fase;
        }
    }
}

#endif
