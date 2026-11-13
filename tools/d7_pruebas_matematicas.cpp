// D-7 (act. 53): pruebas numericas de las formulas de justificacion_complejas.md contra el CODIGO REAL del proyecto.
// No usa OpenGL. Compilar y correr desde la raiz del repo (Linux o Developer PowerShell con g++/clang; en Visual Studio basta
// crear un proyecto de consola con estos 3 archivos y la carpeta src/ en las rutas de inclusion):
//   g++ -std=c++17 -Isrc tools/d7_pruebas_matematicas.cpp src/Animacion.cpp src/Silueta.cpp -o d7_pruebas && ./d7_pruebas
// Termina con codigo 0 solo si TODAS las comprobaciones pasan.
#include <cstdio>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/constants.hpp>
#include "Animacion.h"
#include "Silueta.h"
#include "DejarFlor.h"

static int fallos = 0;
static void comprueba(const char* que, bool ok, double valor, const char* esperado)
{
    std::printf("[%s] %-62s -> %.6g  (esperado %s)\n", ok ? " OK " : "FALLA", que, valor, esperado);
    if (!ok) fallos++;
}
static double angulo(const glm::quat& q) { return 2.0 * std::acos(std::fmin(1.0, std::fabs((double)q.w))); }  // rad, giro total de q

int main()
{
    using namespace anim;
    // ---- 1. Slerp (Animacion.cpp) ----
    const glm::quat q0(1, 0, 0, 0);
    const glm::quat q1 = glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0));
    glm::quat m = Slerp(q0, q1, 0.5f);
    comprueba("slerp(0 -> 90 grados en X, s = 0.5): giro de", std::fabs(angulo(m) - glm::radians(45.0f)) < 1e-5, glm::degrees(angulo(m)), "45 grados");
    comprueba("slerp: s = 0 devuelve q0", glm::length(Slerp(q0, q1, 0.0f) - q0) < 1e-6, glm::length(Slerp(q0, q1, 0.0f) - q0), "0");
    comprueba("slerp: s = 1 devuelve q1", glm::length(Slerp(q0, q1, 1.0f) - q1) < 1e-6, glm::length(Slerp(q0, q1, 1.0f) - q1), "0");
    double maxDesvio = 0, maxNorma = 0;
    for (int i = 0; i <= 100; i++)
    {
        float s = i / 100.0f;
        glm::quat q = Slerp(q0, q1, s);
        maxDesvio = std::fmax(maxDesvio, std::fabs(angulo(q) - s * glm::radians(90.0f)));   // velocidad angular constante: angulo lineal en s
        maxNorma = std::fmax(maxNorma, std::fabs(glm::length(q) - 1.0f));                   // sigue siendo cuaternion unitario
    }
    comprueba("slerp: desvio maximo del angulo respecto de la recta (rad)", maxDesvio < 1e-5, maxDesvio, "< 1e-5");
    comprueba("slerp: | |q| - 1 | maximo", maxNorma < 1e-6, maxNorma, "< 1e-6");
    glm::quat menosQ1 = -q1;   // misma rotacion que q1: debe tomar el camino corto (45 grados, no 315)
    comprueba("slerp con -q1 toma el camino corto: giro a s = 0.5", std::fabs(angulo(Slerp(q0, menosQ1, 0.5f)) - glm::radians(45.0f)) < 1e-5, glm::degrees(angulo(Slerp(q0, menosQ1, 0.5f))), "45 grados");
    glm::quat casi = glm::angleAxis(glm::radians(0.5f), glm::vec3(0, 1, 0));   // cos(theta/2) > 0.9995: rama nlerp
    comprueba("slerp casi iguales (nlerp): sin NaN y unitario", std::fabs(glm::length(Slerp(q0, casi, 0.5f)) - 1.0f) < 1e-6, glm::length(Slerp(q0, casi, 0.5f)), "1");

    // ---- 2. Smoothstep y sus mitades (Animacion.cpp) ----
    comprueba("S(0.25) = 0.15625", std::fabs(Suavizar(0.25f, Curva::Smoothstep) - 0.15625f) < 1e-6, Suavizar(0.25f, Curva::Smoothstep), "0.15625");
    comprueba("S(0.75) = 0.84375", std::fabs(Suavizar(0.75f, Curva::Smoothstep) - 0.84375f) < 1e-6, Suavizar(0.75f, Curva::Smoothstep), "0.84375");
    const float h = 1e-3f;
    double dS = (Suavizar(0.5f + h, Curva::Smoothstep) - Suavizar(0.5f - h, Curva::Smoothstep)) / (2 * h);
    comprueba("S'(1/2) por diferencias finitas", std::fabs(dS - 1.5) < 1e-3, dS, "1.5");
    double dS0 = (Suavizar(h, Curva::Smoothstep) - Suavizar(0.0f, Curva::Smoothstep)) / h;
    comprueba("S'(0) ~ 0 (diferencia hacia delante = 3h - 2h^2 con h = 1e-3)", dS0 < 5e-3, dS0, "~ 0.003");
    double dIn1 = (Suavizar(1.0f, Curva::SmoothIn) - Suavizar(1.0f - h, Curva::SmoothIn)) / h;
    comprueba("SmoothIn'(1) = 1.5 (velocidad con la que entra al crucero)", std::fabs(dIn1 - 1.5) < 5e-3, dIn1, "1.5");
    double dOut0 = (Suavizar(h, Curva::SmoothOut) - Suavizar(0.0f, Curva::SmoothOut)) / h;
    comprueba("SmoothOut'(0) = 1.5", std::fabs(dOut0 - 1.5) < 5e-3, dOut0, "1.5");
    comprueba("SmoothIn(1) = 1 y SmoothOut(1) = 1", std::fabs(Suavizar(1.0f, Curva::SmoothIn) - 1) < 1e-6 && std::fabs(Suavizar(1.0f, Curva::SmoothOut) - 1) < 1e-6, Suavizar(1.0f, Curva::SmoothOut), "1");

    // ---- 3. Caminata (Silueta.cpp) ----
    Silueta s;
    comprueba("duracion de la caminata (s)", std::fabs(s.Duracion() - 18.333f) < 0.01, s.Duracion(), "18.33");
    auto zTorso = [&](float t) { s.Actualizar(t); return glm::vec3(s.Nodos()[0].mundo * glm::vec4(0, 0, 0, 1)); };
    comprueba("z inicial del torso (orilla)", std::fabs(zTorso(0).z - 24.0f) < 1e-4, zTorso(0).z, "24");
    comprueba("z final del torso", std::fabs(zTorso(s.Duracion()).z - 0.2f) < 1e-3, zTorso(s.Duracion()).z, "0.2");
    double v = (zTorso(2.0f - 0.01f).z - zTorso(2.0f + 0.01f).z) / 0.02;   // m/s hacia -z al terminar la rampa
    comprueba("velocidad al terminar la rampa de arranque (m/s)", std::fabs(v - 1.4) < 0.03, v, "1.4");
    double vf = (zTorso(s.Duracion() - 2.0f - 0.01f).z - zTorso(s.Duracion() - 2.0f + 0.01f).z) / 0.02;
    comprueba("velocidad al empezar el frenado (m/s)", std::fabs(vf - 1.4) < 0.03, vf, "1.4");
    // Slerp de la pierna derecha: ciclo de medio periodo de desfase respecto de la izquierda (en un instante con apoyo)
    s.Actualizar(8.0f);
    float angI = 0, angD = 0;
    for (const Nodo& n : s.Nodos()) { if (n.nombre == "muslo izq") angI = glm::degrees(glm::eulerAngles(n.rot).x); if (n.nombre == "muslo der") angD = glm::degrees(glm::eulerAngles(n.rot).x); }
    comprueba("muslos en oposicion a t = 8 s (suma de angulos ~ 0)", std::fabs(angI + angD) < 1.5, angI + angD, "~ 0");

    // ---- 4. Bezier cuadratica = caida libre (DejarFlor.h) ----
    float p0[3] = { 0.67f, 1.336f, -0.389f }, p2[3] = { 1.05f, 0.435f, -0.389f }, o[3];
    float hh = p0[1] - p2[1], Tb = dejarflor::TiempoCaida(hh);
    double maxErr = 0;
    for (int i = 0; i <= 200; i++)
    {
        float tau = Tb * i / 200.0f;
        dejarflor::Posicion(p0, p2, tau, o);
        maxErr = std::fmax(maxErr, std::fabs(o[1] - (p0[1] - 0.5f * dejarflor::G * tau * tau)));
    }
    comprueba("Tb = sqrt(2h/g) (s)", std::fabs(Tb - 0.4286) < 1e-3, Tb, "0.429");
    comprueba("error maximo Bezier contra y0 - g*tau^2/2 (m)", maxErr < 1e-5, maxErr, "< 1e-5");
    dejarflor::Posicion(p0, p2, Tb, o);
    comprueba("la flor llega al destino en Tb", std::fabs(o[0] - p2[0]) < 1e-5 && std::fabs(o[1] - p2[1]) < 1e-5, o[1], "0.435");

    std::printf("\n%s (%d fallos)\n", fallos ? "HAY FALLOS" : "TODO OK", fallos);
    return fallos ? 1 : 0;
}
