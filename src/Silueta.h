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
    // Despues de Duracion() la silueta se queda quieta en el punto de llegada.
    void Actualizar(float t);

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
    anim::Pista altura_;      // altura de la cadera (sube y baja 2 veces por ciclo)
    anim::Pista giro_;        // giro del torso alrededor de Y (una vez por ciclo)
    anim::Pista muslo_, pierna_, brazo_, antebrazo_;  // lado izquierdo
};
