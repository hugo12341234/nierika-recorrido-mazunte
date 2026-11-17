// X-52: plaza detras de la capilla (entrega 2 de la ofrenda mayor). Solo datos y matematica (sin GPU).
// X-53: la plaza es solo la ofrenda comunitaria al final del camino de animas (x = 0, z = -9.5): un altar de piramide al centro, con espacio libre alrededor.
// Giro Y como glm::rotate: un punto local (x, z) pasa a (x cos g + z sen g, -x sen g + z cos g); el frente local +z mira a (sen g, cos g).
#ifndef PLAZA_TRASERA_H
#define PLAZA_TRASERA_H

#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include "OfrendaMayor.h"
#include "VidaColinasDatos.h"

namespace PlazaTrasera
{
    constexpr float PI = 3.14159265f;
    constexpr float CZ = -9.5f;                 // z del centro de la plaza
    constexpr float SEMI_PIRAMIDE = 1.62f;      // huella real de altar_piramide (x, z -1.62..1.62), alto 2.60 m
    constexpr float ALTO_PIRAMIDE = 2.60f;
    constexpr float X_MAYOR = 5.6f;             // altar_mayor a x = -5.6 (giro +90, mira a +x) y x = +5.6 (giro -90, mira a -x)

    inline glm::vec3 Girar(glm::vec3 p, float gradosY)
    {
        const float g = gradosY * PI / 180.0f, c = std::cos(g), s = std::sin(g);
        return glm::vec3(p.x * c + p.z * s, p.y, -p.x * s + p.z * c);
    }

    // Objetos del altar de piramide en coordenadas locales (origen en el centro del piso). Un lado (+z) se repite girado 0, 90, 180 y 270 grados.
    inline std::vector<OfrendaMayor::Objeto> ListaPiramide()
    {
        using namespace OfrendaMayor;
        std::vector<Objeto> o;
        for (int k = 0; k < 4; k++)
        {
            const float g = 90.0f * k;
            auto add = [&](Tipo t, glm::vec3 p, float esc, float giroItem = 0.0f) { o.push_back({ t, Girar(p, g), esc, g + giroItem }); };
            // nivel 1 (tope 0.30; franja z 1.30..1.60): 4 velas (la luz guia)
            for (int i = 0; i < 4; i++) add(VELA, glm::vec3(-1.05f + 0.70f * i, 0.30f, 1.45f), 1.3f);
            // nivel 2 (tope 0.60): racimos a los lados y pan al centro
            add(RACIMO, glm::vec3(-0.75f, 0.60f, 1.15f), 0.42f); add(RACIMO, glm::vec3(0.75f, 0.60f, 1.15f), 0.42f);
            add(PAN, glm::vec3(0.0f, 0.64f, 1.15f), 1.5f);
            // nivel 3 (tope 0.90): calavera al centro y velas
            add(CALAVERA, glm::vec3(0.0f, 0.90f, 0.85f), 1.5f);
            add(VELA, glm::vec3(-0.55f, 0.90f, 0.85f), 1.3f); add(VELA, glm::vec3(0.55f, 0.90f, 0.85f), 1.3f);
            // nivel 4 (tope 1.20): copal al centro
            add(COPALERO, glm::vec3(0.0f, 1.20f, 0.55f), 1.6f);
            // cima (tope 1.50): retrato que mira hacia fuera
            add(MARCO, glm::vec3(0.0f, 1.50f, 0.20f), 3.0f);
        }
        // piso: 12 velas en un circulo de radio 2.30 m
        for (int i = 0; i < 12; i++)
        {
            const float a = 2.0f * PI * i / 12.0f;
            o.push_back({ VELA, glm::vec3(2.30f * std::sin(a), 0.0f, 2.30f * std::cos(a)), 1.2f });
        }
        return o;
    }


    // X-54: panteon velado detras de la capilla. Tumbas en FILAS simetricas a ambos lados del camino de animas (x = 0): columnas x = +-4.8 y +-7.4,
    // filas z = -5.2, -8.2, -11.2, -14.2 (3 m entre filas; las tumbas miden ~2 m de largo). Se omite el sitio que quede a menos de la distancia
    // de seguridad de un farol (1.4 m), roca (2.0), palma (1.2), arbusto (1.3) o arbol (1.8) de las listas de VidaColinasDatos (nada se cruza).
    // modelo: 1..4 = tumba1..4 (copias, cuentan una vez en el inventario). Cada una lleva 2 racimos de cempasuchil y 3 velas altas al pie.
    struct Tumba { int modelo; float x, z; };
    inline std::vector<Tumba> Panteon()
    {
        namespace D = VidaColinasDatos;
        const float cols[4] = { -7.4f, -4.8f, 4.8f, 7.4f };
        const float filas[4] = { -5.2f, -8.2f, -11.2f, -14.2f };
        std::vector<Tumba> t;
        auto libre = [&](float x, float z)
        {
            auto cerca = [&](float px, float pz, float r) { return (x - px) * (x - px) + (z - pz) * (z - pz) < r * r; };
            for (int i = 0; i < D::N_FAROLES; i++)  if (cerca(D::FAROLES[i].x,  D::FAROLES[i].z,  1.4f)) return false;
            for (int i = 0; i < D::N_ROCAS; i++)    if (cerca(D::ROCAS[i].x,    D::ROCAS[i].z,    2.0f)) return false;
            for (int i = 0; i < D::N_PALMAS; i++)   if (cerca(D::PALMAS[i].x,   D::PALMAS[i].z,   1.2f)) return false;
            for (int i = 0; i < D::N_ARBUSTOS; i++) if (cerca(D::ARBUSTOS[i].x, D::ARBUSTOS[i].z, 1.3f)) return false;
            for (int i = 0; i < D::N_ARBOLES; i++)  if (cerca(D::ARBOLES[i].x,  D::ARBOLES[i].z,  1.8f)) return false;
            return true;
        };
        for (int r = 0; r < 4; r++)
            for (int c = 0; c < 4; c++)
            {
                if (!libre(cols[c], filas[r])) continue;
                // simetria: el modelo depende de |x| y de la fila, no del lado
                const int tipo = (c == 0 || c == 3) && r == 1 ? 1 : 2 + (r + (c == 1 || c == 2 ? 0 : 1)) % 3;
                t.push_back({ tipo, cols[c], filas[r] });
            }
        return t;
    }
    // Vigilias: pares de personas arrodilladas al lado de las tumbas de la columna interior (x = +-4.8) de las filas 1 y 3, mirando a la tumba
    struct Vigilia { float x, z, giro; };
    inline std::vector<Vigilia> Vigilias(const std::vector<Tumba>& t)
    {
        std::vector<Vigilia> v;
        for (const Tumba& u : t)
        {
            if (std::fabs(std::fabs(u.x) - 4.8f) > 0.01f) continue;
            if (std::fabs(u.z + 5.2f) > 0.01f && std::fabs(u.z + 11.2f) > 0.01f) continue;
            const float lado = u.x > 0 ? -1.0f : 1.0f;   // del lado del camino
            v.push_back({ u.x + lado * 1.25f, u.z - 0.35f, u.x > 0 ? 90.0f : -90.0f });
            v.push_back({ u.x + lado * 1.25f, u.z + 0.45f, u.x > 0 ? 90.0f : -90.0f });
        }
        return v;
    }

    // Arco de cempasuchil: umbral por donde llega el camino de animas a la ofrenda comunitaria (al sur de la piramide, entre ella y la capilla)
    constexpr int N_ARCOS = 1;
    inline const glm::vec2 ARCOS[N_ARCOS] = { glm::vec2(0.0f, -5.6f) };
}

#endif
