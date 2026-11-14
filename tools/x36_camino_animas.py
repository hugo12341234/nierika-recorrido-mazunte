r"""X-36: «Camino de las animas». Modelos y texturas nuevos, SIN Blender (los .obj se escriben a mano, como x35_atuendos.py):
  Models/arbol_selva/           arbol de copa ancha (tronco + 5 masas de hojas) · corteza.png y hojas.png (1K)
  Models/arco_cempasuchil/      arco de carrizo con guirnalda de cempasuchil de 2.1 m de ancho · flores_arco.png y carrizo.png (1K)
  Models/racimo_cempasuchil/    7 flores de cempasuchil juntas (3 planos con alfa cada una, como Models/cempasuchil) · cempasuchil.png (copia)
  Models/sendero/               textura tierra_sendero.png (1K, con alfa en los bordes); la malla la escribe x33_vida_colinas.py (sigue el terreno)
1 u = 1 m. Pivote en la base (arbol, arco, racimo). Es un extra: NO cuenta entre los 18 modelos propios (ESTADO, decision X-36).
Uso (raiz del repo):   python tools/x36_camino_animas.py        (determinista, semilla 36)"""
import math, os, shutil, random
import numpy as np
from PIL import Image, ImageDraw, ImageFilter

RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
N = 1024
r = np.random.default_rng(36)
rnd = random.Random(36)


# ------------------------------------------------------------------ escritura de .obj (cada vertice con su uv y su normal)
class Parte:
    def __init__(self, material, textura):
        self.material, self.textura = material, textura
        self.v, self.vt, self.vn, self.caras = [], [], [], []

    def vert(self, p, uv, n):
        self.v.append(p); self.vt.append(uv); self.vn.append(n)
        return len(self.v) - 1

    def cara(self, *ids):
        self.caras.append(ids)


def unit(a):
    L = math.sqrt(sum(c * c for c in a)) or 1.0
    return tuple(c / L for c in a)


def orienta(p, c):
    """Invierte el orden de la cara si su normal geometrica apunta contra las normales de sus vertices (el shader usa gl_FrontFacing)."""
    a, b, d = p.v[c[0]], p.v[c[1]], p.v[c[-1]]
    u = (b[0] - a[0], b[1] - a[1], b[2] - a[2]); w = (d[0] - a[0], d[1] - a[1], d[2] - a[2])
    g = (u[1] * w[2] - u[2] * w[1], u[2] * w[0] - u[0] * w[2], u[0] * w[1] - u[1] * w[0])
    n = [sum(p.vn[i][k] for i in c) for k in range(3)]
    return tuple(reversed(c)) if sum(g[k] * n[k] for k in range(3)) < 0 else tuple(c)


def escribe(carpeta, nombre, partes):
    d = os.path.join(RAIZ, "Models", carpeta); os.makedirs(d, exist_ok=True)
    L = ["# X-36 (tools/x36_camino_animas.py)", "mtllib %s.mtl" % nombre, "o %s" % nombre]
    off = 0
    cuerpos = []
    for p in partes:
        for x in p.v: L.append("v %.5f %.5f %.5f" % x)
    for p in partes:
        for t in p.vt: L.append("vt %.5f %.5f" % t)
    for p in partes:
        for n in p.vn: L.append("vn %.5f %.5f %.5f" % n)
    off = 0
    for p in partes:
        L.append("usemtl %s" % p.material); L.append("s 1")
        for c in p.caras:
            c = orienta(p, c)
            L.append("f " + " ".join("%d/%d/%d" % (i + 1 + off, i + 1 + off, i + 1 + off) for i in c))
        off += len(p.v)
    open(os.path.join(d, nombre + ".obj"), "w", newline="\n").write("\n".join(L) + "\n")
    M = ["# X-36"]
    for p in partes:
        M += ["newmtl %s" % p.material, "Ns 250.000000", "Ka 1.000000 1.000000 1.000000", "Ks 0.100000 0.100000 0.100000", "Ke 0.000000 0.000000 0.000000",
              "Ni 1.500000", "d 1.000000", "illum 2", "map_Kd %s" % p.textura, ""]
    open(os.path.join(d, nombre + ".mtl"), "w", newline="\n").write("\n".join(M) + "\n")
    caras = sum(len(p.caras) for p in partes)
    print("%s: %d vertices, %d caras" % (nombre, sum(len(p.v) for p in partes), caras))


def guarda(carpeta, nombre, arr, modo="RGB"):
    d = os.path.join(RAIZ, "Models", carpeta); os.makedirs(d, exist_ok=True)
    Image.fromarray((np.clip(arr, 0, 1) * 255).astype(np.uint8), modo).save(os.path.join(d, nombre))


def ruido2(tam, escala, semilla):
    g = np.random.default_rng(semilla).random((escala, escala))
    x = np.linspace(0, escala, tam, endpoint=False); i = x.astype(int); f = x - i; u = f * f * (3 - 2 * f); i1 = (i + 1) % escala
    a = g[np.ix_(i, i)]; b = g[np.ix_(i, i1)]; c = g[np.ix_(i1, i)]; dd = g[np.ix_(i1, i1)]
    return (a * (1 - u[None, :]) + b * u[None, :]) * (1 - u[:, None]) + (c * (1 - u[None, :]) + dd * u[None, :]) * u[:, None]


# ------------------------------------------------------------------ primitivas
def tubo(p, p0, p1, r0, r1, seg=8, v0=0.0, v1=1.0, urep=1.0):
    ex, ey, ez = (p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2]); e = unit((ex, ey, ez))
    a = (1, 0, 0) if abs(e[0]) < 0.9 else (0, 1, 0)
    u = unit((e[1] * a[2] - e[2] * a[1], e[2] * a[0] - e[0] * a[2], e[0] * a[1] - e[1] * a[0]))
    w = (e[1] * u[2] - e[2] * u[1], e[2] * u[0] - e[0] * u[2], e[0] * u[1] - e[1] * u[0])
    anillos = []
    for (c, rad, vv) in ((p0, r0, v0), (p1, r1, v1)):
        fila = []
        for k in range(seg + 1):
            th = 2 * math.pi * k / seg
            n = tuple(u[i] * math.cos(th) + w[i] * math.sin(th) for i in range(3))
            fila.append(p.vert(tuple(c[i] + n[i] * rad for i in range(3)), (urep * k / seg, vv), n))
        anillos.append(fila)
    for k in range(seg):
        p.cara(anillos[0][k], anillos[0][k + 1], anillos[1][k + 1], anillos[1][k])


def elipsoide(p, c, rx, ry, rz, seg=10, anillos=6, rugosidad=0.0, urep=2.0):
    filas = []
    for i in range(anillos + 1):
        phi = math.pi * i / anillos
        fila = []
        for k in range(seg + 1):
            th = 2 * math.pi * k / seg
            kk = k % seg
            f = 1.0 + rugosidad * (math.sin(3 * kk + 7 * i) * 0.5 + math.sin(5 * kk * 1.7 + 3 * i) * 0.5) if 0 < i < anillos else 1.0
            d = (math.sin(phi) * math.cos(th), math.cos(phi), math.sin(phi) * math.sin(th))
            pos = (c[0] + rx * f * d[0], c[1] + ry * f * d[1], c[2] + rz * f * d[2])
            n = unit((d[0] / rx, d[1] / ry, d[2] / rz))
            fila.append(p.vert(pos, (urep * k / seg, i / anillos), n))
        filas.append(fila)
    for i in range(anillos):
        for k in range(seg):
            p.cara(filas[i][k], filas[i + 1][k], filas[i + 1][k + 1], filas[i][k + 1])


# ------------------------------------------------------------------ arbol de la selva baja (copa ancha, simplificado)
def arbol():
    tron = Parte("corteza", "corteza.png")
    # tronco con ensanche en la base y dos ramas gruesas hacia las masas de hojas
    tubo(tron, (0, 0, 0), (0, 0.6, 0), 0.42, 0.26, 9, 0.0, 0.2, 2.0)
    tubo(tron, (0, 0.6, 0), (0, 3.0, 0), 0.26, 0.20, 9, 0.2, 0.8, 2.0)
    tubo(tron, (0, 2.6, 0), (1.5, 4.2, 0.4), 0.15, 0.09, 7, 0.0, 1.0, 1.0)
    tubo(tron, (0, 2.8, 0), (-1.4, 4.0, -0.7), 0.14, 0.08, 7, 0.0, 1.0, 1.0)
    tubo(tron, (0, 3.0, 0), (0.2, 4.6, -1.4), 0.13, 0.08, 7, 0.0, 1.0, 1.0)
    hoj = Parte("hojas", "hojas.png")
    for (cx, cy, cz, rx, ry, rz) in ((0.0, 4.9, 0.0, 2.6, 1.0, 2.5), (1.9, 4.5, 0.6, 1.9, 0.85, 1.8), (-1.8, 4.3, -0.8, 1.9, 0.8, 1.9),
                                      (0.3, 4.4, -1.8, 1.7, 0.8, 1.6), (-0.4, 4.2, 1.8, 1.6, 0.75, 1.6)):
        elipsoide(hoj, (cx, cy, cz), rx, ry, rz, 12, 6, 0.12, 3.0)
    escribe("arbol_selva", "arbol_selva", [tron, hoj])
    # texturas
    yy, xx = np.mgrid[0:N, 0:N]
    surco = 0.55 + 0.45 * np.sin(xx * 2 * math.pi / 40 + 6 * ruido2(N, 16, 1))
    cort = np.array((0.25, 0.19, 0.14))[None, None, :] * (0.55 + 0.6 * surco[..., None]) * (0.8 + 0.4 * ruido2(N, 64, 2)[..., None])
    guarda("arbol_selva", "corteza.png", cort)
    g = 0.5 * ruido2(N, 8, 3) + 0.3 * ruido2(N, 32, 4) + 0.2 * ruido2(N, 128, 5)
    verde1, verde2 = np.array((0.05, 0.17, 0.06)), np.array((0.22, 0.42, 0.12))
    hojas = verde1 * (1 - g[..., None]) + verde2 * g[..., None]
    hojas *= (0.8 + 0.4 * r.random((N, N, 1)))
    guarda("arbol_selva", "hojas.png", hojas)


# ------------------------------------------------------------------ arco de cempasuchil (carrizo + guirnalda)
def arco():
    car = Parte("carrizo", "carrizo.png")
    for sx in (-1, 1):
        tubo(car, (sx * 1.05, 0, 0), (sx * 1.05, 1.95, 0), 0.04, 0.035, 6, 0.0, 1.0, 1.0)
    # travesano de carrizo justo bajo la guirnalda
    tubo(car, (-1.05, 1.95, 0), (1.05, 1.95, 0), 0.03, 0.03, 6, 0.0, 1.0, 1.0)
    flo = Parte("flores_arco", "flores_arco.png")
    # guirnalda: semicirculo de radio 1.05 centrado en y = 1.95, tubo de radio 0.13 (con flores) que sube 0.0..1.05 de altura
    pasos, seg = 48, 12
    filas = []
    for i in range(pasos + 1):
        th = math.pi * i / pasos
        c = (math.cos(th) * 1.05, 1.95 + math.sin(th) * 1.05, 0.0)
        rad = (math.cos(th), math.sin(th), 0.0)
        fila = []
        for k in range(seg + 1):
            a = 2 * math.pi * k / seg
            n = tuple(rad[j] * math.cos(a) + (0, 0, 1)[j] * math.sin(a) for j in range(3))
            rr = 0.13 * (1.0 + 0.30 * abs(math.sin(9 * th)) * abs(math.sin(k * 1.3 + 4 * th)))   # relieve de racimos (pompones); antes un tubo casi liso
            fila.append(flo.vert(tuple(c[j] + n[j] * rr for j in range(3)), (4.0 * i / pasos, 1.0 * k / seg), n))
        filas.append(fila)
    for i in range(pasos):
        for k in range(seg):
            flo.cara(filas[i][k], filas[i][k + 1], filas[i + 1][k + 1], filas[i + 1][k])
    escribe("arco_cempasuchil", "arco_cempasuchil", [car, flo])
    # texturas: flores de cempasuchil (naranja y amarillo con centro oscuro) y carrizo (tallo con nudos)
    im = Image.new("RGB", (N, N), (14, 40, 12)); d = ImageDraw.Draw(im)
    for _ in range(420):                                                   # flores grandes y con contraste (antes 1500 chicas: de lejos era un tubo amarillo liso)
        x, y = rnd.randrange(N), rnd.randrange(N); rad = rnd.randrange(34, 62)
        terciopelo = rnd.random() < 0.10                                   # flor de terciopelo (magenta), tambien tipica de la ofrenda
        col = (150, 20, 75) if terciopelo else rnd.choice(((240, 130, 10), (250, 165, 20), (215, 95, 5), (255, 190, 40)))
        sombra = tuple(int(v * 0.55) for v in col)
        for k in range(10):
            a = 2 * math.pi * k / 10
            cx_, cy_ = x + rad * 0.62 * math.cos(a), y + rad * 0.62 * math.sin(a)
            d.ellipse((cx_ - rad * 0.46, cy_ - rad * 0.46, cx_ + rad * 0.46, cy_ + rad * 0.46), fill=sombra)
            d.ellipse((cx_ - rad * 0.40, cy_ - rad * 0.40, cx_ + rad * 0.40, cy_ + rad * 0.40), fill=col)
        d.ellipse((x - rad * 0.22, y - rad * 0.22, x + rad * 0.22, y + rad * 0.22), fill=(95, 40, 5))
    guarda("arco_cempasuchil", "flores_arco.png", np.asarray(im).astype(float) / 255.0)
    yy, xx = np.mgrid[0:N, 0:N]
    nudo = (np.abs(((yy % 300) - 150)) > 140).astype(float)
    carr = np.array((0.70, 0.58, 0.32))[None, None, :] * (0.75 + 0.25 * np.sin(xx * 2 * math.pi / 64)[..., None]) * (1 - 0.35 * nudo[..., None])
    guarda("arco_cempasuchil", "carrizo.png", carr * (0.9 + 0.1 * r.random((N, N, 1))))


# ------------------------------------------------------------------ racimo de cempasuchil (7 flores de 3 planos)
def racimo():
    p = Parte("cempasuchil", "cempasuchil.png")
    for i in range(7):
        a = rnd.uniform(0, 2 * math.pi); d = rnd.uniform(0.0, 0.42) if i else 0.0
        cx, cz, cy = d * math.cos(a), d * math.sin(a), rnd.uniform(0.045, 0.10)
        s = rnd.uniform(0.055, 0.075)                                      # semiancho de cada flor (las del sendero de X-24 miden 0.06)
        giro = rnd.uniform(0, math.pi)
        for (ex, ey, ez) in ((1, 0, 0), (0, 0, 1)):                        # 2 planos verticales cruzados + 1 horizontal
            cs, sn = math.cos(giro), math.sin(giro)
            dx, dz = ex * cs - ez * sn, ex * sn + ez * cs
            n = unit((-dz, 0.3, dx))
            ids = [p.vert((cx + sgx * s * dx, cy + sgy * s, cz + sgx * s * dz), uv, n) for (sgx, sgy, uv) in ((-1, -1, (0, 0)), (1, -1, (1, 0)), (1, 1, (1, 1)), (-1, 1, (0, 1)))]
            p.cara(*ids)
        ids = [p.vert((cx + sx * s * math.cos(giro) - sz * s * math.sin(giro), cy, cz + sx * s * math.sin(giro) + sz * s * math.cos(giro)), uv, (0, 1, 0))
               for (sx, sz, uv) in ((-1, -1, (0, 0)), (1, -1, (1, 0)), (1, 1, (1, 1)), (-1, 1, (0, 1)))]
        p.cara(*ids)
    escribe("racimo_cempasuchil", "racimo_cempasuchil", [p])
    shutil.copyfile(os.path.join(RAIZ, "Models", "cempasuchil", "cempasuchil.png"), os.path.join(RAIZ, "Models", "racimo_cempasuchil", "cempasuchil.png"))


# ------------------------------------------------------------------ textura del sendero de tierra (RGBA: el shader descarta alfa < 0.1)
def sendero_textura():
    base = np.array((0.34, 0.25, 0.15))
    t = 0.5 * ruido2(N, 16, 6) + 0.3 * ruido2(N, 64, 7) + 0.2 * r.random((N, N))
    col = base[None, None, :] * (0.7 + 0.6 * t[..., None])
    guijarros = (ruido2(N, 128, 8) > 0.82).astype(float)[..., None]
    col = col * (1 - guijarros) + np.array((0.52, 0.47, 0.40)) * guijarros * (0.8 + 0.4 * r.random((N, N, 1)))
    xs = (np.arange(N) + 0.5) / N
    borde = 0.80 + 0.17 * (ruido2(N, 32, 9)[:, 0] * 2 - 1)               # el ancho util cambia a lo largo (borde irregular)
    ancho = np.abs(2 * xs[None, :] - 1) < borde[:, None]
    alfa = ancho.astype(float)
    rgba = np.concatenate([np.clip(col, 0, 1), alfa[..., None]], axis=2)
    guarda("sendero", "tierra_sendero.png", rgba, "RGBA")
    guarda("sendero", "tierra_plaza.png", np.clip(col, 0, 1))              # misma tierra, opaca y sin borde (plazas del mirador y la cima)


if __name__ == "__main__":
    arbol(); arco(); racimo(); sendero_textura()
    print("camino de las animas OK")
