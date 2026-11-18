#pragma once

// X-35: atuendos de las personas. Cada persona (poses de persona_pie_a/b y persona_agachada_a/b) recibe, segun un numero de semilla, una
// ropa, un tono de piel y un color de pelo distintos, y a veces una prenda tipica de la costa de Oaxaca: falda de olanes bordada
// (huipil y falda), rebozo o sombrero de palma. Todo es DETERMINISTA (hash entero), sin azar en tiempo de ejecucion.
// Texturas y prendas: tools/x35_atuendos.py. Las texturas se cargan UNA vez (Atuendos::Atuendos) y se liberan con cleanup() al final de main.

#include <cstdint>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Model.h"

class Atuendos
{
public:
    static constexpr int N_ROPAS = 15, N_PIELES = 4, N_PELOS = 4, N_FALDAS = 5, N_REBOZOS = 3, N_SOMBREROS = 2;

    // Eleccion para una persona (-1 = sin esa prenda)
    struct Seleccion { int ropa, piel, pelo, falda, rebozo, sombrero; };

    // Mezcla de bits (hash entero de 32 bits) para repartir los atuendos sin patrones visibles
    static uint32_t mezcla(uint32_t h)
    {
        h ^= h >> 16; h *= 0x7feb352du; h ^= h >> 15; h *= 0x846ca68bu; h ^= h >> 16;
        return h;
    }

    // 14 estilos (X-36; antes 7): ropa 0..13 de Models/atuendos (huipiles bordados, blusas, guayaberas, camisas a cuadros, camisetas, manta).
    // Llevan falda de olanes los estilos 0, 1, 2, 6, 7, 8 y 13; rebozo el 1, 6 y 13; sombrero de palma el 3, 4, 9, 10 y 12.
    // De rodillas (agachada) no se dibujan falda ni rebozo (la pose no coincide con la prenda de pie); si el estilo lleva sombrero, si.
    // X-40: estiloForzado >= 0 pone esa ropa (14 = uniforme de la banda: sin falda ni rebozo; pelo, piel y sombrero siguen saliendo de la semilla).
    static Seleccion elegir(int semilla, bool agachada, int estiloForzado = -1)
    {
        const uint32_t h = mezcla((uint32_t)semilla * 2654435761u + 35u);
        const int estilo = (int)((h >> 4) % 14u);
        static const bool FALDA[14]  = { true, true, true, false, false, false, true, true, true, false, false, false, false, true };
        static const bool REBOZO[14] = { false, true, false, false, false, false, true, false, false, false, false, false, false, true };
        static const bool SOMBR[14]  = { false, false, false, true, true, false, false, false, false, true, true, false, true, false };
        Seleccion s;
        s.ropa = estilo;                                   // el estilo i usa la ropa i
        s.piel = (int)((h >> 8) % (uint32_t)N_PIELES);
        s.pelo = (int)((h >> 11) % (uint32_t)N_PELOS);
        s.falda = (FALDA[estilo] && !agachada) ? (int)((h >> 14) % (uint32_t)N_FALDAS) : -1;
        s.rebozo = (REBOZO[estilo] && !agachada) ? (int)((h >> 18) % (uint32_t)N_REBOZOS) : -1;
        s.sombrero = SOMBR[estilo] ? (int)((h >> 21) % (uint32_t)N_SOMBREROS) : -1;
        if (estiloForzado >= 0) { s.ropa = estiloForzado; s.falda = -1; s.rebozo = -1; }
        return s;
    }

    // Talla propia: altura x 0.88..1.10 y ancho x 0.86..1.14 (X-36 pulido; antes 0.94..1.06 y 0.90..1.10) (hash de la semilla; no cambia las cajas de colision)
    static glm::vec3 talla(int semilla)
    {
        const uint32_t h = mezcla((uint32_t)semilla * 40503u + 7u);
        const float a = 0.88f + 0.22f * (float)(h & 0xFFu) / 255.0f;
        const float w = 0.86f + 0.28f * (float)((h >> 8) & 0xFFu) / 255.0f;
        return glm::vec3(w, a, w);
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
    // X-40: conTalla=false dibuja a escala 1 (los musicos de la banda: sus instrumentos estan modelados a esa altura); estilo = ropa forzada (-1 = por semilla).
    void dibujar(Shader& shader, Model& m, bool agachada, int semilla, const glm::mat4& mm, bool conTalla = true, int estilo = -1)
    {
        const Seleccion s = elegir(semilla, agachada, estilo);
        const GLint loc = glGetUniformLocation(shader.Program, "model");
        const glm::mat4 mt = conTalla ? glm::scale(mm, talla(semilla)) : mm;   // X-36: talla propia (las prendas la heredan)
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mt));
        m.DrawAtuendo(shader, ropa[s.ropa], piel[s.piel], pelo[s.pelo]);
        if (s.falda >= 0)  faldaM.DrawConTextura(shader, falda[s.falda]);
        if (s.rebozo >= 0) rebozoM.DrawConTextura(shader, rebozo[s.rebozo]);
        if (s.sombrero >= 0)
        {
            const glm::mat4 mh = agachada ? glm::translate(mt, glm::vec3(0.0f, -0.61f, -0.29f)) : mt;
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mh));
            sombreroM.DrawConTextura(shader, sombrero[s.sombrero]);
        }
    }


    // X-55: dibuja una persona ARTICULADA (Models/persona_art: una pieza por articulacion). modelos[i] se dibuja con la matriz mundos[i] (nullptr = no se dibuja,
    // p. ej. la raiz sin malla); las prendas (falda, rebozo, sombrero) siguen al torso. Los mundos ya incluyen la talla. No cambia ningun metodo anterior.
    void dibujarPartes(Shader& shader, Model* const* modelos, const glm::mat4* mundos, int n, int idxTorso, int semilla, int estilo = -1)
    {
        // estilo >= 0: se busca la primera semilla vecina cuyo estilo de ropa sea ese, para conservar su falda, rebozo y sombrero (elegir() con
        // estiloForzado los quita: es el uniforme de la banda). Salen en promedio ~14 pruebas: no pesa.
        Seleccion s = elegir(semilla, false, -1);
        for (int k = 1; estilo >= 0 && s.ropa != estilo && k < 4000; k++) s = elegir(semilla + 977 * k, false, -1);
        const GLint loc = glGetUniformLocation(shader.Program, "model");
        for (int i = 0; i < n; i++)
        {
            if (!modelos[i]) continue;
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mundos[i]));
            modelos[i]->DrawAtuendo(shader, ropa[s.ropa], piel[s.piel], pelo[s.pelo]);
        }
        const glm::mat4 mp = glm::translate(mundos[idxTorso], glm::vec3(0.0f, -0.90f, 0.0f));   // las prendas se modelaron con el origen en el piso (cadera a 0.90 m)
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mp));
        if (s.falda >= 0)  faldaM.DrawConTextura(shader, falda[s.falda]);
        if (s.rebozo >= 0) rebozoM.DrawConTextura(shader, rebozo[s.rebozo]);
        if (s.sombrero >= 0)
        {
            const glm::mat4 mh = glm::translate(mp, glm::vec3(0.0f, 0.05f, 0.0f));               // la cabeza de persona_art es 0.07 m mas alta que la de persona_pie
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
