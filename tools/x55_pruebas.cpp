// X-55: pruebas numericas de las animaciones de personas (sin OpenGL). Compilar y correr desde la raiz del repo:
//   g++ -std=c++17 -Isrc tools/x55_pruebas.cpp src/PersonasAnim.cpp src/Animacion.cpp -o x55_pruebas && ./x55_pruebas
// (en Windows: cl /std:c++17 /EHsc /Isrc tools\x55_pruebas.cpp src\PersonasAnim.cpp src\Animacion.cpp). Sale con codigo 1 si falla algo.
#include <cstdio>
#include <cmath>
#include "PersonasAnim.h"

static int fallos = 0;
static void Chequea(const char* nombre, bool ok, double valor, const char* unidad) { std::printf("%s %-62s %.4f %s\n", ok ? "OK  " : "FALLA", nombre, valor, unidad); if (!ok) fallos++; }
static float Dist(const glm::vec3& a, const glm::vec3& b) { return glm::length(a - b); }
static float DifMundo(const pers::Persona& a, const pers::Persona& b)   // mayor distancia entre las articulaciones de dos poses
{
    float m = 0.0f;
    for (int i = 0; i < pers::N_ART; i++) m = std::fmax(m, Dist(glm::vec3(a.mundo[i][3]), glm::vec3(b.mundo[i][3])));
    for (int i = 0; i < pers::N_ART; i++) m = std::fmax(m, Dist(glm::vec3(a.mundo[i] * glm::vec4(0, -0.3f, 0, 1)), glm::vec3(b.mundo[i] * glm::vec4(0, -0.3f, 0, 1))));
    return m;
}

int main()
{
    using namespace pers;
    const float EPS = 1e-3f;   // 1 mm
    // 1. Baile: periodico en 8 s (orbita) y en 2 s (paso); sin saltos de pose
    { BaileFamiliar b; b.Actualizar(0.0f); Persona p0[3] = { b.persona[0], b.persona[1], b.persona[2] };
      b.Actualizar(BaileFamiliar::T_VUELTA - 1e-4f);
      float m = 0; for (int k = 0; k < 3; k++) m = std::fmax(m, DifMundo(p0[k], b.persona[k]));
      Chequea("Baile: pose en t = 0 y t = 8 s - eps (periodo de la orbita)", m < 0.01f, m, "m"); }
    // 1b. La orbita es un circulo: distancia del papa al centro entre KeyFrames (poligono de 12 lados: error maximo R(1-cos 15 grados))
    { BaileFamiliar b; float peor = 0;
      for (float t = 0; t < 8.0f; t += 0.01f) { b.Actualizar(t); const glm::vec3 c(b.persona[0].mundo[RAIZ][3]); peor = std::fmax(peor, std::fabs(std::hypot(c.x, c.z) - BaileFamiliar::RADIO_ORBITA)); }
      Chequea("Baile: error de la orbita poligonal contra el circulo (< 4 cm)", peor < 0.04f, peor, "m"); }
    // 1c. Papa y mama siempre enfrentados (distancia entre sus caderas = 2 R)
    { BaileFamiliar b; float peor = 0;
      for (float t = 0; t < 8.0f; t += 0.01f) { b.Actualizar(t); peor = std::fmax(peor, std::fabs(Dist(glm::vec3(b.persona[0].mundo[RAIZ][3]), glm::vec3(b.persona[1].mundo[RAIZ][3])) - 2.0f * BaileFamiliar::RADIO_ORBITA)); }
      Chequea("Baile: papa y mama a 2R uno del otro (error < 12 cm por la rebote y el poligono)", peor < 0.12f, peor, "m"); }
    // 2. Perro: ciclo cerrado; la pelota en la mano al inicio y al final; aterriza en (0, 0.055, -5.9); el perro la alcanza; el nino la recoge
    { JuegoPerro j; j.Actualizar(0.0f); const JuegoPerro j0 = j; const glm::vec3 pel0 = j.pelota; const Persona n0 = j.nino;
      j.Actualizar(JuegoPerro::T_CICLO - 1e-4f);
      Chequea("Perro: pelota en t = 0 y t = 10 s - eps (misma posicion)", Dist(pel0, j.pelota) < 0.01f, Dist(pel0, j.pelota), "m");
      Chequea("Perro: nino en t = 0 y t = 10 s - eps (misma pose)", DifMundo(n0, j.nino) < 0.01f, DifMundo(n0, j.nino), "m");
      Chequea("Perro: perro en t = 0 y t = 10 s - eps (cuerpo)", Dist(glm::vec3(j0.perro.mCuerpo[3]), glm::vec3(j.perro.mCuerpo[3])) < 0.01f, Dist(glm::vec3(j0.perro.mCuerpo[3]), glm::vec3(j.perro.mCuerpo[3])), "m");
      j.Actualizar(JuegoPerro::T_SUELTA + JuegoPerro::VUELO);
      Chequea("Perro: la pelota aterriza en y = radio (0.055)", std::fabs(j.pelota.y - 0.055f) < EPS, j.pelota.y, "m");
      Chequea("Perro: aterriza en z = -5.9", std::fabs(j.pelota.z + 5.9f) < EPS, j.pelota.z, "m");
      float apice = 0; for (float t = JuegoPerro::T_SUELTA; t < JuegoPerro::T_SUELTA + JuegoPerro::VUELO; t += 0.005f) { j.Actualizar(t); apice = std::fmax(apice, j.pelota.y); }
      Chequea("Perro: altura maxima del tiro (< 4 m)", apice < 4.0f, apice, "m");
      j.Actualizar(JuegoPerro::T_RECOGE_PERRO + 0.01f);
      Chequea("Perro: al recoger, el hocico esta sobre la pelota (< 10 cm)", Dist(j.perro.Hocico(), j.pelota) < 0.10f, Dist(j.perro.Hocico(), j.pelota), "m");
      // el hocico a la hora de recoger: distancia a (0, 0.055, -5.9)
      j.Actualizar(JuegoPerro::T_RECOGE_PERRO - 1e-4f);
      Chequea("Perro: el hocico llega a la pelota caida (< 12 cm)", Dist(j.perro.Hocico(), glm::vec3(0, 0.055f, -5.9f)) < 0.12f, Dist(j.perro.Hocico(), glm::vec3(0, 0.055f, -5.9f)), "m");
      { float peorPata = 0; for (float t = 2.9f; t <= 3.3f; t += 0.01f) { j.Actualizar(t); for (int i = 0; i < 4; i++) peorPata = std::fmax(peorPata, std::fabs((j.perro.mPata[i] * glm::vec4(0, -0.35f, 0, 1)).y)); }
        Chequea("Perro: en la reverencia las 4 patas tocan el suelo (|y| de la punta < 6 cm)", peorPata < 0.06f, peorPata, "m"); }
      j.Actualizar(JuegoPerro::T_RECOGE_NINO);
      Chequea("Perro: la mano del nino al recoger esta sobre la pelota (altura de la mano - 0.055 m)", std::fabs(j.nino.Mano(true).y - 0.055f) < 0.10f, j.nino.Mano(true).y - 0.055f, "m");
      Chequea("Perro: la pelota no se mueve en el instante de recogerla (< 8 cm)", Dist(j.pelota, j.nino.Mano(true)) < 0.08f, Dist(j.pelota, j.nino.Mano(true)), "m");
      float saltoMax = 0; glm::vec3 ant; bool primero = true;   // continuidad de la pelota (velocidad maxima razonable: < 12 m/s)
      for (float t = 0; t < 10.0f; t += 1.0f / 60.0f) { j.Actualizar(t); if (!primero) saltoMax = std::fmax(saltoMax, Dist(ant, j.pelota) * 60.0f); ant = j.pelota; primero = false; }
      Chequea("Perro: velocidad maxima de la pelota (< 12 m/s; sin teletransportes)", saltoMax < 12.0f, saltoMax, "m/s");
      float vp = 0; glm::vec3 a2; primero = true;
      for (float t = 0; t < 10.0f; t += 1.0f / 60.0f) { j.Actualizar(t); glm::vec3 c(j.perro.mCuerpo[3]); if (!primero) vp = std::fmax(vp, Dist(a2, c) * 60.0f); a2 = c; primero = false; }
      Chequea("Perro: velocidad maxima del perro (< 6 m/s)", vp < 6.0f, vp, "m/s"); }
    // 3. Sahumerio: periodico en 9 s
    { Sahumerio s; s.Actualizar(0.0f); const Persona p0 = s.persona; s.Actualizar(Sahumerio::T_CICLO - 1e-4f);
      Chequea("Sahumerio: pose en t = 0 y t = 9 s - eps", DifMundo(p0, s.persona) < 0.01f, DifMundo(p0, s.persona), "m"); }
    // 4. Banda: el rebote nunca es negativo ni pasa de A; el pulso: |sen| se anula una vez por pulso
    { float mn = 1e9f, mx = -1e9f; for (int i = 0; i < 8; i++) for (float t = 0; t < 10; t += 0.01f) { MovBanda m = BandaAlCompas(i, t, 1.0f, 90.0f); mn = std::fmin(mn, m.dy); mx = std::fmax(mx, m.dy); }
      Chequea("Banda: el rebote es >= 0", mn >= 0.0f, mn, "m"); Chequea("Banda: el rebote maximo es < 4 cm", mx < 0.04f, mx, "m");
      const float Tb = 60.0f / 90.0f; MovBanda a = BandaAlCompas(0, 0.0f, 0.0f, 90.0f), b = BandaAlCompas(0, Tb, 0.0f, 90.0f);
      Chequea("Banda: el rebote se repite cada pulso (Tb = 0.667 s)", std::fabs(a.dy - b.dy) < EPS, std::fabs(a.dy - b.dy), "m"); }
    // 5. Anfitriona: lejos no saluda (brazo abajo), cerca si (mano sobre el hombro) y gira hacia el visitante
    { Anfitriona f(180.0f); for (int k = 0; k < 200; k++) f.Actualizar(k / 60.0f, 1.0f / 60.0f, 0.0f, 20.0f);
      const float lejos = f.persona.Mano(true).y;
      Anfitriona g(180.0f); for (int k = 0; k < 400; k++) g.Actualizar(k / 60.0f, 1.0f / 60.0f, 3.0f, -4.0f);
      Chequea("Anfitriona: a 20 m la mano queda abajo (y < 1.0 m)", lejos < 1.0f, lejos, "m");
      Chequea("Anfitriona: a 5 m la mano queda arriba (y > 1.2 m)", g.persona.Mano(true).y > 1.2f, g.persona.Mano(true).y, "m");
      const float objetivo = glm::degrees(std::atan2(-3.0f, 4.0f));   // visitante en (3, -4): el frente (-sen g, -cos g) debe apuntar a el
      Chequea("Anfitriona: gira hacia el visitante (error de giro < 1 grado)", std::fabs(std::fmod(g.giro() - objetivo + 540.0f, 360.0f) - 180.0f) < 1.0f, g.giro(), "grados"); }
    std::printf(fallos ? "\nTOTAL fallos: %d\n" : "\nTOTAL fallos: 0\n", fallos);
    return fallos ? 1 : 0;
}
