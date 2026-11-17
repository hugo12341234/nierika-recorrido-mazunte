// Hud.h - X-60: estado de los avisos en pantalla (ImGui los dibuja desde Fichas.cpp, que ya tiene el unico contexto de ImGui).
#pragma once
#include <string>

struct HudEstado
{
    std::string aviso;            // mensaje breve arriba de la pantalla (V, M)
    float avisoSeg = 0.0f;        // segundos que le quedan
    bool ayuda = false;           // F1: lista de controles
    bool cercaAltar = false;      // true a < 3 m del altar: se muestra el recordatorio de la tecla V
    bool velasEncendidas = true;  // para decir "encender" o "apagar" en el recordatorio
};
