#ifndef EXTRAS_H
#define EXTRAS_H

// X-19..X-22: datos y formulas de los extras de ambiente (fase "mejorar y producir", ver docs/EXTRAS_Y_MEJORAS.md).
// Solo <cmath>: se puede probar aparte (tools/x19_pruebas_extras.cpp). SIN KeyFrames: todo es matematica simple.

#include <cmath>

namespace Extras
{
    constexpr float PI = 3.14159265358979f;

    // Balanceo senoidal: theta(t) = A * sen(2*pi*t/T + fase), en grados. Lo usan las figuras (balanceo suave sobre su pivote)
    // y los tendederos de papel picado (pendulo de pequena amplitud).
    inline float Balanceo(float t, float periodo, float fase, float amplitudGrados)
    {
        return amplitudGrados * std::sin(2.0f * PI * t / periodo + fase);
    }

    // Figuras de la ceremonia (poses FIJAS; no hay IA). tipo: 0 persona_pie_a, 1 persona_pie_b, 2 persona_agachada_a, 3 persona_agachada_b.
    struct Figura { float x, z, giroY; int tipo; float fase; };
    constexpr int N_FIGURAS = 12;
    // giroY = 180: mira a +z (hacia el mar). Los petates estan en z ~ 20 (la playa es plana hasta z = +20)
    inline const Figura FIGURAS[N_FIGURAS] = {
        {  8.0f, 19.5f, 180.0f, 2, 0.0f }, {  8.7f, 19.5f, 180.0f, 3, 1.1f },     // junto al petate 1
        { 11.0f, 18.7f, 180.0f, 3, 2.0f }, { 11.8f, 18.8f, 175.0f, 2, 0.4f },     // junto al petate 2
        { 14.0f, 19.7f, 180.0f, 2, 3.1f }, { 14.7f, 19.6f, 185.0f, 3, 4.2f },     // junto al petate 3
        {  7.2f, 17.6f, 180.0f, 0, 0.7f }, { 10.0f, 17.0f, 175.0f, 1, 1.9f },     // de pie, detras de los que rezan
        { 12.8f, 17.4f, 185.0f, 0, 2.6f }, { 15.5f, 18.0f, 180.0f, 1, 3.8f },
        {  5.0f, 20.8f, 150.0f, 1, 5.0f }, { 17.2f, 20.9f, 210.0f, 0, 0.3f }      // mirando el mar
    };
    // Petates con ofrenda (centro x, z y giro)
    struct Petate { float x, z, giroY; };
    constexpr int N_PETATES = 3;
    inline const Petate PETATES[N_PETATES] = { { 8.4f, 20.35f, 0.0f }, { 11.4f, 19.55f, 0.0f }, { 14.35f, 20.55f, 0.0f } };
}

#endif
