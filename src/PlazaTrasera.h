// X-52: plaza detras de la capilla (entrega 2 de la ofrenda mayor). Solo datos y matematica (sin GPU).
// La plaza se centra en el camino de animas (x = 0, z = -9.5): un altar de piramide al centro y dos altar_mayor de frente a el.
// Giro Y como glm::rotate: un punto local (x, z) pasa a (x cos g + z sen g, -x sen g + z cos g); el frente local +z mira a (sen g, cos g).
#ifndef PLAZA_TRASERA_H
#define PLAZA_TRASERA_H

#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include "OfrendaMayor.h"

namespace PlazaTrasera
{
    constexpr float PI = 3.14159265f;
    constexpr float CZ = -9.5f;                 // z del centro de la plaza
    constexpr float SEMI_PIRAMIDE = 1.62f;      // huella real de altar_piramide (x, z -1.62..1.62), alto 2.60 m
    constexpr float ALTO_PIRAMIDE = 2.60f;
    constexpr float X_MAYOR = 5.6f;             // altar_mayor a x = -5.6 (giro +90, mira a +x) y x = +5.6 (giro -90, mira a -x)

    inline glm::vec3 Girar(glm::vec3 p, float gradosY)
    {
        const float g = gradosY * PI / 180.0f, c = std::cos(g), s = std::sin(g);
        return glm::vec3(p.x * c + p.z * s, p.y, -p.x * s + p.z * c);
    }

    // Caja de colision (AABB) de un altar_mayor girado 90 grados con centro (cx, cz): giro +90 -> x = cx + z_local, z = cz - x_local
    inline void CajaMayor(int lado, glm::vec3& mn, glm::vec3& mx)
    {
        const float cx = X_MAYOR * lado;
        if (lado < 0) { mn = glm::vec3(cx + OfrendaMayor::Z_ATRAS, 0.0f, CZ - OfrendaMayor::SEMI_X); mx = glm::vec3(cx + OfrendaMayor::Z_FRENTE, OfrendaMayor::ALTO, CZ + OfrendaMayor::SEMI_X); }
        else          { mn = glm::vec3(cx - OfrendaMayor::Z_FRENTE, 0.0f, CZ - OfrendaMayor::SEMI_X); mx = glm::vec3(cx - OfrendaMayor::Z_ATRAS, OfrendaMayor::ALTO, CZ + OfrendaMayor::SEMI_X); }
    }

    // Objetos del altar de piramide en coordenadas locales (origen en el centro del piso). Un lado (+z) se repite girado 0, 90, 180 y 270 grados.
    inline std::vector<OfrendaMayor::Objeto> ListaPiramide()
    {
        using namespace OfrendaMayor;
        std::vector<Objeto> o;
        for (int k = 0; k < 4; k++)
        {
            const float g = 90.0f * k;
            auto add = [&](Tipo t, glm::vec3 p, float esc, float giroItem = 0.0f) { o.push_back({ t, Girar(p, g), esc, g + giroItem }); };
            // nivel 1 (tope 0.30; franja z 1.30..1.60): 4 velas
            for (int i = 0; i < 4; i++) add(VELA, glm::vec3(-1.05f + 0.70f * i, 0.30f, 1.45f), 1.3f);
            // nivel 2 (tope 0.60; franja z 1.00..1.30): racimos, pan y flores
            add(RACIMO, glm::vec3(-0.75f, 0.60f, 1.15f), 0.42f); add(RACIMO, glm::vec3(0.75f, 0.60f, 1.15f), 0.42f);
            add(PAN, glm::vec3(0.0f, 0.64f, 1.15f), 1.5f); add(PAN, glm::vec3(-0.35f, 0.64f, 1.15f), 1.3f); add(PAN, glm::vec3(0.35f, 0.64f, 1.15f), 1.3f);
            add(FLOR, glm::vec3(-0.25f, 0.685f, 1.10f), 2.2f); add(FLOR, glm::vec3(0.25f, 0.685f, 1.20f), 2.2f);
            // nivel 3 (tope 0.90; franja z 0.70..1.00): calavera y velas
            add(CALAVERA, glm::vec3(0.0f, 0.90f, 0.85f), 1.5f);
            add(VELA, glm::vec3(-0.55f, 0.90f, 0.85f), 1.3f); add(VELA, glm::vec3(0.55f, 0.90f, 0.85f), 1.3f);
            add(REFRESCO, glm::vec3(-0.30f, 0.90f, 0.85f), 1.4f); add(COPALERO, glm::vec3(0.30f, 0.90f, 0.85f), 1.2f);
            // nivel 4 (tope 1.20; franja z 0.40..0.70): flores y copalero
            add(FLOR, glm::vec3(-0.35f, 1.285f, 0.55f), 2.2f); add(FLOR, glm::vec3(0.35f, 1.285f, 0.55f), 2.2f);
            add(RACIMO, glm::vec3(0.0f, 1.20f, 0.55f), 0.38f);
            // cima (tope 1.50; semilado 0.40): retrato grande que mira hacia fuera y dos velas
            add(MARCO, glm::vec3(0.0f, 1.50f, 0.20f), 3.0f);
            add(VELA, glm::vec3(-0.30f, 1.50f, 0.30f), 1.6f); add(VELA, glm::vec3(0.30f, 1.50f, 0.30f), 1.6f);
        }
        // piso: 16 velas en un circulo de radio 2.30 m y 8 racimos a 2.05 m (entre velas)
        for (int i = 0; i < 16; i++)
        {
            const float a = 2.0f * PI * i / 16.0f;
            o.push_back({ VELA, glm::vec3(2.30f * std::sin(a), 0.0f, 2.30f * std::cos(a)), 1.2f });
        }
        for (int i = 0; i < 8; i++)
        {
            const float a = 2.0f * PI * (i + 0.5f) / 8.0f;
            o.push_back({ RACIMO, glm::vec3(2.05f * std::sin(a), 0.0f, 2.05f * std::cos(a)), 0.9f });
        }
        return o;
    }

    // Tumbas nuevas (copias de tumba1..4, cuentan una vez en el inventario) y cruces, con ofrenda al pie (1 vela + 3 flores)
    // modelo: 1..4 = tumba1..4, 0 = cruz. 'ofX/ofZ' = posicion de la ofrenda.
    struct Tumba { int modelo; float x, z, giro; float ofX, ofZ; };
    constexpr int N_TUMBAS = 8;
    inline const Tumba TUMBAS[N_TUMBAS] = {
        { 2, -4.0f,  -4.8f, 0.0f, -4.0f,  -3.6f }, { 3,  4.0f,  -4.8f, 0.0f,  4.0f,  -3.6f },
        { 4, -8.8f, -13.0f, 0.0f, -8.8f, -11.9f }, { 1,  8.8f, -13.0f, 0.0f,  8.8f, -12.4f },
        { 3, -4.2f, -14.2f, 0.0f, -4.2f, -13.0f }, { 2,  3.9f, -14.2f, 0.0f,  3.9f, -13.0f },
        { 0, -6.9f,  -4.2f, 0.0f, -6.9f,  -3.7f }, { 0,  6.9f,  -4.2f, 0.0f,  6.9f,  -3.7f } };

    // Arcos de cempasuchil sobre el camino de animas (entrada sur y norte de la plaza)
    constexpr int N_ARCOS = 2;
    inline const glm::vec2 ARCOS[N_ARCOS] = { glm::vec2(0.0f, -5.6f), glm::vec2(0.0f, -13.8f) };
}

#endif
