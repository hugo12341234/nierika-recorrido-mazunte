r"""X-53: apoya TODO sobre el suelo visible (auditoria del 17/11/2026, 2.a parte: "roca flotando, cosas hundidas, pasto raro, suelo levantado").
Es IDEMPOTENTE y no necesita Blender. Uso (raiz del repo), en este orden:
    python tools/x53_colinas_obj.py          (colinas.obj con UV correctas)
    python tools/x53_ajustar_alturas.py
    python tools/x53_verificar_alturas.py    (debe decir TOTAL violaciones: 0)
    python tools/x48_verificar_solapes.py    (debe seguir en 0)

Que corrige (la "altura del suelo" es SIEMPRE la malla visible, tools/x53_terreno.py == Terreno::altura de src/Terreno.h):
  P1  Plantas, faroles, fogatas, gente, racimos, arcos, tendederos y ofrendas: y = suelo visible (antes algunas usaban la formula lisa, hasta 0.5 m de mas o de menos).
  P2  Rocas de las colinas (VidaColinasDatos.h): se DIBUJAN inclinadas con la ladera (main.cpp, Terreno::inclinacion) y y se elige para que su base plana toque el
      punto mas bajo de la huella respecto a ese plano. (Una roca de base plana sobre una pendiente de 0.4 queda colgando por un lado o enterrada por el otro.)
  P3  Promontorio rocoso del fondo (PuebloFondo.h): las 6 rocas grandes estaban a la altura del suelo de su centro: sus salientes colgaban 5-25 m sobre la
      playa y la bahia y salian de la malla (z > 42). Ahora cada roca busca (partiendo de su tamano y sitio originales) el sitio mas cercano donde cabe en x -146..-58, z 4..40
      con la mitad o menos de su altura enterrada, achicandose lo minimo, y se apoya en el punto mas bajo de su huella. El mirador (palapa) se apoya en la cima de la roca mayor.
  P4  Casas del pueblo de fondo (PuebloFondo.h): la casa se apoya en
      el punto MAS ALTO de su huella y un CIMIENTO de piedra (Models/cimientos/cimientos.obj, una sola malla en coordenadas del mundo) llena el hueco hasta el suelo mas bajo
      (con terraza el cimiento es solo un zocalo de 0.3 m).
  P5  Calles y senderos (Models/calles, Models/sendero): cada vertice a 0.03 m sobre el suelo visible (las calles flotaban hasta 0.6 m).
Al final llama a x48_corregir_solapes.aplicar(): quita las plantas que hayan quedado dentro de una roca o una casa tras mover las rocas y las casas."""
import math, os, re, shutil, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import x53_terreno as T
_argv = sys.argv
sys.argv = [sys.argv[0]]
import x48_corregir_solapes as C
sys.argv = _argv
V = C.V
RAIZ = T.RAIZ

# ---- huellas ----
CASA_X = (-2.54, 2.54)        # huella de la base de casa_a / casa_b (vertices con y < 0.05): x +-2.54, z -3.5 (porche) .. 2.04
CASA_Z = (-3.50, 2.04)
ZONA_X = (-146.0, -58.0)      # promontorio: caja donde cabe sobre la malla (la malla llega a z = 42 y la playa a x = -50)
ZONA_Z = (4.0, 40.0)


def rect(x, z, giro, ax, az, margen=0.0):
    g = math.radians(giro); c, s = math.cos(g), math.sin(g)
    pts = ((ax[0] - margen, az[0] - margen), (ax[1] + margen, az[0] - margen), (ax[1] + margen, az[1] + margen), (ax[0] - margen, az[1] + margen))
    return [(x + c * px + s * pz, z - s * px + c * pz) for px, pz in pts]


def muestras(poly, paso=0.6):
    xs = [p[0] for p in poly]; zs = [p[1] for p in poly]
    out = list(poly)
    x = min(xs)
    while x <= max(xs):
        z = min(zs)
        while z <= max(zs):
            if V.dentro((x, z), poly): out.append((x, z))
            z += paso
        x += paso
    return out


def alturas(poly, paso=0.6):
    ts = [T.altura(*p) for p in muestras(poly, paso)]
    if any(t is None for t in ts): return None
    return min(ts), max(ts)


# ---------- utilidades de filas ----------
def poner_y(linea, y):
    """Cambia el 2.o numero de una fila `{ x, y, z, ... }` (conserva el resto)."""
    return re.sub(r"^(\s*\{\s*-?\d+\.?\d*f,\s*)-?\d+\.?\d*f", lambda m: m.group(1) + "%.3ff" % y, linea, count=1)


def fila_y(txt, nombre, col_x=0, col_z=2, umbral=0.05):
    """Reescribe y = suelo visible en las filas del arreglo cuya y se aparte mas de `umbral`. Devuelve (txt, n)."""
    _, lineas = C.filas_de(txt, nombre)
    nuevas, n = [], 0
    for l in lineas:
        v = C.nums(l)
        ys = T.altura(v[col_x], v[col_z])
        if ys is not None and abs(v[1] - ys) > umbral:
            l = poner_y(l, ys); n += 1
        nuevas.append(l)
    return (C.reescribe(txt, nombre, nuevas) if n else txt), n


def fila_puntos(ruta, nombres):
    txt, eol = C.leer(ruta); total = 0
    for nombre in nombres:
        txt, n = fila_y(txt, nombre); total += n
    if total: C.escribir(ruta, txt, eol)
    return total


# ---------- P1: plantas y demas, a la altura del suelo visible ----------
def p1_puntuales():
    n = fila_puntos("src/VegetacionColinas.h", ("PALMAS", "ARBUSTOS"))
    n += fila_puntos("src/VidaColinasDatos.h", ("FAROLES", "RACIMOS", "ARCOS", "TENDEDEROS", "OFRENDAS", "FOGATAS", "PERSONAS", "PALMAS", "ARBUSTOS", "ARBOLES"))
    return n


# ---------- objetos duros que una roca o una casa no deben pisar ----------
def puntos_duros():
    E = V.cargar()
    pts = [(f[2], f[3]) for f in E["faroles"]] + [(f[2], f[3]) for f in E["fogatas"]] + [(f[2], f[3]) for f in E["cruces"]] + [(p[2], p[3]) for p in E["personas"]]
    for nombre in ("ARCOS", "TENDEDEROS", "OFRENDAS"):
        pts += [(r[0], r[2]) for r in V.leer_h("VidaColinasDatos.h", nombre)]
    return pts


def puntos_sendero():
    ptsl = []
    for l in open(os.path.join(RAIZ, "Models", "sendero", "sendero.obj"), encoding="utf-8"):
        if l.startswith("v "):
            t = l.split(); ptsl.append((float(t[1]), float(t[3])))
    return ptsl[::3]


def zona_libre_de_playa(x, z):
    return not (abs(x) <= 56.0 and z >= -26.0)


_REJ = {}


# ---------- P3: promontorio ----------
def suelo_rej(x, z):
    k = (round(x / 2.0), round(z / 2.0))
    if k not in _REJ: _REJ[k] = T.altura(k[0] * 2.0, k[1] * 2.0)
    return _REJ[k]


def p3_promontorio():
    """Cada roca grande busca (en pasos de 4 m dentro de la zona) el sitio (a 24 m o menos del original) y el tamano (hasta -75 % del original) con la MAYOR escala tal que:
    cabe en la zona, la mitad como maximo de su altura queda enterrada del lado de la subida y se apoya en el punto mas bajo de su huella."""
    txt, eol = C.leer("src/PuebloFondo.h")
    _, lineas = C.filas_de(txt, "ROCAS")
    nuevas, nota, mirador = [], [], None
    for k, l in enumerate(lineas):
        x0, y0, z0, giro, esc0 = C.nums(l)
        esc_orig = {0: 22.0, 1: 30.0, 2: 14.0, 3: 26.0, 4: 12.0, 5: 9.0}.get(k, esc0)   # tamano original (X-30): la busqueda siempre parte de el, asi es idempotente
        x_orig = {0: -84.0, 1: -92.0, 2: -78.0, 3: -98.0, 4: -88.0, 5: -72.0}.get(k, x0)
        z_orig = {0: 30.0, 1: 36.0, 2: 38.0, 3: 26.0, 4: 24.0, 5: 28.0}.get(k, z0)
        mejor = None
        for umbral in (0.5, 0.65, 0.8):
          s = esc_orig
          while s >= 0.25 * esc_orig:
              for cx in range(int(ZONA_X[0]), int(ZONA_X[1]) + 1, 4):
                  for cz in range(int(ZONA_Z[0]), int(ZONA_Z[1]) + 1, 4):
                      poly = V.poligono("roca_mar", float(cx), float(cz), giro, s)
                      xs = [p[0] for p in poly]; zs = [p[1] for p in poly]
                      if min(xs) < ZONA_X[0] or max(xs) > ZONA_X[1] or min(zs) < ZONA_Z[0] or max(zs) > ZONA_Z[1]: continue
                      ts = [suelo_rej(px, pz) for px in range(int(min(xs)) - 1, int(max(xs)) + 2, 2) for pz in range(int(min(zs)) - 1, int(max(zs)) + 2, 2)
                            if V.dentro((float(px), float(pz)), poly)]
                      ts = [t for t in ts if t is not None]
                      if not ts: continue
                      razon = (max(ts) - min(ts)) / (0.92 * s)
                      if razon > umbral: continue
                      d = math.hypot(cx - x_orig, cz - z_orig)
                      if d > 24.0: continue                  # la roca no se aleja mas de 24 m de su sitio original
                      if mejor is None or d < mejor[0]: mejor = (d, float(cx), float(cz), s)
              if mejor: break
              s *= 0.92
          if mejor: break
        if mejor is None:
            raise SystemExit("promontorio: la roca %d no encuentra sitio" % k)
        _, nx, nz, ns = mejor
        t = alturas(V.poligono("roca_mar", nx, nz, giro, ns), 1.0)
        ny = t[0] - 0.05 * ns
        nuevas.append("        { %.2ff, %.2ff, %.2ff, %.2ff, %.2ff }%s" % (nx, ny, nz, giro, ns, "," if l.rstrip().endswith(",") else ""))
        nota.append((k, esc_orig, ns, nx - x_orig, nz - z_orig, ny))
        if k == 0:
            techos = [V.techo_roca("roca_mar", nx, nz, giro, ns, ny, nx + dx, nz + dz2) for dx in (-1.76, 1.76) for dz2 in (-1.76, 1.76)]
            techos = [q for q in techos if q is not None] + [V.techo_roca("roca_mar", nx, nz, giro, ns, ny, nx, nz)]
            mirador = (nx, min(techos) - 0.10, nz)
    txt = C.reescribe(txt, "ROCAS", nuevas)
    if mirador:
        txt = re.sub(r"constexpr float MIRADOR_X = [^;]*;", "constexpr float MIRADOR_X = %.2ff, MIRADOR_Y = %.2ff, MIRADOR_Z = %.2ff;" % mirador, txt)
    C.escribir("src/PuebloFondo.h", txt, eol)
    return nota


# ---------- P2: rocas de las colinas (apoyadas en el plano de la ladera) ----------
def p2_rocas(casas):
    """La roca se dibuja inclinada con el suelo (Terreno::inclinacion): su base plana pasa por el plano de la ladera. y se elige para que ese plano toque el
    punto mas bajo (respecto al plano) de la huella y la roca quede enterrada solo 6 % de su tamano."""
    txt, eol = C.leer("src/VidaColinasDatos.h")
    _, lineas = C.filas_de(txt, "ROCAS")
    nuevas, cambios, quitadas = [], 0, 0
    for l in lineas:
        x, y, z, giro, esc = C.nums(l)
        poly = V.poligono("roca_mar", x, z, giro, esc)
        hc = T.altura(x, z)
        if hc is None:
            nuevas.append(l); continue
        gx, gz = T.inclinacion(x, z, esc)
        desv = []
        for p in muestras(poly):
            hp = T.altura(*p)
            if hp is not None: desv.append(hp - (hc + gx * (p[0] - x) + gz * (p[1] - z)))
        ny = hc + min(desv) - 0.06 * esc
        if max(desv) - min(desv) > 0.92 * esc:      # en el borde escarpado de una terraza el suelo sube mas que la roca: queda tragada, se quita
            quitadas += 1
            continue
        if abs(ny - y) > 0.005: cambios += 1
        nuevas.append("        { %.2ff, %.3ff, %.2ff, %.1ff, %.2ff }%s" % (x, ny, z, giro, esc, "," if l.rstrip().endswith(",") else ""))
    txt = C.reescribe(txt, "ROCAS", nuevas)
    C.escribir("src/VidaColinasDatos.h", txt, eol)
    return cambios, quitadas


# ---------- P4: casas del fondo + cimientos ----------
DESNIVEL_MAX = 3.0        # una casa cuyo suelo, bajo su huella, tenga mas desnivel que esto (tras buscarle sitio) se QUITA: el cimiento seria un torreon


def p4_casas():
    """Cada casa busca (hasta 24 m a la redonda, sin cruzarse con casas, rocas ni objetos) el sitio con menos desnivel bajo su huella; se apoya en el punto mas alto de
    la huella y un cimiento de piedra llena el hueco hasta el suelo mas bajo. Si ni el mejor sitio baja de DESNIVEL_MAX, la casa se quita (son decorado del fondo)."""
    txt, eol = C.leer("src/PuebloFondo.h")
    _, lineas = C.filas_de(txt, "CASAS")
    E = V.cargar()
    rocas = [r for r in E["rocas"] if r[3] >= 1.0]
    duros = puntos_duros()
    datos = [C.nums(l) for l in lineas]
    huellas = [V.poligono("casa_a", d[0], d[2], d[3]) for d in datos]
    vivas = [True] * len(datos)
    nuevas, specs, movidas, quitadas = [], [], 0, []
    for i, l in enumerate(lineas):
        x, y, z, giro, tipo = datos[i]
        otras = [huellas[j] for j in range(len(huellas)) if j != i and vivas[j]] + [c[2] for c in E["casas"] if c[0] != "fondo"]

        def rango(px, pz):
            t = alturas(rect(px, pz, giro, CASA_X, CASA_Z, 0.05))
            return t[1] - t[0] if t else None

        r0 = rango(x, z)
        mejor = (r0 if r0 is not None else 99.0, x, z)
        if r0 is None or r0 > 1.2:
            for r in (1.5, 3.0, 4.5, 6.0, 8.0, 10.0, 12.0, 15.0, 18.0, 21.0, 24.0):
                for a in range(16):
                    px, pz = x + r * math.cos(a * math.pi / 8), z + r * math.sin(a * math.pi / 8)
                    if not zona_libre_de_playa(px, pz): continue
                    rg = rango(px, pz)
                    if rg is None: continue
                    costo = rg + 0.12 * r
                    if costo >= mejor[0] - 0.3: continue
                    poly = V.poligono("casa_a", px, pz, giro)
                    if any(V.se_cruzan(poly, o) for o in otras): continue
                    if any(V.rocas_tocan_casa(r_, poly) for r_ in rocas): continue
                    if any(V.dist_poly(p, poly) < 0.8 for p in duros): continue
                    mejor = (costo, px, pz)
        _, nx, nz = mejor
        rg = rango(nx, nz)
        if rg is None or rg > DESNIVEL_MAX:
            vivas[i] = False; quitadas.append((i, round(x, 1), round(z, 1), round(rg or 99, 1))); continue
        if (nx, nz) != (x, z):
            movidas += 1; huellas[i] = V.poligono("casa_a", nx, nz, giro)
        t = alturas(rect(nx, nz, giro, CASA_X, CASA_Z, 0.05))
        ny = t[1] - 0.10
        nuevas.append("        { %.2ff, %.2ff, %.2ff, %.2ff, %d }%s" % (nx, ny, nz, giro, int(tipo), "," if l.rstrip().endswith(",") else ""))
        specs.append((nx, nz, giro, ny + 0.02, t[0] - 0.30))
    if nuevas and not nuevas[-1].rstrip().endswith(","):
        pass
    txt = C.reescribe(txt, "CASAS", [n.rstrip(",") + ("," if k < len(nuevas) - 1 else "") for k, n in enumerate(nuevas)])
    C.escribir("src/PuebloFondo.h", txt, eol)
    escribir_cimientos(specs)
    return movidas, specs, quitadas


def escribir_cimientos(specs):
    """Models/cimientos/cimientos.obj: un bloque de piedra bajo cada casa (cuatro caras laterales y la tapa), en coordenadas del mundo."""
    carpeta = os.path.join(RAIZ, "Models", "cimientos")
    os.makedirs(carpeta, exist_ok=True)
    shutil.copyfile(os.path.join(RAIZ, "Models", "casa_a", "zocalo.png"), os.path.join(carpeta, "cimiento.png"))
    with open(os.path.join(carpeta, "cimientos.mtl"), "w", encoding="utf-8", newline="\n") as f:
        f.write("# X-53\nnewmtl cimiento\nNs 250.000000\nKa 1.000000 1.000000 1.000000\nKs 0.100000 0.100000 0.100000\nKe 0.000000 0.000000 0.000000\n"
                "Ni 1.500000\nd 1.000000\nillum 2\nmap_Kd cimiento.png\n")
    v, vt, vn, caras = [], [], [], []
    TILE = 1.5

    def cara(ps, uvs, n):
        base = len(v)
        for p, uv in zip(ps, uvs):
            v.append(p); vt.append(uv); vn.append(n)
        caras.append(tuple(range(base + 1, base + 5)))

    for (x, z, giro, ytop, ybot) in specs:
        P = rect(x, z, giro, (CASA_X[0] - 0.04, CASA_X[1] + 0.04), (CASA_Z[0] - 0.04, CASA_Z[1] + 0.04))
        for k in range(4):
            a, b = P[k], P[(k + 1) % 4]
            L = math.hypot(b[0] - a[0], b[1] - a[1])
            dx, dz = b[0] - a[0], b[1] - a[1]
            n = (dz / L, 0.0, -dx / L)                   # hacia afuera (la huella va (x0,z0) (x1,z0) (x1,z1) (x0,z1): el interior queda a la izquierda de a->b)
            # orden abajo-a, arriba-a, arriba-b, abajo-b: el producto vectorial (a->arriba) x (a->b) da (dz, 0, -dx) = n (cara hacia afuera con giro antihorario)
            cara([(a[0], ybot, a[1]), (a[0], ytop, a[1]), (b[0], ytop, b[1]), (b[0], ybot, b[1])],
                 [(0.0, ybot / TILE), (0.0, ytop / TILE), (L / TILE, ytop / TILE), (L / TILE, ybot / TILE)], n)
        cara([(P[0][0], ytop, P[0][1]), (P[3][0], ytop, P[3][1]), (P[2][0], ytop, P[2][1]), (P[1][0], ytop, P[1][1])],
             [(0, 0), (0, 1), (1, 1), (1, 0)], (0.0, 1.0, 0.0))
    lin = ["# X-53: generado por tools/x53_ajustar_alturas.py (no editar a mano). Cimientos de las casas del pueblo de fondo (coordenadas del mundo).",
           "mtllib cimientos.mtl", "o cimientos"]
    lin += ["v %.4f %.4f %.4f" % p for p in v] + ["vt %.4f %.4f" % p for p in vt] + ["vn %.4f %.4f %.4f" % p for p in vn]
    lin += ["s off", "usemtl cimiento"]
    lin += ["f " + " ".join("%d/%d/%d" % (i, i, i) for i in c) for c in caras]
    with open(os.path.join(carpeta, "cimientos.obj"), "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lin) + "\n")


# ---------- P5: calles y senderos sobre el suelo ----------
ARISTA_MAX = 0.8   # m: calles y sendero se parten hasta esta arista para seguir el relieve


def subdividir_calles(ruta):
    """Parte cada cara de 4 vertices en una rejilla n x m (interpolacion bilineal de
    posicion, UV y normal). Idempotente: si todas las aristas ya miden <= ARISTA_MAX no cambia nada."""
    with open(ruta, encoding="utf-8", newline="") as f:
        crudo = f.read()
    eol = "\r\n" if "\r\n" in crudo else "\n"
    lin = crudo.replace("\r\n", "\n").split("\n")
    V, VT, VN, F, pre = [], [], [], [], []
    mat = None
    for l in lin:
        t = l.split()
        if not t:
            continue
        if t[0] == "v": V.append([float(a) for a in t[1:4]])
        elif t[0] == "vt": VT.append([float(a) for a in t[1:3]])
        elif t[0] == "vn": VN.append([float(a) for a in t[1:4]])
        elif t[0] == "f": F.append([tuple(int(a) - 1 for a in w.split("/")) for w in t[1:]])
        elif t[0] in ("mtllib", "o", "usemtl", "s"): pre.append(l)
    def arista(a, b): return math.hypot(V[a][0] - V[b][0], V[a][2] - V[b][2])
    if all(max(arista(c[i][0], c[(i + 1) % len(c)][0]) for i in range(len(c))) <= ARISTA_MAX for c in F):
        return 0
    NV, NVT, NVN, NF = [], [], [], []
    def lerp(a, b, t): return [a[i] + (b[i] - a[i]) * t for i in range(len(a))]
    def nuevo(pp, uu, nn):
        m = math.sqrt(sum(a * a for a in nn)) or 1.0
        NV.append(pp); NVT.append(uu); NVN.append([a / m for a in nn]); return len(NV) - 1
    def tri(a, b, c, prof=0):
        # triangulo (pos, uv, normal) por 3 tuplas; se parte en 4 mientras tenga una arista > ARISTA_MAX
        if prof < 6 and max(math.hypot(a[0][0] - b[0][0], a[0][2] - b[0][2]), math.hypot(b[0][0] - c[0][0], b[0][2] - c[0][2]), math.hypot(c[0][0] - a[0][0], c[0][2] - a[0][2])) > ARISTA_MAX:
            m = lambda p, q: tuple(lerp(p[k], q[k], 0.5) for k in range(3))
            ab, bc, ca = m(a, b), m(b, c), m(c, a)
            for t in ((a, ab, ca), (ab, b, bc), (ca, bc, c), (ab, bc, ca)): tri(*t, prof + 1)
        else:
            NF.append(tuple(nuevo(*q) for q in (a, b, c)))
    for c in F:
        P = [V[i[0]] for i in c]; U = [VT[i[1]] for i in c]; N = [VN[i[2]] for i in c]
        if len(c) == 3:
            tri(*[(P[k], U[k], N[k]) for k in range(3)]); continue
        if len(c) != 4:
            continue
        nu = max(1, math.ceil(max(arista(c[0][0], c[1][0]), arista(c[3][0], c[2][0])) / ARISTA_MAX))
        nv = max(1, math.ceil(max(arista(c[1][0], c[2][0]), arista(c[0][0], c[3][0])) / ARISTA_MAX))
        # ciclo p0 p1 p2 p3: u recorre p0->p1 (y p3->p2), v recorre p0->p3 (y p1->p2)
        base = len(NV)
        for j in range(nv + 1):
            for i in range(nu + 1):
                u, v = i / nu, j / nv
                pp = lerp(lerp(P[0], P[1], u), lerp(P[3], P[2], u), v)
                uu = lerp(lerp(U[0], U[1], u), lerp(U[3], U[2], u), v)
                nn = lerp(lerp(N[0], N[1], u), lerp(N[3], N[2], u), v)
                nuevo(pp, uu, nn)
        for j in range(nv):
            for i in range(nu):
                a = base + j * (nu + 1) + i; b = a + 1; d = a + (nu + 1); e = d + 1
                NF.append((a, b, e, d))
    out = [l for l in pre if l.startswith("mtllib")] + [l for l in pre if l.startswith("o ")]
    out += ["v %.6f %.6f %.6f" % tuple(p) for p in NV]
    out += ["vt %.6f %.6f" % tuple(p) for p in NVT]
    out += ["vn %.6f %.6f %.6f" % tuple(p) for p in NVN]
    out += [l for l in pre if l.startswith("s ")] + [l for l in pre if l.startswith("usemtl")]
    out += ["f " + " ".join("%d/%d/%d" % (k + 1, k + 1, k + 1) for k in q) for q in NF]
    with open(ruta, "w", encoding="utf-8", newline="") as f:
        f.write(eol.join(out) + eol)
    return len(NF)


def p5_mallas():
    cambios = 0
    for n_ in ("calles", "sendero"):
        subdividir_calles(os.path.join(RAIZ, "Models", n_, n_ + ".obj"))
    for nombre in ("calles", "sendero"):
        ruta = os.path.join(RAIZ, "Models", nombre, nombre + ".obj")
        with open(ruta, encoding="utf-8", newline="") as f:
            crudo = f.read()
        eol = "\r\n" if "\r\n" in crudo else "\n"
        salida = []
        for l in crudo.replace("\r\n", "\n").split("\n"):
            if l.startswith("v "):
                t = l.split()
                x, y, z = float(t[1]), float(t[2]), float(t[3])
                s = T.altura(x, z)
                if s is not None and abs(y - (s + 0.03)) > 0.02:
                    l = "v %.6f %.6f %.6f" % (x, s + 0.03, z); cambios += 1
            salida.append(l)
        with open(ruta, "w", encoding="utf-8", newline="") as f:
            f.write(eol.join(salida))
    return cambios


def aplicar():
    import x53_colinas_obj
    print("colinas.obj: %d vertices, %d caras" % x53_colinas_obj.escribir(os.path.join(RAIZ, "Models", "colinas", "colinas.obj")))
    print("P1 filas con y corregida:", p1_puntuales())
    nota = p3_promontorio()
    for k, esc, s, dx, dz, ny in nota:
        print("P3 roca %d: esc %.1f -> %.1f, desplazada (%.0f, %.0f) m, y %.2f" % (k, esc, s, dx, dz, ny))
    E = V.cargar()
    casas = [c[2] for c in E["casas"]]
    print("P2 rocas cambiadas (y) / quitadas por quedar tragadas:", p2_rocas(casas))
    mov, specs, quitadas = p4_casas()
    print("P4 casas movidas:", mov, "| cimientos:", len(specs), "| quitadas por ladera muy empinada:", quitadas)
    print("P5 vertices de calles/senderos corregidos:", p5_mallas())
    print("x48: casas quitadas", C.g1_casas(), "plantas quitadas", len(C.g2_plantas()))


if __name__ == "__main__":
    aplicar()
