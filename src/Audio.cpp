// Audio ambiental (D-6). Aqui (y solo aqui) se compila la implementacion de miniaudio.
#define MA_NO_ENCODING      // no escribimos audio: menos codigo compilado
#define MA_NO_GENERATION
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

#include "Audio.h"
#include "Extras.h"   // X-40: posicion de la banda (BANDA_X, BANDA_Z)
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <cstdlib>

namespace
{
    // Pistas ambientales: nombre base dentro de Audio/ y volumen (0..1). Ver Audio/README.md
    struct Pista
    {
        const char* nombre;
        float       volumen;
    };
    const Pista PISTAS[] = {
        { "oleaje", 0.015f }, // mar: volumen inicial = el de lejos de la orilla; ActualizarZona lo ajusta
        { "musica", 0.05f }  // musica de fondo; ActualizarZona la ajusta
    };   // D-6h (14/11): solo dos pistas (mar y musica). Se quitaron "banda" (D-6c) y "voces" (sintetica, X-24)
    const int NUM_PISTAS = 2;

    // D-6h: SOLO .mp3 (nombres fijos Audio/oleaje.mp3 y Audio/musica.mp3). Antes se probaban tambien .wav y .flac,
    // y un .wav viejo olvidado en la carpeta podia tapar al mp3 sin avisar.
    const char* EXTENSIONES[] = { ".mp3" };
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
            std::cout << "[Audio] Falta Audio/" << PISTAS[i].nombre << ".mp3: se omite esa pista (ver Audio/README.md)." << std::endl;
    }

    if (sonando == 0)
    {
        // Sin pistas no hace falta mantener el dispositivo abierto
        cleanup();
        return false;
    }
    return true;
}

void Audio::ActualizarZona(float x, float z, float fx, float fz)
{
    if (!impl) return;
    auto fija = [&](int i, float v) { if (impl->activo[i]) ma_sound_set_volume(&impl->sonidos[i], v); };
    auto lim = [](float v) { return (std::min)(1.0f, (std::max)(0.0f, v)); };  // parentesis: windows.h define min/max como macros
    // Oleaje (D-6b): sube con s^2.5 (mas lento cerca que s^2) segun la cercania a la orilla (z = 24); lejos casi no se oye
    const float sMar = lim(1.0f - (std::max)(0.0f, 24.0f - z) / 24.0f);
    fija(0, 0.015f + 0.075f * std::pow(sMar, 2.5f));   // D-6g (14/11): 0.015 + 0.075*s^2.5 (-6 dB); antes 0.03 + 0.15*s^2.5
    // Musica (X-40): la FUENTE es la banda. Modelo: ley del inverso de la distancia (cada vez que d se duplica, -6 dB), con tope V0 = 0.24 a d0 = 3 m
    // (V0 = 0.48 * 0.50, el maximo que tenia la curva del altar en D-6b; 0.48 iguala la sonoridad del mp3 de -10 LUFS) y ventana lineal que la apaga a 60 m.
    const float dx = Extras::BANDA_X - x, dz = Extras::BANDA_Z - z;
    const float d = std::sqrt(dx * dx + dz * dz);
    const float D0 = 3.0f, V0 = 0.24f;
    fija(1, V0 * D0 / (std::max)(d, D0) * (1.0f - lim((d - 30.0f) / 30.0f)));
    // Paneo: derecha = (-fz, fx) en el plano XZ de OpenGL (frente x derecha con arriba +Y); cos del angulo con la direccion a la banda, reducido a 0.7
    if (impl->activo[1] && d > 0.01f)
    {
        const float fl = std::sqrt(fx * fx + fz * fz);
        if (fl > 0.0001f)
        {
            const float der = ((-fz / fl) * dx + (fx / fl) * dz) / d;      // +1: la banda esta a la derecha; -1: a la izquierda
            ma_sound_set_pan(&impl->sonidos[1], 0.7f * der * lim(d / 1.5f));   // junto a la banda (d < 1.5 m) el paneo se funde a centro
        }
    }

    // D-6i: diagnostico opcional (apagado por defecto). Con la variable de entorno NIERIKA_DEBUG_AUDIO=1 imprime una vez por
    // segundo la posicion y el volumen que el MOTOR tiene de verdad en cada pista (ma_sound_get_volume), para comprobar que cambia con la zona.
    static const bool depurar = (std::getenv("NIERIKA_DEBUG_AUDIO") != nullptr);
    if (depurar)
    {
        static auto ultimo = std::chrono::steady_clock::now();
        auto ahora = std::chrono::steady_clock::now();
        if (std::chrono::duration<float>(ahora - ultimo).count() >= 1.0f)
        {
            ultimo = ahora;
            std::cout << "[Audio] x=" << x << " z=" << z << " | oleaje vol=" << (impl->activo[0] ? ma_sound_get_volume(&impl->sonidos[0]) : -1.0f)
                      << " | musica vol=" << (impl->activo[1] ? ma_sound_get_volume(&impl->sonidos[1]) : -1.0f) << std::endl;
        }
    }
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
