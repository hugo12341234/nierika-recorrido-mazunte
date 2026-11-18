// X-53: prueba de Terreno.h contra tools/x53_terreno.py (lo corre tools/x53_comparar_cpp.py).
// Lee lineas "x z" de la entrada estandar y escribe "x z altura inclinacion_x inclinacion_z(esc 1.5)" con Terreno::altura y Terreno::inclinacion.
#include <cstdio>
#include "../src/Terreno.h"
int main()
{
    double x, z;
    while (std::scanf("%lf %lf", &x, &z) == 2)
    {
        float gx, gz;
        Terreno::inclinacion((float)x, (float)z, 1.5f, gx, gz);
        std::printf("%.6f %.6f %.6f %.6f %.6f\n", x, z, Terreno::altura((float)x, (float)z), gx, gz);
    }
    return 0;
}
