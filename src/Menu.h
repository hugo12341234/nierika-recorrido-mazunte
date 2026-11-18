// Menu.h - X-63: menu de inicio y de pausa con Dear ImGui (Iniciar, Controles, Creditos, Salir).
// Usa el unico contexto de ImGui que crea Fichas.cpp (FichasInit); aqui no se crea ni se libera nada de GPU,
// asi que cleanup() de main no cambia. Todo el adorno (papel picado, cempasuchil, velas) se dibuja con
// ImDrawList: sin imagenes ni archivos nuevos.
#pragma once

#include <GLFW/glfw3.h>

enum class MenuPagina { Principal, Controles, Creditos };

// Lo que el menu le pide a main
enum class MenuAccion { Ninguna, Iniciar, Continuar, Salir };

struct MenuEstado
{
    bool abierto = false;                       // true = se dibuja el menu y el recorrido no avanza
    bool pausa = false;                         // false = menu de inicio (boton "Iniciar recorrido"); true = pausa ("Continuar")
    MenuPagina pagina = MenuPagina::Principal;
    int seleccion = 0;                          // boton resaltado con el teclado (0..3 en la pagina principal)
    float desplazaKey = 0.0f;                   // pixeles que piden las flechas arriba/abajo en Creditos (se consumen al dibujar)
    float irAlFinal = 0.0f;                     // solo capturas de desarrollo: >0 = baja Creditos hasta el final
};

// Abre el menu: libera el cursor y deja que ImGui reciba el raton. pausa = true desde la tecla Esc en el recorrido.
void MenuAbrir(GLFWwindow* window, MenuEstado& m, bool pausa, MenuPagina pagina = MenuPagina::Principal);
// Cierra el menu: vuelve a capturar el cursor para la camara y quita el raton a ImGui (las fichas son solo de lectura).
void MenuCerrar(GLFWwindow* window, MenuEstado& m);

// Teclado mientras el menu esta abierto (solo en PRESS). Devuelve lo que hay que hacer, si algo.
//   Esc: en una subpagina vuelve al menu; en el menu de inicio sale; en la pausa continua. Flechas / W S: elegir. Enter / Espacio: aceptar.
MenuAccion MenuTecla(MenuEstado& m, int key);

// Raton y rueda: main los reenvia a ImGui solo con el menu abierto (los callbacks de ImGui no estan instalados, ver Fichas.cpp)
void MenuRatonPos(GLFWwindow* window, double x, double y);
void MenuRatonBoton(GLFWwindow* window, int boton, int accion, int mods);
void MenuRueda(GLFWwindow* window, double dx, double dy);

// Dibuja el menu sobre la escena (llamar en lugar de FichasDibuja, justo antes de glfwSwapBuffers). tiempo = segundos (el papel
// picado se mece y las llamas parpadean con senos de ese tiempo). Devuelve la accion si se hizo clic en un boton.
MenuAccion MenuDibuja(MenuEstado& m, double tiempo);
