// Fichas.cpp - C-7b: fichas culturales con Dear ImGui (ver Fichas.h)
#include "Fichas.h"

#include <cmath>
#include <cstddef>
#include <string>

#include <vector>
#include "OfrendasFamiliares.h"   // X-55: puntos de la ficha de las ofrendas familiares (frente de cada una)
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
        const char* refs;    // referencias completas (APA 7), separadas por salto de linea
        int nPos;            // cuantos puntos activan la ficha (X-52: las fichas nuevas cubren una zona con varios puntos)
        glm::vec3 pos[4];    // puntos que se miden contra la camara (solo X y Z)
        bool deFamilias = false;   // X-55: true = los puntos son el frente de cada una de las 14 ofrendas familiares (se calculan de OfrendasFamiliares.h)
    };

    // Referencias APA 7. Son macros (y no const char*) para poder unirlas con literales contiguos.
    // Las 3 primeras vienen de la Propuesta v2.0 y se verificaron el 12/11 (docs/fichas_culturales.md sec. 6); las demas son de X-52 (17/11).
    #define REF_SEP "Secretaría de Educación Pública. (2026, 22 de mayo). Elementos básicos para la ofrenda de Día de Muertos. Nueva Escuela Mexicana Digital. https://nuevaescuelamexicana.sep.gob.mx/contenido/recurso/elementos-basicos-para-la-ofrenda-de-dia-de-muertos/"
    #define REF_BANOS "Baños, L. (2025, 27 de octubre). Los pueblos mágicos de Oaxaca donde el Día de Muertos se vive con más alma. Oaxaca Informa. https://www.oaxacainforma.com/es/cultura/2025/pueblos-magicos-oaxaca-dia-de-muertos-2025"
    #define REF_UNESCO "UNESCO. (s. f.). Las fiestas indígenas dedicadas a los muertos. https://ich.unesco.org/es/RL/las-fiestas-indigenas-dedicadas-a-los-muertos-00054"
    #define REF_BANDAS "México Desconocido. (2023, 19 de diciembre). Bandas de viento de Oaxaca, música popular que engalana las fiestas comunales. https://www.mexicodesconocido.com.mx/bandas-de-viento-de-oaxaca.html"
    #define REF_INAH562 "Instituto Nacional de Antropología e Historia. (2025, 27 de octubre). Tepehuas celebran a sus fieles difuntos en el Museo Nacional de Antropología (Boletín No. 562). https://inah.gob.mx/images/boletines/2025/562/Boletin%20562.pdf"
    #define REF_ALUMBRADAS "México Desconocido. (2026, 28 de octubre). Alumbradas de Oaxaca: una noche para reencontrarse con los difuntos. https://www.mexicodesconocido.com.mx/alumbradas-de-oaxaca.html"
    #define REF_COMPARSA "La Razón Online. (2025, 1 de noviembre). Calles de Oaxaca se llenan de vida con la Gran Comparsa del Día de Muertos. La Razón. https://www.razon.com.mx/estados/2025/10/31/calles-de-oaxaca-se-llenan-de-vida-con-la-gran-comparsa-del-dia-de-muertos/"
    #define REF_MONARCA "Expansión Digital. (2025, 20 de octubre). Mariposa monarca: por qué se cree que son las almas que regresan el Día de Muertos. Expansión. https://expansion.mx/tendencias/2025/10/20/por-que-mariposas-monarca-son-almas-regresan-dia-de-muertos"
    #define REF_TORTUGA "México Desconocido. (2010, 10 de agosto). El Centro Mexicano de la tortuga en Mazunte, Oaxaca. https://www.mexicodesconocido.com.mx/el-centro-mexicano-de-la-tortuga-en-mazunte-oaxaca.html"

    // Textos de docs/fichas_culturales.md (C-7a y X-52). Fuente UTF-8: el .vcxproj compila con /utf-8. Sin raya larga ni comillas tipograficas (ImGui).
    // POSICIONES: papel picado, altar y pan se movieron en B-13 (12/11) a donde están hoy los modelos; velas y cempasúchil siguen provisionales.
    // Velas y cempasúchil van en z = 23 porque la zona caminable llega solo hasta z = 24 (Colisiones::Z_MAX, A-9).
    const Ficha fichas[] = {
        { "Velas",
          "La flama de la vela guía a los difuntos de regreso a casa (Secretaría de Educación Pública [SEP], 2026). "
          "En la ceremonia local que se vive al atardecer, se lanzan velas y flores al mar para recordar a quienes partieron (Baños, 2025).",
          "Tradición general (SEP, 2026) y local documentado (Baños, 2025)",
          REF_BANOS "\n" REF_SEP,
          1, { glm::vec3(-3.0f, 0.0f, 23.0f) } },
        { "Flor de cempasúchil",
          "El aroma de las flores, sobre todo del cempasúchil, guía a las almas hacia el altar (Secretaría de Educación Pública [SEP], 2026). "
          "Al atardecer, las flores también se lanzan al mar para recordar a quienes partieron (Baños, 2025).",
          "Tradición general (SEP, 2026) y local documentado (Baños, 2025)",
          REF_BANOS "\n" REF_SEP,
          1, { glm::vec3(3.0f, 0.0f, 23.0f) } },
        { "Papel picado",
          "El papel picado da color a la ofrenda y representa el aire (Secretaría de Educación Pública [SEP], 2026). "
          "En el recorrido, las tiras cuelgan de soportes de madera en el arco del cuarto.",
          "Tradición general (SEP, 2026); la escena es del estudio",
          REF_SEP,
          1, { glm::vec3(0.0f, 1.95f, 6.0f) } },   // B-13: colgado de los soportes (0, 1.95, 6)
        { "Altar de ofrenda",
          "En el altar, cada objeto cumple una función para recibir a las ánimas: velas, flores, pan, agua y sal, y copal, entre otros "
          "(Secretaría de Educación Pública [SEP], 2026). Los platillos del difunto se colocan alrededor del altar familiar y de la tumba (UNESCO, s. f.). "
          "Aquí la ofrenda se presenta sobre un altar de tres niveles, con una forma que es adaptación del estudio.",
          "Tradición general (SEP, 2026); altar de tres niveles y cuarto: adaptación del estudio",
          REF_SEP "\n" REF_UNESCO,
          1, { glm::vec3(1.35f, 0.0f, 0.6f) } },   // B-13 (X-46: valor original tras revertir X-45): desde la puerta (d ~ 1.5 m) gana a la del pan
        { "Pan de muerto",
          "El pan de muerto representa el afecto y la fraternidad hacia los difuntos (Secretaría de Educación Pública [SEP], 2026). "
          "Se coloca en el altar junto con las demás ofrendas para recibir a las ánimas.",
          "Tradición general (SEP, 2026)",
          REF_SEP,
          1, { glm::vec3(1.40f, 0.80f, 0.0f) } },  // B-13 (X-46): pan_muerto en el nivel medio del altar3; a < 0.9 m de la repisa gana a la del altar

        // ---- X-52 (17/11, pedido de D2): fichas de lo que se agrego despues de la propuesta. Cambio de alcance: de 5 a 12 fichas (ESTADO sec. 4) ----
        { "Banda de viento",
          "Las bandas de viento son una tradición oaxaqueña que da identidad y hermana a los pueblos indígenas del estado, y acompañan sus fiestas comunales (México Desconocido, 2023). "
          "Los músicos y los instrumentos de esta banda son una representación del estudio. La música que suena en el recorrido es Xochipitzahuatl, "
          "un canto en náhuatl, y no es música oaxaqueña.",
          "Tradición regional de Oaxaca (México Desconocido, 2023); la banda y la música elegida: adaptación del estudio",
          REF_BANDAS,
          2, { glm::vec3(12.6f, 0.0f, 10.4f), glm::vec3(15.4f, 0.0f, 10.4f) } },   // frente a la fila delantera (z = 9, X-42); la banda esta en (14, 8)
        { "Copal y copalero",
          "El copal forma parte de la ofrenda, junto con las velas, las flores y el pan (Secretaría de Educación Pública [SEP], 2026). "
          "En el panteón tepehua de Pisaflores, Veracruz, las tumbas se adornan en medio del humo del copal (Instituto Nacional de Antropología e Historia [INAH], 2025). "
          "Aquí los dos copaleros y su humo son una adaptación del estudio: el humo es un modelo fijo, sin partículas.",
          "Tradición general (SEP, 2026; INAH, 2025); copaleros y humo: adaptación del estudio",
          REF_SEP "\n" REF_INAH562,
          2, { glm::vec3(-1.05f, 0.0f, 1.75f), glm::vec3(1.05f, 0.0f, 1.75f) } },   // X-47: copaleros a (+-1.05, 1.75); el altar (1.35, 0.6) gana desde su lado
        { "Panteón y tumbas",
          "En las alumbradas de Oaxaca, las familias pasan la noche del 31 de octubre junto a las tumbas de sus seres queridos, y las velas sobre las lápidas iluminan de forma simbólica el camino de regreso (México Desconocido, 2026). "
          "Los platillos del difunto se colocan alrededor del altar familiar y de la tumba (UNESCO, s. f.). "
          "El panteón de cuatro tumbas de este recorrido es una adaptación del estudio.",
          "Tradición regional de Oaxaca (fuente de contenido de marca) y general (UNESCO, s. f.); el panteón: adaptación del estudio",
          REF_ALUMBRADAS "\n" REF_UNESCO,
          4, { glm::vec3(-4.0f, 0.0f, 5.75f), glm::vec3(4.0f, 0.0f, 6.2f), glm::vec3(-4.5f, 0.0f, 9.95f), glm::vec3(4.5f, 0.0f, 10.6f) } },   // al pie de las 4 tumbas (Extras::OFRENDAS_TUMBA)
        { "Ofrendas de las familias",
          "Cada familia prepara su ofrenda a su manera: los platillos del difunto se colocan alrededor del altar familiar y de la tumba (UNESCO, s. f.), "
          "y la flama de las velas y el aroma de las flores guían a las almas de regreso (Secretaría de Educación Pública [SEP], 2026). "
          "Al atardecer también se lanzan velas y flores al mar (Baños, 2025). "
          "Las 14 ofrendas de esta playa, sus familias, los objetos de cada difunto (una lancha, una guitarra, una hamaca, una tabla de remo), las frutas, los colores y las "
          "personas de rodillas son una adaptación del estudio: no son familias reales ni una costumbre documentada de Mazunte.",
          "Tradición general (SEP, 2026; UNESCO, s. f.) y local documentado (Baños, 2025); las familias, sus objetos y sus colores: adaptación del estudio",
          REF_SEP "\n" REF_UNESCO "\n" REF_BANOS,
          0, { glm::vec3(0.0f) }, true },   // X-55: nPos = 0 y deFamilias: sus puntos salen de OfrendasFamiliares::Colocadas()
        { "Comparsa en la orilla",
          "En la Gran Comparsa del Día de Muertos de la ciudad de Oaxaca desfilan más de 50 contingentes, entre catrinas, esqueletos y disfraces de papel, con música y color (La Razón Online, 2025). "
          "La comparsa que marcha por la orilla de este recorrido es una representación del estudio, no una comparsa documentada de Mazunte.",
          "Tradición regional de Oaxaca, ciudad (periodismo); la comparsa de la escena: adaptación del estudio",
          REF_COMPARSA,
          1, { glm::vec3(-12.0f, 0.0f, 22.0f) } },   // la comparsa recorre z = 22.6 +-0.9 (Extras::ComparsaN); punto en la orilla oeste
        { "Camino de las ánimas",
          "La flama de la vela guía a los difuntos de regreso a casa (Secretaría de Educación Pública [SEP], 2026). "
          "En las alumbradas de Oaxaca, una comparsa recorre el pueblo entre música y baile (México Desconocido, 2026). "
          "Los faroles, el cempasúchil y las personas que suben la colina con velas son una procesión inventada por el estudio, no una peregrinación documentada de Mazunte.",
          "Tradición general (SEP, 2026) y regional de Oaxaca (México Desconocido, 2026); la procesión: adaptación del estudio",
          REF_SEP "\n" REF_ALUMBRADAS,
          1, { glm::vec3(0.5f, 0.0f, -28.0f) } },   // X-36: sendero norte con personas con vela (VidaColinasDatos, z -22 a -64)
        { "Mariposas monarca",
          "Según una creencia asociada al pueblo purépecha de Michoacán, las mariposas monarca que llegan a los bosques de oyamel a finales de octubre y principios de noviembre "
          "son las almas de los difuntos que regresan (Expansión Digital, 2025). Es una interpretación simbólica y no una tradición de Mazunte: "
          "las mariposas de estas colinas son una adaptación del estudio.",
          "Creencia de Michoacán (periodismo, sin fuente primaria); las mariposas de la escena: adaptación del estudio",
          REF_MONARCA,
          1, { glm::vec3(0.0f, 0.0f, -37.5f) } },   // X-49: tres mariposas en (5.7, -35.6), (-3.8, -36.1) y (-4.9, -38.4)
        { "Tortugas de Mazunte",
          "En los años sesenta la explotación de tortugas fue la actividad principal de Mazunte, hasta que en mayo de 1990 se decretó la veda total. "
          "Con el apoyo del Centro Mexicano de la Tortuga, el pueblo pasó a la conservación y al ecoturismo; la golfina es la especie que más se reproduce en estas playas (México Desconocido, 2010). "
          "Las tortugas y las crías de este recorrido son una representación del estudio.",
          "Contexto local de Mazunte (México Desconocido, 2010), no de Día de Muertos; los modelos: adaptación del estudio",
          REF_TORTUGA,
          1, { glm::vec3(21.0f, 0.0f, 22.4f) } },   // crias (Extras::CriaX, x 18 a 24.3; z 20.2 a 24.4)
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
    // X-55: punto de cada ofrenda familiar = 1.4 m delante de su frente (se calcula una vez; las posiciones son fijas)
    static const std::vector<glm::vec3> puntosFamilias = []()
    {
        std::vector<glm::vec3> v;
        for (const OfrendasFamiliares::Familia& f : OfrendasFamiliares::Colocadas())
        {
            const float g = f.giro * OfrendasFamiliares::PI / 180.0f;
            v.push_back(glm::vec3(f.x + 1.4f * std::sin(g), 0.0f, f.z + 1.4f * std::cos(g)));
        }
        return v;
    }();
    for (int i = 0; i < NUM_FICHAS; i++)
    {
        for (int k = 0; k < fichas[i].nPos; k++)
        {
            float d = DistanciaXZ(posCamara, fichas[i].pos[k]);
            if (d < mejor) { mejor = d; elegida = i; }
        }
        if (fichas[i].deFamilias)
            for (const glm::vec3& p : puntosFamilias)
            {
                float d = DistanciaXZ(posCamara, p);
                if (d < mejor) { mejor = d; elegida = i; }
            }
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
    ImGui::TextColored(ImVec4(0.55f, 0.55f, 0.55f, 1.0f), "Referencias:\n%s", f.refs);
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
