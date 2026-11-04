// Fichas.cpp - C-7b: fichas culturales con Dear ImGui (ver Fichas.h)
#include "Fichas.h"

#include <cmath>
#include <cstddef>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

namespace
{
    struct Ficha
    {
        const char* titulo;
        const char* texto;
        const char* cita;   // cita en el texto (APA 7)
        const char* nivel;  // nivel de evidencia (CONTEXTO sec. 5)
        glm::vec3 pos;      // punto que se mide contra la camara (solo X y Z)
    };

    // TEXTOS PROVISIONALES (C-7b): usan solo lo que dice la Propuesta v2.0 (tabla 3). Los textos finales y las
    // referencias completas en APA 7 salen de C-7a (docs/fichas_culturales.md). ASCII + acentos latinos: la
    // fuente por defecto de ImGui no trae comillas ni guiones largos tipograficos.
    // POSICIONES PROVISIONALES: velas, cempasuchil y papel picado aun no existen como modelos (B-2, B-3, B-7);
    // se ajustan en B-13 / C-3 / C-4. Altar y pan si existen (capilla provisional en (0, 0, -0.6)).
    const Ficha fichas[] = {
        { "Velas y veladoras",
          "Su flama guia a los difuntos de regreso. En Mazunte, al atardecer, residentes y visitantes tambien "
          "lanzan velas al mar para recordar a quienes partieron.",
          "(SEP, 2026; Banos, 2025)", "Tradicion general (SEP) y local documentado (Banos)",
          glm::vec3(-3.0f, 0.0f, 26.0f) },
        { "Flor de cempasuchil",
          "Su aroma y su color guian a las almas hacia el altar. En la orilla de Mazunte tambien se lanzan flores "
          "al mar durante la ceremonia.",
          "(SEP, 2026; Banos, 2025)", "Tradicion general (SEP) y local documentado (Banos)",
          glm::vec3(3.0f, 0.0f, 26.0f) },
        { "Papel picado",
          "Da color a la ofrenda y representa el aire. En este recorrido cuelga del arco del cuarto; esa "
          "disposicion es una adaptacion del estudio.",
          "(SEP, 2026)", "Tradicion general; ubicacion: adaptacion del estudio",
          glm::vec3(0.0f, 2.0f, 1.6f) },
        { "Altar de ofrenda",
          "La ofrenda familiar es el acto central de la velacion. Cada objeto cumple una funcion para recibir a las "
          "animas. La forma del cuarto y del altar es una adaptacion del estudio, no una reproduccion del panteon real.",
          "(UNESCO, s. f.; SEP, 2026)", "Adaptacion del estudio sobre tradicion general",
          glm::vec3(0.0f, 0.0f, 0.3f) },
        { "Pan de muerto",
          "Es uno de los objetos de la ofrenda; cada objeto cumple una funcion para recibir a las animas. "
          "[Significado especifico del pan: por verificar contra la fuente en C-7a.]",
          "(SEP, 2026)", "Tradicion general",
          glm::vec3(0.0f, 0.39f, -0.43f) },
    };
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
    io.FontGlobalScale = 1.3f;                         // legible a 1280x720
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
    ImGui::TextColored(ImVec4(0.7f, 0.85f, 1.0f, 1.0f), "Fuente: %s", f.cita);
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Nivel de evidencia: %s", f.nivel);
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
