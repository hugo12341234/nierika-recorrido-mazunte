r"""X-33: vida en las colinas y en la franja norte de la playa. Genera src/VidaColinasDatos.h con posiciones (todas con la altura del terreno
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
    casas = [(float(a), float(b)) for a, _, b in re.findall(r"\{ (-?[\d.]+)f, (-?[\d.]+)f, (-?[\d.]+)f, -?[\d.]+f, \d \}", t)]
    return casas


def lee_vegetacion():
    t = open(os.path.join(RAIZ, "src", "VegetacionColinas.h"), encoding="utf-8").read()
    return [(float(a), float(b)) for a, _, b in re.findall(r"\{ (-?[\d.]+)f, (-?[\d.]+)f, (-?[\d.]+)f, -?[\d.]+f, -?[\d.]+f \}", t)]


CASAS = lee_pueblo()
VEG = lee_vegetacion()

# --- Sitios (elegidos por busqueda: pendiente <= 0.35 en 4 m a la redonda, altura 3..22 m, a >= 9 m de las casas del pueblo) ---
SENDEROS = {
    "N": {"S": (0.0, -21.0),  "M": (0.0, -38.0)},
    "E": {"S": (52.0, -9.0),  "M": (62.0, -32.0)},
    "O": {"S": (-52.0, -9.0), "M": (-62.0, -32.0)},
}
# Cima de cada sendero: el punto mas alto a 30..70 m del mirador cuyo tramo tenga pendiente <= 0.55
def busca_cima(M, hacia):
    mejor = None
    for k in range(400):
        a = hacia + rnd.uniform(-1.1, 1.1); d = rnd.uniform(30, 70)
        P = (M[0] + d * math.cos(a), M[1] + d * math.sin(a))
        if abs(P[0]) > 125 or P[1] < -150 or P[1] > 30: continue
        H = alt(*P)
        if min(math.hypot(P[0] - a2, P[1] - b2) for a2, b2 in CASAS) < 12: continue
        pts = tramo(M, P, 4.0, 2.0)
        if pend_max(pts) > 0.55: continue
        if mejor is None or H > mejor[0]: mejor = (H, P)
    return mejor

res = {"faroles": [], "flores": [], "fogatas": [], "figuras": [], "luciernagas": [], "palmas": [], "arbustos": [], "palapas": [], "sendero_pts": []}
RUTA = []   # todos los puntos de los senderos (para no plantar vegetacion encima)

def faroles_en(pts, cada, lado0=1):
    acum, lado = cada * 0.5, lado0
    for (x0, z0), (x1, z1) in zip(pts, pts[1:]):
        acum += math.hypot(x1 - x0, z1 - z0)
        if acum >= cada:
            acum = 0.0
            dx, dz = x1 - x0, z1 - z0; L = math.hypot(dx, dz)
            nx, nz = -dz / L, dx / L
            fx, fz = x1 + nx * 1.4 * lado, z1 + nz * 1.4 * lado
            res["faroles"].append((fx, alt(fx, fz), fz, rnd.choice((0, 90, 180, 270))))
            lado = -lado

def flores_en(pts, cada):
    acum = 0.0
    for (x0, z0), (x1, z1) in zip(pts, pts[1:]):
        acum += math.hypot(x1 - x0, z1 - z0)
        if acum >= cada:
            acum = 0.0
            dx, dz = x1 - x0, z1 - z0; L = math.hypot(dx, dz)
            nx, nz = -dz / L, dx / L
            o = rnd.uniform(-0.4, 0.4)
            fx, fz = x1 + nx * o, z1 + nz * o
            res["flores"].append((fx, alt(fx, fz) + 0.035, fz, rnd.uniform(0, 360)))

def campamento(cx, cz, n_sentados, n_de_pie, radio=1.7, fogata=True, ang0=0.0):
    """Fogata al centro, n_sentados (de rodillas) en circulo mirando al fuego y n_de_pie un poco mas lejos."""
    if fogata:
        res["fogatas"].append((cx, alt(cx, cz), cz))
    for i in range(n_sentados):
        a = ang0 + 2 * math.pi * (i + 0.3 * rnd.random()) / n_sentados
        fx, fz = cx + radio * math.cos(a), cz + radio * math.sin(a)
        giro = math.degrees(math.atan2(-(cx - fx), -(cz - fz)))
        res["figuras"].append((fx, alt(fx, fz), fz, giro, 2 + (i % 2), 0.62 if i == n_sentados - 1 and n_sentados > 3 else 1.0, rnd.uniform(0, 6.28)))
    for i in range(n_de_pie):
        a = ang0 + 2 * math.pi * (i + 0.5) / max(n_de_pie, 1) + 0.6
        fx, fz = cx + (radio + 1.1) * math.cos(a), cz + (radio + 1.1) * math.sin(a)
        giro = math.degrees(math.atan2(-(cx - fx), -(cz - fz)))
        res["figuras"].append((fx, alt(fx, fz), fz, giro, i % 2, 1.0, rnd.uniform(0, 6.28)))

# --- senderos, miradores y cimas ---
for k, d in SENDEROS.items():
    S, M = d["S"], d["M"]
    t1 = tramo(S, M, 3.0, 1.5)
    assert pend_max(t1) <= 0.45, (k, pend_max(t1))
    hacia = math.atan2(M[1] - S[1], M[0] - S[0])
    H, P = busca_cima(M, hacia)
    t2 = tramo(M, P, 4.0, 2.0)
    d["P"], d["H"] = P, H
    print("sendero", k, "mirador", M, "h=%.1f" % alt(*M), "cima", tuple(round(v, 1) for v in P), "h=%.1f" % H, "pend max %.2f / %.2f" % (pend_max(t1), pend_max(t2)))
    RUTA += t1 + t2
    flores_en(t1, 1.0); flores_en(t2, 2.2)
    faroles_en(t1, 6.0, 1); faroles_en(t2, 9.0, -1)
    # campamento del mirador y mirada al mar desde la cima
    campamento(M[0] + (3.0 if k != "O" else -3.0), M[1] - 3.0, 4, 2)
    # cima: fogata con dos personas de pie, mirando hacia el mar (+z)
    res["fogatas"].append((P[0], alt(*P), P[1]))
    for dx in (-1.2, 1.3):
        res["figuras"].append((P[0] + dx, alt(P[0] + dx, P[1] + 1.4), P[1] + 1.4, 180.0, rnd.choice((0, 1)), 1.0, rnd.uniform(0, 6.28)))
    # caminantes en el tramo bajo (de pie, a un lado del camino)
    for idx in (len(t1) // 3, 2 * len(t1) // 3):
        x, z = t1[idx]
        x2 = x + 1.0
        res["figuras"].append((x2, alt(x2, z), z, rnd.uniform(0, 360), rnd.choice((0, 1)), rnd.choice((1.0, 0.62)), rnd.uniform(0, 6.28)))

# --- franja norte de la playa (z -19..-4): campamentos con fogata, palapas, faroles ---
for (cx, cz, ns, np_) in ((-16, -9, 4, 2), (18, -11, 4, 1), (-34, -14, 3, 2), (34, -12, 3, 1)):
    campamento(cx, cz, ns, np_, 1.6, True, rnd.uniform(0, 6))
res["palapas"] = [(-25.0, -16.0, 20.0), (26.0, -17.0, -15.0)]
for x in range(-36, 37, 9):
    if abs(x) < 4: continue
    zf = -5.5 - 1.5 * math.sin(x * 0.3)
    res["faroles"].append((float(x), alt(x, zf), zf, rnd.choice((0, 90, 180, 270))))

# --- luciernagas (centros): a lo largo de los senderos y en la franja norte ---
for i in range(0, len(RUTA), 6):
    x, z = RUTA[i]
    cx, cz = x + rnd.uniform(-3, 3), z + rnd.uniform(-3, 3)
    res["luciernagas"].append((cx, alt(cx, cz) + rnd.uniform(0.7, 2.0), cz, rnd.uniform(0, 6.2832)))
for _ in range(24):
    cx, cz = rnd.uniform(-38, 38), rnd.uniform(-19, -3)
    res["luciernagas"].append((cx, alt(cx, cz) + rnd.uniform(0.6, 1.8), cz, rnd.uniform(0, 6.2832)))
for _ in range(22):
    cx, cz = rnd.uniform(-110, 110), rnd.uniform(-90, -22)
    h = alt(cx, cz)
    if 1.0 < h < 22 and not (abs(cx) < 52 and cz > -20):
        res["luciernagas"].append((cx, h + rnd.uniform(0.8, 2.2), cz, rnd.uniform(0, 6.2832)))

# --- mas vegetacion en las laderas (misma regla que x18_colinas.py y sin tapar senderos, fogatas ni casas) ---
ocupado = [(x, z, 3.0) for x, z in RUTA] + [(f[0], f[2], 5.0) for f in res["fogatas"]] + [(x, z, 5.5) for x, z in CASAS] + [(x, z, 1.5) for x, z in VEG]
def libre(x, z):
    return all(math.hypot(x - a, z - b) > r for a, b, r in ocupado)
intentos = 0
while (len(res["palmas"]) < 45 or len(res["arbustos"]) < 90) and intentos < 60000:
    intentos += 1
    x, z = rnd.uniform(-135, 135), rnd.uniform(-125, 38)
    if abs(x) < 52 and z > -22: continue
    h = alt(x, z)
    if h < 0.8 or h > 24 or pend(x, z) > 0.8 or not libre(x, z): continue
    reg = (x, h, z, rnd.uniform(0, 360), rnd.uniform(0.8, 1.5))
    if rnd.random() < 0.33 and len(res["palmas"]) < 45:
        res["palmas"].append(reg); ocupado.append((x, z, 2.0))
    elif len(res["arbustos"]) < 90:
        res["arbustos"].append(reg); ocupado.append((x, z, 1.5))

def arr(nombre, tipo, lst, fmt):
    return "    inline const %s %s[%d] = {\n%s\n    };\n    constexpr int N_%s = %d;\n" % (tipo, nombre, len(lst), ",\n".join("        { " + fmt % tuple(r) + " }" for r in lst), nombre, len(lst))

txt = ("#ifndef VIDA_COLINAS_DATOS_H\n#define VIDA_COLINAS_DATOS_H\n"
       "// GENERADO por tools/x33_vida_colinas.py (no editar a mano). X-33: senderos, faroles, fogatas, gente, luciernagas y vegetacion extra sobre las colinas y la\n"
       "// franja norte de la playa. y = altura del terreno caminable (src/Terreno.h). Los giros estan en grados alrededor de Y.\n"
       "namespace VidaColinasDatos\n{\n"
       "    struct Punto { float x, y, z, giro; };                          // farol / flor\n"
       "    struct Fuego { float x, y, z; };                                // fogata\n"
       "    struct Persona { float x, y, z, giro; int tipo; float esc, fase; };   // tipo: 0 pie_a, 1 pie_b, 2 agachada_a, 3 agachada_b\n"
       "    struct Lucier { float cx, cy, cz, fase; };\n"
       "    struct Planta { float x, y, z, giro, esc; };\n"
       "    struct Techo { float x, z, giro; };                             // palapa de la franja norte\n"
       + arr("FAROLES", "Punto", res["faroles"], "%.2ff, %.2ff, %.2ff, %.1ff")
       + arr("FLORES", "Punto", res["flores"], "%.2ff, %.3ff, %.2ff, %.1ff")
       + arr("FOGATAS", "Fuego", res["fogatas"], "%.2ff, %.2ff, %.2ff")
       + arr("PERSONAS", "Persona", res["figuras"], "%.2ff, %.2ff, %.2ff, %.1ff, %d, %.2ff, %.2ff")
       + arr("LUCIERNAGAS", "Lucier", res["luciernagas"], "%.2ff, %.2ff, %.2ff, %.3ff")
       + arr("PALMAS", "Planta", res["palmas"], "%.2ff, %.2ff, %.2ff, %.1ff, %.2ff")
       + arr("ARBUSTOS", "Planta", res["arbustos"], "%.2ff, %.2ff, %.2ff, %.1ff, %.2ff")
       + arr("PALAPAS", "Techo", res["palapas"], "%.1ff, %.1ff, %.1ff")
       + "}\n#endif\n")
open(os.path.join(RAIZ, "src", "VidaColinasDatos.h"), "w", encoding="utf-8", newline="\n").write(txt)
print({k: len(v) for k, v in res.items()})
