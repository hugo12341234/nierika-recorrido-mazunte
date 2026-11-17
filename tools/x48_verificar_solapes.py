r"""X-48: verificador de solapes y cruces entre objetos de la escena (auditoria de integridad del 17/11/2026).
Lee las posiciones REALES que se dibujan (src/Extras.h, src/PuebloFondo.h, src/VegetacionColinas.h, src/VidaColinasDatos.h y las listas de palmas y
arbustos de la playa en src/main.cpp) y las huellas de los modelos (Models/*/*.obj), y busca:
  R1 casa contra casa (huellas giradas se cruzan)           R5 tronco de palma contra una linea de papel picado (tendedero)
  R2 casa contra roca                                        R6 palma contra farol (< 1.5 m entre centros)
  R3 palma / arbusto / farol / fogata / cruz / persona       R7 persona contra farol (< semiancho + 0.25 m)
     dentro de una casa                                      R8 fogata contra cruz u ofrenda (< 1.2 m)
  R4 palma / arbusto dentro de una roca (huella real)
Es SOLO LECTURA. Sale con codigo 1 si hay violaciones (para correrlo antes de cada PR que mueva objetos).
Uso (raiz del repo):   python tools/x48_verificar_solapes.py [--detalle]
Las huellas se toman de la envolvente convexa de los vertices del .obj (en el plano x-z), girada con la misma convencion que glm::rotate(Y):
    x' = x cos(g) + z sen(g);   z' = -x sen(g) + z cos(g)."""
import math, os, re, sys
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
DETALLE = "--detalle" in sys.argv


def leer_h(archivo, nombre):
    """Filas numericas de `inline const T NOMBRE[n] = { {..}, ... };` (un solo bloque)."""
    txt = open(os.path.join(RAIZ, "src", archivo), encoding="utf-8").read()
    m = re.search(r"inline const \w+ %s\[\w+\] = \{(.*?)\};" % nombre, txt, re.S)
    if not m:
        raise SystemExit("no se encontro %s en %s" % (nombre, archivo))
    filas = []
    for fila in re.findall(r"\{([^{}]*)\}", m.group(1)):
        filas.append([float(v) for v in re.findall(r"-?\d+\.?\d*", fila)])
    return filas


def leer_main(nombre):
    txt = open(os.path.join(RAIZ, "src", "main.cpp"), encoding="utf-8").read()
    m = re.search(r"glm::vec3 %s\[\d+\] = \{(.*?)\};" % nombre, txt, re.S)
    return [[float(v) for v in re.findall(r"-?\d+\.?\d*", fila)] for fila in re.findall(r"\{([^{}]*)\}", m.group(1))]


def hull(pts):
    pts = sorted(set(pts))
    if len(pts) < 3:
        return pts
    def cruz(o, a, b): return (a[0] - o[0]) * (b[1] - o[1]) - (a[1] - o[1]) * (b[0] - o[0])
    lo, up = [], []
    for p in pts:
        while len(lo) >= 2 and cruz(lo[-2], lo[-1], p) <= 0: lo.pop()
        lo.append(p)
    for p in reversed(pts):
        while len(up) >= 2 and cruz(up[-2], up[-1], p) <= 0: up.pop()
        up.append(p)
    return lo[:-1] + up[:-1]


_HUELLAS = {}
def huella(modelo):
    """Envolvente convexa (x, z) de Models/<modelo>/<modelo>.obj, en coordenadas locales (escala 1)."""
    if modelo not in _HUELLAS:
        pts = []
        for l in open(os.path.join(RAIZ, "Models", modelo, modelo + ".obj"), encoding="utf-8", errors="ignore"):
            if l.startswith("v "):
                t = l.split(); pts.append((round(float(t[1]), 2), round(float(t[3]), 2)))
        _HUELLAS[modelo] = hull(pts)
    return _HUELLAS[modelo]


def poligono(modelo, x, z, giro, esc=1.0):
    g = math.radians(giro); c, s = math.cos(g), math.sin(g)
    return [(x + esc * (c * px + s * pz), z + esc * (-s * px + c * pz)) for px, pz in huella(modelo)]


def dist_seg(p, a, b):
    ax, az = a; bx, bz = b; px, pz = p
    dx, dz = bx - ax, bz - az
    t = 0.0 if dx == dz == 0 else max(0.0, min(1.0, ((px - ax) * dx + (pz - az) * dz) / (dx * dx + dz * dz)))
    return math.hypot(px - (ax + t * dx), pz - (az + t * dz))


def dentro(p, poly):
    """Punto dentro de un poligono convexo (cualquier orientacion)."""
    s = 0
    for i in range(len(poly)):
        a, b = poly[i], poly[(i + 1) % len(poly)]
        c = (b[0] - a[0]) * (p[1] - a[1]) - (b[1] - a[1]) * (p[0] - a[0])
        if c != 0: s = s or (1 if c > 0 else -1)
        if c * s < 0: return False
    return True


def dist_poly(p, poly):
    """0 si el punto esta dentro; si no, distancia al borde."""
    if dentro(p, poly): return 0.0
    return min(dist_seg(p, poly[i], poly[(i + 1) % len(poly)]) for i in range(len(poly)))


_TRIS = {}
def triangulos(modelo):
    """Triangulos (x, y, z) locales del .obj (caras en abanico)."""
    if modelo not in _TRIS:
        V, T = [], []
        for l in open(os.path.join(RAIZ, "Models", modelo, modelo + ".obj"), encoding="utf-8", errors="ignore"):
            t = l.split()
            if not t: continue
            if t[0] == "v": V.append(tuple(map(float, t[1:4])))
            elif t[0] == "f":
                idx = [int(a.split("/")[0]) - 1 for a in t[1:]]
                for k in range(1, len(idx) - 1): T.append((V[idx[0]], V[idx[k]], V[idx[k + 1]]))
        _TRIS[modelo] = T
    return _TRIS[modelo]


def techo_roca(modelo, rx, rz, giro, esc, oy, px, pz):
    """Altura (mundo) de la superficie mas alta de la roca sobre el punto (px, pz); None si el punto cae fuera de la roca."""
    g = math.radians(giro); c, s = math.cos(g), math.sin(g)
    dx, dz = (px - rx) / esc, (pz - rz) / esc
    lx, lz = c * dx - s * dz, s * dx + c * dz              # inversa de la rotacion de glm::rotate(Y)
    mejor = None
    for (x0, y0, z0), (x1, y1, z1), (x2, y2, z2) in triangulos(modelo):
        d = (z1 - z2) * (x0 - x2) + (x2 - x1) * (z0 - z2)
        if abs(d) < 1e-9: continue
        a = ((z1 - z2) * (lx - x2) + (x2 - x1) * (lz - z2)) / d
        b = ((z2 - z0) * (lx - x2) + (x0 - x2) * (lz - z2)) / d
        if a >= -1e-6 and b >= -1e-6 and 1 - a - b >= -1e-6:
            y = a * y0 + b * y1 + (1 - a - b) * y2
            if mejor is None or y > mejor: mejor = y
    return None if mejor is None else oy + esc * mejor


def se_cruzan(A, B):
    """Dos poligonos convexos se solapan (SAT)."""
    for P in (A, B):
        for i in range(len(P)):
            a, b = P[i], P[(i + 1) % len(P)]
            nx, nz = b[1] - a[1], a[0] - b[0]
            pa = [nx * q[0] + nz * q[1] for q in A]; pb = [nx * q[0] + nz * q[1] for q in B]
            if max(pa) <= min(pb) or max(pb) <= min(pa): return False
    return True


def cargar():
    E = {}
    # casas
    E["casas"] = [("playa", i, poligono("casa_a", c[0], c[1], c[2])) for i, c in enumerate(leer_h("Extras.h", "CASAS"))]
    E["casas"] += [("fondo", i, poligono("casa_a", c[0], c[2], c[3])) for i, c in enumerate(leer_h("PuebloFondo.h", "CASAS"))]
    E["casas"] += [("palapa", i, poligono("palapa", c[0], c[1], c[2], c[3])) for i, c in enumerate(leer_h("Extras.h", "PALAPAS"))]   # R1/R2/R3 tambien las cuentan
    # rocas (las de VidaColinasDatos solo cuentan desde 1.0 m de escala: las chicas estan semienterradas)
    def roca(origen, i, x, z, giro, esc, oy):
        return (origen, i, poligono("roca_mar", x, z, giro, esc), esc, (x, z, giro, oy))
    E["rocas"] = [roca("orilla", i, r[0], r[1], r[2], r[3], 0.0 if r[1] < 23.5 else -0.30) for i, r in enumerate(leer_h("Extras.h", "ROCAS"))]
    E["rocas"] += [roca("promontorio", i, r[0], r[2], r[3], r[4], r[1]) for i, r in enumerate(leer_h("PuebloFondo.h", "ROCAS"))]
    E["rocas"] += [roca("colinas", i, r[0], r[2], r[3], r[4], r[1]) for i, r in enumerate(leer_h("VidaColinasDatos.h", "ROCAS")) if r[4] >= 1.0]
    # plantas: (origen, i, x, z, radio, y_base)
    palmas = [("playa", i, p[0], p[2], 0.3, 0.0) for i, p in enumerate(leer_main("palmas"))]
    palmas += [("lados", i, p[0], p[1], 0.3 * p[3], 0.0) for i, p in enumerate(leer_h("Extras.h", "PALMAS_LADOS"))]
    palmas += [("colinas", i, p[0], p[2], 0.3 * p[4], p[1]) for i, p in enumerate(leer_h("VegetacionColinas.h", "PALMAS"))]
    palmas += [("vida", i, p[0], p[2], 0.3 * p[4], p[1]) for i, p in enumerate(leer_h("VidaColinasDatos.h", "PALMAS"))]
    arbustos = [("playa", i, p[0], p[2], 0.6, 0.0) for i, p in enumerate(leer_main("arbustos"))]
    arbustos += [("colinas", i, p[0], p[2], 0.6 * p[4], p[1]) for i, p in enumerate(leer_h("VegetacionColinas.h", "ARBUSTOS"))]
    arbustos += [("vida", i, p[0], p[2], 0.6 * p[4], p[1]) for i, p in enumerate(leer_h("VidaColinasDatos.h", "ARBUSTOS"))]
    E["palmas"], E["arbustos"] = palmas, arbustos
    faroles = [("playa", i, f[0], f[1]) for i, f in enumerate(leer_h("Extras.h", "FAROLES"))]
    faroles += [("vida", i, f[0], f[2]) for i, f in enumerate(leer_h("VidaColinasDatos.h", "FAROLES"))]
    E["faroles"] = faroles
    E["fogatas"] = [("lados", i, f[0], f[1]) for i, f in enumerate(leer_h("Extras.h", "FOGATAS"))]
    E["fogatas"] += [("vida", i, f[0], f[2]) for i, f in enumerate(leer_h("VidaColinasDatos.h", "FOGATAS"))]
    cruces = [("vida", i, f[0], f[2]) for i, f in enumerate(leer_h("VidaColinasDatos.h", "OFRENDAS"))]
    cruces.append(("capilla", 0, -2.5, 3.7))            # main.cpp: colocar(cruz, (-2.5, 0, 3.7))
    E["cruces"] = cruces
    # personas: (origen, i, x, z, semiancho)
    per = [("figuras", i, f[0], f[1], 0.26 * f[5]) for i, f in enumerate(leer_h("Extras.h", "FIGURAS_N"))]
    per += [("vida", i, p[0], p[2], 0.26 * p[5]) for i, p in enumerate(leer_h("VidaColinasDatos.h", "PERSONAS"))]
    E["personas"] = per
    # tendederos de la playa (segmento z fijo) y los de las colinas se ignoran aqui (van sobre el sendero, sin palmas cerca)
    tend = []
    for t in leer_h("Extras.h", "TENDEDEROS"):
        tend.append(((t[0], t[1]), (t[0] + 1.85 * int(t[2]), t[1])))
    E["tendederos"] = tend
    return E


def rocas_tocan_casa(r, poly):
    """La roca (triangulos reales proyectados) se solapa con la huella de la casa."""
    if not se_cruzan(r[2], poly): return False
    rx, rz, giro, _ = r[4]; g = math.radians(giro); c, s = math.cos(g), math.sin(g); esc = r[3]
    for tri in triangulos("roca_mar"):
        T = [(rx + esc * (c * x + s * z), rz + esc * (-s * x + c * z)) for x, _, z in tri]
        if se_cruzan(T, poly): return True
    return False


def verificar():
    E = cargar(); V = []
    def anota(regla, texto): V.append((regla, texto))
    casas = E["casas"]
    # R1 casa-casa
    for i in range(len(casas)):
        for j in range(i + 1, len(casas)):
            if se_cruzan(casas[i][2], casas[j][2]):
                anota("R1", "casa %s#%d se cruza con casa %s#%d" % (casas[i][0], casas[i][1], casas[j][0], casas[j][1]))
    # R2 casa-roca
    for c in casas:
        for r in E["rocas"]:
            if r[3] >= 1.0 and rocas_tocan_casa(r, c[2]):
                anota("R2", "casa %s#%d se cruza con roca %s#%d (esc %.1f)" % (c[0], c[1], r[0], r[1], r[3]))
    # R3 cosas dentro de una casa (palmas y arbustos: tronco/mata con su radio; los demas: el punto)
    def dentro_casa(nombre, lista, r_extra):
        for it in lista:
            p = (it[2], it[3]); rad = (it[4] if len(it) > 4 else 0.0) + r_extra
            for c in casas:
                if dist_poly(p, c[2]) < rad:
                    anota("R3", "%s %s#%d (%.2f, %.2f) dentro de casa %s#%d" % (nombre, it[0], it[1], it[2], it[3], c[0], c[1]))
    dentro_casa("palma", E["palmas"], 0.0); dentro_casa("arbusto", E["arbustos"], 0.0)
    dentro_casa("farol", E["faroles"], 0.1); dentro_casa("fogata", E["fogatas"], 0.5); dentro_casa("cruz", E["cruces"], 0.3)
    dentro_casa("persona", E["personas"], 0.0)
    # R4 palmas y arbustos dentro de una roca: la base queda MAS BAJA que la superficie de la roca sobre ese punto (enterrada en ella)
    for nombre, lista in (("palma", E["palmas"]), ("arbusto", E["arbustos"])):
        for it in lista:
            for r in E["rocas"]:
                if r[3] < 1.0 or dist_poly((it[2], it[3]), r[2]) > it[4]: continue
                rx, rz, giro, oy = r[4]
                # se prueba el centro y 4 puntos del borde del tronco / mata: basta uno enterrado para que se vea atravesar la roca
                for dx, dz in ((0, 0), (it[4], 0), (-it[4], 0), (0, it[4]), (0, -it[4])):
                    t = techo_roca("roca_mar", rx, rz, giro, r[3], oy, it[2] + dx, it[3] + dz)
                    if t is not None and t > it[5] + 0.3:
                        anota("R4", "%s %s#%d (%.2f, %.2f) dentro de roca %s#%d (esc %.1f; la roca sube a %.1f m sobre ese punto, la base esta a %.1f)" %
                              (nombre, it[0], it[1], it[2], it[3], r[0], r[1], r[3], t, it[5]))
                        break
    # R5 palma contra tendedero
    for it in E["palmas"]:
        for a, b in E["tendederos"]:
            if dist_seg((it[2], it[3]), a, b) < it[4] + 0.35:
                anota("R5", "palma %s#%d (%.2f, %.2f) cruza el tendedero z=%.1f" % (it[0], it[1], it[2], it[3], a[1]))
    # R6 palma-farol, R7 persona-farol
    for f in E["faroles"]:
        for it in E["palmas"]:
            if math.hypot(it[2] - f[2], it[3] - f[3]) < 1.5:
                anota("R6", "palma %s#%d (%.2f, %.2f) pegada al farol %s#%d (%.2f m)" % (it[0], it[1], it[2], it[3], f[0], f[1], math.hypot(it[2] - f[2], it[3] - f[3])))
        for it in E["personas"]:
            if math.hypot(it[2] - f[2], it[3] - f[3]) < it[4] + 0.25:
                anota("R7", "persona %s#%d (%.2f, %.2f) a %.2f m del farol %s#%d" % (it[0], it[1], it[2], it[3], math.hypot(it[2] - f[2], it[3] - f[3]), f[0], f[1]))
    # R8 fogata-cruz
    for g in E["fogatas"]:
        for c in E["cruces"]:
            if math.hypot(g[2] - c[2], g[3] - c[3]) < 1.2:
                anota("R8", "fogata %s#%d (%.2f, %.2f) a %.2f m de la cruz %s#%d" % (g[0], g[1], g[2], g[3], math.hypot(g[2] - c[2], g[3] - c[3]), c[0], c[1]))
    return E, V


if __name__ == "__main__":
    E, V = verificar()
    print("Revisados: %d casas, %d rocas, %d palmas, %d arbustos, %d faroles, %d fogatas, %d cruces, %d personas" % tuple(len(E[k]) for k in
          ("casas", "rocas", "palmas", "arbustos", "faroles", "fogatas", "cruces", "personas")))
    cuenta = {}
    for r, t in V: cuenta[r] = cuenta.get(r, 0) + 1
    for r in sorted(cuenta): print("  %s: %d" % (r, cuenta[r]))
    if DETALLE or len(V) < 60:
        for r, t in V: print(r, t)
    print("TOTAL violaciones:", len(V))
    sys.exit(1 if V else 0)
