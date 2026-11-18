// X-64: revisa que la comparsa (los "peregrinos" que marchan por la orilla) no cruce lanchas, rocas, palmas, redes, casas ni otras figuras.
// Compilar y correr desde la raiz del repo:  g++ -std=c++17 -Isrc tools/x64_pruebas_peregrinos.cpp -o x64_pruebas && ./x64_pruebas
// Sale con codigo 1 si alguna figura de la comparsa (cuerpo de radio 0.3 m) toca un obstaculo en algun instante del ciclo.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include "Extras.h"

struct Caja { float x0, z0, x1, z1; std::string nombre; };
int main()
{
    using namespace Extras;
    std::vector<Caja> o;
    auto c = [&](float x, float z, float mx, float mz, const std::string& n) { o.push_back({ x - mx, z - mz, x + mx, z + mz, n }); };
    for (int i = 0; i < N_LANCHAS_VARADAS; i++) c(LANCHAS_VARADAS[i].x, LANCHAS_VARADAS[i].z, 2.7f, 0.85f, "lancha varada " + std::to_string(i));
    for (int i = 0; i < N_ROCAS; i++) c(ROCAS[i].x, ROCAS[i].z, 0.9f * ROCAS[i].esc, 0.8f * ROCAS[i].esc, "roca " + std::to_string(i));
    for (int i = 0; i < N_REDES; i++) c(REDES[i].x, REDES[i].z, 1.25f, 0.1f, "red " + std::to_string(i));
    for (int i = 0; i < N_PALMAS_LADOS; i++) c(PALMAS_LADOS[i].x, PALMAS_LADOS[i].z, 0.3f * PALMAS_LADOS[i].esc, 0.3f * PALMAS_LADOS[i].esc, "palma " + std::to_string(i));
    for (int i = 0; i < N_PALAPAS; i++) for (int sx = -1; sx <= 1; sx += 2) for (int sz = -1; sz <= 1; sz += 2) c(PALAPAS[i].x + 2.2f * sx, PALAPAS[i].z + 2.2f * sz, 0.12f, 0.12f, "palapa " + std::to_string(i));
    for (int i = 0; i < N_CASAS; i++) c(CASAS[i].x, CASAS[i].z, 2.0f, 2.6f, "casa " + std::to_string(i));
    for (int i = 0; i < N_FOGATAS; i++) c(FOGATAS[i].x, FOGATAS[i].z, 0.6f, 0.6f, "fogata " + std::to_string(i));
    for (int i = 0; i < N_HAMACAS; i++) for (int sx = -1; sx <= 1; sx += 2) c(HAMACAS[i].x + 1.6f * sx, HAMACAS[i].z, 0.06f, 0.06f, "hamaca " + std::to_string(i));
    for (int i = 0; i < N_FIGURAS; i++) c(FIGURAS[i].x, FIGURAS[i].z, 0.25f, 0.25f, "figura ceremonia " + std::to_string(i));
    for (int i = 0; i < N_FIGURAS_N; i++) c(FIGURAS_N[i].x, FIGURAS_N[i].z, 0.2f, 0.2f, "figura narrativa " + std::to_string(i));
    for (int i = 0; i < N_FAROLES; i++) c(FAROLES[i].x, FAROLES[i].z, 0.05f, 0.05f, "farol " + std::to_string(i));
    for (int t = 0; t < N_TENDEDEROS; t++) for (int k = 0; k <= TENDEDEROS[t].n; k++) c(TENDEDEROS[t].x0 + ANCHO_BASTIDOR * k, TENDEDEROS[t].z, 0.05f, 0.05f, "poste tendedero " + std::to_string(t));
    for (int i = 0; i < N_BANDA; i++) c(BANDA[i].x, BANDA[i].z, 0.3f, 0.3f, "musico " + std::to_string(i));

    const float R = 0.25f;   // cuerpo de la figura (Colisiones::RADIO)
    std::vector<std::string> vistos;
    int malos = 0;
    for (float t = 0.0f; t < COMPARSA_CICLO; t += 0.1f)
        for (int i = 0; i < N_COMPARSA; i++)
        {
            float x, z, y; ComparsaN(i, t, x, z, y);
            for (const Caja& b : o)
                if (x > b.x0 - R && x < b.x1 + R && z > b.z0 - R && z < b.z1 + R)
                {
                    bool nuevo = true; for (auto& v : vistos) if (v == b.nombre) nuevo = false;
                    if (nuevo) { vistos.push_back(b.nombre); std::printf("CHOCA  figura %2d con %-22s en t=%5.1f (%.2f, %.2f)\n", i, b.nombre.c_str(), t, x, z); }
                    malos++;
                }
        }
    // Entre las propias figuras: falda de 0.84 m (radio 0.42); el tambor (radio 0.22, 0.62 m al frente del cuerpo) lo llevan las figuras 0, 4 y 8
    int malosFig = 0; float minDist = 1e9f;
    for (float t = 0.0f; t < COMPARSA_CICLO; t += 0.1f)
    {
        float px[N_COMPARSA], pz[N_COMPARSA], y;
        for (int i = 0; i < N_COMPARSA; i++) ComparsaN(i, t, px[i], pz[i], y);
        const float g = ComparsaGiro(t) * PI / 180.0f;   // el modelo mira a -z: frente = (-sen g, -cos g)
        for (int i = 0; i < N_COMPARSA; i++)
            for (int j = i + 1; j < N_COMPARSA; j++)
            {
                const float d = std::hypot(px[i] - px[j], pz[i] - pz[j]);
                minDist = std::fmin(minDist, d);
                if (d < 0.84f) { malosFig++; std::printf("CHOCA  figura %d con figura %d en t=%.1f (d=%.2f)\n", i, j, t, d); }
            }
        for (int i = 0; i < N_COMPARSA; i += 4)
        {
            const float tx = px[i] - 0.62f * std::sin(g), tz = pz[i] - 0.62f * std::cos(g);
            for (int j = 0; j < N_COMPARSA; j++)
                if (j != i && std::hypot(tx - px[j], tz - pz[j]) < 0.22f + 0.42f) { malosFig++; std::printf("CHOCA  tambor de %d con figura %d en t=%.1f\n", i, j, t); }
            for (const Caja& b : o)
                if (tx > b.x0 - 0.22f && tx < b.x1 + 0.22f && tz > b.z0 - 0.22f && tz < b.z1 + 0.22f) { malosFig++; std::printf("CHOCA  tambor de %d con %s en t=%.1f\n", i, b.nombre.c_str(), t); }
        }
    }
    std::printf("%s: figuras y tambores entre si, %d choques (distancia minima entre figuras %.2f m)\n", malosFig ? "FALLA" : "OK", malosFig, minDist);
    malos += malosFig;
    // La silueta (KeyFrames de D-1/D-5) camina por x = 0 .. -0.22 de z = 24 a z = 0.2: tampoco debe cruzar nada de esta lista
    int malosSil = 0;
    for (float z = 0.2f; z <= 24.0f; z += 0.05f)
        for (float x = -0.22f; x <= 0.0f; x += 0.11f)
            for (const Caja& b : o)
                if (x > b.x0 - R && x < b.x1 + R && z > b.z0 - R && z < b.z1 + R) { malosSil++; std::printf("CHOCA  silueta con %s en (%.2f, %.2f)\n", b.nombre.c_str(), x, z); }
    std::printf("%s: silueta, %d muestras dentro de obstaculos\n", malosSil ? "FALLA" : "OK", malosSil);
    malos += malosSil;
    std::printf("%s: %d muestras dentro de obstaculos, %d obstaculos distintos\n", malos ? "FALLA" : "OK", malos, (int)vistos.size());
    return malos ? 1 : 0;
}
