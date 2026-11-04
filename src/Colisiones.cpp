#include "Colisiones.h"

#include <algorithm>

void Colisiones::agregar(const AABB& caja)
{
    cajas.push_back(caja);
}

float Colisiones::alturaSuelo(float z)
{
    return (z <= 20.0f) ? 0.0f : -0.05f * (z - 20.0f);
}

bool Colisiones::choca(float x, float z) const
{
    // Caja del cuerpo: de los pies (suelo) a la cabeza
    float suelo = alturaSuelo(z);
    glm::vec3 cMin(x - RADIO, suelo, z - RADIO);
    glm::vec3 cMax(x + RADIO, suelo + ALTURA_CUERPO, z + RADIO);

    for (const AABB& c : cajas)
    {
        // Dos AABB se traslapan solo si lo hacen en los tres ejes
        bool traslapa = cMin.x < c.max.x && cMax.x > c.min.x &&
                        cMin.y < c.max.y && cMax.y > c.min.y &&
                        cMin.z < c.max.z && cMax.z > c.min.z;
        if (traslapa) return true;
    }
    return false;
}

glm::vec3 Colisiones::resolver(const glm::vec3& actual, const glm::vec3& deseada) const
{
    float x = actual.x, z = actual.z;

    // Primero el eje X (con z antes del movimiento), luego el eje Z (con la x ya resuelta)
    float nx = std::clamp(deseada.x, -X_MAX + RADIO, X_MAX - RADIO);
    if (!choca(nx, z)) x = nx;

    float nz = std::clamp(deseada.z, Z_MIN + RADIO, Z_MAX - RADIO);
    if (!choca(x, nz)) z = nz;

    // Siempre a 1.6 m sobre el suelo de ese punto (camina en el plano XZ; sigue la pendiente de la orilla)
    return glm::vec3(x, alturaSuelo(z) + ALTURA_OJOS, z);
}
