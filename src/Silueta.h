// Silueta.h — Silueta jerarquica de cubos y su caminata por KeyFrames (tareas D-1 y D-2, adelantadas el 04/11/2026)
//
// Sin OpenGL: solo calcula la matriz de mundo de cada pieza; main.cpp las dibuja con un cubo.
// Los cubos son PROVISIONALES: cuando exista el modelo de D-1 (Models/silueta/) se sustituyen sin tocar la jerarquia.
// Jerarquia (CONTEXTO_PROYECTO.md, seccion 4): Torso (raiz) -> Cabeza, Brazo izq/der -> Antebrazo izq/der,
// Muslo izq/der -> Pierna izq/der. Cada articulo rota alrededor de su articulacion (hombro, codo, cadera, rodilla).
// La silueta mira hacia -z (de la orilla, z = +24, hacia el altar, z ~ 0).
#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Animacion.h"

struct Nodo
{
    std::string nombre;
    int padre;            // indice del padre en la lista (-1 = raiz); el padre siempre va antes que sus hijos
    glm::vec3 offset;     // posicion de la articulacion respecto de la articulacion del padre (m)
    glm::quat rot;        // rotacion local de la articulacion (se anima)
    glm::vec3 centro;     // centro del cubo respecto de su articulacion (m)
    glm::vec3 tam;        // lados del cubo (m)
    glm::vec3 color;
    glm::mat4 mundo;      // resultado de Actualizar()
};

class Silueta
{
public:
    Silueta();

    // Calcula la pose de la caminata en el instante t (segundos desde el inicio) y las matrices de mundo.
    // Despues de Duracion() la silueta se queda en el punto de llegada y hace la animacion de dejar la flor (D-4):
    // tau = t - Duracion(): 0-1 s extiende el brazo derecho, 2 s suelta la flor, 3-4 s regresa el brazo.
    void Actualizar(float t);

    // D-4: tiempos (segundos desde que termina la caminata) de la animacion de dejar la flor
    static constexpr float T_EXTIENDE = 1.0f;   // el brazo termina de extenderse
    static constexpr float T_SUELTA = 2.0f;     // la flor sale de la mano
    static constexpr float T_REGRESA = 3.0f;    // empieza a regresar el brazo
    static constexpr float T_FIN = 4.0f;        // brazo en reposo otra vez

    // D-4: posicion de la mano derecha (punta del antebrazo) segun la pose calculada por el ultimo Actualizar()
    glm::vec3 PosicionMano() const;
    // D-4: donde esta la mano en el instante de soltar la flor (Duracion() + T_SUELTA); no cambia la pose actual
    glm::vec3 ManoAlSoltar() const;

    float Duracion() const { return duracion_; }
    const std::vector<Nodo>& Nodos() const { return nodos_; }

    // Matriz para dibujar el cubo unitario (lado 1, centrado) de la pieza i: mundo * T(centro) * S(tam)
    glm::mat4 MatrizCubo(size_t i) const;

private:
    int Agregar(const char* nombre, int padre, glm::vec3 offset, glm::vec3 centro, glm::vec3 tam, glm::vec3 color);

    std::vector<Nodo> nodos_;
    float duracion_;

    // indices de las piezas
    int torso_, cabeza_, brazoIzq_, antebrazoIzq_, brazoDer_, antebrazoDer_;
    int musloIzq_, piernaIzq_, musloDer_, piernaDer_;

    // Pistas de KeyFrames. La del recorrido va en segundos; las del ciclo de paso, en segundos de ciclo
    // (T_CICLO = 1 s a velocidad de crucero). La pierna/brazo derechos usan la misma pista que los izquierdos
    // con medio ciclo de desfase.
    anim::Pista recorrido_;   // posicion del torso a lo largo de z (arranque, crucero, frenado)
    anim::Pista lateral_;     // paso al costado en x despues de cruzar el arco, para no tapar el altar
    anim::Pista altura_;      // altura de la cadera (sube y baja 2 veces por ciclo)
    anim::Pista giro_;        // giro del torso alrededor de Y (una vez por ciclo)
    anim::Pista muslo_, pierna_, brazo_, antebrazo_;  // lado izquierdo
    anim::Pista brazoFlor_, antebrazoFlor_;           // D-4: brazo derecho al dejar la flor (tau desde el fin de la caminata)
};
