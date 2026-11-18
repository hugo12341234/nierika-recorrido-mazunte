#pragma once
// X-68: contador de tiempos por etapa (solo medicion; no cambia nada de lo que se ve).
// Activar: tecla F9 (no aparece en el manual) o variable de entorno NIERIKA_PERF=1. Apagado cuesta una comparacion por marca.
// Cada marca PERF("nombre") cierra la etapa anterior y mide dos cosas:
//   CPU = tiempo que tardo el programa en ENVIAR esa etapa a la GPU (llamadas de OpenGL).
//   GPU = tiempo que tardo la GPU en EJECUTARLA (consulta GL_TIMESTAMP, nucleo de OpenGL 3.3; se lee con 2 cuadros de retraso).
// Cada 2 s imprime en la consola una tabla con el promedio por cuadro, las llamadas de dibujo y los triangulos enviados.
// Si en una laptop con graficos integrados la GPU de una etapa es mucho mayor que su CPU, esa etapa esta limitada por pixeles (fragment shader).
#include <GL/glew.h>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

namespace Perf
{
    inline bool activo = false;
    inline long llamadasDibujo = 0;      // glDrawElements enviados en este cuadro (los suma Mesh::Draw)
    inline long triangulos = 0;          // triangulos enviados en este cuadro
    inline long llamadasPrev = 0, trisPrev = 0;
    inline long descartadas = 0;         // mallas omitidas por el descarte de frustum (Cull.h) en este cuadro
    inline long sumDescartadas = 0;

    struct Marca { std::string nombre; double cpuMs; GLuint consulta; long llamadas; long tris; };
    struct Acum { double cpu = 0.0, gpu = 0.0; double llamadas = 0.0, tris = 0.0; };

    inline const int RETRASO = 3;                       // cuadros en vuelo (anillo de consultas)
    inline std::vector<Marca> anillo[RETRASO];          // marcas de cada cuadro del anillo
    inline std::vector<GLuint> pool;                    // consultas libres
    inline int cuadro = 0;
    inline std::vector<std::string> orden;
    inline std::map<std::string, Acum> acum;
    inline double totalCpu = 0.0, totalGpu = 0.0;
    inline long sumLlamadas = 0, sumTriangulos = 0;
    inline int cuadrosAcum = 0;
    inline std::chrono::steady_clock::time_point t0, tUltima, tImpresion;
    inline bool inicializado = false;
    inline int cuadrosMaximos = 0;                      // NIERIKA_PERF_FRAMES=N: cierra tras N cuadros (pruebas de desarrollo)
    inline bool terminado = false;
    inline double tituloFps = 0.0;

    inline double AhoraMs() { return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count(); }

    inline GLuint NuevaConsulta()
    {
        if (!pool.empty()) { GLuint q = pool.back(); pool.pop_back(); return q; }
        GLuint q; glGenQueries(1, &q); return q;
    }

    inline void Leer(std::vector<Marca>& marcas)
    {
        // el tiempo de cada etapa = marca[i] - marca[i-1] (la marca 0 es el inicio del cuadro)
        GLuint64 previo = 0;
        for (size_t i = 0; i < marcas.size(); i++)
        {
            GLuint64 t = 0;
            glGetQueryObjectui64v(marcas[i].consulta, GL_QUERY_RESULT, &t);
            if (i > 0)
            {
                const double gpuMs = (double)(t - previo) * 1e-6;
                Acum& a = acum[marcas[i].nombre];
                if (a.cpu == 0.0 && a.gpu == 0.0 && std::find(orden.begin(), orden.end(), marcas[i].nombre) == orden.end()) orden.push_back(marcas[i].nombre);
                a.gpu += gpuMs;
                a.cpu += marcas[i].cpuMs;
                a.llamadas += (double)marcas[i].llamadas; a.tris += (double)marcas[i].tris;
                totalGpu += gpuMs;
                totalCpu += marcas[i].cpuMs;
            }
            previo = t;
            pool.push_back(marcas[i].consulta);
        }
        marcas.clear();
    }

    inline void Imprimir()
    {
        if (cuadrosAcum <= 0) return;
        const double n = (double)cuadrosAcum;
        std::printf("\n[Perf] promedio de %d cuadros (ms por cuadro): CPU = enviar a la GPU, GPU = ejecutar\n", cuadrosAcum);
        std::printf("[Perf] %-22s %8s %8s %8s %10s\n", "etapa", "CPU", "GPU", "dibujos", "triangulos");
        for (const std::string& nombre : orden)
        {
            const Acum& a = acum[nombre];
            std::printf("[Perf] %-22s %8.2f %8.2f %8.0f %10.0f\n", nombre.c_str(), a.cpu / n, a.gpu / n, a.llamadas / n, a.tris / n);
        }
        std::printf("[Perf] %-22s %8.2f %8.2f  -> GPU %.1f FPS maximos\n", "TOTAL", totalCpu / n, totalGpu / n, totalGpu > 0 ? 1000.0 * n / totalGpu : 0.0);
        std::printf("[Perf] por cuadro: %ld dibujos, %ld triangulos, %ld mallas descartadas por frustum\n", sumLlamadas / cuadrosAcum, sumTriangulos / cuadrosAcum, sumDescartadas / cuadrosAcum);
        std::fflush(stdout);
        acum.clear();
        orden.clear();
        totalCpu = totalGpu = 0.0;
        sumLlamadas = sumTriangulos = sumDescartadas = 0;
        cuadrosAcum = 0;
    }

    inline void Alternar()
    {
        activo = !activo;
        std::printf("[Perf] medicion por etapas %s\n", activo ? "ACTIVADA" : "apagada");
        if (activo) { acum.clear(); orden.clear(); totalCpu = totalGpu = 0.0; sumLlamadas = sumTriangulos = 0; cuadrosAcum = 0; tImpresion = std::chrono::steady_clock::now(); }
        else for (auto& v : anillo) { for (auto& m : v) pool.push_back(m.consulta); v.clear(); }
    }

    inline void Iniciar()
    {
        if (const char* e = std::getenv("NIERIKA_PERF")) { if (e[0] && e[0] != '0') activo = true; }
        if (const char* f = std::getenv("NIERIKA_PERF_FRAMES")) { cuadrosMaximos = std::atoi(f); activo = true; }
        tImpresion = std::chrono::steady_clock::now();
        inicializado = true;
    }

    // Al empezar el cuadro (despues de glfwPollEvents): marca 0
    inline void InicioCuadro()
    {
        if (!activo) return;
        llamadasDibujo = 0; triangulos = 0; llamadasPrev = 0; trisPrev = 0;
        std::vector<Marca>& v = anillo[cuadro % RETRASO];
        GLuint q = NuevaConsulta();
        glQueryCounter(q, GL_TIMESTAMP);
        v.push_back({ "inicio", 0.0, q, 0, 0 });
        tUltima = std::chrono::steady_clock::now();
    }

    // Cierra la etapa anterior con ese nombre
    inline void Etapa(const char* nombre)
    {
        if (!activo) return;
        std::vector<Marca>& v = anillo[cuadro % RETRASO];
        if (v.empty()) return;   // se activo a mitad del cuadro
        const auto ahora = std::chrono::steady_clock::now();
        const double cpu = std::chrono::duration<double, std::milli>(ahora - tUltima).count();
        GLuint q = NuevaConsulta();
        glQueryCounter(q, GL_TIMESTAMP);
        v.push_back({ nombre, cpu, q, llamadasDibujo - llamadasPrev, triangulos - trisPrev });
        llamadasPrev = llamadasDibujo; trisPrev = triangulos;
        tUltima = std::chrono::steady_clock::now();
    }

    // Antes de glfwSwapBuffers: cierra el cuadro, lee el de hace RETRASO-1 cuadros y, cada 2 s, imprime
    inline void FinCuadro()
    {
        if (!activo) return;
        Etapa("imgui+resto");
        sumLlamadas += llamadasDibujo; sumDescartadas += descartadas; descartadas = 0;
        sumTriangulos += triangulos;
        // el cuadro mas viejo del anillo ya termino en la GPU
        std::vector<Marca>& viejo = anillo[(cuadro + 1) % RETRASO];
        if (!viejo.empty()) { Leer(viejo); cuadrosAcum++; }
        cuadro++;
        if (std::chrono::duration<double>(std::chrono::steady_clock::now() - tImpresion).count() >= 2.0)
        {
            Imprimir();
            tImpresion = std::chrono::steady_clock::now();
        }
        if (cuadrosMaximos > 0 && cuadro >= cuadrosMaximos) { Imprimir(); terminado = true; }
    }
}

#define PERF(nombre) ::Perf::Etapa(nombre)
