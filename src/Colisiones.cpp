#include "Colisiones.h"

#include <algorithm>
#include "Terreno.h"

void Colisiones::agregar(const AABB& caja)
{
    cajas.push_back(caja);
}

float Colisiones::alturaSuelo(float z)
{
    return Terreno::alturaPlaya(z);
}

float Colisiones::alturaTerreno(float x, float z)
{
    return Terreno::altura(x, z);
}

bool Colisiones::choca(float x, float z) const
{
    // Caja del cuerpo: de los pies (suelo) a la cabeza
    float suelo = alturaTerreno(x, z);   // X-33: el suelo depende tambien de x (colinas)
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

    // Primero el eje X (con z antes del movimiento), luego el eje Z (con la x ya resuelta).
    // X-33: un paso solo vale si el terreno es caminable (dentro de la malla, fuera del mar y con pendiente <= 0.70) y no hay caja delante
    if (Terreno::caminable(deseada.x, z, RADIO) && !choca(deseada.x, z)) x = deseada.x;
    if (Terreno::caminable(x, deseada.z, RADIO) && !choca(x, deseada.z)) z = deseada.z;

    // Siempre a 1.6 m sobre el suelo de ese punto (camina en el plano XZ y sigue el relieve: orilla y colinas)
    return glm::vec3(x, alturaTerreno(x, z) + ALTURA_OJOS, z);
}
