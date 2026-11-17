#pragma once

// X-49: mariposas de las colinas con los modelos de D1 (Models/mariposa_cuerpo, mariposa_ala_der, mariposa_ala_izq; normalizados por
// tools/x49_normalizar.py a x0.075: cuerpo 7.5 cm, envergadura 14.5 cm). Animacion PROCEDURAL (funciones cerradas, sin KeyFrames ni particulas).
// Extra ambiental: NO cuenta entre las 5 animaciones del plan ni entre los modelos propios. Posiciones de MariposasDatos.h (tools/x49_mariposas.py).
//
// Trayectoria de la mariposa i (curva de Lissajous suave; t en segundos; k = 1 + 0.08 (i mod 5)):
//   x(t) = cx + A sen(0.36 k t + f)         z(t) = cz + B sen(0.53 k t + 2 f)
//   y(t) = h(x, z) + 1.8 + 0.5 sen(0.71 k t + 3 f) + 0.03 cos(2 pi fa t + f)       h = Terreno::altura (sigue el suelo; el ultimo termino es el vaiven del aleteo)
// Rumbo = direccion de la velocidad:  guinada = atan2(vx, vz),  cabeceo = -atan2(vy, hypot(vx, vz))   (la cabeza del modelo apunta a +z).
// Aleteo: angulo de cada ala respecto a su pose de reposo (levantada ~26 grados) alrededor del eje del cuerpo (z) por la bisagra (0, 0.0195, 0):
//   d(t) = -6 grados + 50 grados sen(2 pi fa t + f),   fa = 4.0 + 0.3 (i mod 4) Hz;  ala izquierda +d, ala derecha -d (espejo).
//   (Resultado: las alas oscilan entre ~-30 y ~+70 grados sobre la horizontal.)

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MariposasDatos.h"
#include "Terreno.h"

namespace Mariposas
{
    constexpr float DOS_PI = 6.2831853f;
    constexpr float BISAGRA_Y = 0.0195f;   // 0.26 m (modelo original) x 0.075

    inline float Cadencia(int i) { return 4.0f + 0.3f * (i % 4); }                 // Hz
    inline float Velocidad(int i) { return 1.0f + 0.08f * (i % 5); }                // factor k

    struct Pose { glm::vec3 pos; float guinada, cabeceo; };

    inline Pose PoseEn(int i, float t)
    {
        const MariposasDatos::Mariposa& m = MariposasDatos::MARIPOSAS[i];
        const float k = Velocidad(i), f = m.fase, fa = Cadencia(i);
        const float a1 = 0.36f * k, a2 = 0.53f * k, a3 = 0.71f * k;
        const float x = m.cx + m.A * std::sin(a1 * t + f);
        const float z = m.cz + m.B * std::sin(a2 * t + 2.0f * f);
        const float y = Terreno::altura(x, z) + 1.8f + 0.5f * std::sin(a3 * t + 3.0f * f) + 0.03f * std::cos(DOS_PI * fa * t + f);
        const float vx = m.A * a1 * std::cos(a1 * t + f);
        const float vz = m.B * a2 * std::cos(a2 * t + 2.0f * f);
        const float vy = 0.5f * a3 * std::cos(a3 * t + 3.0f * f);
        Pose p;
        p.pos = glm::vec3(x, y, z);
        p.guinada = std::atan2(vx, vz);
        p.cabeceo = -std::atan2(vy, std::hypot(vx, vz));
        return p;
    }

    // Matriz del cuerpo (las alas parten de ella); angulo de aleteo d(t) en radianes
    inline glm::mat4 Cuerpo(const Pose& p)
    {
        glm::mat4 m = glm::translate(glm::mat4(1.0f), p.pos);
        m = glm::rotate(m, p.guinada, glm::vec3(0.0f, 1.0f, 0.0f));
        return glm::rotate(m, p.cabeceo, glm::vec3(1.0f, 0.0f, 0.0f));
    }
    inline float Aleteo(int i, float t)
    {
        return glm::radians(-6.0f + 50.0f * std::sin(DOS_PI * Cadencia(i) * t + MariposasDatos::MARIPOSAS[i].fase));
    }
    // lado = +1 ala izquierda (x > 0), -1 ala derecha (x < 0)
    inline glm::mat4 Ala(const glm::mat4& cuerpo, float d, int lado)
    {
        glm::mat4 m = glm::translate(cuerpo, glm::vec3(0.0f, BISAGRA_Y, 0.0f));
        m = glm::rotate(m, lado * d, glm::vec3(0.0f, 0.0f, 1.0f));
        return glm::translate(m, glm::vec3(0.0f, -BISAGRA_Y, 0.0f));
    }
}
