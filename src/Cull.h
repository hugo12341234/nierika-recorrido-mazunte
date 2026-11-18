#pragma once
// X-68: descarte por frustum (frustum culling) por malla. No cambia nada de lo que se ve: solo se omiten las mallas
// cuya esfera envolvente queda COMPLETAMENTE fuera de la piramide de vision (la GPU las habria recortado de todos modos,
// pero antes habia que enviar sus ~1.27 M triangulos y ~3700 llamadas de dibujo por cuadro).
//
// Como funciona (sin tocar las ~230 llamadas que ya ponen la matriz "model" en main.cpp y Atuendos.h):
//   1) Mesh calcula al cargar una esfera envolvente (centro y radio en coordenadas locales).
//   2) Este archivo redefine glUniformMatrix4fv con una macro: si la ubicacion es la del uniform "model" del programa
//      en uso (la fija Shader::Use), la matriz se GUARDA aqui y se sube a la GPU hasta el primer Mesh::Draw visible
//      (subida perezosa; las mallas descartadas ya no cuestan ni la subida). Cualquier otro uniform pasa directo.
//   3) main.cpp llama Cull::FijarFrustum(proyeccion * vista) una vez por cuadro.
//   4) Mesh::Draw pregunta Cull::Visible(centro, radio): esfera mundo = M * centro, radio * mayor escala de M.
// NIERIKA_SIN_CULLING=1 desactiva todo (vuelve al comportamiento anterior, para comparar FPS).
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <cstdint>
#include <climits>
#include <cmath>
#include <string>
#include <vector>

namespace Cull
{
    inline bool activo = std::getenv("NIERIKA_SIN_CULLING") == nullptr;
    inline GLint locModelo = -1;            // ubicacion de "model" en el programa en uso (la fija Shader::Use)
    inline glm::mat4 modelo(1.0f);          // ultima matriz "model" puesta por el programa
    inline bool pendiente = false;          // true = 'modelo' aun no se ha subido a la GPU
    inline bool frustumListo = false;
    inline glm::vec4 planos[6];             // planos normalizados (ax+by+cz+d >= 0 = dentro)

    // Sube la matriz guardada si falta (usa la funcion real de GLEW: la macro de abajo aun no existe en este punto)
    inline void Subir()
    {
        if (pendiente && locModelo >= 0)
            glUniformMatrix4fv(locModelo, 1, GL_FALSE, glm::value_ptr(modelo));
        pendiente = false;
    }

    // Atiende glUniformMatrix4fv: guarda "model" (subida perezosa) o lo manda tal cual
    inline void Matriz(GLint loc, GLsizei n, GLboolean transponer, const GLfloat* p)
    {
        if (activo && loc >= 0 && loc == locModelo && n == 1 && !transponer)
        {
            modelo = glm::make_mat4(p);
            pendiente = true;
            return;
        }
        glUniformMatrix4fv(loc, n, transponer, p);
    }

    // Shader::Use: antes de cambiar de programa se sube lo pendiente al programa anterior
    inline void CambiarPrograma(GLint nuevoLocModelo)
    {
        Subir();
        locModelo = nuevoLocModelo;
        pendiente = false;
    }

    // Planos del frustum a partir de P*V (Gribb-Hartmann): fila4 +- fila_i de la matriz de recorte
    inline void FijarFrustum(const glm::mat4& pv)
    {
        const glm::vec4 f1(pv[0][0], pv[1][0], pv[2][0], pv[3][0]);
        const glm::vec4 f2(pv[0][1], pv[1][1], pv[2][1], pv[3][1]);
        const glm::vec4 f3(pv[0][2], pv[1][2], pv[2][2], pv[3][2]);
        const glm::vec4 f4(pv[0][3], pv[1][3], pv[2][3], pv[3][3]);
        planos[0] = f4 + f1; planos[1] = f4 - f1;   // izquierdo, derecho
        planos[2] = f4 + f2; planos[3] = f4 - f2;   // inferior, superior
        planos[4] = f4 + f3; planos[5] = f4 - f3;   // cercano, lejano
        for (int i = 0; i < 6; i++)
        {
            const float l = glm::length(glm::vec3(planos[i]));
            if (l > 0.0f) planos[i] /= l;
        }
        frustumListo = true;
    }

    // true si la esfera (centro y radio locales, con la matriz "model" actual) toca el frustum. Conservador: ante la duda, true.
    inline bool Visible(const glm::vec3& centro, float radio)
    {
        if (!activo || !frustumListo) return true;
        const glm::vec3 c = glm::vec3(modelo * glm::vec4(centro, 1.0f));
        const float s = glm::max(glm::length(glm::vec3(modelo[0])), glm::max(glm::length(glm::vec3(modelo[1])), glm::length(glm::vec3(modelo[2]))));
        const float r = radio * s;
        for (int i = 0; i < 6; i++)
            if (planos[i].x * c.x + planos[i].y * c.y + planos[i].z * c.z + planos[i].w < -r) return false;
        return true;
    }
}

namespace Cull
{
    // ---- Cache de ubicaciones de uniforms: glGetUniformLocation busca por nombre en el controlador (muy lento en Intel) y main.cpp lo llama
    // ~2000 veces por cuadro (una por objeto, para "model"). Se pregunta una sola vez por (programa, nombre) y se recuerda. Los programas
    // nunca se vuelven a enlazar, asi que la ubicacion no cambia. Con NIERIKA_SIN_CULLING=1 se consulta siempre al controlador (comportamiento anterior).
    struct EntradaLoc { GLuint prog = 0; uint32_t hash = 0; std::string nombre; GLint loc = -1; };
    inline const size_t N_LOC = 4096;   // potencia de 2; el programa de iluminacion tiene ~300 nombres distintos
    inline std::vector<EntradaLoc> tablaLoc;

    inline GLint Loc(GLuint prog, const char* nombre)
    {
        if (!activo) return glGetUniformLocation(prog, nombre);
        if (tablaLoc.empty()) tablaLoc.resize(N_LOC);
        uint32_t h = 2166136261u ^ (uint32_t)prog;
        for (const char* c = nombre; *c; ++c) { h ^= (uint8_t)*c; h *= 16777619u; }
        size_t i = h & (N_LOC - 1);
        for (size_t k = 0; k < N_LOC; k++, i = (i + 1) & (N_LOC - 1))
        {
            EntradaLoc& e = tablaLoc[i];
            if (e.prog == 0)
            {
                e.prog = prog; e.hash = h; e.nombre = nombre; e.loc = glGetUniformLocation(prog, nombre);
                return e.loc;
            }
            if (e.hash == h && e.prog == prog && e.nombre == nombre) return e.loc;
        }
        return glGetUniformLocation(prog, nombre);   // tabla llena (no deberia pasar)
    }

    // ---- Uniforms que solo pone Mesh::Draw (sampler, tieneRelieve, esVidrio, shininess): se omite glUniform si el valor ya es el que tiene el programa.
    struct EstadoPrograma { GLuint prog = 0; std::vector<int> enteros; std::vector<float> reales; };
    inline EstadoPrograma estados[4];
    inline EstadoPrograma& Estado(GLuint prog)
    {
        for (EstadoPrograma& e : estados) if (e.prog == prog) return e;
        for (EstadoPrograma& e : estados) if (e.prog == 0) { e.prog = prog; return e; }
        estados[3] = EstadoPrograma(); estados[3].prog = prog; return estados[3];
    }
    inline void UniformeI(GLuint prog, GLint loc, int v)
    {
        if (loc < 0) return;
        if (!activo) { glUniform1i(loc, v); return; }
        EstadoPrograma& e = Estado(prog);
        if ((size_t)loc >= e.enteros.size()) e.enteros.resize(loc + 1, INT_MIN);
        if (e.enteros[loc] == v) return;
        e.enteros[loc] = v;
        glUniform1i(loc, v);
    }
    inline void UniformeF(GLuint prog, GLint loc, float v)
    {
        if (loc < 0) return;
        if (!activo) { glUniform1f(loc, v); return; }
        EstadoPrograma& e = Estado(prog);
        if ((size_t)loc >= e.reales.size()) e.reales.resize(loc + 1, NAN);
        if (e.reales[loc] == v) return;
        e.reales[loc] = v;
        glUniform1f(loc, v);
    }

    // ---- Texturas enlazadas por unidad (0..3). Mesh::Draw solo enlaza si cambia la textura. Se olvida todo al empezar cada cuadro (Olvidar)
    // porque ImGui y la carga de modelos enlazan texturas por su cuenta. Las unidades >= 4 no se recuerdan (main usa la 6 a mano).
    inline const int N_UNIDADES = 4;
    inline GLuint texEnlazada[N_UNIDADES] = { UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX };
    inline void OlvidarTexturas() { for (int i = 0; i < N_UNIDADES; i++) texEnlazada[i] = UINT_MAX; }
    inline void EnlazarTextura(GLuint unidad, GLuint id)
    {
        if (activo && unidad < (GLuint)N_UNIDADES)
        {
            if (texEnlazada[unidad] == id) return;
            texEnlazada[unidad] = id;
        }
        glActiveTexture(GL_TEXTURE0 + unidad);
        glBindTexture(GL_TEXTURE_2D, id);
    }
}

#undef glGetUniformLocation
#define glGetUniformLocation(prog, nombre) ::Cull::Loc((prog), (nombre))

// A partir de aqui, TODA llamada a glUniformMatrix4fv (main.cpp, Atuendos.h, Mesh.h...) pasa por Cull::Matriz
#undef glUniformMatrix4fv
#define glUniformMatrix4fv(loc, n, transponer, p) ::Cull::Matriz((loc), (n), (transponer), (p))
