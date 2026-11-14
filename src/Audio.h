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

    // X-24: volumen por zona segun donde esta el visitante (x, z en metros). Cada cuadro desde main:
    //   oleaje = 0.03 + 0.15 * s^2.5, s = clamp(1 - max(0, 24 - z) / 24, 0, 1)  (D-6b/C-1b: maximo 0.18 en la orilla z = 24)
    //   musica = 0.48 * (0.10 + 0.40 * clamp(1 - d_altar / 10, 0, 1))      (D-6b: 0.48 iguala la sonoridad del mp3; cerca del altar, en (1.35, 0))
    //   voces  = 0.55 * clamp(1 - d / 14, 0, 1), d = distancia a la ceremonia (11, 19.5) o al panteon (0, 7), la menor
    void ActualizarZona(float x, float z);

private:
    AudioImpl* impl = nullptr;
};
