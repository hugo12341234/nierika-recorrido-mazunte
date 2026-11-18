// Hud.h - X-60: estado de los avisos en pantalla (ImGui los dibuja desde Fichas.cpp, que ya tiene el unico contexto de ImGui).
#pragma once
#include <string>

struct HudEstado
{
    std::string aviso;            // mensaje breve arriba de la pantalla (V, M)
    float avisoSeg = 0.0f;        // segundos que le quedan
    bool ayuda = false;           // F1: lista de controles
    bool cercaAltar = false;      // true a < 3 m del altar: se muestra el recordatorio de la tecla V
    bool tour = false;            // X-62: recorrido guiado activo (subtitulos)
    std::string tourTitulo;       // X-62: 'Estacion n/12 · titulo'
    std::string tourTexto;        // X-62: lo que dice la voz (vacio mientras la camara va en camino)
    bool velasEncendidas = true;  // para decir "encender" o "apagar" en el recordatorio
};
