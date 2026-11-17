#pragma once

// X-49: llama de las fogatas con el modelo `fuego` (D1, Models/fuego, normalizado por tools/x49_normalizar.py: UNA llama de 0.30 m con el pivote en
// su base). Cada fogata dibuja 3 lenguas (alturas 1.00, 0.75 y 0.65) con la misma malla. No es KeyFrames ni particulas: el parpadeo es una suma de senos.
//   alto:      sy(t) = 1 + 0.12 sen(2 pi 7.3 t + f) + 0.06 sen(2 pi 12.1 t + 1.7 f)            (f = fase de la lengua)
//   ancho:     sxz(t) = 1 / sqrt(sy)                                                          (el volumen sx*sy*sz se conserva: si sube, adelgaza)
//   balanceo:  ax(t) = 0.10 sen(2 pi 1.9 t + f),  az(t) = 0.10 sen(2 pi 2.3 t + 2 f)          (cizalla: la punta se desplaza 10 % de su altura)
//   brillo:    b(t) = 1.55 + 0.20 sen(2 pi 5.3 t + f) + 0.10 sen(2 pi 11.7 t + 2 f)           (multiplica la textura; llama = 5 en lighting.frag)
// Matriz del modelo:  M = T(base + (dx, 0, dz)) * Ry(giro) * Cizalla(ax, az) * Escala(sxz * e, sy * e, sxz * e).
// Funciones inline; cabecera sin .cpp (como pide CLAUDE.md para las clases heredadas del curso y los ayudantes de escena).

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace LlamasFogata
{
    constexpr float DOS_PI = 6.2831853f;
    constexpr int N_LENGUAS = 3;
    struct Lengua { float dx, dz, esc, giroGrados, fase; };
    inline const Lengua LENGUAS[N_LENGUAS] = {
        {  0.00f,  0.00f, 1.00f,   0.0f, 0.0f },
        {  0.11f,  0.06f, 0.75f, 120.0f, 2.1f },
        { -0.09f,  0.08f, 0.65f, 240.0f, 4.2f } };

    // 'i' es el numero de la fogata: desfasa las llamas entre fogatas para que no parpadeen al unisono
    inline float Fase(int i, int l) { return LENGUAS[l].fase + 1.3f * i; }

    inline glm::mat4 Modelo(const glm::vec3& base, int i, int l, float t)
    {
        const Lengua& g = LENGUAS[l];
        const float f = Fase(i, l);
        const float sy = 1.0f + 0.12f * std::sin(DOS_PI * 7.3f * t + f) + 0.06f * std::sin(DOS_PI * 12.1f * t + 1.7f * f);
        const float sxz = 1.0f / std::sqrt(sy);
        glm::mat4 ciz(1.0f);
        ciz[1][0] = 0.10f * std::sin(DOS_PI * 1.9f * t + f);        // x += ax * y
        ciz[1][2] = 0.10f * std::sin(DOS_PI * 2.3f * t + 2.0f * f); // z += az * y
        glm::mat4 m = glm::translate(glm::mat4(1.0f), base + glm::vec3(g.dx, 0.0f, g.dz));
        m = glm::rotate(m, glm::radians(g.giroGrados), glm::vec3(0.0f, 1.0f, 0.0f));
        m = m * ciz;
        return glm::scale(m, glm::vec3(sxz * g.esc, sy * g.esc, sxz * g.esc));
    }

    inline float Brillo(int i, int l, float t)
    {
        const float f = Fase(i, l);
        return 1.55f + 0.20f * std::sin(DOS_PI * 5.3f * t + f) + 0.10f * std::sin(DOS_PI * 11.7f * t + 2.0f * f);
    }
}
