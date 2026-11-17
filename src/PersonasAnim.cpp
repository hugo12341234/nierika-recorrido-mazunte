#include "PersonasAnim.h"

#include <initializer_list>
#include <utility>

using anim::Curva;

namespace pers
{
    namespace
    {
        const int PADRE[N_ART] = { -1, RAIZ, TORSO, BRAZO_I, TORSO, BRAZO_D, RAIZ, MUSLO_I, RAIZ, MUSLO_D };
        // Posicion de cada articulacion respecto de la de su padre (m), con la persona de pie
        const glm::vec3 OFFSET[N_ART] = {
            glm::vec3(0.0f, ALTURA_CADERA, 0.0f), glm::vec3(0.0f),
            glm::vec3(-0.22f, 0.54f, 0.0f), glm::vec3(0.0f, -0.30f, 0.0f),   // brazo izq., antebrazo izq. (codo)
            glm::vec3(0.22f, 0.54f, 0.0f),  glm::vec3(0.0f, -0.30f, 0.0f),   // brazo der., antebrazo der.
            glm::vec3(-0.09f, 0.0f, 0.0f),  glm::vec3(0.0f, -0.45f, 0.0f),   // muslo izq., pierna izq. (rodilla)
            glm::vec3(0.09f, 0.0f, 0.0f),   glm::vec3(0.0f, -0.45f, 0.0f) };
        const glm::quat ID(1.0f, 0.0f, 0.0f, 0.0f);

        struct KF { float t; glm::quat q; };
        // Agrega a una pista de rotaciones la lista de (t, cuaternion), todas con la misma curva de salida
        void Rotaciones(anim::Pista& p, std::initializer_list<KF> lista, Curva c = Curva::Smoothstep)
        {
            for (const KF& k : lista) p.Agregar(k.t, glm::vec3(0.0f), k.q, c);
        }
        // Pista escalar (se guarda en pos.y)
        void Escalar(anim::Pista& p, std::initializer_list<std::pair<float, float>> lista, Curva c = Curva::Smoothstep)
        {
            for (const auto& k : lista) p.Agregar(k.first, glm::vec3(0.0f, k.second, 0.0f), ID, c);
        }
        float Ease(float u) { return anim::Suavizar(u, Curva::Smoothstep); }
    }

    // ---------------------------------------------------------------- Persona
    void Persona::Reposo()
    {
        for (int i = 0; i < N_ART; i++) rot[i] = ID;
        despRaiz = glm::vec3(0.0f);
    }

    void Persona::Calcular(const glm::mat4& base)
    {
        for (int i = 0; i < N_ART; i++)
        {
            glm::vec3 off = OFFSET[i];
            if (i == RAIZ) off += despRaiz;
            const glm::mat4 local = glm::translate(glm::mat4(1.0f), off) * glm::mat4_cast(rot[i]);
            mundo[i] = (PADRE[i] < 0) ? base * local : mundo[PADRE[i]] * local;
        }
    }

    glm::vec3 Persona::Mano(bool derecha) const
    {
        return glm::vec3(mundo[derecha ? ANTEB_D : ANTEB_I] * glm::vec4(0.0f, -0.28f, 0.0f, 1.0f));
    }

    // ---------------------------------------------------------------- Perro
    void Perro::Reposo()
    {
        rotCuerpo = rotCabeza = rotCola = ID;
        for (int i = 0; i < 4; i++) rotPata[i] = ID;
        despCuerpo = glm::vec3(0.0f);
    }

    void Perro::Calcular(const glm::mat4& base)
    {
        mCuerpo = base * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.45f, 0.0f) + despCuerpo) * glm::mat4_cast(rotCuerpo);
        mCabeza = mCuerpo * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.12f, -0.34f)) * glm::mat4_cast(rotCabeza);
        mCola = mCuerpo * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.08f, 0.38f)) * glm::mat4_cast(rotCola);
        const glm::vec3 hombro[4] = { glm::vec3(-0.09f, -0.10f, -0.25f), glm::vec3(0.09f, -0.10f, -0.25f),
                                      glm::vec3(-0.09f, -0.10f, 0.25f),  glm::vec3(0.09f, -0.10f, 0.25f) };
        for (int i = 0; i < 4; i++) mPata[i] = mCuerpo * glm::translate(glm::mat4(1.0f), hombro[i]) * glm::mat4_cast(rotPata[i]);
    }

    glm::vec3 Perro::Hocico() const
    {
        return glm::vec3(mCabeza * glm::vec4(0.0f, 0.06f, -0.30f, 1.0f));
    }

    // ================================================================ 1. BAILE FAMILIAR
    BaileFamiliar::BaileFamiliar() : orbita_(true), giroNina_(true), muslo_(true), pierna_(true), cintura_(true), rebote_(true)
    {
        for (int k = 0; k < N; k++) brazoD_[k] = anteD_[k] = brazoI_[k] = anteI_[k] = anim::Pista(true);   // los arreglos nacen no ciclicos: todos son ciclos de 2 s
        // Orbita de papa y mama: 12 KeyFrames cada 30 grados sobre un circulo de radio R. En cada uno la posicion es (R sen a, 0, R cos a) y la rotacion
        // es un giro de a grados sobre Y (queda mirando al centro). Tramos lineales (velocidad angular constante). El ultimo repite el primero.
        for (int k = 0; k <= 12; k++)
        {
            const float a = 2.0f * PI * k / 12.0f;
            orbita_.Agregar(T_VUELTA * k / 12.0f, glm::vec3(RADIO_ORBITA * std::sin(a), 0.0f, RADIO_ORBITA * std::cos(a)), Ry(30.0f * k), Curva::Lineal);
        }
        // La nina gira sobre si misma: 4 KeyFrames cada 90 grados en 4 s (slerp entre cuaterniones a 90 grados: un giro de velocidad constante)
        for (int k = 0; k <= 4; k++) giroNina_.Agregar(1.0f * k, glm::vec3(0.0f), Ry(90.0f * k), Curva::Lineal);

        // Zapateado de UNA pierna en un ciclo de 2 s: levanta el muslo (45 grados) con la rodilla flexionada, pisa, y da un golpe corto de repique.
        // La otra pierna usa la misma pista con medio ciclo (1 s) de desfase.
        Rotaciones(muslo_,  { {0.0f, ID}, {0.25f, Rx(+42)}, {0.5f, ID}, {0.62f, Rx(+16)}, {0.74f, ID}, {2.0f, ID} });
        Rotaciones(pierna_, { {0.0f, ID}, {0.25f, Rx(-58)}, {0.5f, ID}, {0.62f, Rx(-20)}, {0.74f, ID}, {2.0f, ID} });
        // Cintura: balanceo lateral de +-7 grados con giro del torso de +-8 grados, una oscilacion completa por ciclo
        Rotaciones(cintura_, { {0.0f, Rz(+7) * Ry(+8)}, {1.0f, Rz(-7) * Ry(-8)}, {2.0f, Rz(+7) * Ry(+8)} });
        // Rebote de la cadera: sube 4 cm con cada paso (2 por ciclo)
        Escalar(rebote_, { {0.0f, 0.0f}, {0.5f, 0.04f}, {1.0f, 0.0f}, {1.5f, 0.04f}, {2.0f, 0.0f} });

        // ---- Brazos de cada integrante (ciclo de 2 s). Papa: pañuelo en alto con la derecha, la izquierda en la cintura.
        Rotaciones(brazoD_[0], { {0.0f, Rz(+20) * Rx(+140)}, {1.0f, Rz(+38) * Rx(+155)}, {2.0f, Rz(+20) * Rx(+140)} });
        Rotaciones(anteD_[0],  { {0.0f, Rx(+25)}, {0.5f, Rx(+60)}, {1.0f, Rx(+25)}, {1.5f, Rx(+60)}, {2.0f, Rx(+25)} });        // el pañuelo ondea a 2 Hz
        Rotaciones(brazoI_[0], { {0.0f, Rz(-32)}, {1.0f, Rz(-38)}, {2.0f, Rz(-32)} });
        Rotaciones(anteI_[0],  { {0.0f, Rz(+75)}, {1.0f, Rz(+70)}, {2.0f, Rz(+75)} });
        // Mama: sostiene la falda con las dos manos y las mece al compas (opuestas)
        Rotaciones(brazoI_[1], { {0.0f, Rz(-30) * Rx(+22)}, {1.0f, Rz(-48) * Rx(+32)}, {2.0f, Rz(-30) * Rx(+22)} });
        Rotaciones(anteI_[1],  { {0.0f, Rx(+50)}, {1.0f, Rx(+65)}, {2.0f, Rx(+50)} });
        Rotaciones(brazoD_[1], { {0.0f, Rz(+48) * Rx(+32)}, {1.0f, Rz(+30) * Rx(+22)}, {2.0f, Rz(+48) * Rx(+32)} });
        Rotaciones(anteD_[1],  { {0.0f, Rx(+65)}, {1.0f, Rx(+50)}, {2.0f, Rx(+65)} });
        // Nina: los dos brazos en alto, moviendose en contrafase (cada 0.5 s)
        Rotaciones(brazoD_[2], { {0.0f, Rz(+30) * Rx(+160)}, {0.5f, Rz(+40) * Rx(+172)}, {1.0f, Rz(+30) * Rx(+160)}, {1.5f, Rz(+40) * Rx(+172)}, {2.0f, Rz(+30) * Rx(+160)} });
        Rotaciones(brazoI_[2], { {0.0f, Rz(-40) * Rx(+172)}, {0.5f, Rz(-30) * Rx(+160)}, {1.0f, Rz(-40) * Rx(+172)}, {1.5f, Rz(-30) * Rx(+160)}, {2.0f, Rz(-40) * Rx(+172)} });
        Rotaciones(anteD_[2],  { {0.0f, Rx(+10)}, {2.0f, Rx(+10)} });
        Rotaciones(anteI_[2],  { {0.0f, Rx(+10)}, {2.0f, Rx(+10)} });
    }

    void BaileFamiliar::Actualizar(float t)
    {
        for (int k = 0; k < N; k++)
        {
            Persona& p = persona[k];
            p.Reposo();
            const float tp = (k == 1) ? t + 0.5f : t;            // mama: medio segundo de desfase en los pasos
            p.rot[MUSLO_I] = muslo_.Evaluar(tp).rot;           p.rot[PIERNA_I] = pierna_.Evaluar(tp).rot;
            p.rot[MUSLO_D] = muslo_.Evaluar(tp + 1.0f).rot;    p.rot[PIERNA_D] = pierna_.Evaluar(tp + 1.0f).rot;
            p.rot[TORSO] = cintura_.Evaluar(tp).rot;
            p.despRaiz.y = rebote_.Evaluar(k == 2 ? 2.0f * t : tp).pos.y * (k == 2 ? 2.0f : 1.0f);   // la nina brinca el doble de rapido y mas alto
            p.rot[BRAZO_D] = brazoD_[k].Evaluar(tp).rot;  p.rot[ANTEB_D] = anteD_[k].Evaluar(tp).rot;
            p.rot[BRAZO_I] = brazoI_[k].Evaluar(tp).rot;  p.rot[ANTEB_I] = anteI_[k].Evaluar(tp).rot;
            glm::mat4 base(1.0f);
            if (k < 2)
            {
                const anim::Pose o = orbita_.Evaluar(t + (k == 1 ? 0.5f * T_VUELTA : 0.0f));    // mama va al otro lado del circulo
                base = glm::translate(glm::mat4(1.0f), o.pos) * glm::mat4_cast(o.rot);
            }
            else
                base = glm::mat4_cast(giroNina_.Evaluar(t).rot) * glm::scale(glm::mat4(1.0f), glm::vec3(ESCALA_NINA));
            p.Calcular(base);
        }
    }

    // ================================================================ 2. JUEGO CON EL PERRO
    namespace
    {
        const glm::vec3 CASA(0.60f, 0.0f, -0.70f);       // H: donde espera el perro
        const glm::vec3 PELOTA_CAE(0.0f, 0.055f, -5.90f); // donde cae la pelota
        const glm::vec3 PERRO_RECOGE(0.0f, 0.0f, -5.42f); // con el pecho abajo (22 grados) y la cabeza bajada, el hocico queda sobre la pelota (0.48 m delante del tronco)
        const float R_PELOTA = 0.055f;
        const float ZANCADA_PERRO = 1.2f;                // m por ciclo de patas (trote largo)
    }

    JuegoPerro::JuegoPerro() : ruta_(false), marcha_(false), cabeza_(false), paso_(true), agachar_(false)
    {
        // ---- Nino (tc = segundos del ciclo de 10 s). Tiro: giro del torso hacia atras -35, brazo atras -80 (0.7 s), lanzamiento a +155 en 0.18 s, seguimiento.
        Rotaciones(giroTorso_, { {0.0f, ID}, {0.7f, Ry(-35)}, {0.95f, Ry(+25)}, {1.8f, Ry(+8)}, {2.8f, ID}, {10.0f, ID} });
        Rotaciones(inclina_,   { {0.0f, ID}, {6.4f, ID}, {7.0f, Rx(+60)}, {7.6f, Rx(+60)}, {8.6f, ID}, {10.0f, ID} });
        Rotaciones(brazoD_,    { {0.0f, Rx(+10)}, {0.7f, Rx(-80)}, {0.88f, Rx(+155)}, {1.5f, Rx(+95)}, {2.7f, Rx(+10)}, {6.6f, Rx(+10)},
                                 {7.2f, Rx(-55)}, {7.6f, Rx(-55)}, {8.5f, Rx(+10)}, {10.0f, Rx(+10)} });
        Rotaciones(anteD_,     { {0.0f, Rx(+15)}, {0.7f, Rx(+100)}, {0.88f, Rx(+5)}, {1.5f, Rx(+30)}, {2.7f, Rx(+15)}, {6.6f, Rx(+15)},
                                 {7.2f, Rx(+5)}, {7.6f, Rx(+5)}, {8.5f, Rx(+15)}, {10.0f, Rx(+15)} });
        Rotaciones(brazoI_,    { {0.0f, Rx(+5)}, {0.7f, Rx(+40)}, {0.95f, Rx(+70)}, {2.0f, Rx(+5)}, {10.0f, Rx(+5)} });
        // Cuclillas para recoger la pelota (7.0 s a 7.6 s): la cadera baja 0.43 m, muslos +75, rodillas -120
        Escalar(cuclillas_, { {0.0f, 0.0f}, {6.4f, 0.0f}, {7.0f, -0.43f}, {7.6f, -0.43f}, {8.6f, 0.0f}, {10.0f, 0.0f} });
        Rotaciones(muslo_,  { {0.0f, ID}, {6.4f, ID}, {7.0f, Rx(+75)}, {7.6f, Rx(+75)}, {8.6f, ID}, {10.0f, ID} });
        Rotaciones(pierna_, { {0.0f, ID}, {6.4f, ID}, {7.0f, Rx(-120)}, {7.6f, Rx(-120)}, {8.6f, ID}, {10.0f, ID} });

        // ---- Perro: ruta (posicion y giro) por KeyFrames. Sale a 4.55 m de la pelota con arranque y frenado suaves, recoge, da la vuelta y regresa.
        auto kf = [&](float t, const glm::vec3& p, float yaw) { ruta_.Agregar(t, p, Ry(yaw), Curva::Smoothstep); };
        kf(0.0f, CASA, 0.0f);    kf(0.9f, CASA, 7.5f);    kf(2.6f, PERRO_RECOGE, 7.5f);  kf(3.3f, PERRO_RECOGE, 0.0f);
        kf(3.45f, PERRO_RECOGE, 90.0f);  kf(3.6f, PERRO_RECOGE, 172.5f);  kf(5.95f, CASA, 172.5f);  kf(6.1f, CASA, 139.0f);
        kf(6.4f, CASA, 139.0f);  kf(6.8f, CASA, 90.0f);   kf(7.2f, CASA, 0.0f);          kf(10.0f, CASA, 0.0f);
        // marcha: 1 mientras corre (peso de las patas), 0 parado
        Escalar(marcha_, { {0.0f, 0.0f}, {0.9f, 0.0f}, {1.1f, 1.0f}, {2.4f, 1.0f}, {2.6f, 0.0f}, {3.6f, 0.0f}, {3.8f, 1.0f}, {5.8f, 1.0f}, {5.95f, 0.0f}, {10.0f, 0.0f} });
        // cabeza: mira la pelota, baja a recogerla (55 grados) y la lleva con el hocico un poco abajo
        // (+ sobre X levanta el hocico, - lo baja). Con el pecho a -22 grados, la cabeza baja 56 grados mas y el hocico toca la pelota.
        Rotaciones(cabeza_, { {0.0f, Rx(+4)}, {2.6f, Rx(+4)}, {3.0f, Rx(-56)}, {3.35f, Rx(-56)}, {3.7f, Rx(+6)}, {5.95f, Rx(+6)}, {6.3f, Rx(+14)}, {10.0f, Rx(+4)} });
        // reverencia de juego (peso 0..1): llega a 1 al recoger la pelota (2.9 s a 3.3 s) y se levanta a las 3.7 s
        Escalar(agachar_, { {0.0f, 0.0f}, {2.3f, 0.0f}, {2.9f, 1.0f}, {3.3f, 1.0f}, {3.7f, 0.0f}, {10.0f, 0.0f} });
        // ciclo de patas (1 s por zancada): pendulo de +-36 grados
        Rotaciones(paso_, { {0.0f, Rx(+36)}, {0.5f, Rx(-36)}, {1.0f, Rx(+36)} });

        // El nino recoge la pelota donde queda su mano en el instante T_RECOGE_NINO (la pelota se deja ahi, apoyada en el suelo)
        Persona p;
        PoseNino(T_RECOGE_NINO, p);
        puntoRecoge_ = p.Mano(true);
        puntoRecoge_.y = R_PELOTA;
        Actualizar(0.0f);
    }

    void JuegoPerro::PoseNino(float tc, Persona& p) const
    {
        p.Reposo();
        p.rot[TORSO] = giroTorso_.Evaluar(tc).rot * inclina_.Evaluar(tc).rot;
        p.rot[BRAZO_D] = brazoD_.Evaluar(tc).rot;   p.rot[ANTEB_D] = anteD_.Evaluar(tc).rot;
        p.rot[BRAZO_I] = brazoI_.Evaluar(tc).rot;
        p.despRaiz.y = cuclillas_.Evaluar(tc).pos.y;
        p.rot[MUSLO_I] = p.rot[MUSLO_D] = muslo_.Evaluar(tc).rot;
        p.rot[PIERNA_I] = p.rot[PIERNA_D] = pierna_.Evaluar(tc).rot;
        p.Calcular(glm::scale(glm::mat4(1.0f), glm::vec3(ESCALA_NINO)));
    }

    void JuegoPerro::PosePerro(float tc, Perro& d, glm::mat4& base, float& distancia) const
    {
        d.Reposo();
        const anim::Pose r = ruta_.Evaluar(tc);
        base = glm::translate(glm::mat4(1.0f), r.pos) * glm::mat4_cast(r.rot);
        const float D = glm::length(PERRO_RECOGE - CASA);
        distancia = (tc < 3.5f) ? glm::length(r.pos - CASA) : D + glm::length(r.pos - PERRO_RECOGE);
        const float w = marcha_.Evaluar(tc).pos.y;                       // 0 parado, 1 corriendo
        const float fase = distancia / ZANCADA_PERRO;                    // zancadas completadas: las patas no patinan
        // trote en diagonal: delantera izq. y trasera der. juntas, delantera der. y trasera izq. medio ciclo despues
        const glm::quat g0 = anim::Slerp(ID, paso_.Evaluar(fase).rot, w), g1 = anim::Slerp(ID, paso_.Evaluar(fase + 0.5f).rot, w);
        // reverencia de juego (peso wA): pecho a -22 grados y tronco 10 cm mas bajo; las patas traseras quedan verticales (+22 respecto del tronco) y las
        // delanteras se estiran 84 grados al frente: con h = 0.349 m las cuatro patas tocan el suelo (0.186 + 0.35 cos 62 = 0.349)
        const float wA = agachar_.Evaluar(tc).pos.y;
        d.rotPata[0] = anim::Slerp(g0, Rx(+84), wA);  d.rotPata[1] = anim::Slerp(g1, Rx(+84), wA);
        d.rotPata[2] = anim::Slerp(g1, Rx(+22), wA);  d.rotPata[3] = anim::Slerp(g0, Rx(+22), wA);
        d.rotCuerpo = anim::Slerp(ID, Rx(-22), wA);
        d.despCuerpo.y = w * 0.025f * std::fabs(std::sin(2.0f * PI * fase)) - 0.101f * wA;
        d.rotCabeza = cabeza_.Evaluar(tc).rot;
        // cola: levantada 35 grados y moviendose de lado: theta(t) = 30 sen(2 pi 3.2 t)
        d.rotCola = Ry(30.0f * std::sin(2.0f * PI * 3.2f * tc)) * Rx(-35.0f);
        d.Calcular(base);
    }

    void JuegoPerro::Actualizar(float t)
    {
        const float tc = std::fmod(std::fmod(t, T_CICLO) + T_CICLO, T_CICLO);
        PoseNino(tc, nino);
        baseNino = glm::scale(glm::mat4(1.0f), glm::vec3(ESCALA_NINO));
        float dist;
        PosePerro(tc, perro, basePerro, dist);

        // ---- Pelota (todo en forma cerrada, sin integrar: no depende de los FPS)
        const float tl = T_SUELTA + VUELO;                      // aterriza
        if (tc < T_SUELTA)
            pelota = nino.Mano(true);
        else if (tc < tl)
        {
            // tiro parabolico: x(t), z(t) lineales; y(t) = y0 + vy*tau - g tau^2 / 2 con vy tal que y(VUELO) = radio de la pelota
            Persona p0; PoseNino(T_SUELTA, p0);
            const glm::vec3 a = p0.Mano(true);
            const float tau = tc - T_SUELTA, u = tau / VUELO;
            const float vy = (0.5f * G * VUELO * VUELO - (a.y - R_PELOTA)) / VUELO;
            pelota = glm::vec3(glm::mix(a.x, PELOTA_CAE.x, u), a.y + vy * tau - 0.5f * G * tau * tau, glm::mix(a.z, PELOTA_CAE.z, u));
        }
        else if (tc < T_RECOGE_PERRO)
            pelota = PELOTA_CAE;
        else if (tc < T_SUELTA_PERRO)
            pelota = perro.Hocico();                           // la lleva en el hocico
        else if (tc < T_SUELTA_PERRO + T_TOSS)
        {
            // el perro la suelta: caida parabolica del hocico al punto donde el nino la recoge
            Perro d; glm::mat4 b; float dd; PosePerro(T_SUELTA_PERRO, d, b, dd);
            const glm::vec3 a = d.Hocico();
            const float tau = tc - T_SUELTA_PERRO, u = tau / T_TOSS;
            const float vy = (0.5f * G * T_TOSS * T_TOSS - (a.y - puntoRecoge_.y)) / T_TOSS;
            pelota = glm::vec3(glm::mix(a.x, puntoRecoge_.x, u), a.y + vy * tau - 0.5f * G * tau * tau, glm::mix(a.z, puntoRecoge_.z, u));
        }
        else if (tc < T_RECOGE_NINO)
            pelota = puntoRecoge_;
        else
            pelota = nino.Mano(true);
    }

    // ================================================================ 3. SAHUMERIO
    Sahumerio::Sahumerio() : torso_(false), brazoD_(false), anteD_(false), brazoI_(false), anteI_(false)
    {
        // Reverencia: el torso se inclina 17 grados al centro del ciclo
        Rotaciones(torso_,  { {0.0f, ID}, {1.5f, Rx(8)}, {3.0f, Rx(17)}, {4.5f, Rx(8)}, {5.5f, ID}, {9.0f, ID} });
        // Brazo derecho: sube el copalero al frente (55), lo balancea a izquierda y derecha (+-28 grados sobre Y) tres veces, lo eleva (105) hacia la ofrenda y vuelve
        Rotaciones(brazoD_, { {0.0f, Rx(5)}, {1.5f, Rx(55)}, {2.2f, Ry(+28) * Rx(55)}, {3.0f, Ry(-28) * Rx(55)}, {3.8f, Ry(+28) * Rx(55)}, {4.5f, Rx(55)},
                              {5.4f, Rx(105)}, {6.6f, Rx(105)}, {7.8f, Rx(55)}, {9.0f, Rx(5)} });
        Rotaciones(anteD_,  { {0.0f, Rx(10)}, {1.5f, Rx(55)}, {4.5f, Rx(55)}, {5.4f, Rx(25)}, {6.6f, Rx(25)}, {7.8f, Rx(55)}, {9.0f, Rx(10)} });
        // Brazo izquierdo: la mano al pecho, sujetando el rebozo
        Rotaciones(brazoI_, { {0.0f, Rx(3)}, {1.5f, Rx(30)}, {7.8f, Rx(30)}, {9.0f, Rx(3)} });
        Rotaciones(anteI_,  { {0.0f, Rx(5)}, {1.5f, Rx(100)}, {7.8f, Rx(100)}, {9.0f, Rx(5)} });
        Actualizar(0.0f);
    }

    void Sahumerio::Actualizar(float t)
    {
        const float tc = std::fmod(std::fmod(t, T_CICLO) + T_CICLO, T_CICLO);
        persona.Reposo();
        persona.rot[TORSO] = torso_.Evaluar(tc).rot;
        persona.rot[BRAZO_D] = brazoD_.Evaluar(tc).rot;  persona.rot[ANTEB_D] = anteD_.Evaluar(tc).rot;
        persona.rot[BRAZO_I] = brazoI_.Evaluar(tc).rot;  persona.rot[ANTEB_I] = anteI_.Evaluar(tc).rot;
        persona.Calcular(glm::mat4(1.0f));
        // El copalero cuelga de la mano y se mece como pendulo: theta = 14 sen(2 pi t / 1.1) grados (sin KeyFrames, secundario de la mano)
        const float theta = 14.0f * std::sin(2.0f * PI * tc / 1.1f);
        copalero = glm::translate(glm::mat4(1.0f), persona.Mano(true) + glm::vec3(0.0f, -0.10f, 0.0f)) * glm::mat4_cast(Rx(theta)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.8f));
    }

    // ================================================================ 5. ANFITRIONA
    void Anfitriona::Actualizar(float t, float dt, float dx, float dz)
    {
        const float d = std::sqrt(dx * dx + dz * dz);
        // peso del saludo: 1 a menos de 9 m, 0 a mas de 12 m (smoothstep)
        const float w = anim::Suavizar((ALCANCE_SALUDO - d) / (ALCANCE_SALUDO - ALCANCE_PLENO), Curva::Smoothstep);
        // giro: hacia el visitante si esta cerca (el frente mira a -z: el vector al visitante es (-sen g, -cos g)), si no a su orientacion de siempre
        const float objetivo = (w > 0.01f) ? glm::degrees(std::atan2(-dx, -dz)) : giroBase_;
        float delta = std::fmod(objetivo - giro_ + 540.0f, 360.0f) - 180.0f;       // diferencia en [-180, 180)
        giro_ += delta * (1.0f - std::exp(-3.0f * dt));                              // acercamiento exponencial (tau = 1/3 s)

        persona.Reposo();
        const float onda = std::sin(2.0f * PI * 1.7f * t);                           // la mano ondea a 1.7 Hz
        // brazo derecho: reposo (mano abajo) <-> saludo (brazo a la altura del hombro, antebrazo vertical que oscila +-25 grados)
        persona.rot[BRAZO_D] = anim::Slerp(Rx(6), Rz(95), w);
        persona.rot[ANTEB_D] = anim::Slerp(Rx(12), Rz(85.0f + 25.0f * onda), w);
        persona.rot[BRAZO_I] = Rz(-5.0f - 2.0f * std::sin(2.0f * PI * t / 3.7f));
        persona.rot[TORSO] = Rz(2.5f * std::sin(2.0f * PI * t / 3.1f)) * Rx(1.5f * std::sin(2.0f * PI * t / 4.0f));   // respiracion y balanceo
        persona.despRaiz.x = 0.015f * std::sin(2.0f * PI * t / 3.1f);                  // el peso pasa de un pie al otro
        persona.Calcular(glm::rotate(glm::mat4(1.0f), glm::radians(giro_), glm::vec3(0.0f, 1.0f, 0.0f)));
    }
}
