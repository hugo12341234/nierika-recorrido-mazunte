// X-55: busca el sitio de cada ofrenda familiar (x, z, giro) y escribe el arreglo SITIOS para pegarlo en src/OfrendasFamiliares.h.
// Determinista. Las familias grandes van primero; alternan izquierda/derecha del camino de las animas (x = 0). Para cada una se prueban
// los puntos de una rejilla de 0.5 m y, si el giro preferido no cabe, los giros alternos; gana el punto con mas hueco libre a las demas
// (con un pequeno premio por estar cerca del camino), usando la huella de CIRCULOS de cada familia (Huella).
// g++ -std=c++17 -I src tools/x55_buscar_sitios.cpp -o /tmp/x55b && /tmp/x55b
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include "OfrendasFamiliares.h"
using namespace OfrendasFamiliares;

static float HuecoMin(const std::vector<Circulo>& a, const std::vector<Circulo>& b)
{
    float m = 1e9f;
    for (const Circulo& p : a) for (const Circulo& q : b) m = std::min(m, std::sqrt((p.x - q.x) * (p.x - q.x) + (p.z - q.z) * (p.z - q.z)) - p.r - q.r);
    return m;
}

int main()
{
    std::vector<Familia> fam = Familias();
    const std::vector<Circulo> obs = Obstaculos();
    std::vector<int> orden(fam.size());
    for (size_t i = 0; i < fam.size(); i++) orden[i] = (int)i;
    std::stable_sort(orden.begin(), orden.end(), [&](int a, int b) { return fam[a].radio > fam[b].radio; });
    std::vector<Circulo> ocupado;           // huellas ya puestas
    std::vector<Sitio> res(fam.size(), Sitio{ 0.0f, 0.0f, 0.0f });
    std::vector<bool> ok(fam.size(), false);
    int k = 0;
    for (int idx : orden)
    {
        Familia f = fam[idx];
        const int lado = (k++ % 2 == 0) ? -1 : 1;
        float mejor = -1e9f; Sitio mejorS{ 0, 0, 0 };
        for (int variante = 0; variante < 3 && mejor < -1e8f; variante++)
        {
            const float base = (lado < 0 ? 90.0f : -90.0f) + (Azar(idx * 7 + 3) - 0.5f) * 100.0f;
            const float giro = variante == 0 ? base : variante == 1 ? base + 90.0f : base - 90.0f;
            for (int ladoProbado = 0; ladoProbado < 2; ladoProbado++)
            {
                const int L = ladoProbado == 0 ? lado : -lado;
                for (float z = Z_MAX - 1.0f; z >= Z_MIN + 1.0f; z -= 0.5f)
                    for (float xa = X_PASILLO + 1.0f; xa <= X_MAX - 1.0f; xa += 0.5f)
                    {
                        f.x = L * xa; f.z = z; f.giro = giro;
                        const std::vector<Circulo> h = Huella(f);
                        bool libre = true;
                        for (const Circulo& c : h)
                            if (std::fabs(c.x) - c.r < X_PASILLO || std::fabs(c.x) + c.r > X_MAX || c.z + c.r > Z_MAX || c.z - c.r < Z_MIN) { libre = false; break; }
                        if (!libre) continue;
                        if (HuecoMin(h, obs) < HOLGURA_OBSTACULO) continue;
                        float hueco = HuecoMin(h, ocupado);
                        if (hueco < HOLGURA_FAMILIA) continue;
                        const float puntaje = std::min(hueco, 6.0f) - 0.12f * xa - 0.04f * std::fabs(z + 11.0f);
                        if (puntaje > mejor) { mejor = puntaje; mejorS = { f.x, f.z, giro }; }
                    }
                if (mejor > -1e8f) break;
            }
        }
        if (mejor > -1e8f)
        {
            f.x = mejorS.x; f.z = mejorS.z; f.giro = mejorS.giro;
            for (const Circulo& c : Huella(f)) ocupado.push_back(c);
            res[idx] = mejorS; ok[idx] = true;
        }
    }
    std::printf("    inline const Sitio SITIOS[N_FAMILIAS] = {\n");
    for (size_t i = 0; i < fam.size(); i++)
        std::printf("        { %.2ff, %.2ff, %.1ff },   // %s%s\n", res[i].x, res[i].z, res[i].giro, fam[i].id, ok[i] ? "" : "  <-- SIN SITIO");
    std::printf("    };\n");
    return 0;
}
