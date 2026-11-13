// X-19..X-22: pruebas numericas de las formulas de los extras (docs/manual_tecnico/justificacion_extras.md) contra el CODIGO REAL.
// Sin OpenGL. Compilar y correr desde la raiz del repo:
//   g++ -std=c++17 -Isrc tools/x19_pruebas_extras.cpp -o x19_pruebas && ./x19_pruebas
// Termina con codigo 0 solo si TODAS las comprobaciones pasan.
#include <cstdio>
#include <cmath>
#include <algorithm>
#include "Extras.h"
#include "Olas.h"

static int fallos = 0;
static void comprueba(const char* que, bool ok, double valor, const char* esperado)
{
    std::printf("[%s] %-66s -> %.6g  (esperado %s)\n", ok ? " OK " : "FALLA", que, valor, esperado);
    if (!ok) fallos++;
}

int main()
{
    // 1) Balanceo: |theta| <= A, periodo T, fases distintas dan valores distintos
    float maxAbs = 0.0f;
    for (int k = 0; k < 20000; k++) maxAbs = std::max(maxAbs, std::fabs(Extras::Balanceo(k * 0.001f, 4.5f, 0.7f, 1.2f)));
    comprueba("Balanceo: maximo de |theta| con A = 1.2", std::fabs(maxAbs - 1.2f) < 1e-3f, maxAbs, "1.2 grados");
    comprueba("Balanceo: periodo T = 4.5 s (theta(t) = theta(t + T))", std::fabs(Extras::Balanceo(1.3f, 4.5f, 0.7f, 1.2f) - Extras::Balanceo(5.8f, 4.5f, 0.7f, 1.2f)) < 1e-4f,
              Extras::Balanceo(1.3f, 4.5f, 0.7f, 1.2f) - Extras::Balanceo(5.8f, 4.5f, 0.7f, 1.2f), "0");
    comprueba("Balanceo: dos fases distintas difieren", std::fabs(Extras::Balanceo(2.0f, 4.5f, 0.0f, 1.2f) - Extras::Balanceo(2.0f, 4.5f, 2.0f, 1.2f)) > 0.1f,
              Extras::Balanceo(2.0f, 4.5f, 0.0f, 1.2f) - Extras::Balanceo(2.0f, 4.5f, 2.0f, 1.2f), "> 0.1");
    float maxPapel = 0.0f;
    for (int k = 0; k < 20000; k++) maxPapel = std::max(maxPapel, (float)std::fabs(Extras::Balanceo(k * 0.001f, 3.6f, 1.6f, 6.0f)));
    comprueba("Papel picado: |theta| <= 6 grados", maxPapel <= 6.0001f && maxPapel > 5.99f, maxPapel, "6 grados");
    comprueba("Papel picado: desplazamiento maximo de la punta (0.5 m * sen 6 grados)", std::fabs(0.5 * std::sin(6.0 * Extras::PI / 180.0) - 0.0523) < 1e-3,
              0.5 * std::sin(6.0 * Extras::PI / 180.0), "~0.052 m");

    // 2) Tablas de remo: el circulo, la tangente y la inclinacion maxima por la ola
    double maxDistErr = 0.0, maxTanErr = 0.0, maxPerp = 0.0, maxAlfa = 0.0;
    for (int i = 0; i < Extras::N_TABLAS; i++)
        for (int k = 0; k < 2000; k++)
        {
            const float t = k * 0.07f;
            float x, z, dx, dz;
            Extras::PosicionTabla(Extras::TABLAS[i], t, x, z, dx, dz);
            const float rx = x - Extras::TABLAS[i].cx, rz = z - Extras::TABLAS[i].cz;
            maxDistErr = std::max(maxDistErr, (double)std::fabs(std::sqrt(rx * rx + rz * rz) - Extras::TABLAS[i].radio));
            maxTanErr = std::max(maxTanErr, (double)std::fabs(std::sqrt(dx * dx + dz * dz) - 1.0));
            maxPerp = std::max(maxPerp, (double)(std::fabs(rx * dx + rz * dz) / Extras::TABLAS[i].radio));
            float nx, ny, nz;
            Olas::Normal(x, z, t, nx, ny, nz);
            maxAlfa = std::max(maxAlfa, (double)(std::acos(std::min(1.0, (double)ny)) * 180.0 / Extras::PI));
        }
    comprueba("Tablas: distancia al centro = R (error maximo, m)", maxDistErr < 1e-3, maxDistErr, "< 1e-3");
    comprueba("Tablas: la tangente es unitaria (error maximo)", maxTanErr < 1e-5, maxTanErr, "< 1e-5");
    comprueba("Tablas: la tangente es perpendicular al radio (|cos| maximo)", maxPerp < 1e-4, maxPerp, "< 1e-4");
    comprueba("Tablas: inclinacion maxima por la ola (grados)", maxAlfa <= 6.0, maxAlfa, "<= 6 grados");
    float x0, z0, dx0, dz0, x1, z1, dx1, dz1;
    Extras::PosicionTabla(Extras::TABLAS[0], 0.0f, x0, z0, dx0, dz0);
    Extras::PosicionTabla(Extras::TABLAS[0], Extras::PERIODO_TABLA, x1, z1, dx1, dz1);
    comprueba("Tablas: despues de un periodo vuelve al punto de partida (m)", std::hypot(x1 - x0, z1 - z0) < 5e-3, std::hypot(x1 - x0, z1 - z0), "< 5e-3");
    comprueba("Tablas: rapidez = R * 2*pi / 70 (R = 3 m, m/s)", std::fabs(3.0 * 2.0 * Extras::PI / Extras::PERIODO_TABLA - 0.2693) < 1e-3, 3.0 * 2.0 * Extras::PI / Extras::PERIODO_TABLA, "~0.269");

    // 3) Luciernagas: brillo en [0, 1], posiciones acotadas alrededor de su centro, todas parpadean alguna vez
    double minB = 1e9, maxB = -1e9, maxDx = 0, maxDy = 0;
    int siempreApagadas = 0;
    for (int i = 0; i < Extras::N_LUCIERNAGAS; i++)
    {
        bool algunaVez = false;
        const Extras::Luciernaga l = Extras::LuciernagaN(i);
        for (int k = 0; k < 4000; k++)
        {
            const float t = k * 0.05f;
            const float b = Extras::BrilloLuciernaga(i, t);
            minB = std::min(minB, (double)b); maxB = std::max(maxB, (double)b);
            if (b > 0.5f) algunaVez = true;
            float x, y, z;
            Extras::PosicionLuciernaga(i, t, x, y, z);
            maxDx = std::max(maxDx, (double)std::fabs(x - l.cx)); maxDy = std::max(maxDy, (double)std::fabs(y - l.cy));
        }
        if (!algunaVez) siempreApagadas++;
    }
    comprueba("Luciernagas: brillo minimo", minB >= 0.0, minB, ">= 0");
    comprueba("Luciernagas: brillo maximo", maxB <= 1.0001, maxB, "<= 1");
    comprueba("Luciernagas: desplazamiento horizontal maximo respecto del centro (m)", maxDx <= 1.6001, maxDx, "<= 1.6");
    comprueba("Luciernagas: desplazamiento vertical maximo (m)", maxDy <= 0.4501, maxDy, "<= 0.45");
    comprueba("Luciernagas que nunca brillan (> 0.5)", siempreApagadas == 0, siempreApagadas, "0");

    // 4) Sendero de cempasuchil: sigue el eje serpenteante, no pisa los muros y llena el tramo de la orilla a la puerta
    int dibujadas = 0, enMuro = 0; double maxDesv = 0.0;
    for (int i = 0; i < Extras::NumFloresSendero(); i++)
    {
        float x, z;
        if (!Extras::FlorSenderoEn(i, x, z)) continue;
        dibujadas++;
        if (((z > 2.9f && z < 3.35f) || (z > 11.7f && z < 12.05f)) && std::fabs(x) > 0.95f) enMuro++;
        maxDesv = std::max(maxDesv, (double)std::fabs(x - Extras::EjeSendero(z)));
    }
    comprueba("Sendero: numero de flores dibujadas", dibujadas > 60 && dibujadas <= Extras::NumFloresSendero(), dibujadas, "60..71");
    comprueba("Sendero: flores sobre los muros de la barda", enMuro == 0, enMuro, "0");
    comprueba("Sendero: desviacion maxima respecto del eje (m)", maxDesv <= 0.46, maxDesv, "<= 0.45");
    comprueba("Sendero: el eje cruza las puertas de la barda (|x| < 1 en z = 3 y z = 12)", std::fabs(Extras::EjeSendero(3.1f)) < 1.0f && std::fabs(Extras::EjeSendero(11.9f)) < 1.0f,
              std::fabs(Extras::EjeSendero(11.9f)), "< 1");
    // 5) Murcielagos: distancia al centro = radio, rapidez lineal = VEL_MURCIELAGO, aleteo acotado
    double maxR = 0.0, maxV = 0.0;
    for (int i = 0; i < Extras::N_MURCIELAGOS; i++)
    {
        float x0m, y0m, z0m, dxm, dzm, x1m, y1m, z1m;
        for (int k = 0; k < 500; k++)
        {
            const float t = k * 0.1f;
            Extras::PosicionMurcielago(Extras::MURCIELAGOS[i], t, x0m, y0m, z0m, dxm, dzm);
            maxR = std::max(maxR, (double)std::fabs(std::hypot(x0m - Extras::MURCIELAGOS[i].cx, z0m - Extras::MURCIELAGOS[i].cz) - Extras::MURCIELAGOS[i].radio));
            Extras::PosicionMurcielago(Extras::MURCIELAGOS[i], t + 0.01f, x1m, y1m, z1m, dxm, dzm);
            maxV = std::max(maxV, (double)std::fabs(std::hypot(x1m - x0m, z1m - z0m) / 0.01f - Extras::VEL_MURCIELAGO));
        }
    }
    comprueba("Murcielagos: error maximo de la distancia al centro (m)", maxR < 1e-3, maxR, "< 1e-3");
    comprueba("Murcielagos: error maximo de la rapidez lineal (m/s)", maxV < 0.05, maxV, "< 0.05");
    float maxAle = 0.0f;
    for (int k = 0; k < 20000; k++) maxAle = std::max(maxAle, (float)std::fabs(Extras::AMP_ALETEO * std::sin(2.0f * Extras::PI * Extras::FREC_ALETEO * k * 0.001f)));
    comprueba("Murcielagos: aleteo maximo (grados)", std::fabs(maxAle - 40.0f) < 0.2f, maxAle, "40 grados");

    // 6) Lados (X-25..X-28): aves, tortugas, crias y linternas
    double eAveR = 0.0, eAveV = 0.0, eTorR = 0.0, eTorV = 0.0;
    for (int i = 0; i < Extras::N_AVES; i++)
        for (int k = 0; k < 300; k++)
        {
            const float t = k * 0.2f;
            float x, y, z, dx, dz, x2, y2, z2;
            Extras::PosicionAve(Extras::AVES[i], t, x, y, z, dx, dz);
            eAveR = std::max(eAveR, (double)std::fabs(std::hypot(x - Extras::AVES[i].cx, z - Extras::AVES[i].cz) - Extras::AVES[i].radio));
            Extras::PosicionAve(Extras::AVES[i], t + 0.01f, x2, y2, z2, dx, dz);
            eAveV = std::max(eAveV, (double)std::fabs(std::hypot(x2 - x, z2 - z) / 0.01f - Extras::AVES[i].vel));
        }
    for (int i = 0; i < Extras::N_TORTUGAS; i++)
        for (int k = 0; k < 300; k++)
        {
            const float t = k * 0.5f;
            float x, z, dx, dz, x2, z2;
            Extras::PosicionTortuga(Extras::TORTUGAS[i], t, x, z, dx, dz);
            eTorR = std::max(eTorR, (double)std::fabs(std::hypot(x - Extras::TORTUGAS[i].cx, z - Extras::TORTUGAS[i].cz) - Extras::TORTUGAS[i].radio));
            Extras::PosicionTortuga(Extras::TORTUGAS[i], t + 0.01f, x2, z2, dx, dz);
            eTorV = std::max(eTorV, (double)std::fabs(std::hypot(x2 - x, z2 - z) / 0.01f - Extras::VEL_TORTUGA));
        }
    comprueba("Aves: error maximo de la distancia al centro (m)", eAveR < 1e-3, eAveR, "< 1e-3");
    comprueba("Aves: error maximo de la rapidez lineal (m/s)", eAveV < 0.05, eAveV, "< 0.05");
    comprueba("Tortugas: error maximo de la distancia al centro (m)", eTorR < 1e-3, eTorR, "< 1e-3");
    comprueba("Tortugas: error maximo de la rapidez (m/s)", eTorV < 0.01, eTorV, "< 0.01");
    double zMin = 1e9, zMax = -1e9;
    for (int i = 0; i < Extras::N_CRIAS; i++)
        for (int k = 0; k < 2000; k++) { const double z = Extras::CriaZ(i, k * 0.37f); zMin = std::min(zMin, z); zMax = std::max(zMax, z); }
    comprueba("Crias: z minima >= Z0 (la arena firme)", zMin >= Extras::CRIA_Z0 - 1e-4, zMin, ">= 20.2");
    comprueba("Crias: z maxima < Z0 + L (llegan al borde del agua)", zMax < Extras::CRIA_Z0 + Extras::CRIA_L + 1e-4, zMax, "< 24.4");
    double lxMin = 1e9, lxMax = -1e9, lzMin = 1e9, lzMax = -1e9;
    for (int i = 0; i < Extras::N_LINTERNAS; i++) { float x, z; Extras::LinternaN(i, x, z); lxMin = std::min(lxMin, (double)x); lxMax = std::max(lxMax, (double)x); lzMin = std::min(lzMin, (double)z); lzMax = std::max(lzMax, (double)z); }
    comprueba("Linternas: todas dentro de la bahia (x en [-46, 46], z en [27, 59])", lxMin >= -46.0 && lxMax <= 46.0 && lzMin >= 27.0 && lzMax <= 59.0, lzMax, "<= 59");

    std::printf(fallos == 0 ? "\nTODO OK (0 fallos)\n" : "\n%d FALLOS\n", fallos);
    return fallos == 0 ? 0 : 1;
}
