// X-51: ofrenda mayor del patio del panteon. Solo datos y matematica (sin GPU): posiciones de los objetos en coordenadas LOCALES del altar_mayor
// (origen en el centro del piso, frente = +z, 1 u = 1 m). main.cpp los dibuja con la matriz del altar y las colisiones salen de la huella.
#ifndef OFRENDA_MAYOR_H
#define OFRENDA_MAYOR_H

#include <vector>
#include <cmath>
#include <glm/glm.hpp>

namespace OfrendaMayor
{
    // Huella real de Models/altar_mayor/altar_mayor.obj (peldano de madera incluido): x -1.90..1.90, z -1.02..1.30, alto 2.90 m
    constexpr float SEMI_X = 1.90f, Z_ATRAS = -1.02f, Z_FRENTE = 1.30f, ALTO = 2.90f;
    // Dos ofrendas simetricas a ambos lados del camino (x = 0); z = 8.4 -> z 7.38..9.70, dentro de la barda (z 3.25..11.75, |x| < 6.75)
    constexpr float X_CENTRO = 3.9f, Z_CENTRO = 8.4f;
    constexpr float ARCO_Z = 10.3f;   // arco de cempasuchil sobre el camino, entre las dos ofrendas

    enum Tipo { VELA, FLOR, RACIMO, PAN, CALAVERA, COPALERO, REFRESCO, MARCO };
    struct Objeto { Tipo tipo; glm::vec3 pos; float esc; float giro = 0.0f; };   // giro: grados en Y (los marcos miran hacia fuera)

    // Alturas de tope de los 4 niveles (iguales a las de tools/x51_altar_mayor.py)
    constexpr float Y_A = 0.40f, Y_B = 0.80f, Y_C = 1.20f, Y_D = 1.55f;

    inline float Azar(int i)   // numero repetible en [0,1): sin(i*12.9898)*43758.5453 (hash corto de uso comun en shaders)
    {
        const float s = std::sin((float)i * 12.9898f) * 43758.5453f;
        return s - std::floor(s);
    }

    // Lista de objetos de UNA ofrenda; las velas llevan llama (main.cpp la dibuja en la punta: 0.15 m x escala)
    inline std::vector<Objeto> Lista()
    {
        std::vector<Objeto> o;
        // --- nivel A (tope 0.40): franja del frente z 0.55..1.00
        for (int i = 0; i < 9; i++) o.push_back({ VELA, glm::vec3(-1.6f + 0.4f * i, Y_A, 0.93f), 1.3f });
        const float rx[5] = { -1.40f, -0.70f, 0.0f, 0.70f, 1.40f };
        for (int i = 0; i < 5; i++) o.push_back({ RACIMO, glm::vec3(rx[i], Y_A, 0.74f), 0.62f });
        const float px[4] = { -1.15f, -0.40f, 0.40f, 1.15f };
        for (int i = 0; i < 4; i++) o.push_back({ PAN, glm::vec3(px[i], Y_A + 0.04f, 0.72f), 1.5f });
        o.push_back({ CALAVERA, glm::vec3(-0.80f, Y_A, 0.70f), 1.8f });
        o.push_back({ CALAVERA, glm::vec3( 0.80f, Y_A, 0.70f), 1.8f });
        o.push_back({ COPALERO, glm::vec3(-1.65f, Y_A, 0.30f), 1.4f });
        o.push_back({ COPALERO, glm::vec3( 1.65f, Y_A, 0.30f), 1.4f });
        // --- nivel B (tope 0.80): franja z 0.10..0.55
        for (int i = 0; i < 7; i++) o.push_back({ VELA, glm::vec3(-1.30f + 0.4333f * i, Y_B, 0.49f), 1.3f });
        const float bx[4] = { -1.00f, -0.30f, 0.40f, 1.10f };
        for (int i = 0; i < 4; i++) o.push_back({ RACIMO, glm::vec3(bx[i], Y_B, 0.30f), 0.52f });
        o.push_back({ REFRESCO, glm::vec3(-0.90f, Y_B, 0.30f), 1.8f });
        o.push_back({ REFRESCO, glm::vec3( 0.90f, Y_B, 0.30f), 1.8f });
        o.push_back({ PAN, glm::vec3(-0.15f, Y_B + 0.04f, 0.28f), 1.6f });
        o.push_back({ PAN, glm::vec3( 0.15f, Y_B + 0.04f, 0.28f), 1.6f });
        o.push_back({ COPALERO, glm::vec3(-1.35f, Y_B, 0.30f), 1.4f });
        o.push_back({ COPALERO, glm::vec3( 1.35f, Y_B, 0.30f), 1.4f });
        // --- nivel C (tope 1.20): franja z -0.25..0.10
        const float cx[3] = { -0.80f, 0.0f, 0.80f };
        for (int i = 0; i < 3; i++) o.push_back({ RACIMO, glm::vec3(cx[i], Y_C, -0.10f), 0.48f });
        for (int i = 0; i < 4; i++) o.push_back({ VELA, glm::vec3(-1.05f + 0.70f * i, Y_C, 0.0f), 1.4f });
        o.push_back({ MARCO, glm::vec3(-0.40f, Y_C, -0.15f), 2.2f });
        o.push_back({ MARCO, glm::vec3( 0.40f, Y_C, -0.15f), 2.2f });
        o.push_back({ CALAVERA, glm::vec3(-0.90f, Y_C, -0.15f), 1.4f });
        o.push_back({ CALAVERA, glm::vec3( 0.90f, Y_C, -0.15f), 1.4f });
        // --- nivel D (tope 1.55): retrato grande y velas altas
        o.push_back({ MARCO, glm::vec3(0.0f, Y_D, -0.40f), 4.0f });
        o.push_back({ VELA, glm::vec3(-0.62f, Y_D, -0.40f), 2.0f });
        o.push_back({ VELA, glm::vec3( 0.62f, Y_D, -0.40f), 2.0f });
        // --- piso, frente al peldano (peldano termina en z 1.30)
        for (int i = 0; i < 9; i++) o.push_back({ VELA, glm::vec3(-2.0f + 0.5f * i, 0.0f, 1.62f), 1.2f });
        o.push_back({ RACIMO, glm::vec3(-2.25f, 0.0f, 0.85f), 1.0f });
        o.push_back({ RACIMO, glm::vec3( 2.25f, 0.0f, 0.85f), 1.0f });
        // --- cempasuchil suelto (esc 2.2 = 15 cm; pivote al centro, sube media altura) repartido por los niveles
        const struct { float y, x0, x1, z0, z1; int n; } fr[3] = { { Y_A, -1.75f, 1.75f, 0.58f, 0.88f, 14 }, { Y_B, -1.45f, 1.45f, 0.18f, 0.42f, 10 }, { Y_C, -1.15f, 1.15f, -0.22f, 0.05f, 6 } };
        int k = 0;
        for (const auto& f : fr)
            for (int i = 0; i < f.n; i++, k++)
                o.push_back({ FLOR, glm::vec3(f.x0 + (f.x1 - f.x0) * Azar(2 * k + 1), f.y + 0.085f, f.z0 + (f.z1 - f.z0) * Azar(2 * k + 2)), 2.2f });
        return o;
    }
}

#endif
