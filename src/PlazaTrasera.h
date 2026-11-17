// X-52: plaza detras de la capilla (entrega 2 de la ofrenda mayor). Solo datos y matematica (sin GPU).
// X-53: la plaza es solo la ofrenda comunitaria al final del camino de animas (x = 0, z = -9.5): un altar de piramide al centro, con espacio libre alrededor.
// Giro Y como glm::rotate: un punto local (x, z) pasa a (x cos g + z sen g, -x sen g + z cos g); el frente local +z mira a (sen g, cos g).
#ifndef PLAZA_TRASERA_H
#define PLAZA_TRASERA_H

#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include "OfrendaMayor.h"
#include "VidaColinasDatos.h"

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

    // Objetos del altar de piramide en coordenadas locales (origen en el centro del piso). Un lado (+z) se repite girado 0, 90, 180 y 270 grados.
    inline std::vector<OfrendaMayor::Objeto> ListaPiramide()
    {
        using namespace OfrendaMayor;
        std::vector<Objeto> o;
        for (int k = 0; k < 4; k++)
        {
            const float g = 90.0f * k;
            auto add = [&](Tipo t, glm::vec3 p, float esc, float giroItem = 0.0f) { o.push_back({ t, Girar(p, g), esc, g + giroItem }); };
            // nivel 1 (tope 0.30; franja z 1.30..1.60): 4 velas (la luz guia)
            for (int i = 0; i < 4; i++) add(VELA, glm::vec3(-1.05f + 0.70f * i, 0.30f, 1.45f), 1.3f);
            // nivel 2 (tope 0.60): racimos a los lados y pan al centro
            add(RACIMO, glm::vec3(-0.75f, 0.60f, 1.15f), 0.42f); add(RACIMO, glm::vec3(0.75f, 0.60f, 1.15f), 0.42f);
            add(PAN, glm::vec3(0.0f, 0.64f, 1.15f), 1.5f);
            // nivel 3 (tope 0.90): calavera al centro y velas
            add(CALAVERA, glm::vec3(0.0f, 0.90f, 0.85f), 1.5f);
            add(VELA, glm::vec3(-0.55f, 0.90f, 0.85f), 1.3f); add(VELA, glm::vec3(0.55f, 0.90f, 0.85f), 1.3f);
            // nivel 4 (tope 1.20): copal al centro
            add(COPALERO, glm::vec3(0.0f, 1.20f, 0.55f), 1.6f);
            // cima (tope 1.50): retrato que mira hacia fuera
            add(MARCO, glm::vec3(0.0f, 1.50f, 0.20f), 3.0f);
        }
        // piso: 12 velas en un circulo de radio 2.30 m
        for (int i = 0; i < 12; i++)
        {
            const float a = 2.0f * PI * i / 12.0f;
            o.push_back({ VELA, glm::vec3(2.30f * std::sin(a), 0.0f, 2.30f * std::cos(a)), 1.2f });
        }
        return o;
    }


    // X-55: el panteon de 16 tumbas en filas (X-54) se QUITO de la arena (D2, 17/11: "tumbas en arena en playa" se veia fuera de lugar).
    // Lo sustituyen 14 ofrendas familiares distintas en src/OfrendasFamiliares.h. Las 4 tumbas (modelos tumba1..4, B-9) siguen en el patio de la capilla.

    // Arco de cempasuchil: umbral por donde llega el camino de animas a la ofrenda comunitaria (al sur de la piramide, entre ella y la capilla)
    constexpr int N_ARCOS = 1;
    inline const glm::vec2 ARCOS[N_ARCOS] = { glm::vec2(0.0f, -5.6f) };
}

#endif
