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
    //     v = 0.24 * d0 / max(d, d0) * (1 - clamp((d - 30) / 30, 0, 1)),  d0 = 3 m  (ley del inverso de la distancia: -6 dB al duplicar d; silencio a 60 m)
    //     paneo = 0.7 * (derecha . direccion a la banda): suena por el lado donde esta la banda (se oye mas en el oido de ese lado)
    void ActualizarZona(float x, float z, float fx = 0.0f, float fz = -1.0f);

private:
    AudioImpl* impl = nullptr;
};
