// Recorrido.h - X-62: recorrido virtual guiado (tecla T). Lleva la camara por 12 estaciones con una narracion por estacion.
// La ruta es una FORMULA (Catmull-Rom + smoothstep), no KeyFrames: los KeyFrames del proyecto son solo los de los modelos (silueta, dejar flores,
// personas). Docs: docs/RECORRIDO_GUIADO.md. Una clase por par .h/.cpp; sin GPU: solo posicion y orientacion de la camara.
#pragma once

#include <glm/glm.hpp>

class Audio;

struct RecorridoPose
{
    glm::vec3 pos;      // posicion de los ojos (y ya incluye suelo + 1.6 m)
    float yaw, pitch;   // grados, como Camera::SetOrientacion
};

class Recorrido
{
public:
    static constexpr int N_ESTACIONES = 12;

    bool Activo() const { return activo_; }

    // Arranca desde la pose actual de la camara. 'audio' puede ser nullptr (sin voz). 'duracionCaminataSilueta' = Silueta::Duracion() (para llegar al altar a tiempo).
    // Si ya estaba activo, no hace nada.
    void Iniciar(const RecorridoPose& actual, Audio* audio, float duracionCaminataSilueta);
    // Termina (tecla T, Esc, W A S D): la camara queda donde esta y se corta la voz.
    void Cancelar();
    // Avanza dt segundos y devuelve la pose de la camara. false = no hay recorrido activo (pose sin tocar).
    // Al terminar la ultima estacion se desactiva solo y devuelve true por ultima vez con la pose final.
    bool Actualizar(float dt, RecorridoPose& pose);

    // Centro de la comparsa en planta (x, z): lo fija main cada cuadro; la estacion 2 mira hacia alla (la comparsa camina sola, ver ESTADO X-62)
    void FijarComparsa(const glm::vec2& centro) { comparsa_ = centro; }

    // Tiempo (s) que main debe poner en tSilueta mientras dura el recorrido, o -1 si el recorrido no controla la silueta.
    // La silueta espera en la orilla (t = 0) y arranca para llegar al altar justo cuando la camara esta ahi (estacion 7).
    float TiempoSilueta() const;

    // Para los subtitulos: estacion actual (0..11), titulo y texto; el texto solo se da mientras la camara esta detenida en la estacion
    int Estacion() const { return idx_; }
    const char* Titulo() const;
    const char* Texto() const;
    bool EnEstacion() const { return activo_ && enReten_; }

private:
    bool activo_ = false;
    bool enReten_ = false;      // false = en camino a la estacion idx_; true = detenido en ella
    int idx_ = 0;
    float t_ = 0.0f;            // segundos dentro del tramo o del reten
    float reloj_ = 0.0f;        // segundos desde que empezo el recorrido
    float tramoReal_ = 1.0f;    // duracion efectiva del tramo actual
    float retenReal_ = 1.0f;    // duracion efectiva del reten actual (la voz, si existe, manda)
    float duracionSil_ = 18.4f;
    float siluetaInicio_ = -1.0f;   // reloj_ al que arranca la caminata de la silueta (-1 = aun no)
    Audio* audio_ = nullptr;
    RecorridoPose pose_{};      // ultima pose entregada
    RecorridoPose inicioTramo_{};
    glm::vec2 comparsa_{ -9.5f, 22.6f };
    glm::vec2 comparsaSuave_{ -9.5f, 22.6f };

    void EmpezarTramo();
    void EmpezarReten();
    void Terminar();
};
