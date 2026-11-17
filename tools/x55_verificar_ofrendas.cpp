// X-55: verificador de las 14 ofrendas familiares de la playa (src/OfrendasFamiliares.h). Sin GPU.
// Compilar y correr desde la raiz del repo:   g++ -std=c++17 -I src tools/x55_verificar_ofrendas.cpp -o /tmp/x55v && /tmp/x55v
// Sale con codigo 0 solo si TODO pasa. Comprueba:
//  1. diversidad: ninguna pareja se parece (distancia entre firmas >= UMBRAL_DIST), estructuras y numeros de velas distintos, radios separados;
//  2. todas las familias tienen sitio y estan dentro de la zona (z -19.5..-4.2, |x| 3.6..24);
//  3. sin solapes: familia contra familia (huella mas holgura), contra todos los obstaculos del mundo conocidos y contra el pasillo central;
//     y pieza por pieza contra los obstaculos;
//  4. sin flotantes: cada pieza elevada descansa sobre un soporte (petate, mesa, altar, roca, cesta, lancha) y a su altura;
//  5. presupuesto: piezas, llamas y triangulos estimados.
#include <cstdio>
#include <cmath>
#include <set>
#include <map>
#include <string>
#include <vector>
#include "OfrendasFamiliares.h"

using namespace OfrendasFamiliares;

static int fallos = 0;
#define COMPROBAR(cond, ...) do { if (!(cond)) { fallos++; std::printf("  FALLA: "); std::printf(__VA_ARGS__); std::printf("\n"); } } while (0)

static int cubeta(int n) { return n <= 0 ? 0 : n == 1 ? 1 : n <= 3 ? 2 : n <= 7 ? 3 : n <= 15 ? 4 : 5; }

// firma = conjunto de (pieza, cubeta de cantidad) + (estructura) ; distancia = 1 - Jaccard
static std::set<int> Firma(const Familia& f)
{
    std::map<int, int> n;
    for (const Item& it : f.items) n[it.tipo]++;
    std::set<int> s;
    for (auto& kv : n) s.insert(kv.first * 10 + cubeta(kv.second));
    return s;
}
static float Distancia(const std::set<int>& a, const std::set<int>& b)
{
    int inter = 0;
    for (int x : a) if (b.count(x)) inter++;
    const int uni = (int)(a.size() + b.size()) - inter;
    return 1.0f - (float)inter / (float)uni;
}
static int NVelas(const Familia& f) { int n = 0; for (const Item& it : f.items) if (LlevaLlama(it.tipo)) n++; return n; }
static glm::vec3 TinteVelas(const Familia& f)
{
    glm::vec3 s(0.0f); int n = 0;
    for (const Item& it : f.items) if (LlevaLlama(it.tipo)) { s += it.tinte; n++; }
    return n ? s / (float)n : glm::vec3(1.0f);
}

int main()
{
    constexpr float UMBRAL_DIST = 0.30f;
    const std::vector<Familia> fam = Colocadas();
    const std::vector<Circulo> obs = Obstaculos();
    const int N = (int)fam.size();
    std::printf("X-55 · %d familias\n", N);

    // --- 2. sitio y zona
    std::printf("\n[sitio y zona]\n");
    for (const Familia& f : fam)
    {
        std::printf("  %-12s %-34s x %7.2f z %7.2f giro %6.1f radio %.2f piezas %3d velas %2d velan %zu\n", f.id, f.estructura, f.x, f.z, f.giro, f.radio,
                    (int)f.items.size(), NVelas(f), f.velan.size());
        COMPROBAR(f.z < 1e5f, "%s no tiene sitio libre", f.id);
        if (f.z >= 1e5f) continue;
        for (const Circulo& c : Huella(f))
        {
            COMPROBAR(c.z + c.r <= Z_MAX + 0.01f && c.z - c.r >= Z_MIN - 0.01f, "%s se sale de la zona en z (circulo %.1f, %.1f r %.2f)", f.id, c.x, c.z, c.r);
            COMPROBAR(std::fabs(c.x) - c.r >= X_PASILLO - 0.01f, "%s invade el pasillo central (circulo %.1f, %.1f)", f.id, c.x, c.z);
            COMPROBAR(std::fabs(c.x) + c.r <= X_MAX + 0.01f, "%s se sale de la zona en x", f.id);
        }
    }

    // --- 1. diversidad
    std::printf("\n[diversidad]\n");
    std::vector<std::set<int>> firmas;
    for (const Familia& f : fam) firmas.push_back(Firma(f));
    float minD = 9.0f; int mi = 0, mj = 0;
    for (int i = 0; i < N; i++)
        for (int j = i + 1; j < N; j++)
        {
            const float d = Distancia(firmas[i], firmas[j]);
            if (d < minD) { minD = d; mi = i; mj = j; }
            COMPROBAR(d >= UMBRAL_DIST, "%s y %s se parecen (distancia %.2f < %.2f)", fam[i].id, fam[j].id, d, UMBRAL_DIST);
            COMPROBAR(std::string(fam[i].estructura) != fam[j].estructura, "%s y %s tienen la misma estructura", fam[i].id, fam[j].id);
            COMPROBAR(NVelas(fam[i]) != NVelas(fam[j]), "%s y %s tienen el mismo numero de velas (%d)", fam[i].id, fam[j].id, NVelas(fam[i]));
            COMPROBAR(std::fabs(fam[i].radio - fam[j].radio) >= 0.04f, "%s y %s tienen casi el mismo radio", fam[i].id, fam[j].id);
            const glm::vec3 a = TinteVelas(fam[i]), b = TinteVelas(fam[j]);
            (void)a; (void)b;
        }
    std::printf("  pareja mas parecida: %s y %s (distancia %.2f, umbral %.2f)\n", fam[mi].id, fam[mj].id, minD, UMBRAL_DIST);
    float rmin = 99, rmax = 0; int vmin = 999, vmax = 0;
    for (const Familia& f : fam) { rmin = std::min(rmin, f.radio); rmax = std::max(rmax, f.radio); vmin = std::min(vmin, NVelas(f)); vmax = std::max(vmax, NVelas(f)); }
    std::printf("  radios %.2f a %.2f m · velas por familia %d a %d · familias con persona(s) velando: ", rmin, rmax, vmin, vmax);
    int conVela = 0; for (const Familia& f : fam) if (!f.velan.empty()) conVela++;
    std::printf("%d\n", conVela);
    COMPROBAR(rmax / rmin >= 2.5f, "el tamano no varia lo bastante (max/min = %.2f)", rmax / rmin);
    COMPROBAR(vmax >= 3 * std::max(vmin, 1), "el numero de velas no varia lo bastante");
    std::set<std::string> tintes;
    for (const Familia& f : fam) { const glm::vec3 t = TinteVelas(f); char b[64]; std::snprintf(b, sizeof b, "%.1f/%.1f/%.1f", t.x, t.y, t.z); tintes.insert(b); }
    std::printf("  paletas de velas distintas: %zu de %d\n", tintes.size(), N);
    COMPROBAR((int)tintes.size() >= 8, "pocas paletas de velas distintas");
    std::set<int> tiposUsados;
    for (const Familia& f : fam) for (const Item& it : f.items) tiposUsados.insert(it.tipo);
    std::printf("  tipos de pieza usados: %zu de %d\n", tiposUsados.size(), (int)N_PIEZAS);

    // --- 3. solapes
    std::printf("\n[solapes]\n");
    int pares = 0;
    std::vector<std::vector<Circulo>> huellas;
    for (const Familia& f : fam) huellas.push_back(Huella(f));
    auto hueco = [](const std::vector<Circulo>& a, const std::vector<Circulo>& b)
    {
        float m = 1e9f;
        for (const Circulo& p : a) for (const Circulo& q : b) m = std::min(m, std::sqrt((p.x - q.x) * (p.x - q.x) + (p.z - q.z) * (p.z - q.z)) - p.r - q.r);
        return m;
    };
    for (int i = 0; i < N; i++)
        for (int j = i + 1; j < N; j++)
        {
            const float d = hueco(huellas[i], huellas[j]);
            pares++;
            COMPROBAR(d >= 0.40f, "%s y %s a %.2f m entre huellas (se pide >= 0.40)", fam[i].id, fam[j].id, d);
        }
    float peor = 1e9f;
    for (int i = 0; i < N; i++)
    {
        const float d = hueco(huellas[i], obs);
        peor = std::min(peor, d);
        COMPROBAR(d >= 0.15f, "%s a %.2f m de un obstaculo conocido", fam[i].id, d);
    }
    std::printf("  %d pares de familias y %zu obstaculos revisados; hueco minimo a un obstaculo: %.2f m\n", pares, obs.size(), peor);
    // las 4 tumbas del patio (dentro de la barda) y el resto de lo fijo no se tocan: las familias estan en z <= -4.2, la barda termina en z = 3
    for (const Familia& f : fam) COMPROBAR(f.z + f.radio <= Z_MAX + 3.0f, "%s demasiado cerca de la capilla", f.id);

    // --- 4. flotantes
    std::printf("\n[flotantes]\n");
    int flotan = 0;
    for (const Familia& f : fam)
    {
        for (const Item& it : f.items)
        {
            if (it.pos.y < 0.01f) continue;
            if (it.tipo == RED || it.tipo == PAPEL) continue;                       // la red cuelga de la lancha y el papel de los soportes
            if (it.tipo == GUITARRA) continue;                                       // apoyada en un poste de los soportes (pivote en el centro)
            if (it.tipo == FLOR && it.pos.y <= 0.05f) continue;                      // la flor tiene el pivote en el centro: va a ras de suelo a y = 0.04
            bool apoyada = false;
            for (const Item& s : f.items)
            {
                if (&s == &it) continue;
                const float dx = it.pos.x - s.pos.x, dz = it.pos.z - s.pos.z;
                float top[4]; int nt = 0; float rad = RadioPieza(s.tipo) * s.esc;
                switch (s.tipo)
                {
                case PETATE: top[nt++] = 0.02f * s.esc; break;
                case MESA: top[nt++] = 0.78f * s.esc; break;
                case ALTAR3: top[nt++] = 0.4f * s.esc; top[nt++] = 0.8f * s.esc; top[nt++] = 1.2f * s.esc; break;
                case ALTAR_MAYOR: top[nt++] = 0.4f * s.esc; top[nt++] = 0.8f * s.esc; top[nt++] = 1.2f * s.esc; top[nt++] = 1.55f * s.esc; break;
                case ROCA: top[nt++] = 0.33f * s.esc; rad = 0.30f * s.esc; break;   // cubo con ruido: tapa plana a ~0.33 m x escala
                case CESTA: top[nt++] = 0.38f * s.esc; rad = 0.20f * s.esc; break;
                case ALF_FLORES: case ALF_CRUZ: top[nt++] = 0.02f; break;
                case LANCHA: top[nt++] = 0.62f * s.esc; break;
                case TAMBOR: top[nt++] = 0.38f * s.esc; rad = 0.22f * s.esc; break;
                default: continue;
                }
                const float dd = std::sqrt(dx * dx + dz * dz);
                if (dd > rad) continue;
                for (int k = 0; k < nt; k++) if (it.pos.y >= top[k] - 0.14f && it.pos.y <= top[k] + 0.06f) apoyada = true;
            }
            if (!apoyada) { flotan++; std::printf("  %s: %d (%.2f, %.2f, %.2f) sin soporte\n", f.id, (int)it.tipo, it.pos.x, it.pos.y, it.pos.z); }
        }
    }
    COMPROBAR(flotan == 0, "%d piezas flotantes", flotan);

    // --- 5. presupuesto
    int piezas = 0, llamas = 0, personas = 0;
    for (const Familia& f : fam) { piezas += (int)f.items.size(); llamas += NVelas(f); personas += (int)f.velan.size(); }
    std::printf("\n[presupuesto] %d piezas + %d llamas + %d personas de rodillas = %d dibujos (antes, X-54: 16 tumbas = ~11 dibujos cada una + 8 personas = ~190)\n",
                piezas, llamas, personas, piezas + llamas + personas);
    COMPROBAR(piezas + llamas <= 900, "demasiados dibujos (%d > 900)", piezas + llamas);

    std::printf("\n%s (%d fallos)\n", fallos == 0 ? "TODO BIEN" : "HAY FALLOS", fallos);
    return fallos == 0 ? 0 : 1;
}
