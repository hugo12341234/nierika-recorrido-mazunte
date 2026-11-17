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
    constexpr float ARCO_Z = 12.45f;  // X-53: arco de cempasuchil como UMBRAL, justo afuera de la puerta norte de la barda (muro z 11.75..12.0)

    enum Tipo { VELA, FLOR, RACIMO, PAN, CALAVERA, COPALERO, REFRESCO, MARCO };
    struct Objeto { Tipo tipo; glm::vec3 pos; float esc; float giro = 0.0f; };   // giro: grados en Y (los marcos miran hacia fuera)

    // Alturas de tope de los 4 niveles (iguales a las de tools/x51_altar_mayor.py)
    constexpr float Y_A = 0.40f, Y_B = 0.80f, Y_C = 1.20f, Y_D = 1.55f;

    inline float Azar(int i)   // numero repetible en [0,1): sin(i*12.9898)*43758.5453 (hash corto de uso comun en shaders)
    {
        const float s = std::sin((float)i * 12.9898f) * 43758.5453f;
        return s - std::floor(s);
    }

    // Lista de objetos de UNA ofrenda; las velas llevan llama (main.cpp la dibuja en la punta: 0.15 m x escala).
    // X-53: composicion SIMETRICA respecto a x = 0 y con funciones tradicionales claras (docs/GUION_OFRENDA.md): velas al frente (la luz guia), flores
    // (el aroma guia), pan, copal al centro, fotografias arriba. Sin flores sueltas al azar ni objetos repetidos sin funcion.
    inline std::vector<Objeto> Lista()
    {
        std::vector<Objeto> o;
        // --- nivel A (tope 0.40): velas al frente, racimos de cempasuchil y pan
        for (int i = 0; i < 9; i++) o.push_back({ VELA, glm::vec3(-1.6f + 0.4f * i, Y_A, 0.93f), 1.3f });
        for (int i = 0; i < 5; i++) o.push_back({ RACIMO, glm::vec3(-1.4f + 0.7f * i, Y_A, 0.74f), 0.62f });
        o.push_back({ PAN, glm::vec3(-0.75f, Y_A + 0.04f, 0.72f), 1.5f });
        o.push_back({ PAN, glm::vec3( 0.75f, Y_A + 0.04f, 0.72f), 1.5f });
        o.push_back({ CALAVERA, glm::vec3(-1.15f, Y_A, 0.70f), 1.8f });
        o.push_back({ CALAVERA, glm::vec3( 1.15f, Y_A, 0.70f), 1.8f });
        // --- nivel B (tope 0.80): copal al centro, flores y velas
        for (int i = 0; i < 7; i++) o.push_back({ VELA, glm::vec3(-1.30f + 0.4333f * i, Y_B, 0.49f), 1.3f });
        o.push_back({ COPALERO, glm::vec3(0.0f, Y_B, 0.30f), 1.8f });
        for (int i = 0; i < 4; i++) o.push_back({ RACIMO, glm::vec3((i < 2 ? -1.0f : 0.35f) + 0.65f * (i % 2), Y_B, 0.30f), 0.52f });
        // --- nivel C (tope 1.20): fotografias y velas
        o.push_back({ RACIMO, glm::vec3(-0.80f, Y_C, -0.10f), 0.48f }); o.push_back({ RACIMO, glm::vec3(0.0f, Y_C, -0.10f), 0.48f }); o.push_back({ RACIMO, glm::vec3(0.80f, Y_C, -0.10f), 0.48f });
        for (int i = 0; i < 4; i++) o.push_back({ VELA, glm::vec3(-1.05f + 0.70f * i, Y_C, 0.0f), 1.4f });
        o.push_back({ MARCO, glm::vec3(-0.55f, Y_C, -0.15f), 2.2f });
        o.push_back({ MARCO, glm::vec3( 0.55f, Y_C, -0.15f), 2.2f });
        // --- nivel D (tope 1.55): retrato principal y dos velas altas
        o.push_back({ MARCO, glm::vec3(0.0f, Y_D, -0.40f), 4.0f });
        o.push_back({ VELA, glm::vec3(-0.62f, Y_D, -0.40f), 2.0f });
        o.push_back({ VELA, glm::vec3( 0.62f, Y_D, -0.40f), 2.0f });
        // --- piso, frente al peldano (peldano termina en z 1.30): hilera de velas y racimos en las esquinas
        for (int i = 0; i < 9; i++) o.push_back({ VELA, glm::vec3(-2.0f + 0.5f * i, 0.0f, 1.62f), 1.2f });
        o.push_back({ RACIMO, glm::vec3(-2.25f, 0.0f, 0.85f), 1.0f });
        o.push_back({ RACIMO, glm::vec3( 2.25f, 0.0f, 0.85f), 1.0f });
        return o;
    }
}

#endif
