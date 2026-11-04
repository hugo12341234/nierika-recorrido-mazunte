// Audio ambiental (D-6). Aqui (y solo aqui) se compila la implementacion de miniaudio.
#define MA_NO_ENCODING      // no escribimos audio: menos codigo compilado
#define MA_NO_GENERATION
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

#include "Audio.h"
#include <iostream>
#include <string>

namespace
{
    // Pistas ambientales: nombre base dentro de Audio/ y volumen (0..1). Ver Audio/README.md
    struct Pista
    {
        const char* nombre;
        float       volumen;
    };
    const Pista PISTAS[] = {
        { "oleaje", 0.60f }, // mar: protagonista
        { "musica", 0.25f }  // musica lejana: de fondo
    };
    const int NUM_PISTAS = 2;

    // Formatos que decodifica miniaudio sin librerias extra (se prueban en este orden)
    const char* EXTENSIONES[] = { ".wav", ".mp3", ".flac" };
}

struct AudioImpl
{
    ma_engine engine;
    ma_sound  sonidos[NUM_PISTAS];
    bool      activo[NUM_PISTAS] = { false, false };
};

bool Audio::init()
{
    if (impl) return true; // ya iniciado

    impl = new AudioImpl();
    if (ma_engine_init(nullptr, &impl->engine) != MA_SUCCESS)
    {
        std::cout << "[Audio] No se pudo iniciar el dispositivo de sonido: el programa sigue sin audio." << std::endl;
        delete impl;
        impl = nullptr;
        return false;
    }

    int sonando = 0;
    for (int i = 0; i < NUM_PISTAS; i++)
    {
        bool cargada = false;
        for (const char* ext : EXTENSIONES)
        {
            std::string ruta = std::string("Audio/") + PISTAS[i].nombre + ext; // ruta relativa a la raiz del repo
            // STREAM: decodifica sobre la marcha (la musica puede durar minutos); NO_SPATIALIZATION: es ambiente 2D
            ma_uint32 flags = MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_NO_SPATIALIZATION;
            if (ma_sound_init_from_file(&impl->engine, ruta.c_str(), flags, nullptr, nullptr, &impl->sonidos[i]) == MA_SUCCESS)
            {
                ma_sound_set_looping(&impl->sonidos[i], MA_TRUE);
                ma_sound_set_volume(&impl->sonidos[i], PISTAS[i].volumen);
                if (ma_sound_start(&impl->sonidos[i]) == MA_SUCCESS)
                {
                    impl->activo[i] = true;
                    sonando++;
                    std::cout << "[Audio] Suena en bucle: " << ruta << " (volumen " << PISTAS[i].volumen << ")" << std::endl;
                }
                else
                {
                    ma_sound_uninit(&impl->sonidos[i]);
                    std::cout << "[Audio] No se pudo reproducir: " << ruta << std::endl;
                }
                cargada = true;
                break;
            }
        }
        if (!cargada)
            std::cout << "[Audio] Falta Audio/" << PISTAS[i].nombre << ".wav|.mp3|.flac: se omite esa pista (ver Audio/README.md)." << std::endl;
    }

    if (sonando == 0)
    {
        // Sin pistas no hace falta mantener el dispositivo abierto
        cleanup();
        return false;
    }
    return true;
}

void Audio::cleanup()
{
    if (!impl) return;
    for (int i = 0; i < NUM_PISTAS; i++)
        if (impl->activo[i]) ma_sound_uninit(&impl->sonidos[i]);
    ma_engine_uninit(&impl->engine);
    delete impl;
    impl = nullptr;
}
