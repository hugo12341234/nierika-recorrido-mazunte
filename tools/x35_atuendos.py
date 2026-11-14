r"""X-35: atuendos para las personas (ropa, piel y pelo distintos + prendas tipicas de la costa de Oaxaca). Sin Blender: escribe los .obj a mano.
  Models/atuendos/        ropa_0..5.png (1024 px, tela con bordado/cuadros/rayas), piel_0..3.png y pelo_0..3.png (256 px, color liso con ruido)
  Models/falda_tehuana/   falda de olanes bordada (5 telas)      Models/rebozo/   chal sobre los hombros (3 telas)
  Models/sombrero_palma/  sombrero de palma tejida (2 cintas)
Las prendas estan en coordenadas LOCALES de la persona de pie (persona_pie_*.obj: pivote en los pies, 1.72 m, mira a -Z), 1 u = 1 m.
Uso (raiz del repo):   python tools/x35_atuendos.py
Determinista (semilla 35). Fuentes de las prendas: ver docs/investigacion_mazunte.md y docs/ESTADO_PROYECTO.md (decision X-35)."""
import math, os
import numpy as np
from PIL import Image, ImageDraw

RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
N = 1024
r = np.random.default_rng(35)


def guarda(carpeta, nombre, img, tam=None):
    d = os.path.join(RAIZ, "Models", carpeta); os.makedirs(d, exist_ok=True)
    im = Image.fromarray((np.clip(img, 0, 1) * 255).astype(np.uint8), "RGB")
    if tam: im = im.resize((tam, tam), Image.LANCZOS)
    im.save(os.path.join(d, nombre))


def tela(color, tam=N, grano=0.12):
    """Color liso con trama de hilos y ruido (se ve como tela y no como plastico)."""
    yy, xx = np.mgrid[0:tam, 0:tam]
    trama = 0.5 + 0.5 * np.sin(xx * 2 * math.pi / 8) * np.sin(yy * 2 * math.pi / 8)
    return np.array(color)[None, None, :] * (1 - grano + grano * trama[..., None]) * (0.92 + 0.08 * r.random((tam, tam, 1)))


def a_pil(arr):
    return Image.fromarray((np.clip(arr, 0, 1) * 255).astype(np.uint8), "RGB")


def de_pil(im):
    return np.asarray(im).astype(float) / 255.0


def flor(d, cx, cy, rad, petalo, centro, n=6):
    for k in range(n):
        a = 2 * math.pi * k / n
        px, py = cx + rad * 0.55 * math.cos(a), cy + rad * 0.55 * math.sin(a)
        d.ellipse((px - rad * 0.45, py - rad * 0.45, px + rad * 0.45, py + rad * 0.45), fill=petalo)
    d.ellipse((cx - rad * 0.28, cy - rad * 0.28, cx + rad * 0.28, cy + rad * 0.28), fill=centro)


def hoja(d, cx, cy, rad, verde, ang):
    px, py = cx + rad * math.cos(ang), cy + rad * math.sin(ang)
    d.ellipse((px - rad * 0.35, py - rad * 0.2, px + rad * 0.35, py + rad * 0.2), fill=verde)


def c255(c):
    return tuple(int(round(v * 255)) for v in c)


# ---------------------------------------------------------------- texturas de ropa (huipiles, camisas)
def ropas():
    # 0: huipil blanco con flores bordadas rojas y hojas verdes (punto de cruz en hileras)
    im = a_pil(tela((0.93, 0.91, 0.84))); d = ImageDraw.Draw(im)
    for fila in range(4):
        for col in range(4):
            cx, cy = 128 + col * 256 + (128 if fila % 2 else 0), 128 + fila * 256
            hoja(d, cx, cy, 70, c255((0.12, 0.45, 0.20)), 0.6); hoja(d, cx, cy, 70, c255((0.12, 0.45, 0.20)), 2.5)
            flor(d, cx, cy, 52, c255((0.78, 0.08, 0.12)), c255((0.98, 0.80, 0.15)))
    guarda("atuendos", "ropa_0.png", de_pil(im))
    # 1: huipil azul indigo con franjas y florecitas amarillas y naranjas
    im = a_pil(tela((0.09, 0.15, 0.36))); d = ImageDraw.Draw(im)
    for y in (96, 480, 864):
        d.rectangle((0, y, N, y + 40), fill=c255((0.92, 0.62, 0.12)))
        d.rectangle((0, y + 52, N, y + 60), fill=c255((0.90, 0.88, 0.80)))
    for fila in range(3):
        for col in range(6):
            flor(d, 85 + col * 170, 290 + fila * 384, 36, c255((0.96, 0.78, 0.15)), c255((0.85, 0.30, 0.08)), 5)
    guarda("atuendos", "ropa_1.png", de_pil(im))
    # 2: blusa fucsia con listones naranja y puntos blancos
    im = a_pil(tela((0.80, 0.10, 0.45))); d = ImageDraw.Draw(im)
    for y in (0, 340, 680):
        d.rectangle((0, y, N, y + 56), fill=c255((0.95, 0.50, 0.08)))
    for fila in range(8):
        for col in range(8):
            d.ellipse((col * 128 + 56, fila * 128 + 100, col * 128 + 70, fila * 128 + 114), fill=c255((0.97, 0.94, 0.90)))
    guarda("atuendos", "ropa_2.png", de_pil(im))
    # 3: guayabera blanca con alforzas verticales (tres lineas juntas cada 128 px)
    base = tela((0.95, 0.95, 0.92), grano=0.08)
    xx = np.arange(N)[None, :, None]
    alf = np.zeros((1, N, 1))
    for k in range(8):
        for off in (0, 8, 16):
            alf[0, k * 128 + 40 + off, 0] = 1
    guarda("atuendos", "ropa_3.png", base * (1 - 0.25 * alf))
    # 4: camisa a cuadros rojo y azul marino
    yy, xx2 = np.mgrid[0:N, 0:N]
    a = ((xx2 // 64) % 2).astype(float); b = ((yy // 64) % 2).astype(float)
    rojo, azul, mezcla = np.array((0.70, 0.10, 0.10)), np.array((0.10, 0.14, 0.32)), np.array((0.38, 0.12, 0.21))
    cuad = np.where((a + b)[..., None] == 0, rojo, np.where((a + b)[..., None] == 2, azul, mezcla))
    guarda("atuendos", "ropa_4.png", cuad * (0.85 + 0.15 * tela((1, 1, 1))))
    # 5: camiseta de rayas marineras (playero)
    rayas = ((yy // 64) % 2).astype(float)[..., None]
    guarda("atuendos", "ropa_5.png", (np.array((0.93, 0.93, 0.90)) * (1 - rayas) + np.array((0.10, 0.22, 0.45)) * rayas) * (0.85 + 0.15 * tela((1, 1, 1))))
    # piel y pelo: colores lisos de 256 px (la resolucion no aporta detalle a un color liso; ver decision X-35)
    for i, col in enumerate(((0.55, 0.38, 0.27), (0.64, 0.45, 0.32), (0.45, 0.30, 0.21), (0.72, 0.53, 0.39))):
        guarda("atuendos", "piel_%d.png" % i, np.array(col)[None, None, :] * (0.94 + 0.10 * r.random((256, 256, 1))))
    for i, col in enumerate(((0.06, 0.05, 0.05), (0.20, 0.12, 0.07), (0.62, 0.62, 0.60), (0.05, 0.06, 0.10))):
        guarda("atuendos", "pelo_%d.png" % i, np.array(col)[None, None, :] * (0.75 + 0.50 * r.random((256, 256, 1))))


# ---------------------------------------------------------------- geometria: superficie de revolucion (filas de elipses)
def superficie(carpeta, nombre, filas, seg, ureps, ondas=None, mtl_tex=None):
    """filas = [(y, ax, az)] de arriba abajo o de abajo arriba; ondas = funcion (j, y) -> factor de radio (olanes). Devuelve la fraccion v de cada fila."""
    n_f = len(filas)
    P = []
    for (y, ax, az) in filas:
        fila = []
        for j in range(seg + 1):
            th = 2 * math.pi * j / seg
            f = ondas(th, y) if ondas else 1.0
            fila.append((ax * f * math.cos(th), y, az * f * math.sin(th)))
        P.append(fila)
    arco = [0.0]
    for i in range(1, n_f):
        arco.append(arco[-1] + math.dist(P[i][0], P[i - 1][0]))
    total = arco[-1]
    V = [a / total for a in arco]
    # normales: suma de normales de cara por vertice (la costura j = seg se une con j = 0)
    nrm = [[[0.0, 0.0, 0.0] for _ in range(seg + 1)] for _ in range(n_f)]
    caras = []
    suma_radial = 0.0
    for i in range(n_f - 1):
        for j in range(seg):
            q = ((i, j), (i, j + 1), (i + 1, j + 1), (i + 1, j))
            p = [P[a][b] for a, b in q]
            u = [p[1][k] - p[0][k] for k in range(3)]; v = [p[3][k] - p[0][k] for k in range(3)]
            nn = [u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]]
            suma_radial += nn[0] * p[0][0] + nn[2] * p[0][2]
            caras.append((q, nn))
    giro = -1.0 if suma_radial < 0 else 1.0           # que las normales apunten hacia fuera
    for q, nn in caras:
        for a, b in q:
            for k in range(3):
                nrm[a][b % seg][k] += giro * nn[k]
                nrm[a][b][k] += 0.0
    def normal(a, b):
        v = nrm[a][b % seg]; L = math.sqrt(sum(c * c for c in v)) or 1.0
        return (v[0] / L, v[1] / L, v[2] / L)
    lineas = ["# X-35 (tools/x35_atuendos.py)", "mtllib %s.mtl" % nombre, "o %s" % nombre]
    idx = {}
    k = 0
    for i in range(n_f):
        for j in range(seg + 1):
            x, y, z = P[i][j]
            nx, ny, nz = normal(i, j)
            lineas.append("v %.6f %.6f %.6f" % (x, y, z))
            idx[(i, j)] = k + 1; k += 1
    for i in range(n_f):
        for j in range(seg + 1):
            lineas.append("vt %.6f %.6f" % (ureps * j / seg, 1.0 - V[i]))
    for i in range(n_f):
        for j in range(seg + 1):
            lineas.append("vn %.6f %.6f %.6f" % normal(i, j))
    lineas.append("usemtl %s" % nombre)
    lineas.append("s 1")
    for q, nn in caras:
        ids = [idx[c] for c in q]
        if giro < 0: ids = ids[::-1]
        lineas.append("f " + " ".join("%d/%d/%d" % (t, t, t) for t in ids))
    d = os.path.join(RAIZ, "Models", carpeta); os.makedirs(d, exist_ok=True)
    open(os.path.join(d, nombre + ".obj"), "w", newline="\n").write("\n".join(lineas) + "\n")
    open(os.path.join(d, nombre + ".mtl"), "w", newline="\n").write(
        "# X-35\nnewmtl %s\nNs 250.000000\nKa 1.000000 1.000000 1.000000\nKs 0.100000 0.100000 0.100000\nKe 0.000000 0.000000 0.000000\n"
        "Ni 1.500000\nd 1.000000\nillum 2\nmap_Kd %s\n" % (nombre, mtl_tex))
    ys = [p[1] for f in P for p in f]; xs = [p[0] for f in P for p in f]; zs = [p[2] for f in P for p in f]
    print("%s: %d vertices, caja x %.2f..%.2f y %.2f..%.2f z %.2f..%.2f" % (nombre, n_f * (seg + 1), min(xs), max(xs), min(ys), max(ys), min(zs), max(zs)))
    return V


def falda():
    # corte de la falda (persona de pie: cadera a 0.80..0.96 m): cintura, vuelo y dos olanes escalonados; dobladillo a 0.22 m del piso
    filas = [(0.98, 0.0, 0.0), (0.97, 0.185, 0.165), (0.80, 0.225, 0.205), (0.66, 0.265, 0.245), (0.645, 0.310, 0.290), (0.50, 0.350, 0.330),
             (0.485, 0.395, 0.375), (0.36, 0.425, 0.405), (0.345, 0.455, 0.435), (0.22, 0.480, 0.460)]
    ond = lambda th, y: 1.0 + 0.045 * math.sin(14 * th + 3.0 * y) * min(1.0, max(0.0, (0.75 - y) / 0.5))   # dobladillo ondulado, mas hacia abajo
    superficie("falda_tehuana", "falda_tehuana", filas, 36, 4.0, ond, "falda_0.png")
    cols = (((0.75, 0.08, 0.10), (0.98, 0.85, 0.20), (0.97, 0.95, 0.90)),    # roja con flores amarillas
            ((0.04, 0.38, 0.58), (0.95, 0.45, 0.62), (0.98, 0.90, 0.30)),    # turquesa con flores rosas
            ((0.04, 0.42, 0.24), (0.90, 0.15, 0.55), (0.98, 0.90, 0.40)),    # verde con flores magenta
            ((0.42, 0.10, 0.52), (0.98, 0.58, 0.12), (0.98, 0.92, 0.50)),    # morada con flores naranja
            ((0.90, 0.64, 0.10), (0.78, 0.10, 0.12), (0.30, 0.15, 0.05)))    # ocre con flores rojas
    for i, (base, petalo, centro) in enumerate(cols):
        im = a_pil(tela(base)); d = ImageDraw.Draw(im)
        for y in (330, 520, 700, 880):                                        # franjas de liston donde se escalonan los olanes
            d.rectangle((0, y, N, y + 24), fill=c255(centro))
        for fila in range(4):
            for col in range(4):
                cx, cy = 128 + col * 256 + (128 if fila % 2 else 0), 150 + fila * 210
                hoja(d, cx, cy, 60, c255((0.10, 0.40, 0.18)), 0.5); hoja(d, cx, cy, 60, c255((0.10, 0.40, 0.18)), 2.6)
                flor(d, cx, cy, 50, c255(petalo), c255(centro))
        guarda("falda_tehuana", "falda_%d.png" % i, de_pil(im))


def rebozo():
    # chal sobre los hombros y la espalda: de la nuca a la cintura (torso: caja de 0.36 x 0.20 m, hombros a 1.40 m, brazos a x = +-0.20..0.23)
    filas = [(1.46, 0.0, 0.0), (1.45, 0.13, 0.09), (1.40, 0.255, 0.145), (1.25, 0.275, 0.14), (1.08, 0.255, 0.135), (0.92, 0.235, 0.13)]
    superficie("rebozo", "rebozo", filas, 32, 2.0, None, "rebozo_0.png")
    for i, (base, rayas) in enumerate((((0.06, 0.09, 0.22), (0.92, 0.90, 0.85)), ((0.45, 0.06, 0.12), (0.95, 0.75, 0.25)), ((0.18, 0.28, 0.10), (0.92, 0.88, 0.70)))):
        t = tela(base)
        yy = np.arange(N)[:, None, None]
        franja = ((yy // 6) % 24 == 0).astype(float)
        jasp = (r.random((N, N, 1)) > 0.93).astype(float) * 0.6
        t = t * (1 - franja) + np.array(rayas)[None, None, :] * franja
        t = t * (1 - jasp) + np.array(rayas)[None, None, :] * jasp
        # flecos: el ultimo 8 % de la tela (borde inferior) con hilos verticales claros
        xx = np.arange(N)[None, :, None]
        fleco = ((xx // 3) % 2 == 0).astype(float)
        t[int(N * 0.92):, :, :] = t[int(N * 0.92):, :, :] * (1 - 0.5 * fleco) + np.array(rayas)[None, None, :] * 0.5 * fleco
        guarda("rebozo", "rebozo_%d.png" % i, t)


def sombrero():
    # sombrero de palma: copa de 0.14 m y ala de 0.34 m de radio, ligeramente caida (cabeza de la persona de pie: tope a 1.75 m)
    filas = [(1.80, 0.0, 0.0), (1.795, 0.08, 0.08), (1.77, 0.115, 0.115), (1.70, 0.125, 0.125), (1.665, 0.13, 0.13), (1.66, 0.20, 0.20), (1.65, 0.30, 0.30), (1.62, 0.345, 0.345)]
    V = superficie("sombrero_palma", "sombrero_palma", filas, 32, 3.0, None, "sombrero_0.png")
    banda = (V[3], V[5])                                                      # cinta entre y = 1.70 y el borde interior del ala
    for i, (palma, cinta) in enumerate((((0.80, 0.64, 0.36), (0.08, 0.06, 0.05)), ((0.62, 0.50, 0.28), (0.70, 0.08, 0.10)))):
        yy, xx = np.mgrid[0:N, 0:N]
        cuad = ((xx // 16 + yy // 16) % 2).astype(float)
        hilo = 0.6 + 0.4 * np.where(cuad > 0, np.sin(xx * 2 * math.pi / 8), np.sin(yy * 2 * math.pi / 8))
        t = np.array(palma)[None, None, :] * (0.75 + 0.25 * hilo[..., None]) * (0.92 + 0.08 * r.random((N, N, 1)))
        v0, v1 = int(banda[0] * N), int(banda[1] * N)
        t[v0:v1, :, :] = np.array(cinta)[None, None, :] * (0.85 + 0.15 * r.random((v1 - v0, N, 1)))
        guarda("sombrero_palma", "sombrero_%d.png" % i, t)


if __name__ == "__main__":
    ropas(); falda(); rebozo(); sombrero()
    print("atuendos OK")
