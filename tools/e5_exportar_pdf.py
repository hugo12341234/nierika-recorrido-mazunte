#!/usr/bin/env python3
"""E-5 / E-6 — genera los dos PDF de la entrega final con los nombres que pide el profesor:

    docs/entrega/ManualDeUsuario_Equipo15_319266894_321332086.pdf
    docs/entrega/ManualTecnico_Equipo15_319266894_321332086.pdf

Cadena: Markdown (docs/manual_usuario, docs/manual_tecnico) -> pandoc (HTML autocontenido, fórmulas MathML)
        -> Chromium con Playwright (PDF A4, encabezado/pie, marcadores) -> portada + cuerpo con pdfunite.
Requisitos: python3, pandoc, pdfunite/pdftotext (poppler), Node.js con el paquete `playwright` y Chromium.
Uso:        python3 tools/e5_exportar_pdf.py [usuario|tecnico|todo]      (por omisión: todo)
Los textos de cada integrante viven en docs/manual_tecnico/individual/ y las secciones de otros hilos en
docs/manual_tecnico/secciones/: si el archivo existe se integra; si no, el PDF muestra un aviso de pendiente.
"""
import os, re, subprocess, sys, shutil, tempfile, html, json

RAIZ = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
MT = os.path.join(RAIZ, "docs", "manual_tecnico")
MU = os.path.join(RAIZ, "docs", "manual_usuario")
SAL = os.path.join(RAIZ, "docs", "entrega")
SUF = "Equipo15_319266894_321332086"
JS = os.path.join(RAIZ, "tools", "pdf", "e5_pdf.js")
CSS = os.path.join(RAIZ, "tools", "pdf", "manual.css")

# (archivo, opcional) en el orden del PDF. Los opcionales se saltan si no existen (lo avisa el propio capítulo).
TECNICO = [
    "00_resumen.md", "01_a_arquitectura.md", "02_b_investigacion_cultural.md", "03_c_tecnologias.md",
    "04_d_cronograma.md", "05_e_roles_raci.md", "secciones/roles_raci_real.md",
    "06_f_metodologia.md", "secciones/evidencias_colaborativas.md",
    "07_g_aprendizaje_autonomo.md", "individual/D1_aprendizaje_autonomo.md", "individual/D2_aprendizaje_autonomo.md",
    "08_h_herramientas_colaborativas.md", "09_i_animaciones.md",
    "10_j_redaccion_individual.md", "individual/D1_english.md", "individual/D2_english.md",
    "11_k_bibliografia.md", "12_l_conclusiones.md", "13_m_creditos_licencias.md",
    "14_anexo_modelos.md", "15_anexo_pruebas.md",
    "justificacion_simples.md", "justificacion_complejas.md", "justificacion_personas.md",
]
USUARIO = ["01_inicio.md", "02_controles.md", "03_recorrido.md", "04_fichas_y_audio.md", "05_problemas_frecuentes.md", "06_codigo_fuente.md"]

EMOJI = "✅🔄⏳❓📷🟡🤝❌⚠️🔴🟠🟢📌"

def limpiar(texto, base):
    """Quita marcas internas del repositorio y deja rutas de imagen absolutas."""
    t = texto
    t = re.sub(r"<!--.*?-->", "", t, flags=re.S)
    t = re.sub(r"[✅🟡]\s*\*\*\[(repo|adaptación)[^\]]*\]\*\*", lambda m: "(comprobado en el repositorio)" if m.group(1) == "repo" else "(adaptación del estudio)", t)
    t = re.sub(r"[✅🟡]\s*\[(repo|adaptación)[^\]]*\]", lambda m: "(comprobado en el repositorio)" if m.group(1) == "repo" else "(adaptación del estudio)", t)
    t = t.replace("$[propuesta]$", "(propuesta del estudio)")
    t = re.sub(r"\s*\[repo(?:,[^\]]*)?\]", "", t)
    t = t.replace("[propuesta]", "(propuesta del estudio)").replace("[inferido]", "(inferido)")
    t = re.sub("[" + EMOJI + "]️?", "", t)
    # enlaces internos (.md o carpetas): se dejan como texto
    t = re.sub(r"(?<!\!)\[([^\]]+)\]\((?!https?:|#|mailto:)[^)]*\)", r"\1", t)
    # imágenes relativas -> absolutas
    def img(m):
        ruta, titulo = m.group(2), m.group(3) or ""
        if re.match(r"https?:|data:", ruta):
            return m.group(0)
        return "![%s](%s%s)" % (m.group(1), os.path.normpath(os.path.join(base, ruta)), titulo)
    t = re.sub(r"!\[([^\]]*)\]\(([^)\s]+)(\s+\"[^\"]*\")?\)", img, t)
    return t

# Aviso que el PDF muestra cuando falta un archivo que debe aportar otro hilo o un integrante.
PENDIENTES = {
    "secciones/roles_raci_real.md": "## e.4 RACI real por integrante\n\n> **PENDIENTE DE INTEGRAR.** La matriz RACI de lo que cada integrante hizo realmente la prepara otro hilo (`docs/manual_tecnico/secciones/roles_raci_real.md`). Hasta que exista, este manual solo afirma lo que consta en el repositorio (e.3).",
    "secciones/evidencias_colaborativas.md": "## f.3 Evidencias de Trello y diagrama de Gantt\n\n> **PENDIENTE DE INTEGRAR.** Las capturas de Trello, el Gantt y las demás evidencias las prepara otro hilo (`docs/manual_tecnico/secciones/evidencias_colaborativas.md`).",
}

def adaptar_raci(txt):
    """La sección suelta de otro hilo trae notas internas: se integra como e.4 sin la cabecera, §2.4 (usuario de GitHub), §4 y §5."""
    txt = re.sub(r"^# [^\n]*\n\n(?:>[^\n]*\n)*", "## e.4 RACI real por integrante\n", txt, count=1)
    txt = txt.replace("`thecurlyguy`", "la otra cuenta colaboradora")
    txt = re.sub(r"^### 2\.4 .*?(?=^## )", "", txt, flags=re.M | re.S)
    txt = re.sub(r"^## 4\. .*\Z", "", txt, flags=re.M | re.S)
    txt = re.sub(r"^### ", "#### ", txt, flags=re.M)
    txt = re.sub(r"^## (\d)", r"### \1", txt, flags=re.M)
    txt = re.sub(r"^### (\d+)\. ", r"### e.4.\1 ", txt, flags=re.M)
    txt = re.sub(r"^#### (\d+)\.(\d+) ", r"#### e.4.\1.\2 ", txt, flags=re.M)
    return txt

def adaptar_evidencias(raw):
    """Sección suelta de Trello/Gantt: sus marcas de confianza pasan a palabras (limpiar() borra los emoji) y se integra como f.4."""
    raw = raw.replace("✅ verificado", "(verificado)").replace("✅ **verificado**", "(verificado)")
    raw = raw.replace("✅", "(verificado)").replace("🔍", "(inferido)").replace("❓", "(sin evidencia)").replace("❌", "(no demostrable)")
    raw = re.sub(r"^# [^\n]*\n\n(?:>[^\n]*\n)*", "## f.4 Evidencias de Trello y diagrama de Gantt\n", raw, count=1)
    raw = re.sub(r"^## 7\. .*\Z", "", raw, flags=re.M | re.S)
    raw = re.sub(r"^### ", "#### ", raw, flags=re.M)
    raw = re.sub(r"^## (\d)", r"### \1", raw, flags=re.M)
    raw = re.sub(r"^### (\d+)\. ", r"### f.4.\1 ", raw, flags=re.M)
    raw = re.sub(r"^#### (\d+)\.(\d+) ", r"#### f.4.\1.\2 ", raw, flags=re.M)
    # los rótulos «Figura N» propios de la sección se sustituyen por la numeración global del documento (ver numerar_figuras)
    raw = re.sub(r"^#### Figuras? \d[^\n]*\n\n", "", raw, flags=re.M)
    raw = re.sub(r"!\[Figura (\d+)\. ([^\]]*)\]\(([^)\s]+)\)", r'![\2](\3 "ev\1")', raw)
    raw = re.sub(r"^\*Figura \d+\. ", "*", raw, flags=re.M)
    raw = re.sub(r"Figuras (\d+)(–| y )(\d+)", r"Figuras @@EV\1@@\2@@EV\3@@", raw)
    raw = re.sub(r"Figura (\d+)(?!\d)", r"Figura @@EV\1@@", raw)
    return raw

ANEXOS = {"justificacion_simples.md": "Anexo C. ", "justificacion_complejas.md": "Anexo D. ", "justificacion_personas.md": "Anexo E. "}

def unir(base, archivos):
    partes = []
    for a in archivos:
        ruta = os.path.join(base, a)
        if not os.path.exists(ruta):
            print("  (no existe) " + a)
            if base == MT and a in PENDIENTES:
                partes.append(PENDIENTES[a])
            continue
        with open(ruta, encoding="utf8") as f:
            raw = f.read()
            if base == MT and a == "secciones/evidencias_colaborativas.md":
                raw = adaptar_evidencias(raw)
            txt = limpiar(raw, os.path.dirname(ruta))
            if base == MT and a in ANEXOS:
                txt = re.sub(r"^# ", "# " + ANEXOS[a], txt, count=1, flags=re.M)
                L = ANEXOS[a].split()[1][0]   # «Anexo C. » -> C
                txt = re.sub(r"^(#{2,4}) (\d+(?:\.\d+)*)\.? ", lambda m: "%s %s.%s " % (m.group(1), L, m.group(2)), txt, flags=re.M)
            if base == MT and a == "secciones/roles_raci_real.md":
                txt = adaptar_raci(txt)
            partes.append(txt)
    return "\n\n".join(partes)

def run(cmd, **kw):
    r = subprocess.run(cmd, capture_output=True, text=True, **kw)
    if r.returncode != 0:
        sys.exit("ERROR en %s\n%s" % (" ".join(cmd[:3]), r.stderr))
    return r.stdout

def numerar_figuras(h):
    """Numera todas las figuras del documento en orden (Figura 1, 2, 3…) y les pone pie.
    - Imagen sola en su párrafo -> <figure><img><figcaption>Figura N. …</figcaption></figure>.
    - Si la imagen lleva título «evN» (figuras de la sección f.4), su pie es el párrafo en cursiva que la sigue
      y las referencias @@EVN@@ del texto se sustituyen por el número global."""
    claves, n = {}, [0]
    def fig(m):
        img, resto = m.group(1), m.group(2)
        n[0] += 1
        num = n[0]
        alt = html.unescape(re.search(r'alt="([^"]*)"', img).group(1)) if 'alt="' in img else ""
        k = re.search(r' title="(ev\d+)"', img)
        pie = None
        if k:
            claves[k.group(1)[2:]] = num
            img = img.replace(k.group(0), "")
            c = re.match(r"\s*<p><em>(.*?)</em></p>", resto, flags=re.S)
            if c:
                pie, resto = c.group(1), resto[c.end():]
        if pie is None:
            pie = html.escape(alt, quote=False)
        return '<figure><div class="fimg">%s</div><figcaption><span class="fn">Figura %d.</span> %s</figcaption></figure>%s' % (img, num, pie, resto)
    h = re.sub(r"<p>(<img [^>]*>)</p>(.*?)(?=<p><img |\Z)", lambda m: fig(m), h, flags=re.S)
    h = re.sub(r"@@EV(\d+)@@", lambda m: str(claves.get(m.group(1), "?")), h)
    return h, n[0]

def clasificar_tablas(h):
    """Colorea la columna «Lectura» de la matriz RACI y marca la tabla para darle anchos propios."""
    h = re.sub(r"<td>(Concentrado en D2[^<]*)</td>", r'<td class="lec lec-d2">\1</td>', h)
    h = re.sub(r"<td>(Parcial: aporte de D1[^<]*)</td>", r'<td class="lec lec-parcial">\1</td>', h)
    h = re.sub(r"<td>(Sin evidencia del aporte de D1[^<]*)</td>", r'<td class="lec lec-sin">\1</td>', h)
    h = re.sub(r"<table>(\s*<thead>\s*<tr>\s*<th>Área \(actividades\)</th>\s*<th>Plan \(propuesta\))", r'<table class="raci">\1', h)
    return h

def norm(s):
    return re.sub(r"\s+", " ", html.unescape(re.sub(r"<[^>]+>", "", s))).strip().lower()

def paginar_toc(h, pdf_texto):
    """Pone el número de página (el del pie de página del cuerpo) a cada entrada del índice. pdf_texto: texto de cada página del PDF."""
    entradas = re.findall(r'<a href="#([^"]+)" id="toc-[^"]+">(.*?)</a>', h, flags=re.S)
    paginas = [norm(p) for p in pdf_texto]
    claves = [norm(e[1])[:32] for e in entradas]
    n_toc = 0
    for p in paginas:   # páginas del índice: las iniciales con casi todas las entradas
        if sum(1 for k in claves if k in p) >= 8:
            n_toc += 1
        else:
            break
    actual, num = n_toc, {}
    for (hid, txt), k in zip(entradas, claves):
        hallada = None
        for i in range(actual, len(paginas)):
            if k in paginas[i]:
                hallada = i
                break
        if hallada is None:
            print("   (sin página en el índice) " + k)
            hallada = actual
        actual = hallada
        num[hid] = hallada + 1
    def pon(m):
        hid, resto = m.group(1), m.group(2)
        return '<a href="#%s" id="toc-%s"><span class="t">%s</span><span class="dots"></span><span class="pg">%d</span></a>' % (hid, hid, resto, num[hid])
    return re.sub(r'<a href="#([^"]+)" id="toc-[^"]+">(.*?)</a>', pon, h, flags=re.S), n_toc

def construir(clave, titulo, subtitulo, base, archivos, nombre, version, toc_prof):
    print("== %s" % nombre)
    tmp = tempfile.mkdtemp(prefix="e5pdf_")
    md = os.path.join(tmp, "doc.md")
    with open(md, "w", encoding="utf8") as f:
        f.write(unir(base, archivos))
    portada = """<section class="portada"><div class="marca">NIERIKA STUDIO</div><div class="lema">Ver más allá de lo visible</div>
<h1>%s</h1><h2>Recorrido virtual 3D interactivo<br>Día de Muertos en Mazunte, Oaxaca</h2><p class="sub">%s</p>
<table class="datos"><tr><td>Equipo</td><td>15 · Nierika Studio</td></tr><tr><td>Integrantes (n.º de cuenta)</td><td>319266894 · 321332086</td></tr>
<tr><td>Materia</td><td>Computación Gráfica e Interacción Humano-Computadora</td></tr>
<tr><td>Entrega</td><td>18 de noviembre de 2026</td></tr><tr><td>Versión</td><td>%s</td></tr></table></section>""" % (html.escape(titulo), html.escape(subtitulo), html.escape(version))
    with open(os.path.join(tmp, "portada.html"), "w", encoding="utf8") as f:
        f.write("<!doctype html><html lang='es'><head><meta charset='utf-8'><style>%s</style></head><body class='cover'>%s</body></html>" % (open(CSS, encoding="utf8").read(), portada))
    cuerpo = os.path.join(tmp, "cuerpo.html")
    cmd = ["pandoc", md, "-f", "gfm+tex_math_dollars", "-t", "html5", "-s", "--embed-resources", "--mathml",
           "--css", CSS, "--metadata", "title=%s" % titulo, "--metadata", "lang=es", "--metadata", "pagetitle=%s" % titulo,
           "--resource-path", RAIZ, "-o", cuerpo]
    if toc_prof:
        cmd += ["--toc", "--toc-depth=%d" % toc_prof]
    run(cmd)
    # pandoc pone el título como <header id="title-block-header">: lo quitamos (la portada ya lo trae)
    h = open(cuerpo, encoding="utf8").read()
    h = re.sub(r'<header id="title-block-header">.*?</header>', "", h, flags=re.S)
    h, nfig = numerar_figuras(h)
    h = clasificar_tablas(h)
    if clave == "u":   # manual de usuario: capturas algo más angostas para que quepan dos por página
        h = h.replace("</head>", "<style>figure img{max-width:80%}</style></head>", 1)
    pie = "Nierika Studio · Equipo 15 · %s · 319266894 · 321332086" % titulo
    if toc_prof:
        # dos pasadas: la primera calcula en qué página cae cada título; la segunda escribe esos números en el índice
        h1 = re.sub(r'(<a href="#[^"]+" id="toc-[^"]+">)(.*?)</a>', r'\1<span class="t">\2</span><span class="dots"></span><span class="pg">00</span></a>', h, flags=re.S)
        open(cuerpo, "w", encoding="utf8").write(h1)
        run(["node", JS, tmp, pie])
        texto = run(["pdftotext", "-layout", os.path.join(tmp, "cuerpo.pdf"), "-"]).split("\f")
        h, n_toc = paginar_toc(h, texto)
        print("   índice: %d páginas" % n_toc)
    open(cuerpo, "w", encoding="utf8").write(h)
    run(["node", JS, tmp, pie])
    os.makedirs(SAL, exist_ok=True)
    destino = os.path.join(SAL, nombre)
    run(["pdfunite", os.path.join(tmp, "portada.pdf"), os.path.join(tmp, "cuerpo.pdf"), destino])
    paginas = run(["pdfinfo", destino]).split("Pages:")[1].split()[0]
    print("   %s  (%s páginas, %.1f MB, %d figuras)" % (destino, paginas, os.path.getsize(destino) / 1e6, nfig))
    if os.environ.get("E5_CONSERVAR"):
        print("   temporales:", tmp)
    else:
        shutil.rmtree(tmp, ignore_errors=True)

def main():
    que = sys.argv[1] if len(sys.argv) > 1 else "todo"
    if que in ("usuario", "todo"):
        construir("u", "Manual de usuario", "Cómo ejecutar, recorrer e interactuar con la experiencia", MU, USUARIO,
                  "ManualDeUsuario_%s.pdf" % SUF, "1.0 (18/11/2026)", 2)
    if que in ("tecnico", "todo"):
        construir("t", "Manual técnico", "Arquitectura, investigación, gestión, animaciones, conclusiones y créditos", MT, TECNICO,
                  "ManualTecnico_%s.pdf" % SUF, "1.0 (18/11/2026)", 2)

if __name__ == "__main__":
    main()
