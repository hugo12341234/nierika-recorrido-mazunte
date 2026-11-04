// Fichas.h - C-7b: fichas culturales con Dear ImGui que aparecen por proximidad (< 2 m).
// Solo lectura: el cursor esta capturado por la camara, asi que la ficha no recibe clics.
#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Radio de proximidad (CONTEXTO_PROYECTO sec. 2 y ESTADO sec. 5): la ficha aparece a menos de esta distancia
const float FICHAS_RADIO = 2.0f;

// Crea el contexto de ImGui y sus backends (llamar despues de glewInit)
void FichasInit(GLFWwindow* window);

// Dibuja la ficha del objeto mas cercano si la camara esta a < FICHAS_RADIO (llamar justo antes de glfwSwapBuffers)
void FichasDibuja(const glm::vec3& posCamara);

// Libera ImGui (llamar al final de main, antes de glfwTerminate; no usa destructores)
void FichasCleanup();
