// X-55: ofrendas familiares de la playa (opcion C de D2, 17/11/2026: se quitan las tumbas en filas sobre la arena y se ponen ofrendas de familias).
// Solo datos y matematica (sin GPU): main.cpp dibuja las piezas con la matriz T(P) R(giro) T(pos) R(giroItem) S(esc) y el color `tinte`.
// Cada una de las 14 familias es DISTINTA a proposito: otra estructura (lancha, mesa, soportes de papel picado, altar de tres niveles, arco,
// cruz, altar mayor chico, hamaca, circulo de velas, camino de velas, cestas, roca, tabla de remo, petate de nino), otro tamano (radio 0.8 a 2.4 m),
// otro numero de velas, otros objetos del difunto, otra paleta (tinte de mantel, velas, flores y papel) y otro giro.
// Marco cultural (docs/GUION_OFRENDA.md): lo que dice una fuente verificada (SEP, 2026; UNESCO, s. f.; Banos, 2025) es tradicion; lo demas
// (objetos de cada difunto, frutas, colores, estructuras) es ADAPTACION del estudio y se marca asi en las fichas.
// Marco local: +z = frente de la ofrenda, 1 u = 1 m. Giro Y como glm::rotate: un punto local (x, z) pasa a (x cos g + z sen g, -x sen g + z cos g).
#ifndef OFRENDAS_FAMILIARES_H
#define OFRENDAS_FAMILIARES_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <glm/glm.hpp>
#include "Extras.h"
#include "VidaColinasDatos.h"

namespace OfrendasFamiliares
{
    constexpr float PI = 3.14159265f;
    constexpr float CENTRO_CUERPO = 0.24f;   // X-67: distancia (m) del anclaje de persona_agachada al centro de su cuerpo, hacia donde mira (medida del .obj: z -0.76..0.28)

    enum Pieza { VELA, VELADORA, RACIMO, FLOR, PAN, COPALERO, BOTELLA, REFRESCO, MARCO, CUADRO, CESTA, PETATE, CRUZ, MAZETA, ALF_FLORES, ALF_CRUZ,
                 MESA, ALTAR3, ALTAR_MAYOR, ARCO, SOPORTES, PAPEL, CALABAZA, NARANJAS, GUITARRA, HAMACA, LANCHA, RED, TABLA, TAMBOR, FAROL, ROCA, CALAVERA,
                 N_PIEZAS };

    struct Item { Pieza tipo; glm::vec3 pos; float esc; float giro; glm::vec3 tinte; };   // pos local; giro en grados Y; tinte = color por el que se multiplica la textura
    struct Velador { float x, z, mira; int modelo; };                                      // persona de rodillas (modelo 0 = agachada A, 1 = B); mira = giro local en grados
    struct Caja { float x0, z0, x1, z1, alto; };                                           // colision AABB en el mundo

    struct Familia
    {
        const char* id;            // identificador corto (para el verificador y los comentarios)
        const char* estructura;    // arquetipo, en palabras
        float x, z, giro;          // lugar en el mundo (lo calcula Colocar) y giro Y en grados
        float radio;               // radio de la huella (m), medido sobre las piezas
        std::vector<Item> items;
        std::vector<Velador> velan;
    };

    // Radio de huella de cada pieza con esc = 1 (del .obj; ver tools/x55_verificar_ofrendas.cpp) y si lleva llama
    inline float RadioPieza(Pieza p)
    {
        static const float r[N_PIEZAS] = { 0.03f, 0.05f, 0.35f, 0.05f, 0.08f, 0.09f, 0.04f, 0.04f, 0.10f, 0.13f, 0.20f, 0.61f, 0.30f, 0.12f, 0.71f, 0.71f,
                                           0.75f, 0.92f, 2.15f, 1.20f, 0.93f, 0.85f, 0.11f, 0.09f, 0.14f, 1.65f, 3.30f, 1.20f, 1.65f, 0.22f, 0.25f, 0.30f, 0.10f };
        return r[p];
    }
    inline bool LlevaLlama(Pieza p) { return p == VELA || p == VELADORA; }
    // alto de la mecha de cada vela con esc = 1 (la llama nace ahi)
    inline float AltoVela(Pieza p) { return p == VELADORA ? 0.20f : 0.15f; }

    inline float Azar(int i)   // numero repetible en [0,1)
    {
        const float s = std::sin((float)i * 12.9898f) * 43758.5453f;
        return s - std::floor(s);
    }
    inline glm::vec3 Girar(glm::vec3 p, float gradosY)
    {
        const float g = gradosY * PI / 180.0f, c = std::cos(g), s = std::sin(g);
        return glm::vec3(p.x * c + p.z * s, p.y, -p.x * s + p.z * c);
    }

    // ----- paletas (tinte = multiplicador de la textura; las texturas de vela, veladora, mantel y petate son casi neutras o claras)
    const glm::vec3 BLANCO(1.0f), CREMA(1.0f, 0.93f, 0.78f), ROJO(1.0f, 0.30f, 0.30f), ROSA(1.0f, 0.55f, 0.75f), MORADO(0.65f, 0.35f, 0.95f),
                    AZUL(0.45f, 0.65f, 1.0f), TURQUESA(0.30f, 0.90f, 0.85f), VERDE(0.50f, 0.95f, 0.45f), AMARILLO(1.0f, 0.90f, 0.35f),
                    NARANJA(1.0f, 0.62f, 0.25f), LIMON(0.80f, 1.0f, 0.30f);
    // flores: la textura es naranja, asi que solo se desplaza hacia amarillo, rojizo o apagado (no hay flores blancas ni moradas: no se inventan)
    const glm::vec3 F_NARANJA(1.0f), F_AMARILLA(1.0f, 1.0f, 0.55f), F_ROJIZA(1.0f, 0.62f, 0.55f), F_APAGADA(0.72f, 0.72f, 0.72f);

    // ----- ayudantes de composicion
    struct Cons
    {
        Familia f;
        void add(Pieza t, float x, float y, float z, float esc = 1.0f, float giro = 0.0f, glm::vec3 tinte = glm::vec3(1.0f)) { f.items.push_back({ t, glm::vec3(x, y, z), esc, giro, tinte }); }
        // n velas en fila de (x0, z0) a (x1, z1) a la altura y
        void fila(Pieza t, int n, float x0, float z0, float x1, float z1, float y, float esc, glm::vec3 tinte, glm::vec3 tinte2 = glm::vec3(-1.0f))
        {
            for (int i = 0; i < n; i++)
            {
                const float u = n > 1 ? (float)i / (float)(n - 1) : 0.5f;
                add(t, x0 + (x1 - x0) * u, y, z0 + (z1 - z0) * u, esc, 0.0f, (tinte2.x >= 0.0f && i % 2) ? tinte2 : tinte);
            }
        }
        // n piezas en un arco de a0 a a1 grados (0 = +z) de radio r alrededor de (cx, cz)
        void arco(Pieza t, int n, float cx, float cz, float r, float a0, float a1, float y, float esc, glm::vec3 tinte, glm::vec3 tinte2 = glm::vec3(-1.0f))
        {
            for (int i = 0; i < n; i++)
            {
                const float u = n > 1 ? (float)i / (float)(n - 1) : 0.5f;
                const float a = (a0 + (a1 - a0) * u) * PI / 180.0f;
                add(t, cx + r * std::sin(a), y, cz + r * std::cos(a), esc, 0.0f, (tinte2.x >= 0.0f && i % 2) ? tinte2 : tinte);
            }
        }
        // persona de rodillas cuyo CUERPO queda centrado en (x, z) y que mira al punto (tx, ty) (coordenadas locales).
        // X-67: el modelo persona_agachada mira hacia -Z local (cabeza en z -0.29, manos en z -0.76, pies en z +0.28): antes se giraba con +Z hacia
        // la ofrenda y la gente le daba la espalda. Ahora se suma 180 grados y el punto de anclaje se retrocede 0.48 m (distancia del anclaje al
        // centro del cuerpo x 2) para que el cuerpo siga donde ya habia sido verificado sin solapes y solo las manos apunten a la ofrenda.
        void vela_persona(float x, float z, float tx, float tz, int modelo)
        {
            const float dx = tx - x, dz = tz - z, d = std::max(std::sqrt(dx * dx + dz * dz), 1e-4f);
            f.velan.push_back({ x - CENTRO_CUERPO * 2.0f * dx / d, z - CENTRO_CUERPO * 2.0f * dz / d, std::atan2(dx, dz) * 180.0f / PI + 180.0f, modelo });
        }
    };

    inline Cons Nueva(const char* id, const char* estructura) { Cons c; c.f = Familia{ id, estructura, 0.0f, 0.0f, 0.0f, 0.0f, {}, {} }; return c; }

    // =============================================================================================================================
    // LAS 14 FAMILIAS. Cada una tiene su descripcion: quien es (adaptacion del estudio, no personas reales), que estructura y que objetos.
    // =============================================================================================================================
    inline std::vector<Familia> Familias()
    {
        std::vector<Familia> v;

        // 1. Pescador: su lancha varada con la red colgada, petate con su retrato, botellas y una cruz de piedras. Grande (radio ~2.4 m). Paleta turquesa.
        {
            Cons c = Nueva("pescador", "lancha varada");
            c.add(LANCHA, 0.0f, 0.0f, 0.0f, 0.52f, 0.0f, glm::vec3(0.85f, 1.0f, 1.0f));
            c.add(RED, 0.43f, 0.52f, 0.45f, 0.40f, 90.0f, TURQUESA);
            c.add(PETATE, 1.30f, 0.0f, 0.6f, 1.1f, 90.0f, glm::vec3(0.9f, 0.95f, 1.0f));
            c.add(CUADRO, 1.30f, 0.02f, 0.30f, 1.8f, -90.0f);
            c.fila(VELADORA, 4, 1.05f, 0.98f, 1.55f, 0.98f, 0.02f, 0.9f, TURQUESA, BLANCO);
            c.add(BOTELLA, 1.00f, 0.02f, 0.55f, 1.2f); c.add(BOTELLA, 1.60f, 0.02f, 0.35f, 1.2f); c.add(BOTELLA, 1.60f, 0.02f, 0.70f, 1.2f);
            c.add(PAN, 1.50f, 0.02f, 0.85f, 1.2f);
            c.add(CRUZ, -1.15f, 0.0f, 0.2f, 0.8f);
            for (int i = 0; i < 6; i++) c.add(ROCA, -1.15f + 0.50f * std::sin(i * 1.05f), 0.0f, 0.2f + 0.50f * std::cos(i * 1.05f), 0.55f + 0.1f * (i % 3), 40.0f * i);
            c.add(RACIMO, -1.15f, 0.0f, 0.80f, 0.7f, 0.0f, F_AMARILLA);
            c.add(FAROL, -0.5f, 0.0f, 1.95f, 1.0f, 0.0f);
            c.vela_persona(0.85f, 1.35f, 1.30f, 0.5f, 0); c.vela_persona(2.0f, 1.2f, 1.30f, 0.5f, 1);
            v.push_back(c.f);
        }
        // 2. Cocinera: mesa con mantel rojo cargada de cestas, pan, calabazas y naranjas; petate con mas cestas. Paleta roja.
        {
            Cons c = Nueva("cocinera", "mesa con mantel");
            const float yt = 0.78f * 1.15f;
            c.add(MESA, 0.0f, 0.0f, 0.0f, 1.15f, 0.0f, glm::vec3(1.0f, 0.55f, 0.50f));
            c.add(CESTA, -0.55f, yt, -0.05f, 1.1f, 0.0f, CREMA); c.add(CESTA, 0.05f, yt, -0.12f, 0.85f, 0.0f); c.add(CESTA, 0.55f, yt, -0.05f, 1.25f, 0.0f, CREMA);
            c.fila(PAN, 9, -0.62f, 0.31f, 0.62f, 0.31f, yt, 1.0f, BLANCO);
            c.add(CALABAZA, -0.30f, yt, 0.12f, 1.0f); c.add(CALABAZA, 0.30f, yt, 0.15f, 0.8f, 40.0f); c.add(CALABAZA, 0.0f, yt, 0.05f, 1.2f, 90.0f);
            c.add(NARANJAS, -0.68f, yt, 0.28f, 1.0f); c.add(NARANJAS, 0.68f, yt, 0.30f, 1.0f, 0.0f, AMARILLO);
            c.fila(VELADORA, 6, -0.66f, -0.36f, 0.66f, -0.36f, yt, 1.0f, ROJO, AMARILLO);
            c.add(CUADRO, 0.0f, yt, -0.30f, 1.0f); c.add(COPALERO, 0.45f, yt, 0.33f, 1.0f);
            c.add(PETATE, 0.0f, 0.0f, 1.45f, 1.25f, 0.0f, glm::vec3(1.0f, 0.88f, 0.75f));
            c.add(CESTA, -0.35f, 0.02f, 1.45f, 1.3f); c.add(CESTA, 0.40f, 0.02f, 1.55f, 1.0f, 0.0f, CREMA);
            c.add(RACIMO, -0.20f, 0.02f, 1.75f, 0.8f, 0.0f, F_ROJIZA); c.add(RACIMO, 0.20f, 0.02f, 1.20f, 0.7f, 30.0f);
            c.add(CALABAZA, 0.62f, 0.02f, 1.35f, 1.1f, 20.0f); c.add(CALABAZA, -0.70f, 0.02f, 1.60f, 0.7f);
            c.add(MAZETA, -1.15f, 0.0f, 0.35f, 1.4f); c.add(MAZETA, 1.15f, 0.0f, 0.35f, 1.4f);
            c.vela_persona(-1.05f, 1.55f, 0.0f, 1.0f, 0); c.vela_persona(1.10f, 1.55f, 0.0f, 1.0f, 1); c.vela_persona(0.0f, 2.0f, 0.0f, 1.3f, 0);
            v.push_back(c.f);
        }
        // 3. Musico: soportes con papel picado, guitarra apoyada, tambor y arco de velas altas sobre un petate grande. Paleta rosa y morada.
        {
            Cons c = Nueva("musico", "soportes de papel picado");
            c.add(SOPORTES, 0.0f, 0.0f, -0.95f, 1.2f);
            c.add(PAPEL, 0.0f, 2.33f, -0.95f, 1.2f, 0.0f, glm::vec3(1.0f, 0.85f, 1.0f));
            c.add(PETATE, 0.0f, 0.0f, 0.35f, 1.7f, 0.0f, glm::vec3(1.0f, 0.92f, 0.85f));
            c.add(GUITARRA, -0.85f, 0.23f, -0.80f, 1.0f, 15.0f);
            c.add(TAMBOR, 0.75f, 0.02f, 0.05f, 1.3f);
            c.add(MARCO, -0.35f, 0.02f, -0.1f, 1.7f); c.add(MARCO, 0.30f, 0.02f, -0.22f, 1.4f);
            c.fila(VELADORA, 4, -0.5f, 0.45f, 0.5f, 0.45f, 0.02f, 1.0f, ROSA, MORADO);
            c.arco(VELA, 7, 0.0f, 0.35f, 1.55f, -75.0f, 75.0f, 0.0f, 2.0f, ROSA, BLANCO);
            c.add(RACIMO, -1.15f, 0.0f, -0.35f, 0.9f, 0.0f, F_ROJIZA); c.add(RACIMO, 1.15f, 0.0f, -0.35f, 0.9f);
            c.add(BOTELLA, -0.15f, 0.02f, 0.65f, 1.2f); c.add(BOTELLA, 0.05f, 0.02f, 0.75f, 1.2f);
            c.fila(PAN, 4, -0.2f, 0.9f, 0.7f, 0.9f, 0.02f, 1.0f, BLANCO);
            c.add(FAROL, -1.4f, 0.0f, 1.1f); c.add(FAROL, 1.4f, 0.0f, 1.1f, 1.0f, 180.0f);
            c.vela_persona(-1.0f, 1.0f, -0.2f, 0.2f, 1);
            v.push_back(c.f);
        }
        // 4. Abuela: altar de tres niveles con veladoras amarillas, retrato grande y macetas; alfombra de flores al frente. Paleta calida.
        {
            Cons c = Nueva("abuela", "altar de tres niveles");
            const float s = 1.15f;
            c.add(ALTAR3, 0.0f, 0.0f, 0.0f, s, 0.0f, glm::vec3(1.0f, 0.75f, 0.85f));
            c.fila(VELADORA, 8, -0.65f * s, 0.38f * s, 0.65f * s, 0.38f * s, 0.40f * s, 1.0f, AMARILLO, CREMA);
            c.fila(PAN, 5, -0.55f * s, 0.26f * s, 0.55f * s, 0.26f * s, 0.40f * s, 1.0f, BLANCO);
            c.add(CALABAZA, 0.70f * s, 0.40f * s, 0.30f * s, 0.8f);
            c.add(COPALERO, 0.0f, 0.80f * s, 0.02f * s, 1.2f);
            c.add(RACIMO, -0.50f * s, 0.80f * s, 0.05f * s, 0.55f, 0.0f, F_AMARILLA); c.add(RACIMO, 0.50f * s, 0.80f * s, 0.05f * s, 0.55f);
            c.add(NARANJAS, -0.25f * s, 0.80f * s, 0.10f * s, 0.9f); c.add(NARANJAS, 0.28f * s, 0.80f * s, 0.10f * s, 0.9f, 0.0f, LIMON);
            c.add(MARCO, -0.60f * s, 0.80f * s, -0.12f * s, 1.5f); c.add(MARCO, 0.60f * s, 0.80f * s, -0.12f * s, 1.5f);
            c.add(CUADRO, 0.0f, 1.20f * s, -0.27f * s, 1.7f);
            c.add(VELA, -0.40f * s, 1.20f * s, -0.27f * s, 2.0f, 0.0f, AMARILLO); c.add(VELA, 0.40f * s, 1.20f * s, -0.27f * s, 2.0f, 0.0f, AMARILLO);
            c.add(MAZETA, -1.10f * s, 0.0f, 0.55f * s, 1.2f); c.add(MAZETA, 1.10f * s, 0.0f, 0.55f * s, 1.2f);
            c.add(ALF_FLORES, 0.0f, 0.0f, 1.35f, 1.4f);
            c.fila(VELADORA, 3, -0.4f, 1.9f, 0.4f, 1.9f, 0.0f, 1.0f, CREMA);
            c.vela_persona(-0.75f, 1.65f, 0.0f, 0.9f, 0); c.vela_persona(0.80f, 1.70f, 0.0f, 0.9f, 1);
            v.push_back(c.f);
        }
        // 5. Nino: ofrenda pequena a ras de suelo: un petate chico, calavera de azucar, tamborcito, refresco y tres veladoras rosas. (radio ~0.8 m)
        {
            Cons c = Nueva("nino", "petate chico a ras de suelo");
            c.add(PETATE, 0.0f, 0.0f, 0.0f, 0.75f, 12.0f);
            c.add(CALAVERA, -0.13f, 0.015f, -0.05f, 1.3f, 20.0f);
            c.add(TAMBOR, 0.20f, 0.015f, -0.02f, 0.45f);
            c.add(REFRESCO, 0.05f, 0.015f, 0.12f, 1.0f);
            c.add(NARANJAS, -0.22f, 0.015f, 0.15f, 0.8f, 0.0f, AMARILLO);
            c.add(CALABAZA, 0.27f, 0.015f, 0.17f, 0.55f);
            c.add(MARCO, 0.0f, 0.015f, -0.20f, 0.9f);
            c.fila(VELADORA, 3, -0.15f, 0.26f, 0.15f, 0.26f, 0.015f, 0.8f, ROSA);
            c.add(RACIMO, -0.40f, 0.0f, -0.05f, 0.45f, 0.0f, F_AMARILLA);
            v.push_back(c.f);   // sin persona: la ofrenda chica se deja a ras de suelo, sola

        }
        // 6. Arco de flores: el arco de carrizo y cempasuchil enmarca un retrato sobre alfombra de petalos, con dos filas de velas. Paleta roja y blanca.
        {
            Cons c = Nueva("arco", "arco de flores");
            c.add(ARCO, 0.0f, 0.0f, 0.0f, 1.0f);
            c.add(ALF_FLORES, 0.0f, 0.0f, 0.95f, 2.1f);
            c.add(PETATE, 0.0f, 0.0f, 0.30f, 0.9f, 0.0f, glm::vec3(1.0f, 0.85f, 0.7f));
            c.add(CUADRO, 0.0f, 0.02f, 0.10f, 2.2f);
            c.fila(VELA, 9, -1.0f, 0.78f, 1.0f, 0.78f, 0.0f, 2.4f, ROJO);
            c.fila(VELA, 9, -1.2f, 1.12f, 1.2f, 1.12f, 0.0f, 2.4f, BLANCO);
            c.add(COPALERO, -0.65f, 0.02f, 0.35f, 1.4f); c.add(COPALERO, 0.65f, 0.02f, 0.35f, 1.4f);
            c.add(RACIMO, -1.10f, 0.0f, 0.15f, 1.0f, 0.0f, F_ROJIZA); c.add(RACIMO, 1.10f, 0.0f, 0.15f, 1.0f, 30.0f); c.add(RACIMO, -1.10f, 0.0f, -0.25f, 0.8f); c.add(RACIMO, 1.10f, 0.0f, -0.25f, 0.8f, 60.0f, F_ROJIZA);
            c.add(PAN, -0.25f, 0.02f, 0.55f, 1.2f); c.add(PAN, 0.25f, 0.02f, 0.55f, 1.2f);
            c.vela_persona(-1.45f, 1.45f, -0.3f, 0.8f, 0);
            v.push_back(c.f);
        }
        // 7. Cruz de piedras: cruz alta con anillo de rocas y 16 veladoras verdes y blancas; cruz de flores en el piso y un retrato al pie. Paleta verde.
        {
            Cons c = Nueva("cruz", "cruz de piedras");
            c.add(CRUZ, 0.0f, 0.0f, 0.0f, 1.8f);
            c.add(ALF_CRUZ, 0.0f, 0.0f, 0.55f, 2.4f);
            for (int i = 0; i < 8; i++) c.add(ROCA, 0.85f * std::sin(i * 0.785f), 0.0f, 0.85f * std::cos(i * 0.785f), 0.9f + 0.15f * (i % 3), 47.0f * i);
            c.arco(VELADORA, 16, 0.0f, 0.0f, 1.45f, 0.0f, 337.5f, 0.0f, 1.0f, VERDE, BLANCO);
            for (int i = 0; i < 5; i++) c.add(RACIMO, 0.45f * std::sin(i * 1.26f + 0.4f), 0.0f, 0.45f * std::cos(i * 1.26f + 0.4f), 0.9f - 0.08f * i, 25.0f * i, i % 2 ? F_NARANJA : F_AMARILLA);
            c.add(MARCO, 0.0f, 0.0f, 0.78f, 1.3f);
            c.vela_persona(-0.9f, 1.55f, 0.0f, 0.6f, 1); c.vela_persona(0.9f, 1.55f, 0.0f, 0.6f, 0);
            v.push_back(c.f);
        }
        // 8. Altar mayor chico: el altar_mayor a 0.72 con otra composicion (copal, calabazas, tres retratos y veladoras azules). Paleta azul.
        {
            Cons c = Nueva("altar_chico", "altar mayor a escala 0.72");
            const float s = 0.72f;
            c.add(ALTAR_MAYOR, 0.0f, 0.0f, 0.0f, s, 0.0f, glm::vec3(0.75f, 0.85f, 1.0f));
            c.fila(PAN, 2, -0.75f * s, 0.80f * s, 0.75f * s, 0.80f * s, 0.40f * s, 1.0f, BLANCO);
            c.fila(VELADORA, 5, -1.30f * s, 0.90f * s, 1.30f * s, 0.90f * s, 0.40f * s, 0.9f, AZUL, BLANCO);
            c.add(NARANJAS, -0.15f * s, 0.40f * s + 0.0f, 0.78f * s, 0.9f); c.add(NARANJAS, 0.20f * s, 0.40f * s, 0.80f * s, 0.9f, 0.0f, LIMON);
            c.add(COPALERO, 0.0f, 0.80f * s, 0.30f * s, 1.5f);
            c.add(CALABAZA, -0.80f * s, 0.80f * s, 0.30f * s, 0.9f); c.add(CALABAZA, 0.80f * s, 0.80f * s, 0.30f * s, 0.9f, 60.0f);
            c.fila(VELA, 5, -0.9f * s, 0.49f * s, 0.9f * s, 0.49f * s, 0.80f * s, 1.3f, AZUL);
            c.add(MARCO, -0.55f * s, 1.20f * s, -0.12f * s, 2.0f); c.add(MARCO, 0.0f, 1.20f * s, -0.12f * s, 2.0f); c.add(MARCO, 0.55f * s, 1.20f * s, -0.12f * s, 2.0f);
            c.add(RACIMO, -0.95f * s, 1.20f * s, -0.05f * s, 0.5f, 0.0f, F_APAGADA); c.add(RACIMO, 0.95f * s, 1.20f * s, -0.05f * s, 0.5f);
            c.add(CUADRO, 0.0f, 1.55f * s, -0.40f * s, 2.3f);
            c.add(VELA, -0.55f * s, 1.55f * s, -0.40f * s, 2.0f, 0.0f, BLANCO); c.add(VELA, 0.55f * s, 1.55f * s, -0.40f * s, 2.0f, 0.0f, BLANCO);
            c.fila(VELADORA, 10, -1.3f * s, 1.55f * s, 1.3f * s, 1.55f * s, 0.0f, 1.0f, AZUL, TURQUESA);
            c.vela_persona(-1.15f, 1.80f, 0.0f, 1.1f, 0);
            v.push_back(c.f);
        }
        // 9. Abuelo: su hamaca colgada, petate con botellas, retrato y copal; tres rocas. Paleta amarilla y de madera.
        {
            Cons c = Nueva("abuelo", "hamaca y petate");
            c.add(HAMACA, 0.0f, 0.0f, 0.0f, 0.8f);
            c.add(PETATE, 0.0f, 0.0f, 0.95f, 1.0f, 8.0f, glm::vec3(1.0f, 0.95f, 0.8f));
            c.add(CUADRO, 0.0f, 0.02f, 0.78f, 1.8f);
            for (int i = 0; i < 4; i++) c.add(BOTELLA, -0.38f + 0.13f * i, 0.02f, 1.12f, 1.3f + 0.1f * (i % 2));
            c.add(REFRESCO, 0.40f, 0.02f, 1.05f, 1.2f); c.add(REFRESCO, 0.52f, 0.02f, 0.95f, 1.2f);
            c.fila(VELADORA, 8, -0.50f, 0.72f, 0.50f, 0.72f, 0.02f, 1.0f, AMARILLO, NARANJA);
            c.add(COPALERO, -0.30f, 0.02f, 0.95f, 1.6f);
            c.fila(PAN, 3, 0.15f, 0.92f, 0.45f, 0.92f, 0.02f, 1.0f, BLANCO);
            c.add(ROCA, -1.15f, 0.0f, 1.0f, 0.8f, 20.0f); c.add(ROCA, 1.20f, 0.0f, 0.75f, 1.1f, 80.0f); c.add(ROCA, 1.0f, 0.0f, 1.4f, 0.6f, 140.0f);
            c.add(RACIMO, -0.85f, 0.0f, 0.5f, 0.7f, 0.0f, F_AMARILLA);
            c.vela_persona(-0.85f, 1.75f, 0.0f, 1.0f, 1);
            v.push_back(c.f);
        }
        // 10. Circulo de velas: retrato al centro sobre petalos y dos anillos de velas (14 veladoras y 20 velas), sin estructura. Paleta blanca y calida.
        {
            Cons c = Nueva("circulo", "circulo de velas");
            c.add(ALF_FLORES, 0.0f, 0.0f, 0.0f, 1.6f);
            c.add(PETATE, 0.0f, 0.0f, 0.0f, 0.55f, 0.0f, glm::vec3(1.0f, 0.9f, 0.8f));
            c.add(CUADRO, 0.0f, 0.02f, 0.0f, 2.0f);
            c.arco(VELADORA, 14, 0.0f, 0.0f, 1.00f, 0.0f, 334.0f, 0.0f, 1.0f, BLANCO, CREMA);
            c.arco(VELA, 20, 0.0f, 0.0f, 1.65f, 9.0f, 351.0f, 0.0f, 1.6f, CREMA, AMARILLO);
            c.add(RACIMO, 0.0f, 0.0f, 0.55f, 0.6f, 0.0f, F_AMARILLA); c.add(RACIMO, 0.0f, 0.0f, -0.55f, 0.6f, 90.0f);
            c.vela_persona(0.0f, 1.70f, 0.0f, 0.5f, 0);
            v.push_back(c.f);
        }
        // 11. Camino de velas: dos hileras de veladoras de colores que llevan a un retrato sobre una roca; dos faroles de entrada. Baja y larga.
        {
            Cons c = Nueva("camino", "camino de velas hacia un retrato");
            c.add(ROCA, 0.0f, 0.0f, -1.5f, 1.5f, 30.0f);
            c.add(CUADRO, 0.0f, 0.42f, -1.38f, 1.6f);
            c.fila(VELADORA, 7, -0.55f, -1.0f, -0.55f, 1.4f, 0.0f, 1.0f, NARANJA, ROJO);
            c.fila(VELADORA, 7, 0.55f, -1.0f, 0.55f, 1.4f, 0.0f, 1.0f, AMARILLO, NARANJA);
            c.add(FAROL, -0.9f, 0.0f, 1.7f, 1.0f, 0.0f); c.add(FAROL, 0.9f, 0.0f, 1.7f, 1.0f, 180.0f);
            for (int i = 0; i < 7; i++) c.add(FLOR, (i % 2 ? 0.2f : -0.2f), 0.04f, -0.8f + 0.35f * i, 1.5f, 17.0f * i);
            c.add(RACIMO, -0.40f, 0.0f, -1.45f, 0.7f, 0.0f, F_ROJIZA); c.add(RACIMO, 0.40f, 0.0f, -1.45f, 0.7f);
            c.vela_persona(1.5f, 0.0f, 0.55f, 0.0f, 0);
            v.push_back(c.f);
        }
        // 12. Campo: seis cestas de tamanos distintos con flores y frutas, macetas y una alfombra de petalos grande; retrato sobre petate chico. Paleta verde y naranja.
        {
            Cons c = Nueva("campo", "cestas y macetas");
            c.add(ALF_FLORES, 0.0f, 0.0f, 0.0f, 2.3f);
            const float cx[6] = { -1.05f, -0.45f, 0.30f, 1.05f, -0.75f, 0.70f }, cz[6] = { 0.30f, -0.35f, -0.55f, 0.15f, 0.85f, 0.95f }, ce[6] = { 1.5f, 1.0f, 1.8f, 1.2f, 0.9f, 1.4f };
            for (int i = 0; i < 6; i++)
            {
                c.add(CESTA, cx[i], 0.0f, cz[i], ce[i], 37.0f * i, i % 2 ? CREMA : BLANCO);
                if (i % 3 == 0) c.add(RACIMO, cx[i], 0.38f * ce[i] - 0.05f, cz[i], 0.5f * ce[i], 20.0f * i, i == 0 ? F_NARANJA : F_AMARILLA);
                else if (i % 3 == 1) c.add(NARANJAS, cx[i], 0.38f * ce[i] - 0.1f, cz[i], 1.0f * ce[i] * 0.8f, 0.0f, i == 1 ? LIMON : BLANCO);
                else c.add(CALABAZA, cx[i], 0.38f * ce[i] - 0.1f, cz[i], 0.9f * ce[i] * 0.8f, 0.0f);
            }
            c.add(MAZETA, -1.50f, 0.0f, -0.45f, 1.6f); c.add(MAZETA, 1.45f, 0.0f, -0.35f, 1.3f); c.add(MAZETA, 0.0f, 0.0f, -1.05f, 1.0f);
            c.add(PETATE, 0.05f, 0.0f, 0.15f, 0.6f, 0.0f, glm::vec3(0.9f, 1.0f, 0.8f));
            c.add(CUADRO, 0.05f, 0.02f, 0.10f, 1.5f);
            c.fila(VELA, 7, -0.7f, 1.45f, 0.7f, 1.45f, 0.0f, 1.6f, VERDE, LIMON);
            c.vela_persona(-1.3f, 0.9f, -0.5f, 0.3f, 0);
            v.push_back(c.f);
        }
        // 13. Roca: una roca grande como altar natural con un retrato recargado, veladoras sobre sus salientes y un solo racimo. Paleta gris y blanca. (radio ~1.1 m)
        {
            Cons c = Nueva("roca", "roca como altar");
            c.add(ROCA, 0.0f, 0.0f, 0.0f, 1.8f, 0.0f, glm::vec3(0.95f, 0.95f, 1.0f));   // 0.59 m de alto; arriba plana (cubo con ruido de 7 cm x escala)
            c.add(CUADRO, 0.0f, 0.56f, -0.05f, 1.3f, 0.0f);
            c.add(VELADORA, -0.30f, 0.56f, 0.10f, 1.0f, 0.0f, BLANCO); c.add(VELADORA, 0.30f, 0.56f, 0.10f, 1.0f, 0.0f, BLANCO);
            c.add(VELA, 0.0f, 0.56f, 0.18f, 1.8f, 0.0f, BLANCO);
            c.add(ROCA, 0.85f, 0.0f, 0.55f, 0.8f, 70.0f); c.add(ROCA, -0.9f, 0.0f, 0.60f, 0.7f, 150.0f);
            c.add(RACIMO, 0.0f, 0.0f, 1.05f, 0.9f, 0.0f, F_APAGADA);
            c.add(VELA, -0.25f, 0.0f, 1.05f, 2.0f, 0.0f, BLANCO); c.add(VELA, 0.25f, 0.0f, 1.10f, 2.0f, 0.0f, BLANCO);
            c.vela_persona(0.0f, 1.15f, 0.0f, 0.5f, 1);
            v.push_back(c.f);
        }
        // 14. Tabla de remo: la tabla con su remo clavado en la arena, petate con retrato, velas azules y blancas, racimos. Paleta azul claro.
        {
            Cons c = Nueva("tabla", "tabla de remo");
            c.add(TABLA, 0.0f, 0.0f, 0.0f, 0.62f, 0.0f);
            c.add(PETATE, 1.05f, 0.0f, 0.2f, 0.95f, 90.0f, glm::vec3(0.85f, 0.95f, 1.0f));
            c.add(CUADRO, 1.05f, 0.02f, -0.05f, 1.7f, -90.0f);
            c.fila(VELA, 6, 0.80f, -0.10f, 0.80f, 0.55f, 0.02f, 1.5f, AZUL, BLANCO);
            c.fila(VELADORA, 3, 1.25f, 0.30f, 1.25f, 0.60f, 0.02f, 0.9f, TURQUESA);
            c.add(RACIMO, -0.85f, 0.0f, -0.5f, 0.8f, 0.0f, F_AMARILLA); c.add(RACIMO, -0.85f, 0.0f, 0.6f, 0.7f, 40.0f); c.add(RACIMO, 1.85f, 0.0f, 0.1f, 0.7f, 10.0f);
            c.add(NARANJAS, 1.00f, 0.02f, 0.50f, 0.9f, 0.0f, LIMON);
            c.add(BOTELLA, 1.30f, 0.02f, 0.10f, 1.2f);
            c.vela_persona(1.0f, 0.85f, 1.05f, 0.2f, 0);
            v.push_back(c.f);
        }
        // radio de huella (distancia maxima del centro al borde de una pieza)
        for (Familia& f : v)
        {
            float r = 0.0f;
            for (const Item& it : f.items) r = std::max(r, std::sqrt(it.pos.x * it.pos.x + it.pos.z * it.pos.z) + RadioPieza(it.tipo) * it.esc);
            for (const Velador& p : f.velan) r = std::max(r, std::sqrt(p.x * p.x + p.z * p.z) + 0.28f);   // X-67: el anclaje queda 0.28 m detras de la persona (ella mira hacia dentro de la familia; las manos llegan 0.76 m delante)
            f.radio = r;
        }
        return v;
    }

    // ----- lugar de cada familia en la playa de atras (z -4.2 a -21.5, x +-3.6 a +-29), sin chocar con nada
    struct Circulo { float x, z, r; };
    inline std::vector<Circulo> Obstaculos()
    {
        namespace D = VidaColinasDatos;
        std::vector<Circulo> o;
        for (int i = 0; i < D::N_FAROLES; i++)   o.push_back({ D::FAROLES[i].x,   D::FAROLES[i].z,   0.35f });
        for (int i = 0; i < D::N_ROCAS; i++)     o.push_back({ D::ROCAS[i].x,     D::ROCAS[i].z,     0.9f * D::ROCAS[i].esc });
        for (int i = 0; i < D::N_PALMAS; i++)    o.push_back({ D::PALMAS[i].x,    D::PALMAS[i].z,    0.45f * D::PALMAS[i].esc });
        for (int i = 0; i < D::N_ARBUSTOS; i++)  o.push_back({ D::ARBUSTOS[i].x,  D::ARBUSTOS[i].z,  0.7f * D::ARBUSTOS[i].esc });
        for (int i = 0; i < D::N_ARBOLES; i++)   o.push_back({ D::ARBOLES[i].x,   D::ARBOLES[i].z,   0.8f * D::ARBOLES[i].esc });
        for (int i = 0; i < D::N_PERSONAS; i++)  o.push_back({ D::PERSONAS[i].x,  D::PERSONAS[i].z,  0.5f });
        for (int i = 0; i < D::N_OFRENDAS; i++)  o.push_back({ D::OFRENDAS[i].x,  D::OFRENDAS[i].z,  0.7f });
        for (int i = 0; i < D::N_FOGATAS; i++)   o.push_back({ D::FOGATAS[i].x,   D::FOGATAS[i].z,   1.0f });
        for (int i = 0; i < D::N_ARCOS; i++)     o.push_back({ D::ARCOS[i].x,     D::ARCOS[i].z,     1.4f });
        for (int i = 0; i < D::N_RACIMOS; i++)   o.push_back({ D::RACIMOS[i].x,   D::RACIMOS[i].z,   0.4f });
        for (int i = 0; i < D::N_TENDEDEROS; i++) o.push_back({ D::TENDEDEROS[i].x, D::TENDEDEROS[i].z, 1.0f });
        for (int i = 0; i < Extras::N_FAROLES; i++)  o.push_back({ Extras::FAROLES[i].x, Extras::FAROLES[i].z, 0.35f });
        for (int i = 0; i < Extras::N_ROCAS; i++)    o.push_back({ Extras::ROCAS[i].x, Extras::ROCAS[i].z, 0.9f * Extras::ROCAS[i].esc });
        for (int i = 0; i < Extras::N_PALMAS_LADOS; i++) o.push_back({ Extras::PALMAS_LADOS[i].x, Extras::PALMAS_LADOS[i].z, 0.5f });
        for (int i = 0; i < Extras::N_PALAPAS; i++)  o.push_back({ Extras::PALAPAS[i].x, Extras::PALAPAS[i].z, 3.6f });
        for (int i = 0; i < Extras::N_CASAS; i++)    o.push_back({ Extras::CASAS[i].x, Extras::CASAS[i].z, 3.2f });
        for (int i = 0; i < Extras::N_FIGURAS_N; i++) o.push_back({ Extras::FIGURAS_N[i].x, Extras::FIGURAS_N[i].z, 0.5f });
        for (int i = 0; i < Extras::N_FIGURAS; i++)  o.push_back({ Extras::FIGURAS[i].x, Extras::FIGURAS[i].z, 0.5f });
        for (int i = 0; i < Extras::N_BANDA; i++)    o.push_back({ Extras::BANDA[i].x, Extras::BANDA[i].z, 0.6f });
        for (int i = 0; i < Extras::N_FOGATAS; i++)  o.push_back({ Extras::FOGATAS[i].x, Extras::FOGATAS[i].z, 1.0f });
        // palmas fijas de main.cpp (X-9/X-42) y la plaza: altar de piramide (0, -9.5) con su anillo de 12 velas (r 2.3)
        const float pf[4][2] = { { -13.0f, -4.0f }, { 16.0f, -3.0f }, { -9.5f, -6.4f }, { 7.6f, -7.4f } };
        for (int i = 0; i < 4; i++) o.push_back({ pf[i][0], pf[i][1], 0.5f });
        const float af[3][2] = { { -15.0f, 0.0f }, { 12.0f, 0.0f }, { -6.0f, -4.0f } };   // arbustos fijos de main.cpp (X-9) que caen cerca
        for (int i = 0; i < 3; i++) o.push_back({ af[i][0], af[i][1], 0.8f });
        o.push_back({ 0.0f, -9.5f, 2.9f });
        o.push_back({ 0.0f, -5.6f, 1.4f });    // arco-umbral de la plaza
        return o;
    }

    constexpr float X_PASILLO = 3.6f;     // franja libre a cada lado del camino de las animas (x = 0): una ofrenda no entra a menos de esto
    constexpr float Z_MIN = -21.5f, Z_MAX = -4.2f, X_MAX = 29.0f;   // zona de las familias (la arena termina en z = -20; hasta -21.5 el relieve sube < 4 cm)
    constexpr float HOLGURA_OBSTACULO = 0.20f, HOLGURA_FAMILIA = 0.50f;

    // Huella de una familia como union de circulos en el MUNDO (no un solo disco: una lancha o un altar son largos y delgados).
    // Las piezas largas se cubren con varios circulos a lo largo de su eje mayor; las piezas chicas (< 0.18 m de radio) se omiten porque
    // siempre van sobre una pieza grande. Las personas de rodillas ocupan un circulo de 0.40 m centrado 0.35 m detras de su punto.
    inline std::vector<Circulo> Huella(const Familia& f)
    {
        struct Rect { float hx, hz, oz; };
        auto rect = [](Pieza p, Rect& r) -> bool
        {
            switch (p)
            {
            case LANCHA: r = { 0.85f, 3.13f, 0.135f }; return true;
            case HAMACA: r = { 1.64f, 0.35f, 0.0f }; return true;
            case TABLA: r = { 0.38f, 1.60f, 0.0f }; return true;
            case SOPORTES: r = { 0.93f, 0.20f, 0.0f }; return true;
            case ARCO: r = { 1.19f, 0.25f, 0.0f }; return true;
            case ALTAR_MAYOR: r = { 1.90f, 1.16f, 0.14f }; return true;
            case MESA: r = { 0.64f, 0.39f, 0.0f }; return true;
            case ALTAR3: r = { 0.80f, 0.45f, 0.0f }; return true;
            case PETATE: r = { 0.50f, 0.35f, 0.0f }; return true;
            default: return false;
            }
        };
        std::vector<Circulo> h;
        auto mundo = [&](glm::vec3 p) { const glm::vec3 w = Girar(p, f.giro); return glm::vec2(f.x + w.x, f.z + w.z); };
        for (const Item& it : f.items)
        {
            Rect r;
            if (rect(it.tipo, r))
            {
                const float hx = r.hx * it.esc, hz = r.hz * it.esc, oz = r.oz * it.esc;
                const bool largoX = hx >= hz;
                const float corto = largoX ? hz : hx, largo = largoX ? hx : hz;
                const int n = std::max(1, (int)std::ceil(largo / corto));
                const float paso = largo / n, rad = std::sqrt(corto * corto + paso * paso);
                for (int k = 0; k < n; k++)
                {
                    const float t = -largo + paso * (2 * k + 1);
                    const glm::vec3 q = largoX ? glm::vec3(t, 0.0f, oz) : glm::vec3(0.0f, 0.0f, oz + t);
                    const glm::vec2 w = mundo(it.pos + Girar(q, it.giro));
                    h.push_back({ w.x, w.y, rad });
                }
            }
            else
            {
                const float rr = RadioPieza(it.tipo) * it.esc;
                if (rr < 0.18f) continue;
                const glm::vec2 w = mundo(it.pos);
                h.push_back({ w.x, w.y, rr });
            }
        }
        for (const Velador& p : f.velan)
        {
            const float m = p.mira * PI / 180.0f;
            // X-67: la persona mira hacia -Z local: cuerpo 0.24 m delante del anclaje (circulo de 0.40 m) (las manos, a 0.76 m, quedan hacia dentro de la familia)
            const glm::vec2 w = mundo(glm::vec3(p.x - CENTRO_CUERPO * std::sin(m), 0.0f, p.z - CENTRO_CUERPO * std::cos(m)));
            h.push_back({ w.x, w.y, 0.40f });
        }
        return h;
    }

    // Lugar de cada familia (x, z, giro Y en grados), en el MISMO orden que Familias(). Lo calculo con tools/x55_buscar_sitios.cpp
    // (busqueda determinista del punto libre mas separado, con la huella de circulos) y lo dejo FIJO aqui: sin calculo en tiempo de ejecucion.
    struct Sitio { float x, z, giro; };
    constexpr int N_FAMILIAS = 14;
    inline const Sitio SITIOS[N_FAMILIAS] = {
        { -6.10f, -13.20f, 99.0f },   // pescador
        { 11.60f, -20.20f, -63.4f },   // cocinera
        { -12.10f, -11.70f, 122.0f },   // musico
        { 16.10f, -11.20f, -86.5f },   // abuela
        { 10.60f, -5.20f, -40.4f },   // nino
        { -13.10f, -6.70f, 78.3f },   // arco
        { -25.10f, -13.20f, 86.6f },   // cruz
        { 6.60f, -11.20f, -78.5f },   // altar_chico
        { -16.10f, -15.70f, 92.8f },   // abuelo
        { 22.60f, -18.20f, -59.4f },   // circulo
        { 24.10f, -7.20f, -114.2f },   // camino
        { -9.60f, -19.70f, 135.1f },   // campo
        { -22.10f, -20.20f, 77.9f },   // roca
        { 5.60f, -18.70f, -94.7f },   // tabla
    };

    inline std::vector<Familia> Colocadas()
    {
        std::vector<Familia> fam = Familias();
        for (size_t i = 0; i < fam.size() && i < (size_t)N_FAMILIAS; i++) { fam[i].x = SITIOS[i].x; fam[i].z = SITIOS[i].z; fam[i].giro = SITIOS[i].giro; }
        return fam;
    }

    // Cajas de colision de las piezas solidas, en el mundo (AABB del rectangulo girado). Solo estructuras: mesa, altares, lancha, cruz, soportes, hamaca, tabla, arco, roca grande.
    inline std::vector<Caja> CajasColision(const Familia& f)
    {
        struct Sol { float hx, hz, oz, alto; };     // medio ancho, medio fondo, desplazamiento en z del centro, alto (esc = 1)
        auto sol = [](Pieza p, Sol& s) -> bool
        {
            switch (p)
            {
            case MESA: s = { 0.64f, 0.39f, 0.0f, 0.78f }; return true;
            case ALTAR3: s = { 0.80f, 0.45f, 0.0f, 1.20f }; return true;
            case ALTAR_MAYOR: s = { 1.90f, 1.16f, 0.14f, 2.90f }; return true;
            case LANCHA: s = { 0.85f, 3.13f, 0.135f, 1.10f }; return true;
            case CRUZ: s = { 0.30f, 0.30f, 0.0f, 1.50f }; return true;
            case SOPORTES: s = { 0.93f, 0.06f, 0.0f, 2.00f }; return true;
            case HAMACA: s = { 1.64f, 0.32f, 0.0f, 0.80f }; return true;
            case TABLA: s = { 0.38f, 1.60f, 0.0f, 0.50f }; return true;
            case ROCA: s = { 0.29f, 0.24f, 0.0f, 0.33f }; return true;
            default: return false;
            }
        };
        std::vector<Caja> cajas;
        for (const Item& it : f.items)
        {
            Sol s;
            if (!sol(it.tipo, s)) continue;
            if (it.tipo == ROCA && it.esc < 1.2f) continue;           // las rocas chicas del anillo no estorban
            const float hx = s.hx * it.esc, hz = s.hz * it.esc, oz = s.oz * it.esc;
            const float g = (f.giro + it.giro) * PI / 180.0f;         // el item gira dentro del marco de la familia
            float xmin = 1e9f, xmax = -1e9f, zmin = 1e9f, zmax = -1e9f;
            for (int a = -1; a <= 1; a += 2)
                for (int b = -1; b <= 1; b += 2)
                {
                    const glm::vec3 q = Girar(glm::vec3(a * hx, 0.0f, oz + b * hz), it.giro);          // dentro de la familia
                    const glm::vec3 w = Girar(it.pos + q, f.giro);                                      // al mundo (respecto al centro)
                    xmin = std::min(xmin, w.x); xmax = std::max(xmax, w.x); zmin = std::min(zmin, w.z); zmax = std::max(zmax, w.z);
                }
            (void)g;
            cajas.push_back({ f.x + xmin, f.z + zmin, f.x + xmax, f.z + zmax, s.alto * it.esc });
        }
        return cajas;
    }
}

#endif
