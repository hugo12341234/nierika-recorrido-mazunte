// Audio ambiental (D-6). Aqui (y solo aqui) se compila la implementacion de miniaudio.
#define MA_NO_ENCODING      // no escribimos audio: menos codigo compilado
#define MA_NO_GENERATION
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

#include "Audio.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>

namespace
{
    // Pistas ambientales: nombre base dentro de Audio/ y volumen (0..1). Ver Audio/README.md
    struct Pista
    {
        const char* nombre;
        float       volumen;
    };
    const Pista PISTAS[] = {
        { "oleaje", 0.03f }, // mar: volumen inicial = el de lejos de la orilla; ActualizarZona lo ajusta
        { "musica", 0.05f }, // musica de fondo; ActualizarZona la ajusta
        { "voces",  0.00f }  // X-24: murmullo de voces lejanas (sintetizado); su volumen depende de la zona
    };   // D-6c (14/11): se quito la pista "banda" (musica sintetizada de los musicos): se encimaba con el oleaje y la musica nueva
    const int NUM_PISTAS = 3;

    // Formatos que decodifica miniaudio sin librerias extra (se prueban en este orden; D-6e: .mp3 antes que .wav)
    const char* EXTENSIONES[] = { ".mp3", ".wav", ".flac" };   // D-6e: .mp3 primero, asi un oleaje.wav viejo que quede en la carpeta no tapa al mp3
}

struct AudioImpl
{
    ma_engine engine;
    ma_sound  sonidos[NUM_PISTAS];
    bool      activo[NUM_PISTAS] = { false, false, false };
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

void Audio::ActualizarZona(float x, float z)
{
    if (!impl) return;
    auto fija = [&](int i, float v) { if (impl->activo[i]) ma_sound_set_volume(&impl->sonidos[i], v); };
    auto lim = [](float v) { return (std::min)(1.0f, (std::max)(0.0f, v)); };  // parentesis: windows.h define min/max como macros
    // Oleaje (D-6b): sube con s^2.5 (mas lento cerca que s^2) segun la cercania a la orilla (z = 24); lejos casi no se oye
    const float sMar = lim(1.0f - (std::max)(0.0f, 24.0f - z) / 24.0f);
    fija(0, 0.03f + 0.15f * std::pow(sMar, 2.5f));   // 14/11 (2.a version): 0.03 + 0.15*s^2.5, antes 0.03 + 0.25*s^2
    const float dAltar = std::sqrt((x - 1.35f) * (x - 1.35f) + z * z);
    // Musica (D-6b): el mp3 de Xochipitzahuatl mide -10 LUFS contra -16.4 de la pista sintetizada anterior;
    // 0.48 = 10^(-6.4/20) iguala la sonoridad, asi la curva 0.10..0.50 conserva su balance con el oleaje
    fija(1, 0.48f * (0.10f + 0.40f * lim(1.0f - dAltar / 10.0f)));
    const float dCer = std::sqrt((x - 11.0f) * (x - 11.0f) + (z - 19.5f) * (z - 19.5f));
    const float dPan = std::sqrt(x * x + (z - 7.0f) * (z - 7.0f));
    fija(2, 0.55f * lim(1.0f - (std::min)(dCer, dPan) / 14.0f));
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
