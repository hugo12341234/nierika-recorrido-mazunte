// Animacion.h — Modulo de KeyFrames de Nierika Studio (tarea D-2, adelantada el 04/11/2026)
//
// Contiene SOLO matematica (sin OpenGL), para poder probarla aparte:
//   - Curvas de aceleracion: lineal, smoothstep y sus dos mitades (SmoothIn / SmoothOut).
//   - Slerp propio entre cuaterniones (interpolacion esferica).
//   - KeyFrame (tiempo + posicion + cuaternion) y Pista (lista de KeyFrames que se evalua en el tiempo).
// Se usa SOLO para la caminata de la silueta y para "dejar flores" (restriccion del profesor).
// La justificacion matematica de cada formula esta en docs/manual_tecnico/ (D-7); aqui van comentadas.
#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace anim
{
    // Curva de aceleracion de UN tramo entre dos KeyFrames: convierte el avance lineal u in [0,1] en s in [0,1]
    enum class Curva
    {
        Lineal,      // s = u                      (velocidad constante)
        Smoothstep,  // s = 3u^2 - 2u^3            (arranca y frena suave: velocidad 0 en los dos extremos)
        SmoothIn,    // primera mitad de smoothstep reescalada (arranca suave, termina con velocidad)
        SmoothOut    // segunda mitad de smoothstep reescalada (arranca con velocidad, frena suave)
    };

    // s = Suavizar(u, curva). u se recorta a [0,1].
    float Suavizar(float u, Curva curva);

    // Interpolacion esferica entre dos cuaterniones unitarios: slerp(a, b, s), s in [0,1].
    //   slerp = [ sin((1-s)*theta) * a + sin(s*theta) * b ] / sin(theta),   cos(theta) = a . b
    // Si a . b < 0 se invierte b (q y -q son la misma rotacion) para tomar el camino corto.
    // Si los cuaterniones son casi iguales (sin(theta) ~ 0) se usa nlerp para no dividir entre ~0.
    glm::quat Slerp(const glm::quat& a, const glm::quat& b, float s);

    struct Pose
    {
        glm::vec3 pos = glm::vec3(0.0f);
        glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);  // identidad
    };

    // Cuadro clave: en el instante t (segundos) la pieza esta en pos con rotacion rot.
    // "curva" es la aceleracion del tramo que SALE de este KeyFrame hacia el siguiente.
    struct KeyFrame
    {
        float t;
        glm::vec3 pos;
        glm::quat rot;
        Curva curva;
    };

    // Lista ordenada de KeyFrames de una pieza. La posicion se interpola con mix (lineal) y la rotacion con Slerp,
    // los dos con el mismo s = Suavizar(u, curva del KeyFrame de salida).
    class Pista
    {
    public:
        // ciclica = true: el tiempo se repite (t mod duracion); el ultimo KeyFrame debe repetir la pose del primero.
        explicit Pista(bool ciclica = false) : ciclica_(ciclica) {}

        // Agrega un KeyFrame; los tiempos deben ir en orden creciente.
        void Agregar(float t, const glm::vec3& pos, const glm::quat& rot, Curva curva);

        float Duracion() const;           // tiempo del ultimo KeyFrame menos el del primero
        Pose Evaluar(float t) const;      // pose interpolada en el instante t

    private:
        std::vector<KeyFrame> kf_;
        bool ciclica_;
    };
}
