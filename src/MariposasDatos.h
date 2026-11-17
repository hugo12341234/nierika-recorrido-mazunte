#pragma once

// X-49: GENERADO por tools/x49_mariposas.py (no editar a mano). Centro (cx, cz) del vuelo, semiejes A (x) y B (z) en metros y fase en radianes.
// Las formulas estan en Mariposas.h. Todas las trayectorias verificadas contra casas, rocas, palapas, palmas, arboles, faroles, cruces, fogatas y personas.

namespace MariposasDatos
{
    struct Mariposa { float cx, cz, A, B, fase; };
    constexpr int N_MARIPOSAS = 18;
    inline const Mariposa MARIPOSAS[18] = {
        { 5.70f, -35.58f, 2.0f, 2.3f, 1.608f },
        { -3.84f, -36.10f, 2.8f, 3.0f, 0.255f },
        { -4.90f, -38.39f, 3.6f, 1.6f, 1.435f },
        { -60.36f, -63.26f, 2.0f, 2.3f, 4.127f },
        { -51.92f, -55.28f, 2.8f, 3.0f, 5.594f },
        { -58.11f, -66.36f, 3.6f, 1.6f, 4.346f },
        { -2.59f, -83.35f, 2.0f, 2.3f, 0.913f },
        { -1.67f, -83.32f, 2.8f, 3.0f, 2.655f },
        { -9.19f, -83.14f, 3.6f, 1.6f, 5.602f },
        { 81.62f, -84.93f, 2.0f, 2.3f, 2.062f },
        { 83.92f, -85.25f, 2.8f, 3.0f, 0.218f },
        { 81.09f, -83.37f, 3.6f, 1.6f, 3.720f },
        { -47.67f, -81.67f, 2.0f, 2.3f, 0.385f },
        { -57.15f, -93.62f, 2.8f, 3.0f, 5.834f },
        { -56.65f, -92.03f, 3.6f, 1.6f, 4.357f },
        { -62.22f, -21.55f, 2.0f, 2.3f, 2.525f },
        { -64.35f, -17.53f, 2.8f, 3.0f, 5.004f },
        { -64.60f, -13.71f, 3.6f, 1.6f, 0.731f },
    };
}
