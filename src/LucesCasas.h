#pragma once
// X-56 (fuera del alcance original, como las casas): ventanas encendidas de las casas, sin una luz por casa.
// Cada casa tiene 2 ventanas (x local = -1.65 y +1.65, y 1.55, en la pared z = -2.04 del modelo). Su estado (apagada, tenue, brillante),
// su color calido y su parpadeo salen de un hash fijo del numero de casa: la escena se ve igual en cada ejecucion y no se repite de casa en casa.
// El shader (lighting.frag, esVidrio) suma la emision; main.cpp dibuja un halo aditivo por ventana encendida.
#include <cmath>
#include <glm/glm.hpp>

namespace LucesCasas
{
    // Hash determinista en [0, 1) de (casa, canal)
    inline float Azar(int casa, int canal)
    {
        float v = std::sin(12.9898f * (float)(casa + 1) + 78.233f * (float)(canal + 1) + 3.17f) * 43758.5453f;
        return v - std::floor(v);
    }

    struct Ventanas { glm::vec3 color; float izq, der; };   // brillo 0..~1.15 (0 = apagada)

    // Posiciones locales de los halos (centro de cada ventana, 0.4 m hacia afuera de la pared)
    constexpr float VENTANA_X = 1.65f, VENTANA_Y = 1.55f, VENTANA_Z = -2.45f;

    // casa = indice; salt separa las casas de la playa (Extras) de las del pueblo de fondo. t = segundos.
    inline Ventanas Estado(int casa, int salt, float t)
    {
        const int id = casa + salt;
        Ventanas v;
        // color: ambar 50 %, amarillo palido 25 %, naranja 17 %, azul de television 8 %
        const float c = Azar(id, 0);
        bool tele = false;
        if (c < 0.50f)      v.color = glm::vec3(1.00f, 0.62f, 0.22f);
        else if (c < 0.75f) v.color = glm::vec3(1.00f, 0.82f, 0.48f);
        else if (c < 0.92f) v.color = glm::vec3(1.00f, 0.42f, 0.14f);
        else              { v.color = glm::vec3(0.55f, 0.72f, 1.00f); tele = true; }
        float b[2];
        for (int w = 0; w < 2; w++)
        {
            const float r = Azar(id, 1 + w);
            const float fase = 6.2831853f * Azar(id, 3 + w);
            float base, parp;
            if (r < 0.22f)      { base = 0.0f; parp = 0.0f; }          // apagada
            else if (r < 0.55f) { base = 0.45f; parp = 0.18f; }        // vela o veladora: tenue y parpadea
            else                { base = 0.95f; parp = 0.04f; }        // foco o lampara: brillante y estable
            float f;
            if (tele && base > 0.0f) f = 1.0f + 0.30f * std::sin(9.0f * t + fase) * std::sin(2.3f * t + 2.0f * fase);   // television: cambia de golpe
            else                     f = 1.0f + parp * std::sin(7.3f * t + fase) + 0.6f * parp * std::sin(13.1f * t + 2.0f * fase);
            b[w] = base * f;
        }
        // al menos una ventana encendida en 1 de cada 2 casas apagadas por completo (casas con gente despierta); el resto queda a oscuras
        if (b[0] == 0.0f && b[1] == 0.0f && Azar(id, 5) < 0.45f) b[Azar(id, 6) < 0.5f ? 0 : 1] = 0.45f * (1.0f + 0.18f * std::sin(7.3f * t + 1.3f * id));
        v.izq = b[0];
        v.der = b[1];
        return v;
    }
}
