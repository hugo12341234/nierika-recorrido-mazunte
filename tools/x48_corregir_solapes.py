r"""X-48: correccion de los solapes que encontro la auditoria de integridad del 17/11/2026 (ver docs/ESTADO_PROYECTO.md, decision del 17/11).
Es IDEMPOTENTE: se puede correr cuantas veces se quiera; si ya esta corregido no cambia nada. Los generadores (x18_colinas.py "vegetacion",
x30_pueblo_fondo.py "cabecera" y x33_vida_colinas.py) la llaman al terminar, asi que sus salidas ya salen sin solapes. Despues de correrla:
    python tools/x48_verificar_solapes.py     (debe decir TOTAL violaciones: 0)

REGLAS DE EXCLUSION (genericas, se aplican a lo que haya en los encabezados):
  G1  PuebloFondo.h: una casa que se cruza con otra YA aceptada se descarta (se conserva la primera de cada par).
  G2  VegetacionColinas.h y VidaColinasDatos.h: una palma o un arbusto se descarta si su base queda ENTERRADA en una roca (la superficie de la roca
      sobre ese punto esta mas de 0.3 m sobre su base) o si su tronco / mata toca la huella de una casa.
CORRECCIONES PUNTUALES (coordenadas exactas de los datos escritos a mano o ya generados; si el dato ya no existe, no hacen nada):
  S1  main.cpp: 3 palmas de la playa (una cruzaba el tendedero de papel picado, dos quedaban pegadas a un farol).
  S2  Extras.h: 4 casas de los lados se apartan de las rocas de la orilla y de las palapas; 3 palmas de los lados salen de las rocas y de una casa;
      un musico se separa del farol de la esquina del panteon.
  S3  VidaColinasDatos.h: una persona se separa de un farol y la ofrenda con cruz del mirador este se aparta de la fogata del campamento."""
import math, os, re, sys
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
_argv = sys.argv
sys.argv = [sys.argv[0]]
import x48_verificar_solapes as V
sys.argv = _argv


# ---------- utilidades de archivo (los .h y main.cpp usan CRLF) ----------
def leer(ruta):
    with open(os.path.join(RAIZ, ruta), encoding="utf-8", newline="") as f:
        crudo = f.read()
    return crudo.replace("\r\n", "\n"), ("\r\n" if "\r\n" in crudo else "\n")


def escribir(ruta, txt, eol):
    with open(os.path.join(RAIZ, ruta), "w", encoding="utf-8", newline="") as f:
        f.write(txt.replace("\n", eol))


def filas_de(txt, nombre):
    """(match, [lineas de datos]) de un arreglo `inline const T NOMBRE[n] = {` con una fila por linea."""
    m = re.search(r"    inline const (\w+) %s\[\d+\] = \{\n(.*?)\n    \};\n" % nombre, txt, re.S)
    return m, m.group(2).split("\n")


def reescribe(txt, nombre, lineas, tipo=None):
    """Sustituye las filas del arreglo y actualiza su tamano y la constante N_<nombre> (si existe)."""
    m, _ = filas_de(txt, nombre)
    nuevo = "    inline const %s %s[%d] = {\n%s\n    };\n" % (m.group(1), nombre, len(lineas), "\n".join(lineas))
    txt = txt[:m.start()] + nuevo + txt[m.end():]
    return re.sub(r"constexpr int N_%s = \d+;" % nombre, "constexpr int N_%s = %d;" % (nombre, len(lineas)), txt)


def nums(linea):
    return [float(v) for v in re.findall(r"-?\d+\.?\d*", linea)]


def cambia(txt, viejo, nuevo, ruta):
    """Sustituye una cadena exacta (una sola vez). Devuelve (txt, n_cambios): 0 si ya estaba corregido."""
    if txt.count(viejo) == 0:
        return txt, 0
    if txt.count(viejo) != 1:
        raise SystemExit("%s: la cadena aparece %d veces, hay que revisarla a mano: %s" % (ruta, txt.count(viejo), viejo))
    return txt.replace(viejo, nuevo), 1


# ---------- altura de la malla de colinas (misma que usa x33 para apoyar a la gente) ----------
class Malla:
    def __init__(self):
        Vv, F = [], []
        for l in open(os.path.join(RAIZ, "Models", "colinas", "colinas.obj"), encoding="utf-8"):
            t = l.split()
            if not t: continue
            if t[0] == "v": Vv.append(tuple(map(float, t[1:4])))
            elif t[0] == "f": F.append([int(a.split("/")[0]) - 1 for a in t[1:]])
        self.celdas = {}
        for f in F:
            cx = sum(Vv[i][0] for i in f) / len(f); cz = sum(Vv[i][2] for i in f) / len(f)
            self.celdas[(math.floor((cx + 150) / 4), math.floor((cz + 170) / 4))] = [Vv[i] for i in f]

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


# ---------- G1: casas del fondo que se cruzan ----------
def g1_casas():
    txt, eol = leer("src/PuebloFondo.h")
    _, lineas = filas_de(txt, "CASAS")
    aceptadas, quedan, quitadas = [], [], []
    for l in lineas:
        x, y, z, giro, tipo = nums(l)
        poly = V.poligono("casa_a", x, z, giro)
        if any(V.se_cruzan(poly, q) for q in aceptadas):
            quitadas.append((x, z)); continue
        aceptadas.append(poly); quedan.append(l)
    if quitadas:
        escribir("src/PuebloFondo.h", reescribe(txt, "CASAS", quedan), eol)
    return quitadas


# ---------- G2: plantas enterradas en rocas o dentro de casas ----------
def casas_y_rocas():
    E = V.cargar()
    return E["casas"], [r for r in E["rocas"] if r[3] >= 1.0]


def planta_mala(x, y, z, radio, casas, rocas):
    for c in casas:
        if V.dist_poly((x, z), c[2]) < radio:
            return True
    for r in rocas:
        if V.dist_poly((x, z), r[2]) > radio: continue
        rx, rz, giro, oy = r[4]
        for dx, dz in ((0, 0), (radio, 0), (-radio, 0), (0, radio), (0, -radio)):
            t = V.techo_roca("roca_mar", rx, rz, giro, r[3], oy, x + dx, z + dz)
            if t is not None and t > y + 0.3:
                return True
    return False


def g2_plantas():
    casas, rocas = casas_y_rocas()
    quitadas = []
    for archivo, listas in (("src/VegetacionColinas.h", (("PALMAS", 0.3), ("ARBUSTOS", 0.6))), ("src/VidaColinasDatos.h", (("PALMAS", 0.3), ("ARBUSTOS", 0.6)))):
        txt, eol = leer(archivo); cambio = False
        for nombre, k in listas:
            _, lineas = filas_de(txt, nombre)
            quedan = []
            for l in lineas:
                x, y, z, giro, esc = nums(l)
                if planta_mala(x, y, z, k * esc, casas, rocas):
                    quitadas.append((os.path.basename(archivo), nombre, x, z)); cambio = True
                else:
                    quedan.append(l)
            if cambio: txt = reescribe(txt, nombre, quedan)
        if cambio: escribir(archivo, txt, eol)
    return quitadas


# ---------- S1: main.cpp ----------
def s1_main():
    txt, eol = leer("src/main.cpp"); n = 0
    for viejo, nuevo in (
        ("{ -16.5f, 0.0f, 15.0f }", "{ -19.0f, 0.0f, 17.0f }"),     # M1: el tronco cruzaba el tendedero de papel picado (z = 15, x -22..-5.35)
        ("{ -9.0f, 0.0f, -6.0f }", "{ -9.5f, 0.0f, -6.4f }"),       # B1: pegada al farol (-9.0, -4.9): ahora a 1.6 m
        ("{ 8.0f, 0.0f, -7.0f }", "{ 7.6f, 0.0f, -7.4f }")):        # B1: pegada al farol de x = 9: ahora a >= 1.5 m
        txt, k = cambia(txt, viejo, nuevo, "main.cpp"); n += k
    if n: escribir("src/main.cpp", txt, eol)
    return n


# ---------- S2: Extras.h ----------
def s2_extras():
    txt, eol = leer("src/Extras.h"); n = 0
    for viejo, nuevo in (
        # casas de los lados: se apartan de las rocas de la orilla (x = +-49..52, esc 5..6.5) y de las palapas; las de z = -12 y -14 no se tocan
        ("{ -45.0f, 8.0f, 270.0f, 0 }", "{ -43.0f, 10.0f, 270.0f, 0 }"),
        ("{ -45.0f, -2.0f, 270.0f, 1 }", "{ -43.0f, -5.0f, 270.0f, 1 }"),
        ("{  45.0f, 6.0f, 90.0f, 1 }", "{  41.0f, 8.0f, 90.0f, 1 }"),
        ("{  45.0f, -4.0f, 90.0f, 0 }", "{  42.5f, -7.0f, 90.0f, 0 }"),
        # palmas de los lados: la de x = -47 y la de x = 46 estaban enterradas en una roca de la orilla; la de (43, 9) tocaba la casa de (45, 6)
        ("{ -47.0f, 16.0f, 220.0f, 1.1f }", "{ -45.5f, 14.0f, 220.0f, 1.1f }"),
        ("{  46.0f, 18.0f, 50.0f, 1.1f }", "{  45.0f, 15.0f, 50.0f, 1.1f }"),
        ("{  43.0f, 9.0f, 90.0f, 1.3f }", "{  35.0f, 9.0f, 90.0f, 1.3f }"),
        ("{ -43.0f, 12.0f, 140.0f, 1.3f }", "{ -40.0f, 15.5f, 140.0f, 1.3f }"),   # tras mover la casa de (-45, 8) a (-43, 10) la palma de (-43, 12) quedo dentro
        # musico de la tumba 1: estaba a 0.36 m del farol de la esquina del panteon (6.2, 11.2)
        ("{  5.9f, 11.0f, 200.0f, 1, 1.1f, 1.00f, 1 }", "{  5.4f, 11.1f, 200.0f, 1, 1.1f, 1.00f, 1 }")):
        txt, k = cambia(txt, viejo, nuevo, "Extras.h"); n += k
    if n: escribir("src/Extras.h", txt, eol)
    return n


# ---------- S3: VidaColinasDatos.h ----------
def s3_vida():
    txt, eol = leer("src/VidaColinasDatos.h"); n = 0; malla = Malla()
    # persona de la procesion a 0.42 m del farol (-1.33, -29.28): 0.4 m mas lejos, apoyada en la malla
    m, lineas = filas_de(txt, "PERSONAS"); nuevas = []
    for l in lineas:
        x, y, z, giro, tipo, esc, fase, vela = nums(l)
        if abs(x + 1.20) < 0.005 and abs(z + 28.88) < 0.005:
            nx, nz = -1.08, -28.52
            ny = malla.h(nx, nz)
            l = "        { %.2ff, %.3ff, %.2ff, %.1ff, %d, %.2ff, %.2ff, %d }%s" % (nx, ny, nz, giro, int(tipo), esc, fase, int(vela), "," if l.rstrip().endswith(",") else "")
            n += 1
        nuevas.append(l)
    if n: txt = reescribe(txt, "PERSONAS", nuevas)
    # ofrenda con cruz del mirador este (65.0, -35.3): quedaba a 0.56 m de la fogata del campamento (65.5, -35.0). Se pasa al borde oeste del camino (62.45, -34.0), a 3.2 m de la fogata y a >= 1.2 m de rocas, faroles, gente y arboles
    m, lineas = filas_de(txt, "OFRENDAS"); nuevas = []; k = 0
    for l in lineas:
        x, y, z, giro = nums(l)
        if math.hypot(x - 65.5, z + 35.0) < 1.2 or math.hypot(x - 59.8, z + 33.5) < 0.05:
            nx, nz = 62.45, -34.0
            l = "        { %.2ff, %.3ff, %.2ff, %.1ff }%s" % (nx, malla.h(nx, nz) or y, nz, giro, "," if l.rstrip().endswith(",") else ""); k += 1
        nuevas.append(l)
    if k: txt = reescribe(txt, "OFRENDAS", nuevas); n += k
    if n: escribir("src/VidaColinasDatos.h", txt, eol)
    return n


def aplicar(verbose=True):
    """Orden: puntuales de Extras/main (cambian casas y palmas), luego casas del fondo, luego plantas (ya con las casas definitivas), luego VidaColinasDatos."""
    r = {"main": s1_main(), "extras": s2_extras(), "casas_fondo_quitadas": g1_casas(), "plantas_quitadas": g2_plantas(), "vida": s3_vida()}
    if verbose:
        print("main.cpp: %d cambios; Extras.h: %d cambios; VidaColinasDatos.h (persona, ofrenda): %d cambios" % (r["main"], r["extras"], r["vida"]))
        print("casas del fondo quitadas (cruzaban otra casa):", len(r["casas_fondo_quitadas"]), r["casas_fondo_quitadas"])
        print("plantas quitadas (enterradas en roca o dentro de casa):", len(r["plantas_quitadas"]))
        for t in r["plantas_quitadas"]: print("   ", t)
    return r


if __name__ == "__main__":
    aplicar()
