#!/usr/bin/env python3
"""E-5: genera los diagramas SVG del manual técnico (arquitectura, ciclo de render y cronograma plan vs. real).
Uso: python3 tools/e5_figuras.py    -> escribe docs/manual_tecnico/figuras/*.svg  (solo biblioteca estándar)
Las fechas del cronograma salen de docs/ESTADO_PROYECTO.md (tareas) y de la propuesta v2.0 (plan inicial)."""
import os, datetime as dt
RAIZ = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SAL = os.path.join(RAIZ, "docs", "manual_tecnico", "figuras"); os.makedirs(SAL, exist_ok=True)
F = "font-family='Inter, Liberation Sans, Arial, sans-serif'"

def t(x, y, s, size=12, fill="#1f2430", anchor="start", weight="400"):
    s = s.replace("&", "&amp;").replace("<", "&lt;")
    return f"<text x='{x}' y='{y}' font-size='{size}' fill='{fill}' text-anchor='{anchor}' font-weight='{weight}' {F}>{s}</text>"

def caja(x, y, w, h, titulo, lineas, color, borde):
    o = [f"<rect x='{x}' y='{y}' width='{w}' height='{h}' rx='6' fill='{color}' stroke='{borde}' stroke-width='1.3'/>",
         t(x + w / 2, y + 17, titulo, 12, "#1f2430", "middle", "700")]
    for i, l in enumerate(lineas):
        o.append(t(x + w / 2, y + 33 + i * 13.5, l, 9.4, "#3d4350", "middle"))
    return "\n".join(o)

def flecha(x1, y1, x2, y2):
    return f"<line x1='{x1}' y1='{y1}' x2='{x2}' y2='{y2}' stroke='#7a5a1e' stroke-width='1.6' marker-end='url(#p)'/>"

DEF = "<defs><marker id='p' markerWidth='8' markerHeight='8' refX='7' refY='4' orient='auto'><path d='M0,0 L8,4 L0,8 z' fill='#7a5a1e'/></marker></defs>"

def arquitectura():
    W, H = 760, 560
    s = [f"<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 {W} {H}' width='{W}' height='{H}'>{DEF}<rect width='{W}' height='{H}' fill='#fffdf8'/>",
         t(W / 2, 24, "", 14, "#1f2430", "middle", "700")]
    c1, b1 = "#fdf0d5", "#b4781e"; c2, b2 = "#e8f0fb", "#4a78b5"; c3, b3 = "#eaf5ea", "#4f9a55"; c4, b4 = "#f3e9f6", "#8a5aa0"
    s.append(t(20, 52, "APLICACIÓN DEL RECORRIDO (C++17 · Visual Studio 2022 · src/main.cpp coordina todos los módulos)", 10.5, "#7a5a1e", "start", "700"))
    mods = [
        ("Entrada y cámara", ["Camera.h (W A S D, ratón,", "pitch ±89°) · GLFW"], c1, b1),
        ("Colisiones y terreno", ["Colisiones.cpp (AABB)", "Terreno.h (altura h(x,z))"], c1, b1),
        ("Recorrido guiado", ["Recorrido.cpp (T): 12", "estaciones, Catmull-Rom"], c1, b1),
        ("Animación", ["Animacion.cpp (KeyFrames,", "slerp) · Silueta · PersonasAnim", "Olas · AnimVelas · AnimFlores"], c1, b1),
        ("Escena y datos", ["Extras · VidaColinas · Ofrendas", "Plaza · Pueblo · Mariposas · Fuego", "(datos generados por tools/)"], c1, b1),
        ("Interfaz (Dear ImGui)", ["Fichas.cpp (13 fichas < 2 m)", "Menu.cpp (inicio, pausa)", "Creditos.h · Hud.h"], c1, b1),
        ("Audio", ["Audio.cpp (miniaudio):", "oleaje + música por zona,", "narración del recorrido"], c1, b1),
        ("Rendimiento", ["Cull.h (frustum por malla)", "Perf.h (tabla por etapa, F9)"], c1, b1),
    ]
    x0, y0, w, h, g = 20, 62, 175, 74, 12
    for i, (ti, li, c, b) in enumerate(mods):
        s.append(caja(x0 + (i % 4) * (w + g), y0 + (i // 4) * (h + g), w, h, ti, li, c, b))
    y = 62 + 2 * (h + g) + 6
    s.append(flecha(W / 2, y - 6, W / 2, y + 14))
    s.append(caja(20, y + 16, 720, 62, "Carga de recursos y render", ["Model.h / Mesh.h (Assimp, SOIL2/stb_image: .obj + .mtl + texturas RGBA 1K)  ·  Shader.h (GLSL desde archivos)",
        "Shaders/lighting.vs/.frag: Blinn-Phong (1 direccional, 4 puntuales, reflectores), olas, llama y relieve · sky.vs/.frag: cielo nocturno"], c2, b2))
    s.append(flecha(W / 2, y + 78, W / 2, y + 98))
    s.append(caja(20, y + 100, 720, 40, "OpenGL 3.3 Core Profile (GLEW) · GLFW · GLM", ["Contexto, ventana 1280×720, matrices y cuaterniones"], c3, b3))
    s.append(flecha(W / 2, y + 140, W / 2, y + 158))
    s.append(caja(20, y + 160, 720, 52, "GPU y recursos en disco", ["Models/<objeto>/ (104 carpetas: 119 .obj, .mtl, texturas y 77 .blend) · Shaders/ · Audio/ · Audio/narracion/", "vcpkg.json fija Assimp, GLFW, GLEW y GLM; vendor/ trae SOIL2, stb_image, Dear ImGui y miniaudio"], c4, b4))
    s.append("</svg>")
    open(os.path.join(SAL, "fig_arquitectura.svg"), "w", encoding="utf8").write("\n".join(s))

def ciclo():
    W, H = 760, 330
    s = [f"<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 {W} {H}' width='{W}' height='{H}'>{DEF}<rect width='{W}' height='{H}' fill='#fffdf8'/>",
         t(W / 2, 24, "", 14, "#1f2430", "middle", "700")]
    pasos = [("1. Tiempo", ["deltaTime y reloj", "FPS cada 0.5 s"]), ("2. Eventos", ["teclado y ratón", "menú / pausa"]),
             ("3. Cámara", ["libre, o la lleva", "el recorrido (T)"]), ("4. Movimiento", ["W A S D × deltaTime", "colisiones AABB"]),
             ("5. Animación", ["relojes en forma", "cerrada del tiempo"]), ("6. Luces y cielo", ["uniforms de luz", "cielo procedural"]),
             ("7. Dibujo", ["descarte por frustum", "terreno, mar, modelos"]), ("8. Interfaz", ["ficha < 2 m, HUD,", "menú, subtítulos"])]
    x0, y0, w, h, g = 20, 50, 168, 62, 14
    for i, (ti, li) in enumerate(pasos):
        x = x0 + (i % 4) * (w + g); y = y0 + (i // 4) * (h + 56)
        s.append(caja(x, y, w, h, ti, li, "#fdf0d5" if i < 6 else "#e8f0fb", "#b4781e" if i < 6 else "#4a78b5"))
        if i % 4 < 3:
            s.append(flecha(x + w, y + h / 2, x + w + g, y + h / 2))
    xf = x0 + 3 * (w + g) + w / 2
    s.append(f"<path d='M{xf},{y0 + h} L{xf},{y0 + h + 28} L{x0 + w / 2},{y0 + h + 28} L{x0 + w / 2},{y0 + h + 56}' fill='none' stroke='#7a5a1e' stroke-width='1.6' marker-end='url(#p)'/>")
    s.append(caja(x0 + 3 * (w + g) - 0, 50 + 2 * (62 + 56) - 14, w, 50, "9. Intercambiar buffers", ["y vuelve al paso 1"], "#eaf5ea", "#4f9a55"))
    s.append(flecha(x0 + 3 * (w + g) + w / 2, y0 + (h + 56) + h, x0 + 3 * (w + g) + w / 2, 50 + 2 * (62 + 56) - 14))
    s.append(t(20, H - 28, "Al cerrar: cleanup() explícito de Shader, Mesh y Model (VAO, VBO, EBO, texturas y programas) y glfwTerminate().", 10, "#3d4350"))
    s.append(t(20, H - 12, "Las animaciones no acumulan deltaTime: se evalúan con el reloj (glfwGetTime), así no dependen de los FPS.", 10, "#3d4350"))
    s.append("</svg>")
    open(os.path.join(SAL, "fig_ciclo_render.svg"), "w", encoding="utf8").write("\n".join(s))

def d(x): return dt.date(2026, int(x.split("/")[1]), int(x.split("/")[0]))

def gantt():
    # Plan inicial (propuesta v2.0, 8 semanas) vs real (fechas de cierre del ESTADO). Las barras reales van del
    # primer al último cierre registrado de cada bloque; son ventanas de trabajo, no horas.
    plan = [("Sem. 1 · Arranque e infraestructura", "28/09", "02/10"), ("Sem. 2 · Escenario base y navegación", "05/10", "09/10"),
            ("Sem. 3 · Cuarto del cementerio y panteón", "12/10", "16/10"), ("Sem. 4 · Objetos de la ofrenda", "19/10", "23/10"),
            ("Sem. 5 · Texturizado, integración, luz", "26/10", "30/10"), ("Sem. 6 · Animaciones simples", "02/11", "06/11"),
            ("Sem. 7 · Animaciones complejas, fichas, audio", "09/11", "13/11"), ("Sem. 8 · Pruebas, empaquetado y entrega", "16/11", "18/11")]
    real = [("A · Escena base con luz (A-1…A-12)", "19/10", "04/11"), ("B · Panteón y altar (B-1…B-14)", "04/11", "12/11"),
            ("C · Animaciones simples y fichas (C-1…C-8)", "04/11", "12/11"), ("D · KeyFrames y audio (D-1…D-7)", "04/11", "12/11"),
            ("E · Pruebas, manuales, empaquetado (E-1…E-7)", "04/11", "18/11"),
            ("X · Extras y refinamiento (X-4…X-70)", "12/11", "18/11")]
    ini, fin = dt.date(2026, 9, 28), dt.date(2026, 11, 19)
    W, H = 760, 560; L, R = 250, 20; span = (fin - ini).days
    def X(day): return L + (day - ini).days / span * (W - L - R)
    s = [f"<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 {W} {H}' width='{W}' height='{H}'><rect width='{W}' height='{H}' fill='#fffdf8'/>",
         t(W / 2, 22, "", 13.5, "#1f2430", "middle", "700")]
    cur = ini
    while cur <= fin:  # líneas de semana (lunes)
        if cur.weekday() == 0:
            s.append(f"<line x1='{X(cur)}' y1='40' x2='{X(cur)}' y2='{H - 40}' stroke='#e6dfcc' stroke-width='1'/>")
            s.append(t(X(cur), H - 26, cur.strftime("%d/%m"), 8, "#6b7280", "middle"))
        cur += dt.timedelta(days=1)
    y = 44
    s.append(t(10, y + 8, "PLAN INICIAL (8 semanas)", 10, "#7a5a1e", "start", "700")); y += 16
    for n, a, b in plan:
        s.append(t(L - 6, y + 11, n, 9, "#1f2430", "end"))
        s.append(f"<rect x='{X(d(a))}' y='{y}' width='{max(4, X(d(b) + dt.timedelta(days=1)) - X(d(a)))}' height='14' rx='3' fill='#d8c9a5' stroke='#b4781e'/>"); y += 20
    y += 10
    s.append(t(10, y + 8, "EJECUCIÓN REAL (primer a último cierre del bloque en el ESTADO)", 10, "#2f6b35", "start", "700")); y += 16
    for n, a, b in real:
        s.append(t(L - 6, y + 11, n, 9, "#1f2430", "end"))
        col, bor = ("#cfe7cf", "#4f9a55") if not n.startswith("X") else ("#e4d3f0", "#8a5aa0")
        s.append(f"<rect x='{X(d(a))}' y='{y}' width='{max(4, X(d(b) + dt.timedelta(days=1)) - X(d(a)))}' height='14' rx='3' fill='{col}' stroke='{bor}'/>"); y += 20
    # Actividad en el repositorio: commits por día en la rama develop (API de GitHub, consulta del 18/11/2026; 465 commits).
    COMMITS = {"19/10": 38, "20/10": 22, "26/10": 3, "04/11": 104, "11/11": 24, "12/11": 12, "13/11": 81, "14/11": 23, "15/11": 39, "17/11": 70, "18/11": 49}
    y += 10
    s.append(t(10, y + 8, "ACTIVIDAD EN EL REPOSITORIO (commits por día en develop; el historial empieza el 19/10)", 10, "#8a5aa0", "start", "700")); y += 18
    base = y + 62
    s.append(f"<line x1='{L}' y1='{base}' x2='{W - R}' y2='{base}' stroke='#999' stroke-width='1'/>")
    wbar = (W - L - R) / span * 0.8
    for k, v in COMMITS.items():
        h = v / 104 * 58
        s.append(f"<rect x='{X(d(k)) + 1}' y='{base - h}' width='{wbar}' height='{h}' fill='#b79ac9' stroke='#8a5aa0' stroke-width='0.6'/>")
        s.append(t(X(d(k)) + wbar / 2 + 1, base - h - 3, str(v), 8, "#5b3a72", "middle"))
    s.append(t(L - 6, base - 20, "commits por día", 9, "#1f2430", "end"))
    s.append(t(L - 6, base - 8, "(sin barra = 0 commits)", 8, "#6b7280", "end"))
    ent = X(d("18/11") + dt.timedelta(days=1))
    s.append(f"<line x1='{ent}' y1='40' x2='{ent}' y2='{H - 40}' stroke='#c0392b' stroke-width='1.6' stroke-dasharray='5 3'/>")
    s.append(t(ent - 4, 38, "Entrega 18/11", 9, "#c0392b", "end", "700"))
    s.append(t(10, H - 8, "Fuentes: propuesta v2.0 (plan) y docs/ESTADO_PROYECTO.md §2 (cierres). Los bloques A–E no tienen fechas por tarea antes del 19/10.", 8.5, "#3d4350"))
    s.append("</svg>")
    open(os.path.join(SAL, "fig_cronograma.svg"), "w", encoding="utf8").write("\n".join(s))

if __name__ == "__main__":
    arquitectura(); ciclo(); gantt(); print("Figuras en", SAL)
