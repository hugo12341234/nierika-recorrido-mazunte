// X-51 / X-57: diseños de velas del patio del panteon. Solo datos y matematica (sin GPU): posiciones de los objetos en coordenadas LOCALES del diseño
// (origen en el centro, sobre el piso, 1 u = 1 m). main.cpp los dibuja con la matriz del centro; las colisiones salen de RADIO.
#ifndef OFRENDA_MAYOR_H
#define OFRENDA_MAYOR_H

#include <vector>
#include <cmath>
#include <glm/glm.hpp>

namespace OfrendaMayor
{
    // X-57 (17/11): las dos ofrendas mayores con retablo (X-51, 4 niveles, 2.9 m de alto) se QUITAN del patio y se sustituyen por dos diseños BAJOS de velas
    // (mandala y estrella, <= 0.45 m de alto) con el mismo centro y la misma simetria respecto a x = 0. El modelo altar_mayor sigue en el repo porque
    // lo usan las ofrendas familiares de la playa (X-55). Centros en x = +-3.9, z = 8.6 (antes 8.4; se subio 0.2 m para no pisar a los visitantes de las tumbas), dentro de la barda (z 3.25..11.75, |x| < 6.75)
    constexpr float X_CENTRO = 3.9f, Z_CENTRO = 8.6f;
    constexpr float RADIO = 1.55f;   // radio del diseño (hasta la calavera de la punta de la estrella, 1.50 m); la caja de colision es un cuadrado de este semilado
    constexpr float ALTO_DISENO = 0.45f;  // calavera de la estrella: 0.22 m x 1.8 = 0.40 m
    constexpr float ARCO_Z = 12.45f;  // X-53: arco de cempasuchil como UMBRAL, justo afuera de la puerta norte de la barda (muro z 11.75..12.0)

    enum Tipo { VELA, FLOR, RACIMO, PAN, CALAVERA, COPALERO, REFRESCO, MARCO };
    struct Objeto { Tipo tipo; glm::vec3 pos; float esc; float giro = 0.0f; };   // giro: grados en Y (los marcos miran hacia fuera)

    inline float Azar(int i)   // numero repetible en [0,1): sin(i*12.9898)*43758.5453 (hash corto de uso comun en shaders)
    {
        const float s = std::sin((float)i * 12.9898f) * 43758.5453f;
        return s - std::floor(s);
    }

    // Punto de una circunferencia: radio r y angulo a en grados (0 = +x, 90 = +z), a altura 0 (el piso de tierra del patio)
    inline glm::vec3 Polar(float r, float aGrados)
    {
        const float a = glm::radians(aGrados);
        return glm::vec3(r * std::cos(a), 0.0f, r * std::sin(a));
    }

    // Diseño 1 (lado oeste): MANDALA. Anillos concentricos de velas con el copal sin humo al centro; en el anillo de 1.05 m alternan racimos de
    // cempasuchil (diagonales), pan (eje x) y calavera (eje z) con velas intercaladas. 6 + 12 + 8 + 24 velas, simetrica respecto a x y a z.
    inline std::vector<Objeto> ListaMandala()
    {
        std::vector<Objeto> o;
        o.push_back({ COPALERO, glm::vec3(0.0f), 2.0f });
        for (int i = 0; i < 6; i++)  o.push_back({ VELA, Polar(0.35f, 30.0f + 60.0f * i), 2.0f });           // anillo 1: velas altas
        for (int i = 0; i < 12; i++) o.push_back({ VELA, Polar(0.70f, 30.0f * i), 1.6f });                     // anillo 2
        for (int i = 0; i < 8; i++)                                                                              // anillo 3: ofrenda y velas
        {
            const float a = 45.0f * i;
            Tipo t = (i % 2) ? RACIMO : ((i % 4 == 0) ? PAN : CALAVERA);
            o.push_back({ t, Polar(1.05f, a), t == RACIMO ? 0.9f : (t == PAN ? 1.5f : 1.6f), -a + 90.0f });
            o.push_back({ VELA, Polar(1.05f, a + 22.5f), 1.4f });
        }
        for (int i = 0; i < 24; i++) o.push_back({ VELA, Polar(1.40f, 15.0f * i), 1.2f });                    // anillo 4: orilla
        return o;
    }

    // Diseño 2 (lado este): ESTRELLA de 8 rayos. Cada rayo son 5 velas que crecen hacia fuera (0.45..1.25 m); entre rayos, racimos de cempasuchil;
    // en la punta de los 4 rayos cardinales, una calavera; al centro el copal y 8 velas. 8 + 40 velas.
    inline std::vector<Objeto> ListaEstrella()
    {
        std::vector<Objeto> o;
        o.push_back({ COPALERO, glm::vec3(0.0f), 2.0f });
        for (int i = 0; i < 8; i++) o.push_back({ VELA, Polar(0.28f, 22.5f + 45.0f * i), 1.8f });
        for (int k = 0; k < 8; k++)
        {
            const float a = 45.0f * k;
            for (int j = 0; j < 5; j++) o.push_back({ VELA, Polar(0.45f + 0.20f * j, a), 1.3f + 0.15f * j });   // 1.3 .. 1.9
            o.push_back({ RACIMO, Polar(0.95f, a + 22.5f), 0.9f, -a });
            o.push_back({ RACIMO, Polar(1.30f, a + 22.5f), 0.7f, -a });
            if (k % 2 == 0) o.push_back({ CALAVERA, Polar(1.50f, a), 1.8f, -a + 90.0f });
        }
        return o;
    }
}

#endif
