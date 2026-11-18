#ifndef PUEBLO_FONDO_H
#define PUEBLO_FONDO_H
// GENERADO por tools/x30_pueblo_fondo.py (no editar a mano). FUERA DEL ALCANCE ORIGINAL: casas del pueblo y promontorio tipo Punta Cometa al fondo.
namespace PuebloFondo
{
    struct Casa { float x, y, z, giro; int tipo; };
    struct Roca { float x, y, z, giro, esc; };
    inline const Casa CASAS[21] = {
        { -108.91f, 26.60f, -55.05f, -173.10f, 0 },
        { -107.09f, 24.86f, -40.16f, 6.90f, 0 },
        { -97.78f, 26.38f, -57.14f, -166.30f, 1 },
        { -89.41f, 25.77f, -63.61f, -162.70f, 0 },
        { -74.21f, 23.22f, -79.24f, -162.90f, 1 },
        { -41.43f, 23.32f, -84.29f, 177.60f, 0 },
        { -25.35f, 10.84f, -54.18f, -10.40f, 1 },
        { -14.04f, 9.87f, -51.48f, -15.80f, 0 },
        { 9.96f, 6.62f, -44.09f, -15.80f, 0 },
        { 22.65f, 5.86f, -41.07f, -10.40f, 1 },
        { 36.31f, 11.52f, -54.56f, 177.60f, 0 },
        { 48.81f, 6.41f, -40.03f, 6.20f, 1 },
        { 89.41f, 18.77f, -46.39f, 17.30f, 0 },
        { 94.22f, 23.11f, -67.43f, -166.30f, 1 },
        { 97.78f, 21.00f, -52.86f, 13.70f, 1 },
        { 110.09f, 19.69f, -69.84f, -173.10f, 0 },
        { 108.91f, 21.39f, -54.95f, 6.90f, 0 },
        { -98.95f, 24.63f, -39.91f, -26.10f, 0 },
        { 64.44f, 3.23f, -9.47f, 59.70f, 1 },
        { 67.57f, 4.68f, -16.32f, 48.00f, 1 },
        { 102.19f, 21.61f, -46.92f, 22.60f, 0 }
    };
    inline const Roca ROCAS[6] = {
        { -70.00f, 1.44f, 24.00f, 20.00f, 6.85f },
        { -114.00f, 16.68f, 28.00f, 85.00f, 8.59f },
        { -70.00f, 2.72f, 28.00f, 150.00f, 3.69f },
        { -118.00f, 17.37f, 24.00f, 230.00f, 11.29f },
        { -66.00f, 1.01f, 24.00f, 300.00f, 5.21f },
        { -74.00f, 3.80f, 28.00f, 40.00f, 4.25f }
    };
    constexpr int N_CASAS = 21;
    constexpr int N_ROCAS = 6;
    constexpr float MIRADOR_X = -70.00f, MIRADOR_Y = 5.91f, MIRADOR_Z = 24.00f;
}
#endif
