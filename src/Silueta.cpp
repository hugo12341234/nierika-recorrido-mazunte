#include "Silueta.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

using anim::Curva;

namespace
{
    // ---- Parametros del recorrido (decision del 04/11 en ESTADO, seccion 4) ----
    const float Z_INICIO = 24.0f;    // orilla (z = +24, ver decision "Playa y mar")
    const float N_ZANCADAS = 17.0f;  // zancadas completas (2 pasos) de la orilla al altar
    const float ZANCADA = 1.4f;      // m por zancada (2 pasos de ~0.7 m)
    const float V_CRUCERO = 1.4f;    // m/s, paso normal de un adulto (el ESTADO ya usa 1.4 m/s como referencia)
    const float T_RAMPA = 2.0f;      // s de arranque y de frenado
    const float T_MEZCLA = 0.6f;     // s para pasar de la pose de pie al ciclo de paso (y de regreso al llegar)
    const float T_CICLO = ZANCADA / V_CRUCERO;  // 1.0 s por zancada a velocidad de crucero
    const float DISTANCIA = N_ZANCADAS * ZANCADA;  // 23.8 m -> llega a z = +0.2, frente a la capilla (z = -0.6)
    const float X_FINAL = 1.0f;      // m: se detiene a la DERECHA del altar (el altar mide 1.30 m de ancho, x -0.65..+0.65) y no tapa la vista desde la entrada
    const float T_LATERAL = 1.0f;    // s finales del recorrido en que se desplaza en x (ya paso el arco: en ese instante z ~ +0.78 < +1.31)

    // ---- Medidas de la silueta (m; adulto de ~1.72 m, referencia no medida) ----
    const float ALTURA_REPOSO = 0.90f;  // cadera sobre el piso con las piernas rectas (muslo 0.45 + pierna 0.45)

    glm::quat RotX(float grados) { return glm::angleAxis(glm::radians(grados), glm::vec3(1, 0, 0)); }
    glm::quat RotY(float grados) { return glm::angleAxis(glm::radians(grados), glm::vec3(0, 1, 0)); }
    const glm::quat ID(1.0f, 0.0f, 0.0f, 0.0f);
}

int Silueta::Agregar(const char* nombre, int padre, glm::vec3 offset, glm::vec3 centro, glm::vec3 tam, glm::vec3 color)
{
    nodos_.push_back({ nombre, padre, offset, ID, centro, tam, color, glm::mat4(1.0f) });
    return (int)nodos_.size() - 1;
}

Silueta::Silueta() : recorrido_(false), lateral_(false), altura_(true), giro_(true), muslo_(true), pierna_(true), brazo_(true), antebrazo_(true),
                     brazoFlor_(false), antebrazoFlor_(false)
{
    // ---------- Jerarquia (x+ = derecha de la silueta; mira hacia -z) ----------
    const glm::vec3 cTorso(0.20f, 0.25f, 0.35f), cCabeza(0.55f, 0.55f, 0.60f);
    const glm::vec3 cIzq(0.30f, 0.45f, 0.65f), cDer(0.65f, 0.40f, 0.30f);  // izquierda azul, derecha rojiza: se distingue el desfase

    torso_ = Agregar("torso", -1, glm::vec3(0, ALTURA_REPOSO, 0), glm::vec3(0, 0.30f, 0), glm::vec3(0.34f, 0.60f, 0.20f), cTorso);
    cabeza_ = Agregar("cabeza", torso_, glm::vec3(0, 0.60f, 0), glm::vec3(0, 0.11f, 0), glm::vec3(0.22f, 0.22f, 0.22f), cCabeza);

    brazoIzq_ = Agregar("brazo izq", torso_, glm::vec3(-0.22f, 0.54f, 0), glm::vec3(0, -0.15f, 0), glm::vec3(0.10f, 0.30f, 0.10f), cIzq);
    antebrazoIzq_ = Agregar("antebrazo izq", brazoIzq_, glm::vec3(0, -0.30f, 0), glm::vec3(0, -0.14f, 0), glm::vec3(0.09f, 0.28f, 0.09f), cIzq);
    brazoDer_ = Agregar("brazo der", torso_, glm::vec3(0.22f, 0.54f, 0), glm::vec3(0, -0.15f, 0), glm::vec3(0.10f, 0.30f, 0.10f), cDer);
    antebrazoDer_ = Agregar("antebrazo der", brazoDer_, glm::vec3(0, -0.30f, 0), glm::vec3(0, -0.14f, 0), glm::vec3(0.09f, 0.28f, 0.09f), cDer);

    musloIzq_ = Agregar("muslo izq", torso_, glm::vec3(-0.09f, 0, 0), glm::vec3(0, -0.225f, 0), glm::vec3(0.14f, 0.45f, 0.14f), cIzq);
    piernaIzq_ = Agregar("pierna izq", musloIzq_, glm::vec3(0, -0.45f, 0), glm::vec3(0, -0.225f, 0), glm::vec3(0.12f, 0.45f, 0.12f), cIzq);
    musloDer_ = Agregar("muslo der", torso_, glm::vec3(0.09f, 0, 0), glm::vec3(0, -0.225f, 0), glm::vec3(0.14f, 0.45f, 0.14f), cDer);
    piernaDer_ = Agregar("pierna der", musloDer_, glm::vec3(0, -0.45f, 0), glm::vec3(0, -0.225f, 0), glm::vec3(0.12f, 0.45f, 0.12f), cDer);

    // ---------- KeyFrames del recorrido (posicion del torso en z) ----------
    // Tres tramos con velocidad continua: arranque SmoothIn, crucero lineal, frenado SmoothOut.
    // La velocidad al final de SmoothIn es 1.5 * d / T (pendiente de smoothstep en u = 1/2 por la reescala),
    // asi que para igualar V_CRUCERO: d = V * T / 1.5.
    const float dRampa = V_CRUCERO * T_RAMPA / 1.5f;               // 1.867 m
    const float dCrucero = DISTANCIA - 2.0f * dRampa;              // 20.07 m
    const float tCrucero = dCrucero / V_CRUCERO;                   // 14.33 s
    const float z1 = Z_INICIO - dRampa, z2 = z1 - dCrucero, z3 = Z_INICIO - DISTANCIA;
    recorrido_.Agregar(0.0f, glm::vec3(0, 0, Z_INICIO), ID, Curva::SmoothIn);
    recorrido_.Agregar(T_RAMPA, glm::vec3(0, 0, z1), ID, Curva::Lineal);
    recorrido_.Agregar(T_RAMPA + tCrucero, glm::vec3(0, 0, z2), ID, Curva::SmoothOut);
    recorrido_.Agregar(2.0f * T_RAMPA + tCrucero, glm::vec3(0, 0, z3), ID, Curva::Lineal);
    duracion_ = 2.0f * T_RAMPA + tCrucero;                         // ~18.3 s
    // Paso al costado: x = 0 hasta que el cuerpo cruza el arco (entra por el eje, el arco mide 1.18 m) y x = X_FINAL al llegar
    lateral_.Agregar(0.0f, glm::vec3(0, 0, 0), ID, Curva::Lineal);
    lateral_.Agregar(duracion_ - T_LATERAL, glm::vec3(0, 0, 0), ID, Curva::Smoothstep);
    lateral_.Agregar(duracion_, glm::vec3(X_FINAL, 0, 0), ID, Curva::Lineal);

    // ---------- KeyFrames del ciclo de paso (1 s; la pierna izq. toca el suelo adelante en t = 0) ----------
    // Cada KeyFrame es un EXTREMO del movimiento (el miembro se detiene y regresa), por eso todos los tramos
    // usan smoothstep: velocidad 0 en el extremo y maxima a la mitad, como un pendulo. Angulos en grados sobre X
    // (+ = hacia delante, -z). La rodilla solo se flexiona hacia atras (negativo).
    const Curva S = Curva::Smoothstep;
    muslo_.Agregar(0.00f, glm::vec3(0), RotX(+23), S);
    muslo_.Agregar(0.50f, glm::vec3(0), RotX(-23), S);
    muslo_.Agregar(1.00f, glm::vec3(0), RotX(+23), S);

    pierna_.Agregar(0.00f, glm::vec3(0), RotX(-3), S);    // contacto: casi recta
    pierna_.Agregar(0.15f, glm::vec3(0), RotX(-15), S);   // apoyo: se flexiona al recibir el peso
    pierna_.Agregar(0.45f, glm::vec3(0), RotX(-8), S);    // se extiende al empujar
    pierna_.Agregar(0.75f, glm::vec3(0), RotX(-60), S);   // balanceo: rodilla muy flexionada para no arrastrar el pie
    pierna_.Agregar(1.00f, glm::vec3(0), RotX(-3), S);

    brazo_.Agregar(0.00f, glm::vec3(0), RotX(-20), S);    // brazo izq. atras cuando la pierna izq. va adelante
    brazo_.Agregar(0.50f, glm::vec3(0), RotX(+20), S);
    brazo_.Agregar(1.00f, glm::vec3(0), RotX(-20), S);

    antebrazo_.Agregar(0.00f, glm::vec3(0), RotX(-10), S);
    antebrazo_.Agregar(0.50f, glm::vec3(0), RotX(-35), S); // el codo se flexiona mas con el brazo adelante
    antebrazo_.Agregar(1.00f, glm::vec3(0), RotX(-10), S);

    // Altura de la cadera: baja en el contacto (piernas abiertas) y sube al pasar (pierna de apoyo vertical): 2 veces por ciclo
    altura_.Agregar(0.00f, glm::vec3(0, 0.83f, 0), ID, S);
    altura_.Agregar(0.25f, glm::vec3(0, 0.89f, 0), ID, S);
    altura_.Agregar(0.50f, glm::vec3(0, 0.83f, 0), ID, S);
    altura_.Agregar(0.75f, glm::vec3(0, 0.89f, 0), ID, S);
    altura_.Agregar(1.00f, glm::vec3(0, 0.83f, 0), ID, S);

    // Giro del torso (cuaternion alrededor de Y, +-4 grados): una vez por ciclo, interpolado con slerp
    giro_.Agregar(0.00f, glm::vec3(0), RotY(+4), S);
    giro_.Agregar(0.50f, glm::vec3(0), RotY(-4), S);
    giro_.Agregar(1.00f, glm::vec3(0), RotY(+4), S);

    // ---------- D-4: dejar la flor con el brazo derecho (KeyFrames con slerp; tau = segundos desde el fin de la caminata) ----------
    // Al terminar la caminata el brazo esta en reposo (identidad, por la mezcla w = 0), asi que el primer KeyFrame es la identidad.
    // Hombro +75 grados (hacia delante, -z) y antebrazo +10: la mano queda en el aire frente al altar. Se sostiene hasta
    // T_REGRESA y vuelve a reposo en T_FIN. La flor se suelta en T_SUELTA (la mueve DejarFlor.h desde main.cpp).
    brazoFlor_.Agregar(0.0f, glm::vec3(0), ID, Curva::Smoothstep);
    brazoFlor_.Agregar(T_EXTIENDE, glm::vec3(0), RotX(+75), Curva::Lineal);
    brazoFlor_.Agregar(T_REGRESA, glm::vec3(0), RotX(+75), Curva::Smoothstep);
    brazoFlor_.Agregar(T_FIN, glm::vec3(0), ID, Curva::Lineal);
    antebrazoFlor_.Agregar(0.0f, glm::vec3(0), ID, Curva::Smoothstep);
    antebrazoFlor_.Agregar(T_EXTIENDE, glm::vec3(0), RotX(+10), Curva::Lineal);
    antebrazoFlor_.Agregar(T_REGRESA, glm::vec3(0), RotX(+10), Curva::Smoothstep);
    antebrazoFlor_.Agregar(T_FIN, glm::vec3(0), ID, Curva::Lineal);

    Actualizar(0.0f);
}

void Silueta::Actualizar(float t)
{
    const float tau = glm::max(0.0f, t - duracion_);   // D-4: segundos desde que termino la caminata
    t = glm::clamp(t, 0.0f, duracion_);

    // 1) Donde esta el torso y cuanto ha avanzado
    anim::Pose ruta = recorrido_.Evaluar(t);
    float recorrido = Z_INICIO - ruta.pos.z;                 // m caminados
    // 2) Fase del paso = zancadas completadas. Va ligada a la DISTANCIA (no al tiempo): en el arranque y el frenado
    //    los pasos se acortan en tiempo y los pies no patinan. En crucero 1 zancada = 1.4 m = 1 s.
    float fase = recorrido / ZANCADA;
    float tIzq = fase * T_CICLO;                              // tiempo de ciclo del lado izquierdo
    float tDer = tIzq + 0.5f * T_CICLO;                       // el lado derecho va medio ciclo desfasado

    // 3) Mezcla con la pose de pie (identidad) al salir y al llegar, con slerp: sin saltos al empezar ni al parar
    float w = glm::min(anim::Suavizar(t / T_MEZCLA, Curva::Smoothstep),
                       anim::Suavizar((duracion_ - t) / T_MEZCLA, Curva::Smoothstep));
    auto marcha = [&](const anim::Pista& p, float tc) { return anim::Slerp(ID, p.Evaluar(tc).rot, w); };

    float altura = glm::mix(ALTURA_REPOSO, altura_.Evaluar(tIzq).pos.y, w);
    nodos_[torso_].offset = glm::vec3(ruta.pos.x + lateral_.Evaluar(t).pos.x, altura, ruta.pos.z);
    nodos_[torso_].rot = marcha(giro_, tIzq);

    nodos_[musloIzq_].rot = marcha(muslo_, tIzq);
    nodos_[piernaIzq_].rot = marcha(pierna_, tIzq);
    nodos_[musloDer_].rot = marcha(muslo_, tDer);
    nodos_[piernaDer_].rot = marcha(pierna_, tDer);
    nodos_[brazoIzq_].rot = marcha(brazo_, tIzq);
    nodos_[antebrazoIzq_].rot = marcha(antebrazo_, tIzq);
    nodos_[brazoDer_].rot = marcha(brazo_, tDer);
    nodos_[antebrazoDer_].rot = marcha(antebrazo_, tDer);

    // 3b) D-4: despues de la caminata el brazo derecho deja la flor (sustituye al balanceo, que ya vale identidad)
    if (tau > 0.0f)
    {
        nodos_[brazoDer_].rot = brazoFlor_.Evaluar(tau).rot;
        nodos_[antebrazoDer_].rot = antebrazoFlor_.Evaluar(tau).rot;
    }

    // 4) Jerarquia: mundo(hijo) = mundo(padre) * T(offset) * R(rot). Mover al padre mueve a todos los hijos.
    for (size_t i = 0; i < nodos_.size(); i++)
    {
        Nodo& n = nodos_[i];
        glm::mat4 local = glm::translate(glm::mat4(1.0f), n.offset) * glm::mat4_cast(n.rot);
        n.mundo = (n.padre < 0) ? local : nodos_[n.padre].mundo * local;
    }
}

glm::vec3 Silueta::PosicionMano() const
{
    // La mano es la punta del antebrazo: 0.30 m bajo su articulacion (el codo), igual que el offset del nodo hijo del brazo
    return glm::vec3(nodos_[antebrazoDer_].mundo * glm::vec4(0.0f, -0.30f, 0.0f, 1.0f));
}

glm::vec3 Silueta::ManoAlSoltar() const
{
    Silueta copia = *this;
    copia.Actualizar(duracion_ + T_SUELTA);
    return copia.PosicionMano();
}

glm::mat4 Silueta::MatrizCubo(size_t i) const
{
    const Nodo& n = nodos_[i];
    return glm::scale(glm::translate(n.mundo, n.centro), n.tam);
}
