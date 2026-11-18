// X-49: pruebas de las formulas de las mariposas (Mariposas.h) y de la llama de fogata (Fuego.h). Sin OpenGL.
// Compilar (raiz del repo):  g++ -std=c++17 -Isrc tools/x49_pruebas.cpp -o x49_pruebas && ./x49_pruebas
#include <cstdio>
#include <algorithm>
#include "Mariposas.h"
#include "Fuego.h"

int main()
{
    int fallos = 0;
    auto exige = [&](bool ok, const char* msg) { if (!ok) { std::printf("FALLA: %s\n", msg); fallos++; } };
    float dMin = 1e9f, dMax = -1e9f, hMin = 1e9f, hMax = -1e9f, vMax = 0.0f;
    for (int i = 0; i < MariposasDatos::N_MARIPOSAS; i++)
    {
        const MariposasDatos::Mariposa& m = MariposasDatos::MARIPOSAS[i];
        Mariposas::Pose anterior = Mariposas::PoseEn(i, 0.0f);
        for (float t = 0.0f; t < 600.0f; t += 0.05f)
        {
            const Mariposas::Pose p = Mariposas::PoseEn(i, t);
            exige(std::fabs(p.pos.x - m.cx) <= m.A + 1e-3f && std::fabs(p.pos.z - m.cz) <= m.B + 1e-3f, "la mariposa se sale de su elipse A x B");
            const float h = p.pos.y - Terreno::altura(p.pos.x, p.pos.z);
            hMin = std::min(hMin, h); hMax = std::max(hMax, h);
            const float d = Mariposas::Aleteo(i, t) * 57.29578f;
            dMin = std::min(dMin, d); dMax = std::max(dMax, d);
            vMax = std::max(vMax, glm::length(p.pos - anterior.pos) / 0.05f);
            anterior = p;
        }
    }
    std::printf("mariposas: %d | altura sobre el suelo %.2f..%.2f m | aleteo %.1f..%.1f grados | rapidez max %.2f m/s\n", MariposasDatos::N_MARIPOSAS, hMin, hMax, dMin, dMax, vMax);
    exige(hMin >= 1.2f, "vuela a menos de 1.2 m del suelo");
    exige(dMin >= -56.1f && dMax <= 44.1f, "aleteo fuera de -56..+44 grados");
    exige(vMax < 3.5f, "rapidez mayor que 3.5 m/s");
    float syMin = 9, syMax = 0, bMin = 9, bMax = 0, incMax = 0;
    for (int i = 0; i < 12; i++) for (int l = 0; l < LlamasFogata::N_LENGUAS; l++) for (float t = 0.0f; t < 60.0f; t += 0.01f)
    {
        const glm::mat4 m = LlamasFogata::Modelo(glm::vec3(0.0f), i, l, t);
        const float sy = glm::length(glm::vec3(m[1])) / LlamasFogata::LENGUAS[l].esc;      // la cizalla no cambia el largo de la columna en y mas de ~0.5 %
        syMin = std::min(syMin, sy); syMax = std::max(syMax, sy);
        const float b = LlamasFogata::Brillo(i, l, t); bMin = std::min(bMin, b); bMax = std::max(bMax, b);
    }
    std::printf("llama: |columna y| %.3f..%.3f | brillo %.2f..%.2f\n", syMin, syMax, bMin, bMax);
    exige(syMin > 0.80f && syMax < 1.25f, "alto de llama fuera de 0.8..1.25");
    exige(bMin >= 1.25f && bMax <= 1.85f, "brillo fuera de 1.25..1.85");
    std::printf(fallos ? "HAY FALLOS: %d\n" : "TODO OK\n", fallos);
    return fallos ? 1 : 0;
}
