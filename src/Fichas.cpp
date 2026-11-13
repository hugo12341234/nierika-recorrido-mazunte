// Fichas.cpp - C-7b: fichas culturales con Dear ImGui (ver Fichas.h)
#include "Fichas.h"

#include <cmath>
#include <cstddef>
#include <string>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

namespace
{
    struct Ficha
    {
        const char* titulo;
        const char* texto;   // con su cita en el texto (APA 7)
        const char* nivel;   // nivel de evidencia (CONTEXTO sec. 5)
        glm::vec3 pos;       // punto que se mide contra la camara (solo X y Z)
    };

    // Referencias de la Propuesta v2.0, copiadas en docs/fichas_culturales.md (C-7a). SIGUEN POR VERIFICAR contra la fuente.
    const char* REF_SEP = "Secretaría de Educación Pública. (2026, 22 de mayo). Elementos básicos para la ofrenda de Día de Muertos. Nueva Escuela Mexicana Digital. https://nuevaescuelamexicana.sep.gob.mx/contenido/recurso/elementos-basicos-para-la-ofrenda-de-dia-de-muertos/";
    const char* REF_BANOS = "Baños, L. (2025, 27 de octubre). Los pueblos mágicos de Oaxaca donde el Día de Muertos se vive con más alma. Oaxaca Informa. https://www.oaxacainforma.com/es/cultura/2025/pueblos-magicos-oaxaca-dia-de-muertos-2025";
    const char* REF_UNESCO = "UNESCO. (s. f.). Las fiestas indígenas dedicadas a los muertos. https://ich.unesco.org/es/RL/las-fiestas-indigenas-dedicadas-a-los-muertos-00054";

    // Textos de docs/fichas_culturales.md (C-7a). Fuente UTF-8: el .vcxproj compila con /utf-8.
    // POSICIONES: papel picado, altar y pan se movieron en B-13 (12/11) a donde están hoy los modelos; velas y cempasúchil siguen provisionales.
    // Velas y cempasúchil van en z = 23 porque la zona caminable llega solo hasta z = 24 (Colisiones::Z_MAX, A-9).
    const Ficha fichas[] = {
        { "Velas",
          "La flama de la vela guía a los difuntos de regreso a casa (Secretaría de Educación Pública [SEP], 2026). "
          "En la ceremonia local que se vive al atardecer, se lanzan velas y flores al mar para recordar a quienes partieron (Baños, 2025).",
          "Tradición general (SEP, 2026) y local documentado (Baños, 2025)",
          glm::vec3(-3.0f, 0.0f, 23.0f) },
        { "Flor de cempasúchil",
          "El aroma y el color del cempasúchil guían a las almas hacia el altar (Secretaría de Educación Pública [SEP], 2026). "
          "Al atardecer, las flores también se lanzan al mar para recordar a quienes partieron (Baños, 2025).",
          "Tradición general (SEP, 2026) y local documentado (Baños, 2025)",
          glm::vec3(3.0f, 0.0f, 23.0f) },
        { "Papel picado",
          "El papel picado da color a la ofrenda y representa el aire (Secretaría de Educación Pública [SEP], 2026). "
          "En el recorrido, las tiras cuelgan de soportes de madera en el arco del cuarto.",
          "Tradición general (SEP, 2026); la escena es del estudio",
          glm::vec3(0.0f, 1.95f, 6.0f) },   // B-13: colgado de los soportes (0, 1.95, 6)
        { "Altar de ofrenda",
          "En el altar, cada objeto cumple una función para recibir a las ánimas: velas, flores, pan, agua y sal, y copal, entre otros "
          "(Secretaría de Educación Pública [SEP], 2026). La ofrenda familiar es el acto central de la velación (UNESCO, s. f.). "
          "Aquí la ofrenda se presenta sobre un altar de tres niveles, con una forma que es adaptación del estudio.",
          "Tradición general (SEP, 2026); altar de tres niveles y cuarto: adaptación del estudio",
          glm::vec3(1.35f, 0.0f, 0.6f) },   // B-13: costado del altar3 (en (1.35, 0, 0)); desde la puerta (d ~ 1.5 m) gana a la del pan
        { "Pan de muerto",
          "El pan de muerto representa el afecto y la fraternidad hacia los difuntos (Secretaría de Educación Pública [SEP], 2026). "
          "Se coloca en el altar junto con las demás ofrendas para recibir a las ánimas.",
          "Tradición general (SEP, 2026)",
          glm::vec3(1.40f, 0.80f, 0.0f) },  // B-13: pan_muerto en el nivel medio del altar3 (mundo (1.40, 0.80, 0.00)); a < 0.9 m de la repisa gana a la del altar
    };
    // Referencias de cada ficha (mismo orden que la tabla)
    const char* RefsDe(int i)
    {
        static std::string r[5];
        if (r[0].empty())
        {
            r[0] = r[1] = std::string(REF_BANOS) + "\n" + REF_SEP;
            r[2] = r[4] = REF_SEP;
            r[3] = std::string(REF_SEP) + "\n" + REF_UNESCO;
        }
        return r[i].c_str();
    }
    const int NUM_FICHAS = (int)(sizeof(fichas) / sizeof(fichas[0]));

    // Distancia horizontal (XZ): no depende de la altura de los ojos ni de la del objeto
    float DistanciaXZ(const glm::vec3& a, const glm::vec3& b)
    {
        float dx = a.x - b.x, dz = a.z - b.z;
        return std::sqrt(dx * dx + dz * dz);
    }
}

void FichasInit(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;                          // no escribe imgui.ini
    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;        // fichas de solo lectura
    io.FontGlobalScale = 1.1f;                         // legible a 1280x720
    ImGui::StyleColorsDark();

    // false = ImGui no instala sus callbacks: main.cpp ya usa los suyos para teclado y raton de la camara
    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void FichasDibuja(const glm::vec3& posCamara)
{
    // Ficha mas cercana dentro del radio (si dos coinciden, gana la mas cercana)
    int elegida = -1;
    float mejor = FICHAS_RADIO;
    for (int i = 0; i < NUM_FICHAS; i++)
    {
        float d = DistanciaXZ(posCamara, fichas[i].pos);
        if (d < mejor) { mejor = d; elegida = i; }
    }
    if (elegida < 0) return;   // lejos de todo: no se dibuja nada (ni se pide trabajo a ImGui)

    const Ficha& f = fichas[elegida];

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y - 20.0f), ImGuiCond_Always, ImVec2(0.5f, 1.0f));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.6f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.85f);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs |
                             ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
    ImGui::Begin("Ficha cultural", nullptr, flags);
    ImGui::TextColored(ImVec4(1.0f, 0.75f, 0.2f, 1.0f), "%s", f.titulo);
    ImGui::Separator();
    ImGui::PushTextWrapPos(0.0f);
    ImGui::TextUnformatted(f.texto);
    ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Nivel de evidencia: %s", f.nivel);
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.55f, 0.55f, 0.55f, 1.0f), "Referencias:\n%s", RefsDe(elegida));
    ImGui::PopTextWrapPos();
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void FichasCleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
