#pragma once

// Colisiones AABB (A-9). Caja alineada a los ejes (Axis-Aligned Bounding Box) contra el cuerpo del visitante.
// Unidades: metros (1 u = 1 m). Todo en coordenadas del mundo.

#include <vector>
#include <glm/glm.hpp>

// Caja alineada a los ejes: esquina minima y maxima
struct AABB
{
    glm::vec3 min;
    glm::vec3 max;
};

class Colisiones
{
public:
    // Valores iniciales (decision del 04/11 en ESTADO, seccion 5)
    static constexpr float ALTURA_OJOS = 1.6f;    // ojos sobre el suelo (decision del 19/10)
    static constexpr float ALTURA_CUERPO = 1.7f;  // el cuerpo mide 1.7 m: la cabeza queda 0.1 m sobre los ojos
    static constexpr float RADIO = 0.25f;         // el cuerpo es una caja de 0.5 x 0.5 m en planta

    // Zona caminable en planta (la playa llega a x +-50, z -20...30; la orilla esta en z = +24)
    static constexpr float X_MAX = 50.0f;
    static constexpr float Z_MIN = -20.0f;
    static constexpr float Z_MAX = 24.0f;

    // Agrega una caja solida (muro, capilla...)
    void agregar(const AABB& caja);

    // Altura del suelo en z: y = 0 hasta z = +20 y baja 0.05 m por metro (perfil de playa.obj, A-5)
    static float alturaSuelo(float z);

    // Recibe donde estaba la camara y donde quiere ir; devuelve la posicion final (con ojos a 1.6 m del suelo).
    // Resuelve cada eje por separado: si choca de frente en un eje, se desliza por el otro.
    glm::vec3 resolver(const glm::vec3& actual, const glm::vec3& deseada) const;

private:
    std::vector<AABB> cajas;

    // true si el cuerpo centrado en (x, z) se traslapa con alguna caja
    bool choca(float x, float z) const;
};
