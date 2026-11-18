// Menu.cpp - X-63: menu de inicio y pausa con Dear ImGui (ver Menu.h)
// Estetica: noche oaxaquena. Colores tomados de la escena: los 5 del papel picado (tools/b7_papel_picado.py), el naranja de las
// velas (1.0, 0.55, 0.22 en main.cpp) y el dorado de las fichas (1.0, 0.75, 0.2 en Fichas.cpp). Todo se dibuja con ImDrawList.
#include "Menu.h"
#include "Creditos.h"

#include <algorithm>
#include <cfloat>
#include <cmath>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

namespace
{
    const ImU32 ORO      = IM_COL32(255, 191, 51, 255);
    const ImU32 NARANJA  = IM_COL32(245, 130, 20, 255);
    const ImU32 CREMA    = IM_COL32(255, 232, 196, 255);
    const ImU32 GRIS     = IM_COL32(170, 170, 185, 255);
    const ImU32 PAPEL[5] = { IM_COL32(214, 40, 120, 235), IM_COL32(245, 130, 20, 235), IM_COL32(250, 200, 30, 235),
                             IM_COL32(40, 170, 90, 235),  IM_COL32(30, 120, 200, 235) };   // los de tools/b7_papel_picado.py
    const int N_BOTONES = 4;
    const char* ETIQ_BOTON[N_BOTONES] = { "Iniciar recorrido", "Controles", "Créditos", "Salir" };

    // Texto centrado en cx con sombra; tam en pixeles (la fuente por defecto de ImGui escala sin ser nitida, pero es la unica del proyecto)
    void TextoCentrado(ImDrawList* d, float cx, float y, float tam, ImU32 col, const char* t)
    {
        ImFont* f = ImGui::GetFont();
        ImVec2 s = f->CalcTextSizeA(tam, FLT_MAX, 0.0f, t);
        ImVec2 p(cx - s.x * 0.5f, y);
        d->AddText(f, tam, ImVec2(p.x + 2.0f, p.y + 2.0f), IM_COL32(0, 0, 0, 190), t);
        d->AddText(f, tam, p, col, t);
    }

    // Tiras de papel picado colgadas de una cuerda con catenaria aproximada por parabola: y = 6 + 18 (1 - u^2), u en [-1, 1].
    // Cada tira se mece con dx = 4 sin(1.1 t + 0.9 i) en su borde inferior (formula, sin KeyFrames).
    void PapelPicado(ImDrawList* d, float W, double t)
    {
        const float fw = 64.0f;
        const int n = (int)(W / fw) + 1;
        ImVec2 cuerda[64];
        const int nc = std::min(n + 1, 64);
        for (int i = 0; i < nc; i++)
        {
            float x = i * fw, u = (x - W * 0.5f) / (W * 0.5f);
            cuerda[i] = ImVec2(x, 6.0f + 18.0f * (1.0f - u * u));
        }
        d->AddPolyline(cuerda, nc, IM_COL32(40, 25, 15, 220), ImDrawFlags_None, 2.0f);
        for (int i = 0; i < n; i++)
        {
            float x0 = i * fw + 4.0f, x1 = (i + 1) * fw - 4.0f;
            float xm = (x0 + x1) * 0.5f, u = (xm - W * 0.5f) / (W * 0.5f);
            float y0 = 6.0f + 18.0f * (1.0f - u * u);
            float yb = y0 + 46.0f;                                   // borde inferior de la tira
            float dx = 4.0f * (float)std::sin(1.1 * t + 0.9 * i);    // vaiven
            ImU32 col = PAPEL[i % 5];
            ImVec2 q[4] = { ImVec2(x0, y0), ImVec2(x1, y0), ImVec2(x1 + dx, yb), ImVec2(x0 + dx, yb) };
            d->AddConvexPolyFilled(q, 4, col);
            const float w3 = (x1 - x0) / 3.0f;
            for (int k = 0; k < 3; k++)   // dientes del borde picado
                d->AddTriangleFilled(ImVec2(x0 + k * w3 + dx, yb), ImVec2(x0 + (k + 1) * w3 + dx, yb), ImVec2(x0 + (k + 0.5f) * w3 + dx, yb + 12.0f), col);
            const ImU32 hueco = IM_COL32(8, 8, 24, 150);             // "picado": agujeros que oscurecen, no recortan
            d->AddCircleFilled(ImVec2(xm + dx * 0.5f, y0 + 18.0f), 6.0f, hueco, 4);   // rombo
            d->AddCircleFilled(ImVec2(xm - 14.0f + dx * 0.5f, y0 + 32.0f), 2.5f, hueco, 8);
            d->AddCircleFilled(ImVec2(xm + 14.0f + dx * 0.5f, y0 + 32.0f), 2.5f, hueco, 8);
        }
    }

    // Flor de cempasuchil: dos anillos de petalos (naranja y amarillo) y centro oscuro; gira muy despacio (0.1 rad/s)
    void Cempasuchil(ImDrawList* d, ImVec2 c, float r, double t, float fase)
    {
        const float giro = (float)(0.1 * t) + fase;
        for (int i = 0; i < 12; i++)
        {
            float a = giro + i * 6.2831853f / 12.0f;
            d->AddCircleFilled(ImVec2(c.x + 0.62f * r * std::cos(a), c.y + 0.62f * r * std::sin(a)), 0.38f * r, (i % 2) ? NARANJA : IM_COL32(255, 165, 30, 255), 12);
        }
        for (int i = 0; i < 8; i++)
        {
            float a = -giro + i * 6.2831853f / 8.0f;
            d->AddCircleFilled(ImVec2(c.x + 0.38f * r * std::cos(a), c.y + 0.38f * r * std::sin(a)), 0.30f * r, IM_COL32(250, 200, 30, 255), 10);
        }
        d->AddCircleFilled(c, 0.20f * r, IM_COL32(190, 85, 10, 255), 12);
    }

    // Vela con llama que parpadea: mismo parpadeo que las velas del altar en main.cpp (0.85 + 0.10 sen(7.3 t) + 0.05 sen(13.1 t + 1.3))
    void Vela(ImDrawList* d, ImVec2 base, float alto, double t, float fase)
    {
        const float f = 0.85f + 0.10f * (float)std::sin(7.3 * t + fase) + 0.05f * (float)std::sin(13.1 * t + 1.3 + fase);
        const float ancho = 30.0f;
        ImVec2 punta(base.x, base.y - alto);                       // arriba del cuerpo
        d->AddCircleFilled(ImVec2(punta.x, punta.y - 14.0f), 80.0f * f, IM_COL32(255, 140, 56, 14), 32);   // resplandor
        d->AddCircleFilled(ImVec2(punta.x, punta.y - 14.0f), 50.0f * f, IM_COL32(255, 140, 56, 24), 32);
        d->AddCircleFilled(ImVec2(punta.x, punta.y - 14.0f), 28.0f * f, IM_COL32(255, 170, 70, 40), 24);
        d->AddRectFilled(ImVec2(base.x - 26.0f, base.y - 6.0f), ImVec2(base.x + 26.0f, base.y + 2.0f), IM_COL32(60, 40, 30, 255), 3.0f);   // platillo
        d->AddRectFilledMultiColor(ImVec2(base.x - ancho * 0.5f, punta.y), ImVec2(base.x + ancho * 0.5f, base.y - 6.0f),
                                   IM_COL32(210, 195, 160, 255), IM_COL32(245, 232, 200, 255), IM_COL32(245, 232, 200, 255), IM_COL32(210, 195, 160, 255));
        d->AddEllipseFilled(ImVec2(punta.x, punta.y), ImVec2(ancho * 0.5f, 4.0f), IM_COL32(250, 238, 210, 255));
        d->AddLine(ImVec2(punta.x, punta.y), ImVec2(punta.x, punta.y - 6.0f), IM_COL32(30, 20, 15, 255), 2.0f);   // pabilo
        const float h = 34.0f * f;
        d->AddCircleFilled(ImVec2(punta.x, punta.y - 6.0f - h * 0.30f), 7.5f, IM_COL32(255, 120, 30, 255), 16);   // llama exterior
        d->AddTriangleFilled(ImVec2(punta.x - 7.0f, punta.y - 6.0f - h * 0.38f), ImVec2(punta.x + 7.0f, punta.y - 6.0f - h * 0.38f), ImVec2(punta.x, punta.y - 6.0f - h), IM_COL32(255, 120, 30, 255));
        d->AddCircleFilled(ImVec2(punta.x, punta.y - 6.0f - h * 0.26f), 4.0f, IM_COL32(255, 225, 120, 255), 12);   // nucleo
    }

    // Fondo: oscurece la escena con un degradado nocturno (arriba azul, abajo casi negro); en la pausa se ve mas la escena detenida
    void Fondo(ImDrawList* d, float W, float H, bool pausa)
    {
        const int a0 = pausa ? 120 : 95, a1 = pausa ? 190 : 175;
        d->AddRectFilledMultiColor(ImVec2(0, 0), ImVec2(W, H), IM_COL32(8, 12, 34, a0), IM_COL32(8, 12, 34, a0), IM_COL32(6, 4, 12, a1), IM_COL32(6, 4, 12, a1));
    }

    // Guirnalda de cempasuchil en el borde inferior
    void Guirnalda(ImDrawList* d, float W, float H, double t)
    {
        for (int i = 0; i * 54 < W + 54; i++)
        {
            float x = i * 54.0f + 14.0f * (float)std::sin(i * 1.7f);
            float r = 17.0f + 4.0f * (float)std::sin(i * 2.3f + 1.0f);
            Cempasuchil(d, ImVec2(x, H - 10.0f + 4.0f * (float)std::sin(i * 0.9f)), r, t, i * 0.6f);
        }
    }

    // Linea decorativa dorada con un rombo al centro
    void Filete(ImDrawList* d, float cx, float y, float semiancho)
    {
        d->AddLine(ImVec2(cx - semiancho, y), ImVec2(cx - 12.0f, y), ORO, 1.5f);
        d->AddLine(ImVec2(cx + 12.0f, y), ImVec2(cx + semiancho, y), ORO, 1.5f);
        d->AddQuadFilled(ImVec2(cx - 7.0f, y), ImVec2(cx, y - 6.0f), ImVec2(cx + 7.0f, y), ImVec2(cx, y + 6.0f), NARANJA);
    }

    struct ResBoton { bool clic; bool encima; };

    // Boton a medida: ImGui da el clic (InvisibleButton) y aqui se pinta
    ResBoton Boton(int id, const char* etiqueta, float w, float h, bool elegido)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImDrawList* d = ImGui::GetWindowDrawList();
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - w) * 0.5f);
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImGui::PushID(id);
        ResBoton r;
        r.clic = ImGui::InvisibleButton("b", ImVec2(w, h));
        r.encima = ImGui::IsItemHovered() && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f);
        ImGui::PopID();
        const bool sel = elegido || r.encima;
        ImVec2 q(p.x + w, p.y + h);
        if (sel)
        {
            d->AddRectFilledMultiColor(p, q, IM_COL32(255, 160, 40, 240), IM_COL32(255, 160, 40, 240), IM_COL32(215, 85, 20, 240), IM_COL32(215, 85, 20, 240));
            d->AddRect(p, q, IM_COL32(255, 220, 130, 255), 6.0f, 0, 2.0f);
        }
        else
        {
            d->AddRectFilled(p, q, IM_COL32(22, 24, 52, 205), 6.0f);
            d->AddRect(p, q, IM_COL32(255, 191, 51, 110), 6.0f, 0, 1.5f);
        }
        ImFont* f = ImGui::GetFont();
        const float tam = 22.0f;
        ImVec2 s = f->CalcTextSizeA(tam, FLT_MAX, 0.0f, etiqueta);
        d->AddText(f, tam, ImVec2(p.x + (w - s.x) * 0.5f, p.y + (h - s.y) * 0.5f), sel ? IM_COL32(35, 12, 5, 255) : CREMA, etiqueta);
        if (sel)   // dos rombos a los lados
        {
            const float cy = p.y + h * 0.5f;
            d->AddQuadFilled(ImVec2(p.x + 16.0f, cy), ImVec2(p.x + 22.0f, cy - 6.0f), ImVec2(p.x + 28.0f, cy), ImVec2(p.x + 22.0f, cy + 6.0f), IM_COL32(35, 12, 5, 255));
            d->AddQuadFilled(ImVec2(q.x - 28.0f, cy), ImVec2(q.x - 22.0f, cy - 6.0f), ImVec2(q.x - 16.0f, cy), ImVec2(q.x - 22.0f, cy + 6.0f), IM_COL32(35, 12, 5, 255));
        }
        return r;
    }

    // Una tecla dibujada como "keycap"; devuelve su ancho. p = esquina superior izquierda en pantalla
    float Tecla(ImDrawList* d, ImVec2 p, const char* t)
    {
        ImVec2 s = ImGui::CalcTextSize(t);
        const float w = std::max(s.x + 18.0f, 32.0f), h = s.y + 10.0f;
        d->AddRectFilled(ImVec2(p.x, p.y + 2.0f), ImVec2(p.x + w, p.y + h + 2.0f), IM_COL32(0, 0, 0, 160), 5.0f);   // sombra
        d->AddRectFilled(p, ImVec2(p.x + w, p.y + h), IM_COL32(34, 38, 74, 255), 5.0f);
        d->AddRect(p, ImVec2(p.x + w, p.y + h), IM_COL32(255, 191, 51, 200), 5.0f, 0, 1.5f);
        d->AddText(ImVec2(p.x + (w - s.x) * 0.5f, p.y + 5.0f), CREMA, t);
        return w;
    }

    // Fila de controles: teclas a la izquierda, descripcion a la derecha. Solo teclas que existen en main.cpp
    struct Fila { const char* teclas[4]; int n; const char* desc; };
    const Fila FILAS[] = {
        { { "W", "A", "S", "D" }, 4, "Caminar (también las flechas del teclado)" },
        { { "Ratón" },            1, "Mirar alrededor" },
        { { "V" },                1, "Encender o apagar las velas del altar (a menos de 3 m)" },
        { { "M" },                1, "Sonido: mar y música, solo mar, solo música o silencio" },
        { { "R" },                1, "Reiniciar la caminata de la silueta" },
        { { "T" },                1, "Recorrido guiado (T, Esc o W A S D lo cancelan)" },
        { { "F1" },               1, "Mostrar u ocultar la ayuda de controles" },
        { { "Esc" },              1, "Pausa: abre este menú (en el menú de inicio, sale)" },
    };

    void FilaControl(const Fila& f)
    {
        ImDrawList* d = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();
        float x = p.x;
        for (int i = 0; i < f.n; i++) x += Tecla(d, ImVec2(x, p.y), f.teclas[i]) + 6.0f;
        d->AddText(ImVec2(p.x + 190.0f, p.y + 5.0f), CREMA, f.desc);
        ImGui::Dummy(ImVec2(1.0f, ImGui::GetTextLineHeight() + 22.0f));
    }

    void Titulo(const char* t)
    {
        ImDrawList* d = ImGui::GetWindowDrawList();
        ImVec2 w = ImGui::GetWindowPos();
        TextoCentrado(d, w.x + ImGui::GetWindowWidth() * 0.5f, w.y + 16.0f, 40.0f, ORO, t);
        Filete(d, w.x + ImGui::GetWindowWidth() * 0.5f, w.y + 66.0f, 150.0f);
        ImGui::SetCursorPosY(84.0f);
    }

    void Seccion(const char* t)
    {
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(1.0f, 0.75f, 0.2f, 1.0f), "%s", t);
        ImGui::Separator();
    }

    // Una linea de credito: lo principal en crema y el detalle en gris
    void Linea(const char* principal, const char* detalle = nullptr)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.91f, 0.77f, 1.0f), "%s", principal);
        if (detalle) { ImGui::TextColored(ImVec4(0.67f, 0.67f, 0.73f, 1.0f), "%s", detalle); }
    }

    void Pendiente(const char* t) { ImGui::TextColored(ImVec4(1.0f, 0.55f, 0.35f, 1.0f), "%s", t); }

    // Contenido de Creditos (X-69): TODAS las referencias de docs/REFERENCIAS.md, agrupadas como alli (src/Creditos.h, generado por
    // tools/x69_generar_creditos.py). Es texto plano: el estado de verificacion de cada una vive solo en el documento, no en pantalla.
    void ContenidoCreditos()
    {
        ImGui::PushTextWrapPos(0.0f);
        for (int i = 0; i < Creditos::NUM_SECCIONES; i++)
        {
            const Creditos::Seccion& sec = Creditos::SECCIONES[i];
            Seccion(sec.titulo);
            for (int k = 0; k < sec.n; k++)
            {
                Linea(sec.e[k].texto, sec.e[k].detalle);
                ImGui::Dummy(ImVec2(1.0f, 4.0f));
            }
        }
        ImGui::PopTextWrapPos();
        ImGui::Spacing();
    }
}

void MenuAbrir(GLFWwindow* window, MenuEstado& m, bool pausa, MenuPagina pagina)
{
    m.abierto = true;
    m.pausa = pausa;
    m.pagina = pagina;
    m.seleccion = 0;
    m.desplazaKey = 0.0f;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);              // el raton vuelve a verse para hacer clic
    ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void MenuCerrar(GLFWwindow* window, MenuEstado& m)
{
    m.abierto = false;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);            // camara en primera persona otra vez
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;                 // las fichas son de solo lectura
}

MenuAccion MenuTecla(MenuEstado& m, int key)
{
    if (key == GLFW_KEY_ESCAPE)
    {
        if (m.pagina != MenuPagina::Principal) { m.pagina = MenuPagina::Principal; return MenuAccion::Ninguna; }
        return m.pausa ? MenuAccion::Continuar : MenuAccion::Salir;
    }
    if (m.pagina != MenuPagina::Principal)
    {
        if (key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER || key == GLFW_KEY_BACKSPACE) m.pagina = MenuPagina::Principal;
        else if (m.pagina == MenuPagina::Creditos)
        {
            if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S) m.desplazaKey += 60.0f;
            if (key == GLFW_KEY_UP || key == GLFW_KEY_W) m.desplazaKey -= 60.0f;
            if (key == GLFW_KEY_PAGE_DOWN) m.desplazaKey += 300.0f;
            if (key == GLFW_KEY_PAGE_UP) m.desplazaKey -= 300.0f;
        }
        return MenuAccion::Ninguna;
    }
    if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S) m.seleccion = (m.seleccion + 1) % N_BOTONES;
    if (key == GLFW_KEY_UP || key == GLFW_KEY_W) m.seleccion = (m.seleccion + N_BOTONES - 1) % N_BOTONES;
    if (key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER || key == GLFW_KEY_SPACE)
    {
        switch (m.seleccion)
        {
        case 0: return m.pausa ? MenuAccion::Continuar : MenuAccion::Iniciar;
        case 1: m.pagina = MenuPagina::Controles; break;
        case 2: m.pagina = MenuPagina::Creditos; break;
        default: return MenuAccion::Salir;
        }
    }
    return MenuAccion::Ninguna;
}

void MenuRatonPos(GLFWwindow* window, double x, double y) { ImGui_ImplGlfw_CursorPosCallback(window, x, y); }
void MenuRatonBoton(GLFWwindow* window, int boton, int accion, int mods) { ImGui_ImplGlfw_MouseButtonCallback(window, boton, accion, mods); }
void MenuRueda(GLFWwindow* window, double dx, double dy) { ImGui_ImplGlfw_ScrollCallback(window, dx, dy); }

MenuAccion MenuDibuja(MenuEstado& m, double tiempo)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    const float W = io.DisplaySize.x, H = io.DisplaySize.y;
    ImDrawList* bg = ImGui::GetBackgroundDrawList();
    Fondo(bg, W, H, m.pausa);
    PapelPicado(bg, W, tiempo);
    Guirnalda(bg, W, H, tiempo);
    if (m.pagina == MenuPagina::Principal)   // dos velas a los lados del recuadro
    {
        Vela(bg, ImVec2(W * 0.5f - 395.0f, H - 60.0f), 120.0f, tiempo, 0.0f);
        Vela(bg, ImVec2(W * 0.5f + 395.0f, H - 60.0f), 150.0f, tiempo, 2.1f);
    }

    MenuAccion accion = MenuAccion::Ninguna;
    const ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav;
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(10, 12, 30, 215));
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 191, 51, 150));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.5f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(28.0f, 20.0f));

    if (m.pagina == MenuPagina::Principal)
    {
        const float pw = 520.0f, ph = 480.0f;
        ImGui::SetNextWindowPos(ImVec2(W * 0.5f, H * 0.5f + 28.0f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(pw, ph), ImGuiCond_Always);
        ImGui::Begin("Menu", nullptr, flags);
        ImDrawList* d = ImGui::GetWindowDrawList();
        const ImVec2 w = ImGui::GetWindowPos();
        const float cx = w.x + pw * 0.5f;
        if (m.pausa)
        {
            TextoCentrado(d, cx, w.y + 24.0f, 54.0f, ORO, "Pausa");
            Filete(d, cx, w.y + 92.0f, 170.0f);
            TextoCentrado(d, cx, w.y + 108.0f, 22.0f, CREMA, "Nierika Studio");
            TextoCentrado(d, cx, w.y + 136.0f, 17.0f, GRIS, "Día de Muertos en Mazunte, Oaxaca");
        }
        else
        {
            TextoCentrado(d, cx, w.y + 22.0f, 54.0f, ORO, "Nierika Studio");
            Filete(d, cx, w.y + 90.0f, 190.0f);
            TextoCentrado(d, cx, w.y + 104.0f, 24.0f, CREMA, "Día de Muertos en Mazunte, Oaxaca");
            TextoCentrado(d, cx, w.y + 136.0f, 17.0f, GRIS, "Recorrido virtual 3D interactivo");
        }
        ImGui::SetCursorPosY(184.0f);
        for (int i = 0; i < N_BOTONES; i++)
        {
            const char* et = (i == 0 && m.pausa) ? "Continuar recorrido" : ETIQ_BOTON[i];
            ResBoton r = Boton(i, et, 360.0f, 46.0f, m.seleccion == i);
            ImGui::Dummy(ImVec2(1.0f, 8.0f));
            if (r.encima) m.seleccion = i;
            if (r.clic)
            {
                switch (i)
                {
                case 0: accion = m.pausa ? MenuAccion::Continuar : MenuAccion::Iniciar; break;
                case 1: m.pagina = MenuPagina::Controles; break;
                case 2: m.pagina = MenuPagina::Creditos; break;
                default: accion = MenuAccion::Salir; break;
                }
            }
        }
        TextoCentrado(d, cx, w.y + ph - 36.0f, 15.0f, GRIS, m.pausa ? "Enter elige · Esc continúa" : "Flechas o W S eligen · Enter acepta · Esc sale");
        ImGui::End();
    }
    else
    {
        const bool cred = (m.pagina == MenuPagina::Creditos);
        const float pw = cred ? 820.0f : 760.0f, ph = cred ? 560.0f : 520.0f;
        ImGui::SetNextWindowPos(ImVec2(W * 0.5f, H * 0.5f + 28.0f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(pw, ph), ImGuiCond_Always);
        ImGui::Begin("Pagina", nullptr, flags);
        Titulo(cred ? "Créditos" : "Controles");
        if (cred)
        {
            ImGui::BeginChild("creditos", ImVec2(0.0f, ph - 84.0f - 78.0f), false);
            if (m.desplazaKey != 0.0f) { ImGui::SetScrollY(ImGui::GetScrollY() + m.desplazaKey); m.desplazaKey = 0.0f; }
            if (m.irAlFinal > 0.0f) ImGui::SetScrollY(ImGui::GetScrollMaxY());
            ContenidoCreditos();
            ImGui::EndChild();
        }
        else
        {
            ImGui::Dummy(ImVec2(1.0f, 6.0f));
            for (const Fila& f : FILAS) FilaControl(f);
            ImGui::Spacing();
            ImGui::PushTextWrapPos(0.0f);
            ImGui::TextColored(ImVec4(0.67f, 0.67f, 0.73f, 1.0f), "Acércate a menos de 2 m de un objeto para leer su ficha cultural.");
            ImGui::PopTextWrapPos();
        }
        ImGui::SetCursorPosY(ph - 20.0f - 46.0f);
        if (Boton(100, "Volver", 220.0f, 46.0f, true).clic) m.pagina = MenuPagina::Principal;
        ImGui::End();
    }
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return accion;
}
