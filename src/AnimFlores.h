// C-4 (act. 42): flores de cempasuchil flotando en la orilla.
// Animacion simple (sin KeyFrames): movimiento circular de radio pequeño + rotacion continua + altura de la ola (C-1),
// con velocidad variable y pausas. Todo es una funcion cerrada del tiempo t (sin acumular deltaTime),
// asi que el resultado no depende de los FPS. Solo funciones inline: no hay estado ni recursos de GPU.
#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Parametros de una flor (cada flor tiene los suyos para que no se muevan al unisono)
struct FlorFlotante
{
    glm::vec2 centro;   // centro del circulo en el plano XZ (m)
    float radio;        // radio del circulo R (m)
    float sentido;      // +1 antihorario, -1 horario (visto desde +Y)
    float tMov;         // duracion de la fase de movimiento por ciclo (s)
    float tPausa;       // duracion de la pausa por ciclo (s)
    float desfase;      // adelanto de tiempo t0 (s): evita que todas pausen a la vez
    float omegaGiro;    // velocidad de rotacion propia alrededor de Y (rad/s)
    float faseGiro;     // angulo inicial de esa rotacion (rad)
};

const float PI_FLORES = 3.14159265358979f;

// Angulo recorrido en el circulo, theta(t), con velocidad angular variable y pausas.
// Ciclo de periodo T = tMov + tPausa. Dentro de la fase de movimiento (u = t mod T, 0 <= u < tMov):
//     omega(u) = omegaMax * sin^2(pi * u / tMov)          (0 en los extremos, maximo en la mitad)
// Fuera de ella (tMov <= u < T): omega = 0 (pausa).
// Integrando (sin^2 x = (1 - cos 2x)/2):
//     theta(u) = omegaMax * ( u/2 - tMov * sin(2*pi*u/tMov) / (4*pi) )
// Se fija omegaMax = 2*pi / tMov, asi cada fase de movimiento recorre exactamente
//     dTheta = omegaMax * tMov / 2 = pi  (media vuelta)
// y la flor se detiene en puntos opuestos del circulo. Tras n ciclos: theta = n*pi + theta(u).
// Como omega es 0 en los extremos de la fase, la velocidad es continua: arranca y frena sin saltos.
inline float AnguloConPausas(float t, float tMov, float tPausa)
{
    const float T = tMov + tPausa;
    const float n = std::floor(t / T);
    const float u = t - n * T;                       // 0 <= u < T
    const float uMov = (u < tMov) ? u : tMov;        // en la pausa el angulo se queda en el del final del movimiento
    const float omegaMax = 2.0f * PI_FLORES / tMov;
    const float theta = omegaMax * (uMov * 0.5f - tMov * std::sin(2.0f * PI_FLORES * uMov / tMov) / (4.0f * PI_FLORES));
    return n * PI_FLORES + theta;
}

// Posicion de la flor en el plano XZ en el instante t:
//     x = cx + R cos(theta),  z = cz + R sin(theta)   con theta = sentido * AnguloConPausas(t + desfase)
inline glm::vec2 PosicionXZ(const FlorFlotante& f, float t)
{
    const float theta = f.sentido * AnguloConPausas(t + f.desfase, f.tMov, f.tPausa);
    return glm::vec2(f.centro.x + f.radio * std::cos(theta), f.centro.y + f.radio * std::sin(theta));
}

// Matriz de modelo de una flor en el instante t.
//   alturaAgua: y de la superficie del mar bajo la flor (C-1: Olas::Altura en su posicion XZ), asi sube y baja con las olas
//   centroMalla: centro del .obj, para girar la flor sobre su propio centro y no sobre el origen del modelo
// Altura:    y = alturaAgua + calado   (la flor, de 12 cm de alto, queda 2 cm hundida; la ola la sube y baja)
// Giro:      psi = faseGiro + omegaGiro * t  alrededor de Y (continuo, no se detiene en las pausas)
inline glm::mat4 MatrizFlorFlotante(const FlorFlotante& f, float t, float alturaAgua, glm::vec3 centroMalla)
{
    const float calado = 0.02f;           // m
    const glm::vec2 p = PosicionXZ(f, t);
    const float psi = f.faseGiro + f.omegaGiro * t;

    glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(p.x, alturaAgua + calado, p.y));
    m = glm::rotate(m, psi, glm::vec3(0.0f, 1.0f, 0.0f));
    m = glm::translate(m, -centroMalla);
    return m;
}
