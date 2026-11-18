#pragma once
// Audio ambiental (D-6): oleaje + musica lejana, ambos en bucle, con miniaudio.
// Tolerante: si falta un archivo en Audio/ o no hay dispositivo de sonido, avisa por consola
// y el programa sigue sin ese sonido (no se rompe).
// La memoria se libera con cleanup() explicito al final de main, no en destructor (decision del 19/10).

struct AudioImpl; // detalle de miniaudio oculto: asi main.cpp no incluye miniaudio.h

class Audio
{
public:
    Audio() = default;
    Audio(const Audio&) = delete;            // posee recursos del motor de audio: no se copia
    Audio& operator=(const Audio&) = delete;

    // Inicia el motor y arranca las pistas que existan. Devuelve true si al menos una suena.
    bool init();
    // Detiene y libera todo. Seguro de llamar aunque init() haya fallado, o varias veces.
    void cleanup();

    // X-24/X-40: volumen por zona segun donde esta el visitante (x, z en metros) y hacia donde mira (fx, fz: vector frente en el plano XZ). Cada cuadro desde main:
    //   oleaje = 0.02 * 15^s, s = clamp(1 - max(0, 24 - z) / 24, 0, 1)  (D-6j: 0.02 en z <= 0 a 0.30 en la orilla z = 24; ~1 dB por metro)
    //   musica (X-40, la banda es la fuente): d = distancia al centro de la banda (Extras::BANDA_X, BANDA_Z);
    //     v = 0.40 * d0 / max(d, d0) hasta 30 m (d0 = 3 m, inverso de la distancia: -6 dB al duplicar d); de 30 m en adelante 0.04 * (30 / d)^1.36 con piso 0.012 (D-6m, llega a 0.012 a 73 m y nunca se apaga)
    //     paneo = 0.7 * (derecha . direccion a la banda): suena por el lado donde esta la banda (se oye mas en el oido de ese lado)
    void ActualizarZona(float x, float z, float fx = 0.0f, float fz = -1.0f);

    // X-60: mezcla del visitante (tecla M). Multiplica el volumen de cada pista: false = esa pista en silencio.
    void SetMezcla(bool mar, bool musica);

    // X-62: narracion del recorrido guiado. Carga Audio/narracion/narracion_NN.mp3 (NN = 00..11) y la reproduce una vez.
    // Devuelve su duracion en segundos, o 0 si el archivo no existe o no se pudo reproducir (el recorrido sigue sin voz; no truena).
    // Mientras la voz suena, ActualizarZona baja la musica a x0.35 y el mar a x0.6 con una rampa de 0.6 s (no cambia las curvas de D-6j ni X-40).
    float IniciarVoz(int estacion);
    void DetenerVoz();

private:
    float atenuacion = 0.0f;   // 0 = sin voz, 1 = voz sonando (rampa de 0.6 s)
    bool mezclaMar = true, mezclaMusica = true;
    AudioImpl* impl = nullptr;
};
