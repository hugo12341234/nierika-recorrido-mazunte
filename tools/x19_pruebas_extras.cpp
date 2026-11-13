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

    std::printf(fallos == 0 ? "\nTODO OK (0 fallos)\n" : "\n%d FALLOS\n", fallos);
    return fallos == 0 ? 0 : 1;
}
