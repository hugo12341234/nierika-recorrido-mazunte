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

private:
    AudioImpl* impl = nullptr;
};
