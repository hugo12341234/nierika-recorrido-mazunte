#pragma once

// X-33 / X-34: vida en las colinas y en la franja norte de la playa. Las posiciones salen de VidaColinasDatos.h (generado por
// tools/x33_vida_colinas.py); aqui estan las formulas de las luciernagas y la eleccion de las 2 luces calidas que siguen al visitante.
// Todo con funciones cerradas simples (senos), sin KeyFrames ni sistema de particulas: son extras ambientales, como X-22 y X-24.

#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "VidaColinasDatos.h"
#include "Extras.h"
#include "Colisiones.h"

namespace VidaColinas
{
    constexpr float PI = 3.14159265f;

    // Luciernagas: misma curva de Lissajous 3D que X-22 (Extras::PosicionLuciernaga), alrededor del centro de VidaColinasDatos::LUCIERNAGAS:
    //   p(t) = c + (1.6 sen(0.41 k t + f), 0.45 sen(0.67 k t + 2 f), 1.6 sen(0.53 k t + 3 f)),  k = 1 + 0.07 (i mod 5);  brillo = max(0, sen(2 pi t / 2.6 + 3 f))^3
    inline void PosicionLuciernaga(int i, float t, float& x, float& y, float& z)
    {
        const VidaColinasDatos::Lucier& l = VidaColinasDatos::LUCIERNAGAS[i];
        const float k = 1.0f + 0.07f * (i % 5);
        x = l.cx + 1.6f * std::sin(0.41f * k * t + l.fase);
        y = l.cy + 0.45f * std::sin(0.67f * k * t + 2.0f * l.fase);
        z = l.cz + 1.6f * std::sin(0.53f * k * t + 3.0f * l.fase);
    }
    inline float BrilloLuciernaga(int i, float t)
    {
        const float s = std::sin(2.0f * PI * t / 2.6f + VidaColinasDatos::LUCIERNAGAS[i].fase * 3.0f);
        return s > 0.0f ? s * s * s : 0.0f;
    }

    // Brillo del orbe de un farol (mismo que X-24): 0.55 + 0.30 sen(5.1 t + 1.7 i) + 0.15 sen(11.3 t + 0.9 i)  -> 0.10 .. 1.00
    inline float BrilloFarol(int i, float t) { return 0.55f + 0.30f * std::sin(5.1f * t + 1.7f * i) + 0.15f * std::sin(11.3f * t + 0.9f * i); }
    // Brillo de una fogata: 0.80 + 0.12 sen(9.1 t + i) + 0.08 sen(17.3 t + 2 i)  -> 0.60 .. 1.00
    inline float BrilloFogata(int i, float t) { return 0.80f + 0.12f * std::sin(9.1f * t + i) + 0.08f * std::sin(17.3f * t + 2.0f * i); }

    // Toda fuente de luz calida de la escena: fogatas (llama a 0.35 m del suelo) y faroles (orbe a 1.52 m). 'indice' es el del arreglo de origen
    struct Fuente { glm::vec3 pos; bool fogata; int indice; };

    inline std::vector<Fuente> Fuentes()
    {
        std::vector<Fuente> f;
        for (int i = 0; i < Extras::N_FOGATAS; i++)
            f.push_back({ glm::vec3(Extras::FOGATAS[i].x, Colisiones::alturaSuelo(Extras::FOGATAS[i].z) + 0.35f, Extras::FOGATAS[i].z), true, i });
        for (int i = 0; i < Extras::N_FAROLES; i++)
            f.push_back({ glm::vec3(Extras::FAROLES[i].x, Colisiones::alturaSuelo(Extras::FAROLES[i].z) + 1.52f, Extras::FAROLES[i].z), false, i });
        for (int i = 0; i < VidaColinasDatos::N_FOGATAS; i++)
            f.push_back({ glm::vec3(VidaColinasDatos::FOGATAS[i].x, VidaColinasDatos::FOGATAS[i].y + 0.35f, VidaColinasDatos::FOGATAS[i].z), true, 100 + i });
        for (int i = 0; i < VidaColinasDatos::N_FAROLES; i++)
            f.push_back({ glm::vec3(VidaColinasDatos::FAROLES[i].x, VidaColinasDatos::FAROLES[i].y + 1.52f, VidaColinasDatos::FAROLES[i].z), false, 100 + i });
        return f;
    }

    // Luz puntual lista para enviar al shader (luzCalida[i]); color 0 = apagada
    struct Luz { glm::vec3 pos, color; float lineal, cuadratica; };

    // Elige las 2 fuentes mas cercanas a 'cam' (a menos de ALCANCE m) y calcula su color con parpadeo.
    //   fogata: color (1.00, 0.50, 0.18) x brillo x 1.4, atenuacion (1, 0.22, 0.20)  -> alcanza ~10 m
    //   farol : color (1.00, 0.62, 0.28) x brillo x 0.9, atenuacion (1, 0.70, 1.50)  -> alcanza ~3 m
    constexpr float ALCANCE = 25.0f;
    inline void ElegirLuces(const std::vector<Fuente>& fuentes, const glm::vec3& cam, float t, Luz out[2])
    {
        int mejor[2] = { -1, -1 };
        float dmin[2] = { ALCANCE * ALCANCE, ALCANCE * ALCANCE };
        for (int i = 0; i < (int)fuentes.size(); i++)
        {
            const glm::vec3 d = fuentes[i].pos - cam;
            const float d2 = d.x * d.x + d.y * d.y + d.z * d.z;
            if (d2 < dmin[0])      { dmin[1] = dmin[0]; mejor[1] = mejor[0]; dmin[0] = d2; mejor[0] = i; }
            else if (d2 < dmin[1]) { dmin[1] = d2; mejor[1] = i; }
        }
        for (int k = 0; k < 2; k++)
        {
            out[k] = { glm::vec3(0.0f), glm::vec3(0.0f), 1.0f, 1.0f };
            if (mejor[k] < 0) continue;
            const Fuente& f = fuentes[mejor[k]];
            out[k].pos = f.pos;
            if (f.fogata) { out[k].color = glm::vec3(1.00f, 0.50f, 0.18f) * (BrilloFogata(f.indice, t) * 1.4f); out[k].lineal = 0.22f; out[k].cuadratica = 0.20f; }
            else          { out[k].color = glm::vec3(1.00f, 0.62f, 0.28f) * (BrilloFarol(f.indice, t) * 0.9f);  out[k].lineal = 0.70f; out[k].cuadratica = 1.50f; }
        }
    }
}
