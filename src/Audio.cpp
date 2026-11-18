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
#include <cstdio>

namespace
{
    // Pistas ambientales: nombre base dentro de Audio/ y volumen (0..1). Ver Audio/README.md
    struct Pista
    {
        const char* nombre;
        float       volumen;
    };
    const Pista PISTAS[] = {
        { "oleaje", 0.02f },  // mar: volumen inicial = el de lejos de la orilla (D-6j); ActualizarZona lo ajusta
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
    ma_sound  voz;                 // X-62: narracion del recorrido
    bool      vozCargada = false;
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
    // X-62: atenuacion por la voz del recorrido (rampa de 0.6 s hacia 1 mientras suena, hacia 0 si no)
    {
        static auto previo = std::chrono::steady_clock::now();
        const auto ahora = std::chrono::steady_clock::now();
        const float dt = (std::min)(0.1f, std::chrono::duration<float>(ahora - previo).count());
        previo = ahora;
        const bool habla = impl->vozCargada && !ma_sound_at_end(&impl->voz);
        atenuacion = (std::max)(0.0f, (std::min)(1.0f, atenuacion + (habla ? 1.0f : -1.0f) * dt / 0.6f));
    }
    const float factorMar = 1.0f - 0.4f * atenuacion, factorMusica = 1.0f - 0.65f * atenuacion;   // voz: mar x0.6, musica x0.35
    // Oleaje (D-6j): el volumen crece como 0.02 * 15^s (s = cercania a la orilla, z = 24): cada metro hacia el mar sube ~1 dB, parejo en todo el recorrido
    const float sMar = lim(1.0f - (std::max)(0.0f, 24.0f - z) / 24.0f);
    fija(0, mezclaMar ? factorMar * 0.02f * std::pow(15.0f, sMar) : 0.0f);   // D-6j: exponencial en s = decibelios lineales con la distancia: 0.02 (z <= 0) a 0.30 (z = 24), +23.5 dB en total
    // Musica (X-40, D-6l): la FUENTE es la banda. Modelo: ley del inverso de la distancia (cada vez que d se duplica, -6 dB), con tope V0 = 0.40 a d0 = 3 m
    // (D-6l: 0.355 en D-6k, +1.0 dB) y un PISO de fondo VMIN = 0.04 que nunca se apaga (antes una ventana lineal la callaba a 60 m y a 45 m ya casi no se oia).
    // v = max(V0 * D0 / d, VMIN): el piso toma el relevo a d = V0 * D0 / VMIN = 30 m; mas lejos suena igual (fondo lejano) en toda la playa.
    const float dx = Extras::BANDA_X - x, dz = Extras::BANDA_Z - z;
    const float d = std::sqrt(dx * dx + dz * dz);
    const float D0 = 3.0f, V0 = 0.40f, VMIN = 0.04f;
    fija(1, mezclaMusica ? factorMusica * (std::max)(V0 * D0 / (std::max)(d, D0), VMIN) : 0.0f);   // X-60: M puede silenciar la pista; X-62: factorMusica baja la musica mientras habla la voz (tambien el piso)
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

void Audio::SetMezcla(bool mar, bool musica)
{
    mezclaMar = mar;
    mezclaMusica = musica;
}

float Audio::IniciarVoz(int estacion)
{
    if (!impl) return 0.0f;
    DetenerVoz();
    char ruta[96];
    std::snprintf(ruta, sizeof(ruta), "Audio/narracion/narracion_%02d.mp3", estacion);   // ruta relativa a la raiz del repo
    // Sin STREAM: la narracion es corta y asi la duracion es exacta. NO_SPATIALIZATION: voz 2D, sin paneo
    if (ma_sound_init_from_file(&impl->engine, ruta, MA_SOUND_FLAG_NO_SPATIALIZATION, nullptr, nullptr, &impl->voz) != MA_SUCCESS)
    {
        static bool avisado[64] = {};   // un aviso por estacion, no uno por cuadro ni por repeticion
        if (estacion >= 0 && estacion < 64 && !avisado[estacion]) { avisado[estacion] = true; std::cout << "[Audio] Sin voz para la estacion " << estacion << " (falta " << ruta << "): el recorrido sigue en silencio." << std::endl; }
        return 0.0f;
    }
    ma_sound_set_volume(&impl->voz, 1.0f);
    if (ma_sound_start(&impl->voz) != MA_SUCCESS) { ma_sound_uninit(&impl->voz); return 0.0f; }
    impl->vozCargada = true;
    float largo = 0.0f;
    if (ma_sound_get_length_in_seconds(&impl->voz, &largo) != MA_SUCCESS) largo = 0.0f;
    std::cout << "[Audio] Voz de la estacion " << estacion << ": " << ruta << " (" << largo << " s)" << std::endl;
    return largo;
}

void Audio::DetenerVoz()
{
    if (!impl || !impl->vozCargada) return;
    ma_sound_uninit(&impl->voz);
    impl->vozCargada = false;
}

void Audio::cleanup()
{
    if (!impl) return;
    DetenerVoz();
    for (int i = 0; i < NUM_PISTAS; i++)
        if (impl->activo[i]) ma_sound_uninit(&impl->sonidos[i]);
    ma_engine_uninit(&impl->engine);
    delete impl;
    impl = nullptr;
}
