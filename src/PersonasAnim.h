// PersonasAnim.h — X-55 (17/11/2026): cinco animaciones nuevas de personas y un perro (3 por KeyFrames + 2 simples).
//   KeyFrames (reusan Pista, KeyFrame, Suavizar y Slerp de Animacion.h, el mismo sistema que la caminata de la silueta):
//     1. BaileFamiliar  — una familia baila la chilena (papa, mama y nina).
//     2. JuegoPerro     — un nino lanza la pelota y su perro la trae (la pelota va en tiro parabolico de forma cerrada).
//     3. Sahumerio      — una senora sahuma la ofrenda con el copalero (sin humo de particulas: restriccion del profesor).
//   Simples (solo senos, sin KeyFrames):
//     4. BandaAlCompas  — rebote, balanceo e inclinacion de cada musico segun el pulso (90 BPM) y su seccion.
//     5. Anfitriona     — saluda con la mano y gira hacia el visitante que se acerca.
// Solo matematica y matrices (sin OpenGL): main.cpp dibuja cada pieza con Model::Draw y Atuendos::dibujarPartes. Todo se calcula en
// coordenadas LOCALES del sitio (origen en el suelo, el personaje mira a -z); main.cpp multiplica por la matriz del sitio.
// Justificacion matematica de cada formula: docs/manual_tecnico/justificacion_personas.md.
#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Animacion.h"

namespace pers
{
    constexpr float PI = 3.14159265f;
    constexpr float G = 9.81f;   // m/s^2

    inline glm::quat Rx(float grados) { return glm::angleAxis(glm::radians(grados), glm::vec3(1, 0, 0)); }
    inline glm::quat Ry(float grados) { return glm::angleAxis(glm::radians(grados), glm::vec3(0, 1, 0)); }
    inline glm::quat Rz(float grados) { return glm::angleAxis(glm::radians(grados), glm::vec3(0, 0, 1)); }

    // ---- Sitios (x, z, giro Y en grados, radio libre en m). Filas numericas: tools/x48_verificar_solapes.py las lee (huella de la animacion).
    //   0 baile familiar  1 nino y perro (el nino; la pelota vuela hacia -z local)  2 sahumerio  3 anfitriona
    struct Sitio { float x, z, giro, radio; };
    constexpr int N_SITIOS = 4;
    inline const Sitio SITIOS[N_SITIOS] = {
        { 22.5f, 14.0f, 0.0f, 1.5f }, { -14.5f, 8.0f, -90.0f, 0.4f }, { 1.9f, -6.8f, 0.0f, 0.4f }, { 2.8f, 22.0f, 180.0f, 0.4f } };
    constexpr int SITIO_BAILE = 0, SITIO_PERRO = 1, SITIO_SAHUMERIO = 2, SITIO_ANFITRIONA = 3;

    // ======== Persona articulada (Models/persona_art: torso, brazo, antebrazo, muslo, pierna; pivote en cada articulacion) ========
    // Jerarquia: RAIZ (cadera, a 0.90 m) -> TORSO -> brazos -> antebrazos; RAIZ -> muslos -> piernas. Las piernas cuelgan de la RAIZ (no del torso)
    // para que el balanceo de la cintura no arrastre los pies. Medidas iguales a las de la silueta (Silueta.cpp). Rotaciones: + sobre X = hacia delante (-z);
    // + sobre Z mueve un miembro colgante hacia +x (abduccion del brazo DERECHO); sobre Y > 0 gira a la izquierda.
    enum Art { RAIZ, TORSO, BRAZO_I, ANTEB_I, BRAZO_D, ANTEB_D, MUSLO_I, PIERNA_I, MUSLO_D, PIERNA_D, N_ART };
    constexpr float ALTURA_CADERA = 0.90f;

    struct Persona
    {
        glm::quat rot[N_ART];       // rotacion local de cada articulacion (se anima)
        glm::vec3 despRaiz;         // desplazamiento de la cadera respecto de la pose de pie (rebote, cuclillas), m
        glm::mat4 mundo[N_ART];     // resultado de Calcular()

        Persona() { Reposo(); }
        void Reposo();
        // base = T(piso) * R(giro) * S(talla) en coordenadas locales del sitio
        void Calcular(const glm::mat4& base);
        // Palma de la mano derecha / izquierda (punta del antebrazo) en coordenadas locales
        glm::vec3 Mano(bool derecha = true) const;
    };

    // ======== Perro (Models/perro: cuerpo, cabeza, cola, pata x4) ========
    struct Perro
    {
        glm::quat rotCuerpo, rotCabeza, rotCola, rotPata[4];   // patas: 0 delantera izq., 1 delantera der., 2 trasera izq., 3 trasera der.
        glm::vec3 despCuerpo;
        glm::mat4 mCuerpo, mCabeza, mCola, mPata[4];
        Perro() { Reposo(); }
        void Reposo();
        void Calcular(const glm::mat4& base);       // base = T(piso) * R(giro); el tronco queda a 0.45 m
        glm::vec3 Hocico() const;                   // punta del hocico (donde sostiene la pelota)
    };

    // ======== 1. BAILE FAMILIAR (KeyFrames) ========
    // Ciclo de paso de 2 s (zapateado, pañuelo, cadera). Papa y mama giran uno alrededor del otro en 8 s sobre 12 KeyFrames de posicion
    // (cada 30 grados, orientados al centro); la nina gira sobre si misma una vuelta cada 4 s (KeyFrames cada 90 grados).
    class BaileFamiliar
    {
    public:
        static constexpr int N = 3;            // 0 papa, 1 mama, 2 nina
        static constexpr float T_PASO = 2.0f, T_VUELTA = 8.0f, RADIO_ORBITA = 0.95f, ESCALA_NINA = 0.62f;
        BaileFamiliar();
        void Actualizar(float t);
        Persona persona[N];
    private:
        anim::Pista orbita_, giroNina_;
        anim::Pista muslo_, pierna_, cintura_, rebote_;
        anim::Pista brazoD_[N], anteD_[N], brazoI_[N], anteI_[N];
    };

    // ======== 2. JUEGO CON EL PERRO (KeyFrames) ========
    // Ciclo de 10 s. Sitio: origen = pies del nino, el nino mira a -z, la pelota cae en (0, -5.9). El perro parte de su "casa" H = (0.60, -0.70) y recoge la pelota con una reverencia de juego (pecho abajo, patas delanteras al frente).
    class JuegoPerro
    {
    public:
        static constexpr float T_CICLO = 10.0f;
        static constexpr float T_SUELTA = 0.95f, VUELO = 1.5f;      // la pelota sale de la mano y vuela 1.5 s
        static constexpr float T_RECOGE_PERRO = 3.1f, T_SUELTA_PERRO = 6.2f, T_TOSS = 0.45f, T_RECOGE_NINO = 7.3f;
        static constexpr float ESCALA_NINO = 0.62f;
        JuegoPerro();
        void Actualizar(float t);
        Persona nino;
        Perro perro;
        glm::mat4 baseNino, basePerro;       // matrices base locales (para dibujar con la talla del nino)
        glm::vec3 pelota;                     // posicion local de la pelota
    private:
        void PoseNino(float tc, Persona& p) const;
        void PosePerro(float tc, Perro& d, glm::mat4& base, float& distancia) const;
        anim::Pista giroTorso_, inclina_, brazoD_, anteD_, brazoI_, cuclillas_, muslo_, pierna_;
        anim::Pista ruta_, marcha_, cabeza_, paso_, agachar_;
        glm::vec3 puntoRecoge_;               // donde esta la mano del nino al recoger la pelota
    };

    // ======== 3. SAHUMERIO (KeyFrames) ========
    class Sahumerio
    {
    public:
        static constexpr float T_CICLO = 9.0f;
        Sahumerio();
        void Actualizar(float t);
        Persona persona;
        glm::mat4 copalero;                   // matriz local del copalero que sostiene en la mano derecha
    private:
        anim::Pista torso_, brazoD_, anteD_, brazoI_, anteI_;
    };

    // ======== 4. BANDA AL COMPAS (simple: senos) ========
    // Rebote  dy(t)   = A |sen(pi t / Tb + fase)|             (un golpe del pie por pulso; |sen| tiene un pico en cada pulso)
    // Giro lateral    = R sen(pi t / Tb + fase)               (un ciclo cada 2 pulsos)
    // Inclinacion     = P sen(2 pi t / Tb + fase + pi/2)      (el instrumento sube y baja una vez por pulso)
    // con Tb = 60 / BPM.
    struct MovBanda { float dy, rollGrados, pitchGrados; };
    inline MovBanda BandaAlCompas(int inst, float t, float fase, float bpm)
    {
        // por familia de instrumentos (0 trompeta, 1 clarinete, 2 saxofon, 3 trombon, 4 tuba, 5 tambora, 6 tarola, 7 platillos)
        static const float A[8] = { 0.028f, 0.022f, 0.022f, 0.028f, 0.012f, 0.036f, 0.036f, 0.032f };   // rebote (m)
        static const float R[8] = { 3.0f, 4.0f, 4.0f, 3.5f, 2.0f, 2.5f, 2.5f, 3.0f };                    // giro lateral (grados)
        static const float P[8] = { 5.0f, 3.0f, 3.5f, 5.0f, 2.0f, 1.5f, 1.5f, 6.0f };                    // inclinacion (grados)
        const float Tb = 60.0f / bpm;
        MovBanda m;
        m.dy = A[inst] * std::fabs(std::sin(PI * t / Tb + fase));
        m.rollGrados = R[inst] * std::sin(PI * t / Tb + fase);
        m.pitchGrados = P[inst] * std::sin(2.0f * PI * t / Tb + fase + 0.5f * PI);
        return m;
    }

    // ======== 5. ANFITRIONA (simple: senos + giro hacia el visitante) ========
    class Anfitriona
    {
    public:
        static constexpr float ALCANCE_SALUDO = 12.0f, ALCANCE_PLENO = 9.0f;   // m: saluda con fuerza completa a menos de 9 m, nada a mas de 12 m
        Anfitriona(float giroBaseGrados) : giroBase_(giroBaseGrados), giro_(giroBaseGrados) {}
        // t: reloj (s); dt: duracion del cuadro; dx, dz: vector del sitio a la camara (mundo). Devuelve el giro actual (grados) en 'giro()'
        void Actualizar(float t, float dt, float dx, float dz);
        float giro() const { return giro_; }
        Persona persona;
    private:
        float giroBase_, giro_;
    };
}
