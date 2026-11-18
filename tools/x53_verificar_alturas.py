r"""X-53: verificador de ALTURAS contra el suelo visible (2.a parte de la auditoria del 17/11/2026). Complementa a x48_verificar_solapes.py, que solo mide
huellas en planta y nunca comparaba una y contra el terreno (por eso dejo pasar rocas colgando, casas enterradas, calles levantadas y una franja de pasto con UV rotas).
El suelo es el VISIBLE: playa (A-5) dentro de |x| <= 50, z >= -20; el mar a -0.2 m desde z = 26; la malla de colinas (tools/x53_terreno.py == Terreno::altura).
Reglas:
  R9   plantas, faroles, fogatas, gente, racimos, arcos, tendederos y ofrendas: la base debe quedar a +0.15 m o menos sobre el suelo (flotando) y a no mas de
       0.35 m (palmas, arboles, arbustos) o 0.15 m (lo demas) por debajo (hundido)
  R10  rocas: su base (y + esc * y_min) no puede estar mas de 0.12 m sobre el punto MAS BAJO del suelo bajo su huella (colgando); la parte enterrada del lado de la
       subida no puede pasar de su altura (roca tragada por la ladera: la parte mas alta del suelo bajo su huella queda sobre su cima); toda la huella debe caer sobre la malla (nada de salientes sobre el vacio o el mar)
  R11  casas: la casa no puede quedar enterrada mas de 0.15 m en el punto mas alto de su huella, y si el suelo mas bajo queda mas de 0.30 m bajo su base debe haber un
       cimiento (Models/cimientos/cimientos.obj) que llegue 0.10 m o mas bajo ese punto
  R12  Models/colinas/colinas.obj: UV planas (u = x / 10, v = -z / 10) en todos los vertices, y alturas iguales a las de la malla que usa el codigo (sin costuras ni franjas estiradas)
  R13  Models/calles y Models/sendero: cada vertice entre 0 y 0.12 m sobre el suelo (no levantados ni enterrados)
  R14  borde malla-playa: la malla pegada a la playa (x = +-50, z de -20 a 24) no se aparta mas de 0.06 m de la altura de la playa (sin escalon ni "suelo levantado")
  R15  palapa.obj: la mesa y la banca tienen patas (>= 8 vertices en y = 0 bajo los tablones)
Es SOLO LECTURA. Sale con codigo 1 si hay violaciones. Uso (raiz del repo):   python tools/x53_verificar_alturas.py [--detalle]"""
import math, os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
DETALLE = "--detalle" in sys.argv
_argv = sys.argv
sys.argv = [sys.argv[0]]
import x53_terreno as T
import x48_verificar_solapes as V
sys.argv = _argv
RAIZ = T.RAIZ
CASA_X, CASA_Z = (-2.54, 2.54), (-3.50, 2.04)


def suelo(x, z):
    """Suelo visible; el mar (y = -0.2 desde z = 26, solo en |x| <= 50) tapa la playa que baja de -0.2."""
    s = T.altura(x, z)
    if s is not None and abs(x) <= 50.0 and z >= 26.0:
        return max(s, -0.2)
    return s


def rect(x, z, giro, ax, az, margen=0.0):
    g = math.radians(giro); c, s = math.cos(g), math.sin(g)
    pts = ((ax[0] - margen, az[0] - margen), (ax[1] + margen, az[0] - margen), (ax[1] + margen, az[1] + margen), (ax[0] - margen, az[1] + margen))
    return [(x + c * px + s * pz, z - s * px + c * pz) for px, pz in pts]


def muestras(poly, paso=0.6):
    xs = [p[0] for p in poly]; zs = [p[1] for p in poly]
    out = list(poly); x = min(xs)
    while x <= max(xs):
        z = min(zs)
        while z <= max(zs):
            if V.dentro((x, z), poly): out.append((x, z))
            z += paso
        x += paso
    return out


def extremos(poly, paso=0.6):
    """(min, max, n_sin_suelo) del suelo bajo una huella."""
    ts = [suelo(*p) for p in muestras(poly, paso)]
    ok = [t for t in ts if t is not None]
    return (min(ok), max(ok), len(ts) - len(ok)) if ok else (None, None, len(ts))


_YMIN = {}
def ymin(modelo):
    if modelo not in _YMIN:
        _YMIN[modelo] = min(v[1] for t in V.triangulos(modelo) for v in t)
    return _YMIN[modelo]


def leer_cimientos():
    ruta = os.path.join(RAIZ, "Models", "cimientos", "cimientos.obj")
    if not os.path.exists(ruta): return []
    P = [tuple(map(float, l.split()[1:4])) for l in open(ruta, encoding="utf-8") if l.startswith("v ")]
    bloques = []
    for i in range(0, len(P), 20):           # 5 caras de 4 vertices por casa
        b = P[i:i + 20]
        bloques.append((sum(p[0] for p in b) / len(b), sum(p[2] for p in b) / len(b), min(p[1] for p in b), max(p[1] for p in b)))
    return bloques


def verificar():
    Vi = []
    def anota(r, t): Vi.append((r, t))
    # ---------- R9 puntuales ----------
    def puntual(nombre, origen, i, x, y, z, hund):
        s = suelo(x, z)
        if s is None: return
        if y - s > 0.15: anota("R9", "%s %s#%d (%.2f, %.2f) flota %.2f m sobre el suelo" % (nombre, origen, i, x, z, y - s))
        if s - y > hund: anota("R9", "%s %s#%d (%.2f, %.2f) hundido %.2f m bajo el suelo" % (nombre, origen, i, x, z, s - y))
    for archivo, nombre, hund, cx, cy, cz in (
            ("VegetacionColinas.h", "PALMAS", 0.35, 0, 1, 2), ("VegetacionColinas.h", "ARBUSTOS", 0.35, 0, 1, 2),
            ("VidaColinasDatos.h", "PALMAS", 0.35, 0, 1, 2), ("VidaColinasDatos.h", "ARBUSTOS", 0.35, 0, 1, 2), ("VidaColinasDatos.h", "ARBOLES", 0.35, 0, 1, 2),
            ("VidaColinasDatos.h", "FAROLES", 0.15, 0, 1, 2), ("VidaColinasDatos.h", "RACIMOS", 0.15, 0, 1, 2), ("VidaColinasDatos.h", "ARCOS", 0.15, 0, 1, 2),
            ("VidaColinasDatos.h", "TENDEDEROS", 0.15, 0, 1, 2), ("VidaColinasDatos.h", "OFRENDAS", 0.15, 0, 1, 2), ("VidaColinasDatos.h", "PERSONAS", 0.15, 0, 1, 2)):
        for i, f in enumerate(V.leer_h(archivo, nombre)):
            puntual(nombre.lower(), archivo.split(".")[0], i, f[cx], f[cy], f[cz], hund)
    for i, f in enumerate(V.leer_h("VidaColinasDatos.h", "FOGATAS")):
        puntual("fogata", "VidaColinasDatos", i, f[0], f[1] - 0.08, f[2], 0.15)       # main.cpp la baja 8 cm
    # ---------- R10 rocas ----------
    rocas = [("promontorio", i, r[0], r[1], r[2], r[3], r[4]) for i, r in enumerate(V.leer_h("PuebloFondo.h", "ROCAS"))]
    rocas += [("colinas", i, r[0], r[1], r[2], r[3], r[4]) for i, r in enumerate(V.leer_h("VidaColinasDatos.h", "ROCAS"))]
    for origen, i, x, y, z, giro, esc in rocas:
        poly = V.poligono("roca_mar", x, z, giro, esc)
        paso = 0.6 if esc < 6 else 1.0
        pts = muestras(poly, paso)
        sl = [(p, suelo(*p)) for p in pts]
        sin = sum(1 for _, t in sl if t is None)
        if sin:
            anota("R10", "roca %s#%d (%.1f, %.1f) esc %.1f: %d puntos de su huella caen fuera de la malla (saliente sobre el vacio o el mar)" % (origen, i, x, z, esc, sin)); continue
        base = y + esc * ymin("roca_mar"); alto = esc * (max(v[1] for t in V.triangulos("roca_mar") for v in t) - ymin("roca_mar"))
        gx, gz = T.inclinacion(x, z, esc) if origen == "colinas" else (0.0, 0.0)      # las rocas de las colinas se dibujan inclinadas con la ladera
        plano = lambda p: base + gx * (p[0] - x) + gz * (p[1] - z)
        cuelga = max(plano(p) - t for p, t in sl); enterr = max(t - plano(p) for p, t in sl)
        if cuelga > 0.12: anota("R10", "roca %s#%d (%.1f, %.1f) esc %.1f: su base cuelga %.2f m sobre el suelo bajo su huella" % (origen, i, x, z, esc, cuelga))
        if enterr > 1.0 * alto: anota("R10", "roca %s#%d (%.1f, %.1f) esc %.1f: enterrada %.0f %% de su altura (%.2f de %.2f m)" % (origen, i, x, z, esc, 100 * enterr / alto, enterr, alto))
    # ---------- R11 casas ----------
    cim = leer_cimientos()
    casas = [("fondo", i, c[0], c[1], c[2], c[3], "casa_a") for i, c in enumerate(V.leer_h("PuebloFondo.h", "CASAS"))]
    casas += [("lados", i, c[0], 0.0, c[1], c[2], "casa_a") for i, c in enumerate(V.leer_h("Extras.h", "CASAS"))]
    for origen, i, x, y, z, giro, _ in casas:
        lo, hi, sin = extremos(rect(x, z, giro, CASA_X, CASA_Z, 0.05), 0.5)
        if sin: anota("R11", "casa %s#%d (%.1f, %.1f): %d puntos de su huella caen fuera de la malla" % (origen, i, x, z, sin)); continue
        if hi - y > 0.15: anota("R11", "casa %s#%d (%.1f, %.1f) enterrada %.2f m en el punto mas alto de su huella" % (origen, i, x, z, hi - y))
        if y - lo > 0.30:
            ok = any(math.hypot(b[0] - x, b[1] - z) < 1.0 and b[2] <= lo - 0.10 and b[3] >= y for b in cim)
            if not ok: anota("R11", "casa %s#%d (%.1f, %.1f): el suelo baja %.2f m bajo su base y no tiene cimiento" % (origen, i, x, z, y - lo))
    # ---------- R12 colinas.obj ----------
    ruta = os.path.join(RAIZ, "Models", "colinas", "colinas.obj")
    Vv, Vt = [], []; caras = []
    for l in open(ruta, encoding="utf-8"):
        t = l.split()
        if not t: continue
        if t[0] == "v": Vv.append(tuple(map(float, t[1:4])))
        elif t[0] == "vt": Vt.append(tuple(map(float, t[1:3])))
        elif t[0] == "f": caras.append([tuple(int(q) - 1 for q in a.split("/")[:2]) for a in t[1:]])
    malas = 0; altos = 0; ej = []
    for c in caras:
        for iv, it in c:
            x, y, z = Vv[iv]; u, v = Vt[it]
            if abs(u - x / 10.0) > 1e-3 or abs(v + z / 10.0) > 1e-3:
                malas += 1; ej.append((x, z, u, v)) if len(ej) < 3 else None
            faldon = (49.5 - 1e-6 <= abs(x) <= 50 + 1e-6 or -20 - 1e-6 <= z <= -19.5 + 1e-6) and abs(x) <= 50 + 1e-6 and z >= -20 - 1e-6 and y <= max(T.playa(z), -0.5) - 0.02 + 1e-6   # faldon bajo la playa
            if abs(y - T.h_visible(x, z)) > 0.01 and not (z == T.PLAYA_Z0 and abs(y) < 1e-6) and not faldon:
                altos += 1
    if malas: anota("R12", "colinas.obj: %d vertices de cara con UV que no son planas (p. ej. %s): la textura del pasto se estira en rayas" % (malas, ej))
    if altos: anota("R12", "colinas.obj: %d vertices con altura distinta de la malla que usa el codigo (Terreno::alturaMalla)" % altos)
    # ---------- R13 calles y sendero ----------
    for nombre in ("calles", "sendero"):
        peor = []
        for l in open(os.path.join(RAIZ, "Models", nombre, nombre + ".obj"), encoding="utf-8"):
            if l.startswith("v "):
                t = l.split(); x, y, z = float(t[1]), float(t[2]), float(t[3]); s = suelo(x, z)
                if s is not None and not (-0.005 <= y - s <= 0.12): peor.append((y - s, x, z))
        # centroides de cada cara: una cara grande puede cortar el terreno aunque sus vertices esten bien
        Vt = [tuple(map(float, l.split()[1:4])) for l in open(os.path.join(RAIZ, "Models", nombre, nombre + ".obj"), encoding="utf-8") if l.startswith("v ")]
        for l in open(os.path.join(RAIZ, "Models", nombre, nombre + ".obj"), encoding="utf-8"):
            if l.startswith("f "):
                ix = [Vt[int(w.split("/")[0]) - 1] for w in l.split()[1:]]
                cx = sum(q[0] for q in ix) / len(ix); cz = sum(q[2] for q in ix) / len(ix); cy = sum(q[1] for q in ix) / len(ix)
                s = suelo(cx, cz)
                if s is not None and not (-0.02 <= cy - s <= 0.15): peor.append((cy - s, cx, cz))
        if peor:
            peor.sort()
            anota("R13", "%s.obj: %d vertices/centroides fuera de 0..0.12 m sobre el suelo (de %.2f a %.2f m; p. ej. en %.1f, %.1f)" % (nombre, len(peor), peor[0][0], peor[-1][0], peor[-1][1], peor[-1][2]))
    # ---------- R14 borde malla-playa ----------
    for lado in (-1, 1):
        for k in range(-20, 25):
            m = T.malla(lado * 50.0 + lado * 0.001, float(k)); p = T.playa(float(k))
            if m is not None and abs(m - p) > 0.06:
                anota("R14", "borde de la playa en x=%+d, z=%d: la malla esta a %.2f m y la playa a %.2f m (escalon de %.2f m)" % (lado * 50, k, m, p, abs(m - p)))
    # ---------- R15 patas de la palapa ----------
    P = [tuple(map(float, l.split()[1:4])) for l in open(os.path.join(RAIZ, "Models", "palapa", "palapa.obj"), encoding="utf-8") if l.startswith("v ")]
    patas = [p for p in P if abs(p[1]) < 0.01 and abs(p[0]) < 1.0 and -0.6 < p[2] < 1.1]
    if len(patas) < 8: anota("R15", "palapa.obj: la mesa y la banca no tienen patas (%d vertices en y = 0 bajo los tablones; se esperan >= 8)" % len(patas))
    return Vi


if __name__ == "__main__":
    Vi = verificar()
    cuenta = {}
    for r, t in Vi: cuenta[r] = cuenta.get(r, 0) + 1
    for r in sorted(cuenta): print("  %s: %d" % (r, cuenta[r]))
    if DETALLE or len(Vi) < 60:
        for r, t in Vi: print(r, t)
    print("TOTAL violaciones:", len(Vi))
    sys.exit(1 if Vi else 0)
