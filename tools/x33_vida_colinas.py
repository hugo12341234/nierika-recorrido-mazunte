r"""X-33/X-36: vida en las colinas y en la franja norte de la playa. Genera src/VidaColinasDatos.h con posiciones (todas con la altura del terreno
caminable, misma funcion que src/Terreno.h): 3 senderos de cempasuchil con faroles que suben a las colinas (norte, este y oeste), un campamento
con fogata y gente en cada mirador, una fogata en cada cima, campamentos con fogata y palapa en la franja norte de la playa, luciernagas y mas
palmas y arbustos. TODO es pura posicion: los modelos son los ya existentes (farol, fogata, cempasuchil, palma, arbusto, palapa, personas).
Uso (raiz del repo):   python tools/x33_vida_colinas.py
Determinista (semilla 33). No hace falta Blender."""
import math, os, random, re, sys
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
sys.argv = ["x33", "ninguno"]
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import x18_colinas as c

PENDIENTE_MAX = 0.70
rnd = random.Random(33)


def alt(x, z):
    """Misma funcion que Terreno::altura (playa dentro de |x| <= 50 y z >= -20; colinas en el resto)."""
    if abs(x) <= 50 and z >= -20:
        return 0.0 if z <= 20 else -0.05 * (z - 20)
    return c.alturaColinas(x, z)


def pend(x, z):
    return max(abs(alt(x + .5, z) - alt(x - .5, z)), abs(alt(x, z + .5) - alt(x, z - .5)))


def tramo(S, M, serp=3.0, ciclos=1.5):
    """Polilinea de S a M (puntos cada 1 m) con un serpenteo senoidal que vale 0 en los extremos."""
    L = math.hypot(M[0] - S[0], M[1] - S[1]); n = max(int(L), 2)
    nx, nz = -(M[1] - S[1]) / L, (M[0] - S[0]) / L
    pts = []
    for i in range(n + 1):
        s = i / n
        off = serp * math.sin(2 * math.pi * s * ciclos) * math.sin(math.pi * s)
        pts.append((S[0] + (M[0] - S[0]) * s + nx * off, S[1] + (M[1] - S[1]) * s + nz * off))
    return pts


def pend_max(pts):
    return max(pend(x, z) for x, z in pts)


def lee_pueblo():
    t = open(os.path.join(RAIZ, "src", "PuebloFondo.h"), encoding="utf-8").read()
    return [(float(a), float(b)) for a, _, b in re.findall(r"\{ (-?[\d.]+)f, (-?[\d.]+)f, (-?[\d.]+)f, -?[\d.]+f, \d \}", t)]


def lee_vegetacion():
    t = open(os.path.join(RAIZ, "src", "VegetacionColinas.h"), encoding="utf-8").read()
    return [(float(a), float(b)) for a, _, b in re.findall(r"\{ (-?[\d.]+)f, (-?[\d.]+)f, (-?[\d.]+)f, -?[\d.]+f, -?[\d.]+f \}", t)]


class Malla:
    """Altura EXACTA de Models/colinas/colinas.obj (cuadrilateros triangulados en abanico, como hace Assimp): la malla solo muestrea
    la funcion cada 4 m y entre muestras difiere hasta ~0.26 m. Los objetos generados aqui se apoyan en la malla para no flotar ni hundirse."""
    def __init__(self):
        V, F = [], []
        for l in open(os.path.join(RAIZ, "Models", "colinas", "colinas.obj"), encoding="utf-8"):
            t = l.split()
            if not t: continue
            if t[0] == "v": V.append(tuple(map(float, t[1:4])))
            elif t[0] == "f": F.append([int(a.split("/")[0]) - 1 for a in t[1:]])
        self.celdas = {}
        for f in F:
            cx = sum(V[i][0] for i in f) / len(f); cz = sum(V[i][2] for i in f) / len(f)
            self.celdas[(math.floor((cx + 150) / 4), math.floor((cz + 170) / 4))] = [V[i] for i in f]

    def h(self, x, z):
        f = self.celdas.get((math.floor((x + 150) / 4), math.floor((z + 170) / 4)))
        if f is None: return None
        for tri in ((f[0], f[1], f[2]), (f[0], f[2], f[3])):
            (x0, y0, z0), (x1, y1, z1), (x2, y2, z2) = tri
            d = (z1 - z2) * (x0 - x2) + (x2 - x1) * (z0 - z2)
            if abs(d) < 1e-9: continue
            a = ((z1 - z2) * (x - x2) + (x2 - x1) * (z - z2)) / d
            b = ((z2 - z0) * (x - x2) + (x0 - x2) * (z - z2)) / d
            if a >= -1e-6 and b >= -1e-6 and 1 - a - b >= -1e-6:
                return a * y0 + b * y1 + (1 - a - b) * y2
        return None


MALLA = Malla()


def altm(x, z):
    """Altura donde se APOYAN los objetos: la de la malla en las colinas, la de la playa dentro de su rectangulo."""
    if abs(x) <= 50 and z >= -20: return alt(x, z)
    v = MALLA.h(x, z)
    return alt(x, z) if v is None else v


CASAS = lee_pueblo()
VEG = lee_vegetacion()


def tramo(S, M, serp=3.0, ciclos=1.5):
    L = math.hypot(M[0] - S[0], M[1] - S[1]); n = max(int(L), 2)
    nx, nz = -(M[1] - S[1]) / L, (M[0] - S[0]) / L
    pts = []
    for i in range(n + 1):
        s = i / n
        off = serp * math.sin(2 * math.pi * s * ciclos) * math.sin(math.pi * s)
        pts.append((S[0] + (M[0] - S[0]) * s + nx * off, S[1] + (M[1] - S[1]) * s + nz * off))
    return pts


def pend_max(pts):
    return max(pend(x, z) for x, z in pts)


# --- Sitios (X-33): pendiente <= 0.35 en 4 m a la redonda, altura 3..22 m, a >= 9 m de las casas del pueblo ---
SENDEROS = {
    "N": {"S": (0.0, -21.0),  "M": (0.0, -38.0)},
    "E": {"S": (52.0, -9.0),  "M": (62.0, -32.0)},
    "O": {"S": (-52.0, -9.0), "M": (-62.0, -32.0)},
}


def busca_cima(M, hacia):
    mejor = None
    for k in range(400):
        a = hacia + rnd.uniform(-1.1, 1.1); d = rnd.uniform(30, 70)
        P = (M[0] + d * math.cos(a), M[1] + d * math.sin(a))
        if abs(P[0]) > 125 or P[1] < -150 or P[1] > 30: continue
        H = alt(*P)
        if min(math.hypot(P[0] - a2, P[1] - b2) for a2, b2 in CASAS) < 12: continue
        if pend_max(tramo(M, P, 4.0, 2.0)) > 0.55: continue
        if mejor is None or H > mejor[0]: mejor = (H, P)
    return mejor


res = {k: [] for k in ("faroles", "racimos", "fogatas", "personas", "luciernagas", "palmas", "arbustos", "palapas", "arcos", "tendederos",
                       "ofrendas", "rocas", "arboles")}
RUTA = []
SEND_PTS = []           # (polilinea completa, plaza_M, plaza_P)


def tang(pts, i):
    a, b = pts[max(i - 1, 0)], pts[min(i + 1, len(pts) - 1)]
    L = math.hypot(b[0] - a[0], b[1] - a[1]) or 1.0
    return ((b[0] - a[0]) / L, (b[1] - a[1]) / L)


def a_lo_largo(pts, cada, f, desfase=0.0):
    acum = desfase
    for i in range(1, len(pts)):
        acum += math.hypot(pts[i][0] - pts[i - 1][0], pts[i][1] - pts[i - 1][1])
        if acum >= cada:
            acum = 0.0
            f(i)


def campamento(cx, cz, n_sentados, n_de_pie, radio=1.7, ang0=0.0, velas=0.5):
    res["fogatas"].append((cx, altm(cx, cz), cz))
    for i in range(n_sentados):
        a = ang0 + 2 * math.pi * (i + 0.3 * rnd.random()) / n_sentados
        fx, fz = cx + radio * math.cos(a), cz + radio * math.sin(a)
        res["personas"].append((fx, altm(fx, fz), fz, math.degrees(math.atan2(-(cx - fx), -(cz - fz))), 2 + (i % 2),
                                0.62 if i == n_sentados - 1 and n_sentados > 3 else 1.0, rnd.uniform(0, 6.28), 0))
    for i in range(n_de_pie):
        a = ang0 + 2 * math.pi * (i + 0.5) / max(n_de_pie, 1) + 0.6
        fx, fz = cx + (radio + 1.1) * math.cos(a), cz + (radio + 1.1) * math.sin(a)
        res["personas"].append((fx, altm(fx, fz), fz, math.degrees(math.atan2(-(cx - fx), -(cz - fz))), i % 2, 1.0, rnd.uniform(0, 6.28),
                                1 if rnd.random() < velas else 0))


for k, d in SENDEROS.items():
    S, M = d["S"], d["M"]
    t1 = tramo(S, M, 3.0, 1.5)
    assert pend_max(t1) <= 0.45, (k, pend_max(t1))
    H, P = busca_cima(M, math.atan2(M[1] - S[1], M[0] - S[0]))
    t2 = tramo(M, P, 4.0, 2.0)
    print("sendero", k, "mirador", M, "cima", tuple(round(v, 1) for v in P), "h=%.1f" % H, "pend max %.2f / %.2f" % (pend_max(t1), pend_max(t2)))
    RUTA += t1 + t2
    SEND_PTS.append((t1 + t2[1:], M, P))
    # racimos de cempasuchil en las dos orillas del camino (cada 1.6 m abajo, 2.4 m arriba)
    for pts, cada in ((t1, 1.6), (t2, 2.4)):
        lado = [1]
        def racimo(i, pts=pts, lado=lado):
            tx, tz = tang(pts, i); nx, nz = -tz, tx
            o = lado[0] * rnd.uniform(0.95, 1.30); lado[0] = -lado[0]
            x, z = pts[i][0] + nx * o, pts[i][1] + nz * o
            res["racimos"].append((x, altm(x, z) + 0.01, z, rnd.uniform(0, 360)))
        a_lo_largo(pts, cada, racimo)
    # faroles cada 3.5 m (abajo) / 5 m (arriba), a 1.7 m del eje, alternando lados
    for pts, cada, l0 in ((t1, 3.5, 1), (t2, 5.0, -1)):
        lado = [l0]
        def farol(i, pts=pts, lado=lado):
            tx, tz = tang(pts, i); nx, nz = -tz, tx
            x, z = pts[i][0] + nx * 1.7 * lado[0], pts[i][1] + nz * 1.7 * lado[0]; lado[0] = -lado[0]
            res["faroles"].append((x, altm(x, z), z, rnd.choice((0, 90, 180, 270))))
        a_lo_largo(pts, cada, farol, cada * 0.5)
    # arcos de cempasuchil: al inicio del camino, en el mirador y a mitad de la subida (el arco se cruza en la direccion del camino)
    for pts, i in ((t1, 2), (t1, len(t1) - 3), (t2, len(t2) // 2)):
        tx, tz = tang(pts, i)
        res["arcos"].append((pts[i][0], altm(*pts[i]), pts[i][1], math.degrees(math.atan2(tx, tz))))
    # tendederos de papel picado cruzando el camino (se busca un punto con poca pendiente de lado a lado)
    for pts, frac in ((t1, 0.30), (t1, 0.66), (t2, 0.45)):
        base = int(len(pts) * frac)
        for off in sorted(range(-8, 9), key=abs):
            i = min(max(base + off, 1), len(pts) - 2); tx, tz = tang(pts, i); nx, nz = -tz, tx
            cx, cz = pts[i]
            ya, yb = altm(cx + nx * 0.925, cz + nz * 0.925), altm(cx - nx * 0.925, cz - nz * 0.925)
            if abs(ya - yb) < 0.30: break
        res["tendederos"].append((cx, min(ya, yb) - 0.05, cz, math.degrees(math.atan2(-nz, nx))))
    # ofrenda con cruz en el mirador, un poco mas arriba del camino
    ux, uz = tang(t2, 6)
    ox, oz = M[0] + ux * 4.5, M[1] + uz * 4.5
    res["ofrendas"].append((ox, altm(ox, oz), oz, math.degrees(math.atan2(-ux, -uz))))
    # campamento del mirador y gente
    campamento(M[0] + (3.5 if k != "O" else -3.5), M[1] - 3.0, 4, 2)
    res["fogatas"].append((P[0], altm(*P), P[1]))
    for dx in (-1.2, 1.3):
        res["personas"].append((P[0] + dx, altm(P[0] + dx, P[1] + 1.4), P[1] + 1.4, 180.0, rnd.choice((0, 1)), 1.0, rnd.uniform(0, 6.28), 1))
    # procesion con velas: 8 personas en el tramo bajo y 6 en el alto, a 1.15 m del eje (el camino queda libre), mirando hacia donde sube
    for pts, cada, n in ((t1, 2.9, 8), (t2, 8.0, 6)):
        cnt = [0]; lado = [1]
        def persona(i, pts=pts, cnt=cnt, lado=lado, n=n):
            if cnt[0] >= n: return
            cnt[0] += 1
            tx, tz = tang(pts, i); nx, nz = -tz, tx
            x, z = pts[i][0] + nx * 1.15 * lado[0], pts[i][1] + nz * 1.15 * lado[0]; lado[0] = -lado[0]
            res["personas"].append((x, altm(x, z), z, math.degrees(math.atan2(-tx, -tz)), rnd.choice((0, 1)), 0.62 if cnt[0] % 4 == 0 else 1.0, rnd.uniform(0, 6.28), 1))
        a_lo_largo(pts, cada, persona, cada * 0.6)

# --- franja norte de la playa (z -19..-4): campamentos con fogata, palapas, faroles ---
for (cx, cz, ns, np_) in ((-16, -9, 4, 2), (18, -11, 4, 1), (-34, -14, 3, 2), (34, -12, 3, 1)):
    campamento(cx, cz, ns, np_, 1.6, rnd.uniform(0, 6))
res["palapas"] = [(-25.0, -16.0, 20.0), (26.0, -17.0, -15.0)]
for x in range(-36, 37, 9):
    if abs(x) < 4: continue
    zf = -5.5 - 1.5 * math.sin(x * 0.3)
    res["faroles"].append((float(x), alt(x, zf), zf, rnd.choice((0, 90, 180, 270))))

# --- luciernagas (centros) ---
for i in range(0, len(RUTA), 4):
    x, z = RUTA[i]
    cx, cz = x + rnd.uniform(-3.5, 3.5), z + rnd.uniform(-3.5, 3.5)
    res["luciernagas"].append((cx, altm(cx, cz) + rnd.uniform(0.7, 2.0), cz, rnd.uniform(0, 6.2832)))
for _ in range(30):
    cx, cz = rnd.uniform(-38, 38), rnd.uniform(-19, -3)
    res["luciernagas"].append((cx, alt(cx, cz) + rnd.uniform(0.6, 1.8), cz, rnd.uniform(0, 6.2832)))
for _ in range(30):
    cx, cz = rnd.uniform(-110, 110), rnd.uniform(-90, -22)
    h = alt(cx, cz)
    if 1.0 < h < 22 and not (abs(cx) < 52 and cz > -20):
        res["luciernagas"].append((cx, altm(cx, cz) + rnd.uniform(0.8, 2.2), cz, rnd.uniform(0, 6.2832)))

# --- vegetacion, rocas y arboles (sin tapar caminos, fogatas ni casas) ---
ocupado = [(x, z, 3.0) for x, z in RUTA] + [(f[0], f[2], 5.0) for f in res["fogatas"]] + [(x, z, 5.5) for x, z in CASAS] + [(x, z, 1.5) for x, z in VEG]
libre = lambda x, z, ex=0.0: all(math.hypot(x - a, z - b) > r + ex for a, b, r in ocupado)
def region(x, z): return not (abs(x) < 52 and z > -22)

intentos = 0
while (len(res["palmas"]) < 45 or len(res["arbustos"]) < 90) and intentos < 60000:
    intentos += 1
    x, z = rnd.uniform(-135, 135), rnd.uniform(-125, 38)
    h = alt(x, z)
    if not region(x, z) or h < 0.8 or h > 24 or pend(x, z) > 0.8 or not libre(x, z): continue
    reg = (x, altm(x, z), z, rnd.uniform(0, 360), rnd.uniform(0.8, 1.5))
    if rnd.random() < 0.33 and len(res["palmas"]) < 45: res["palmas"].append(reg); ocupado.append((x, z, 2.0))
    elif len(res["arbustos"]) < 90: res["arbustos"].append(reg); ocupado.append((x, z, 1.5))

intentos = 0
while len(res["arboles"]) < 90 and intentos < 80000:       # 40 junto a los caminos (4..14 m) y el resto repartidos
    intentos += 1
    if len(res["arboles"]) < 40:
        bx, bz = rnd.choice(RUTA); a = rnd.uniform(0, 6.2832); d = rnd.uniform(4.5, 14)
        x, z = bx + d * math.cos(a), bz + d * math.sin(a)
    else:
        x, z = rnd.uniform(-135, 135), rnd.uniform(-125, 38)
    h = alt(x, z)
    if abs(x) > 140 or not region(x, z) or h < 0.8 or h > 23 or pend(x, z) > 0.6 or not libre(x, z, 1.5): continue
    res["arboles"].append((x, altm(x, z) - 0.08, z, rnd.uniform(0, 360), rnd.uniform(0.85, 1.5))); ocupado.append((x, z, 3.0))

intentos = 0
while len(res["rocas"]) < 70 and intentos < 80000:
    intentos += 1
    if len(res["rocas"]) < 40:
        bx, bz = rnd.choice(RUTA); a = rnd.uniform(0, 6.2832); d = rnd.uniform(2.6, 9)
        x, z = bx + d * math.cos(a), bz + d * math.sin(a)
    else:
        x, z = rnd.uniform(-135, 135), rnd.uniform(-125, 38)
    h = alt(x, z)
    if abs(x) > 140 or not region(x, z) or h < 0.5 or h > 26 or pend(x, z) > 0.9 or not libre(x, z, -1.5): continue
    esc = rnd.uniform(0.6, 2.4)
    res["rocas"].append((x, altm(x, z) - 0.12 * esc, z, rnd.uniform(0, 360), esc)); ocupado.append((x, z, 1.0 * esc))

# --- malla del sendero de tierra (sigue la malla de las colinas; 1.7 m de ancho + plazas redondas en el mirador y la cima) ---
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import x36_camino_animas as x36
par = x36.Parte("tierra_sendero", "tierra_sendero.png")
def y_s(x, z): return altm(x, z) + 0.035
for pts, M, P in SEND_PTS:
    izq, der, acum = [], [], 0.0
    for i, (x, z) in enumerate(pts):
        if i: acum += math.hypot(x - pts[i - 1][0], z - pts[i - 1][1])
        tx, tz = tang(pts, i); nx, nz = -tz, tx
        xa, za, xb, zb = x + nx * 0.85, z + nz * 0.85, x - nx * 0.85, z - nz * 0.85
        izq.append(par.vert((xa, y_s(xa, za), za), (0.0, acum / 4.0), (0, 1, 0)))
        der.append(par.vert((xb, y_s(xb, zb), zb), (1.0, acum / 4.0), (0, 1, 0)))
    for i in range(len(pts) - 1):
        par.cara(izq[i], der[i], der[i + 1], izq[i + 1])
    for (cx, cz), rad in ((M, 3.2), (P, 2.5)):                      # plaza redonda de tierra (alfa = 1 en el centro de la textura)
        c = par.vert((cx, y_s(cx, cz), cz), (0.5, 0.0), (0, 1, 0)); borde = []
        for k in range(20):
            a = 2 * math.pi * k / 20; x, z = cx + rad * math.cos(a), cz + rad * math.sin(a)
            borde.append(par.vert((x, y_s(x, z), z), (0.5, rad * 0.5 * (1 + math.sin(a)) / 4.0), (0, 1, 0)))
        for k in range(20):
            par.cara(c, borde[k], borde[(k + 1) % 20])
x36.escribe("sendero", "sendero", [par])

def arr(nombre, tipo, lst, fmt):
    return "    inline const %s %s[%d] = {\n%s\n    };\n    constexpr int N_%s = %d;\n" % (tipo, nombre, len(lst), ",\n".join("        { " + fmt % tuple(r) + " }" for r in lst), nombre, len(lst))

txt = ("#ifndef VIDA_COLINAS_DATOS_H\n#define VIDA_COLINAS_DATOS_H\n"
       "// GENERADO por tools/x33_vida_colinas.py (no editar a mano). X-33/X-36: senderos, faroles, arcos, tendederos, ofrendas, fogatas, gente, luciernagas,\n"
       "// rocas, arboles y vegetacion extra sobre las colinas y la franja norte de la playa. y = altura de la MALLA de colinas (no de la funcion).\n"
       "namespace VidaColinasDatos\n{\n"
       "    struct Punto { float x, y, z, giro; };                          // farol / racimo / arco / tendedero / ofrenda\n"
       "    struct Fuego { float x, y, z; };                                // fogata\n"
       "    struct Persona { float x, y, z, giro; int tipo; float esc, fase; int vela; };   // tipo: 0 pie_a, 1 pie_b, 2 agachada_a, 3 agachada_b; vela: 1 lleva una vela\n"
       "    struct Lucier { float cx, cy, cz, fase; };\n"
       "    struct Planta { float x, y, z, giro, esc; };                    // palma / arbusto / arbol / roca\n"
       "    struct Techo { float x, z, giro; };                             // palapa de la franja norte\n"
       + arr("FAROLES", "Punto", res["faroles"], "%.2ff, %.2ff, %.2ff, %.1ff")
       + arr("RACIMOS", "Punto", res["racimos"], "%.2ff, %.3ff, %.2ff, %.1ff")
       + arr("ARCOS", "Punto", res["arcos"], "%.2ff, %.3ff, %.2ff, %.1ff")
       + arr("TENDEDEROS", "Punto", res["tendederos"], "%.2ff, %.3ff, %.2ff, %.1ff")
       + arr("OFRENDAS", "Punto", res["ofrendas"], "%.2ff, %.3ff, %.2ff, %.1ff")
       + arr("FOGATAS", "Fuego", res["fogatas"], "%.2ff, %.3ff, %.2ff")
       + arr("PERSONAS", "Persona", res["personas"], "%.2ff, %.3ff, %.2ff, %.1ff, %d, %.2ff, %.2ff, %d")
       + arr("LUCIERNAGAS", "Lucier", res["luciernagas"], "%.2ff, %.2ff, %.2ff, %.3ff")
       + arr("PALMAS", "Planta", res["palmas"], "%.2ff, %.3ff, %.2ff, %.1ff, %.2ff")
       + arr("ARBUSTOS", "Planta", res["arbustos"], "%.2ff, %.3ff, %.2ff, %.1ff, %.2ff")
       + arr("ARBOLES", "Planta", res["arboles"], "%.2ff, %.3ff, %.2ff, %.1ff, %.2ff")
       + arr("ROCAS", "Planta", res["rocas"], "%.2ff, %.3ff, %.2ff, %.1ff, %.2ff")
       + arr("PALAPAS", "Techo", res["palapas"], "%.1ff, %.1ff, %.1ff")
       + "}\n#endif\n")
open(os.path.join(RAIZ, "src", "VidaColinasDatos.h"), "w", encoding="utf-8", newline="\n").write(txt)
print({k: len(v) for k, v in res.items()})
