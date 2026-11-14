#pragma once

// X-35: atuendos de las personas. Cada persona (poses de persona_pie_a/b y persona_agachada_a/b) recibe, segun un numero de semilla, una
// ropa, un tono de piel y un color de pelo distintos, y a veces una prenda tipica de la costa de Oaxaca: falda de olanes bordada
// (huipil y falda), rebozo o sombrero de palma. Todo es DETERMINISTA (hash entero), sin azar en tiempo de ejecucion.
// Texturas y prendas: tools/x35_atuendos.py. Las texturas se cargan UNA vez (Atuendos::Atuendos) y se liberan con cleanup() al final de main.

#include <cstdint>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Model.h"

class Atuendos
{
public:
    static constexpr int N_ROPAS = 6, N_PIELES = 4, N_PELOS = 4, N_FALDAS = 5, N_REBOZOS = 3, N_SOMBREROS = 2;

    // Eleccion para una persona (-1 = sin esa prenda)
    struct Seleccion { int ropa, piel, pelo, falda, rebozo, sombrero; };

    // Mezcla de bits (hash entero de 32 bits) para repartir los atuendos sin patrones visibles
    static uint32_t mezcla(uint32_t h)
    {
        h ^= h >> 16; h *= 0x7feb352du; h ^= h >> 15; h *= 0x846ca68bu; h ^= h >> 16;
        return h;
    }

    // 7 estilos: 0 huipil bordado + falda · 1 huipil azul + falda + rebozo · 2 blusa fucsia + falda · 3 guayabera + sombrero ·
    // 4 camisa a cuadros + sombrero · 5 camiseta de rayas (playero) · 6 guayabera sin sombrero.
    // De rodillas (agachada) no se dibujan falda ni rebozo (la pose no coincide con la prenda de pie); si el estilo lleva sombrero, si.
    static Seleccion elegir(int semilla, bool agachada)
    {
        const uint32_t h = mezcla((uint32_t)semilla * 2654435761u + 35u);
        const int estilo = (int)((h >> 4) % 7u);
        static const int ROPA[7]    = { 0, 1, 2, 3, 4, 5, 3 };
        static const bool FALDA[7]  = { true, true, true, false, false, false, false };
        static const bool REBOZO[7] = { false, true, false, false, false, false, false };
        static const bool SOMBR[7]  = { false, false, false, true, true, false, false };
        Seleccion s;
        s.ropa = ROPA[estilo];
        s.piel = (int)((h >> 8) % (uint32_t)N_PIELES);
        s.pelo = (int)((h >> 11) % (uint32_t)N_PELOS);
        s.falda = (FALDA[estilo] && !agachada) ? (int)((h >> 14) % (uint32_t)N_FALDAS) : -1;
        s.rebozo = (REBOZO[estilo] && !agachada) ? (int)((h >> 18) % (uint32_t)N_REBOZOS) : -1;
        s.sombrero = SOMBR[estilo] ? (int)((h >> 21) % (uint32_t)N_SOMBREROS) : -1;
        return s;
    }

    // Carga las prendas y todas las telas (requiere el contexto de OpenGL ya creado)
    Atuendos()
        : faldaM((char*)"Models/falda_tehuana/falda_tehuana.obj"),
          rebozoM((char*)"Models/rebozo/rebozo.obj"),
          sombreroM((char*)"Models/sombrero_palma/sombrero_palma.obj")
    {
        for (int i = 0; i < N_ROPAS; i++)    ropa[i]     = (GLuint)TextureFromFile(("ropa_" + std::to_string(i) + ".png").c_str(), "Models/atuendos");
        for (int i = 0; i < N_PIELES; i++)   piel[i]     = (GLuint)TextureFromFile(("piel_" + std::to_string(i) + ".png").c_str(), "Models/atuendos");
        for (int i = 0; i < N_PELOS; i++)    pelo[i]     = (GLuint)TextureFromFile(("pelo_" + std::to_string(i) + ".png").c_str(), "Models/atuendos");
        for (int i = 0; i < N_FALDAS; i++)   falda[i]    = (GLuint)TextureFromFile(("falda_" + std::to_string(i) + ".png").c_str(), "Models/falda_tehuana");
        for (int i = 0; i < N_REBOZOS; i++)  rebozo[i]   = (GLuint)TextureFromFile(("rebozo_" + std::to_string(i) + ".png").c_str(), "Models/rebozo");
        for (int i = 0; i < N_SOMBREROS; i++) sombrero[i] = (GLuint)TextureFromFile(("sombrero_" + std::to_string(i) + ".png").c_str(), "Models/sombrero_palma");
    }

    // Dibuja la persona 'm' en la matriz 'mm' (la misma que usaria Model::Draw) con el atuendo de 'semilla'.
    // agachada: true para persona_agachada_*; la cabeza queda 0.61 m mas abajo y 0.29 m al frente (-Z) que de pie.
    void dibujar(Shader& shader, Model& m, bool agachada, int semilla, const glm::mat4& mm)
    {
        const Seleccion s = elegir(semilla, agachada);
        const GLint loc = glGetUniformLocation(shader.Program, "model");
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mm));
        m.DrawAtuendo(shader, ropa[s.ropa], piel[s.piel], pelo[s.pelo]);
        if (s.falda >= 0)  faldaM.DrawConTextura(shader, falda[s.falda]);
        if (s.rebozo >= 0) rebozoM.DrawConTextura(shader, rebozo[s.rebozo]);
        if (s.sombrero >= 0)
        {
            const glm::mat4 mh = agachada ? glm::translate(mm, glm::vec3(0.0f, -0.61f, -0.29f)) : mm;
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mh));
            sombreroM.DrawConTextura(shader, sombrero[s.sombrero]);
        }
    }

    void cleanup()
    {
        faldaM.cleanup(); rebozoM.cleanup(); sombreroM.cleanup();
        glDeleteTextures(N_ROPAS, ropa);       glDeleteTextures(N_PIELES, piel);       glDeleteTextures(N_PELOS, pelo);
        glDeleteTextures(N_FALDAS, falda);     glDeleteTextures(N_REBOZOS, rebozo);    glDeleteTextures(N_SOMBREROS, sombrero);
    }

private:
    Model faldaM, rebozoM, sombreroM;
    GLuint ropa[N_ROPAS], piel[N_PIELES], pelo[N_PELOS], falda[N_FALDAS], rebozo[N_REBOZOS], sombrero[N_SOMBREROS];
};
