"""X-58: texturas propias de 1K (1024 px) para vestir a la silueta (hasta hoy solo tenia una tela cruda casi blanca). Sin fuentes externas.
Uso (desde la raiz del repo):  python3 tools/x58_silueta_texturas.py
Genera en Models/silueta/: silueta_huipil.png (blusa blanca con bordado de flores), silueta_pantalon.png (algodon indigo),
silueta_piel.png, silueta_pelo.png, silueta_huarache.png. Todas se repiten sin costuras (la proyeccion UV de X-7 repite cada 0.5 m)."""
import os
import numpy as np
from PIL import Image

N = 1024
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
CARPETA = os.path.join(RAIZ, "Models", "silueta")


def ruido(escala, semilla):
    """Ruido de valor suave y PERIODICO (misma funcion que x6_texturas.py)."""
    r = np.random.default_rng(semilla)
    g = r.random((escala, escala))
    x = np.linspace(0, escala, N, endpoint=False)
    i = x.astype(int); f = x - i; u = f * f * (3 - 2 * f); i1 = (i + 1) % escala
    a = g[np.ix_(i, i)]; b = g[np.ix_(i, i1)]; c = g[np.ix_(i1, i)]; d = g[np.ix_(i1, i1)]
    ux = u[None, :]; uy = u[:, None]
    return (a * (1 - ux) + b * ux) * (1 - uy) + (c * (1 - ux) + d * ux) * uy


def fbm(base, octavas, semilla):
    tot = np.zeros((N, N)); amp = 1.0; suma = 0.0
    for k in range(octavas):
        tot += amp * ruido(base * 2 ** k, semilla + k); suma += amp; amp *= 0.5
    return tot / suma


def mezcla(c1, c2, t):
    return np.array(c1)[None, None, :] * (1 - t[..., None]) + np.array(c2)[None, None, :] * t[..., None]


def guarda(rgb, nombre):
    ruta = os.path.join(CARPETA, nombre)
    Image.fromarray(np.clip(rgb * 255, 0, 255).astype(np.uint8), 'RGB').save(ruta)
    print('->', ruta)


yy, xx = np.mgrid[0:N, 0:N].astype(float)
trama = 0.5 + 0.5 * np.sin(xx * 2 * np.pi / 8) * np.sin(yy * 2 * np.pi / 8)   # cruce de hilos cada 8 px


def pinta(base, mascara, color, suavidad=1.0):
    return base * (1 - mascara[..., None] * suavidad) + np.array(color)[None, None, :] * mascara[..., None] * suavidad


# ---------- Huipil: algodon blanco con dos bandas bordadas por tile (cada 0.25 m) de flores de punto de cruz ----------
huipil = mezcla((0.86, 0.83, 0.76), (0.97, 0.95, 0.90), 0.45 * trama + 0.55 * fbm(8, 4, 5800))
for v0 in (256, 768):                        # centro vertical de cada banda (px)
    dv = np.abs(yy - v0)
    banda = np.clip((70 - dv) / 3, 0, 1)                              # franja de 140 px de alto
    huipil = pinta(huipil, banda, (0.10, 0.45, 0.50), 0.35)           # fondo turquesa tenue
    for borde, col in ((75, (0.80, 0.08, 0.15)), (-75, (0.80, 0.08, 0.15)), (62, (0.95, 0.70, 0.10)), (-62, (0.95, 0.70, 0.10))):
        huipil = pinta(huipil, np.clip((4 - np.abs(yy - v0 - borde)) / 2, 0, 1), col)   # hilos rojos y dorados en los bordes
    # rosetas de 8 petalos cada 128 px (8 por tile: sin costura) alternando fucsia y naranja, con centro amarillo y hojas verdes
    for k in range(8):
        cx = 64 + 128 * k
        petalo = (0.85, 0.08, 0.45) if k % 2 == 0 else (0.97, 0.52, 0.08)
        dx = xx - cx; dy = yy - v0
        r = np.hypot(dx, dy); ang = np.arctan2(dy, dx)
        forma = r < (26 + 12 * np.cos(8 * ang))                       # contorno de 8 petalos
        huipil = pinta(huipil, (forma & (r > 8)).astype(float), petalo)
        huipil = pinta(huipil, (r <= 9).astype(float), (0.98, 0.85, 0.15))
        # hojas entre rosetas
        hx = xx - (cx + 64); hy = yy - v0
        hoja = (np.abs(hx) + np.abs(hy) * 1.8) < 14
        huipil = pinta(huipil, hoja.astype(float), (0.15, 0.50, 0.20))
guarda(huipil, 'silueta_huipil.png')

# ---------- Pantalon / falda: algodon indigo con trama y rayas finas claras ----------
raya = (np.sin(yy * 2 * np.pi / 128) > 0.97).astype(float)
pantalon = mezcla((0.07, 0.10, 0.24), (0.17, 0.22, 0.40), 0.45 * trama + 0.55 * fbm(8, 4, 5810))
pantalon = pinta(pantalon, raya, (0.55, 0.60, 0.75), 0.35)
guarda(pantalon, 'silueta_pantalon.png')

# ---------- Piel: moreno medio con variacion suave ----------
piel = mezcla((0.50, 0.32, 0.22), (0.66, 0.45, 0.32), fbm(4, 4, 5820))
guarda(piel, 'silueta_piel.png')

# ---------- Pelo: negro con mechones finos verticales ----------
mechon = 0.5 + 0.5 * np.sin((xx / N * 90 + fbm(3, 3, 5830) * 6) * 2 * np.pi)
pelo = mezcla((0.03, 0.025, 0.03), (0.14, 0.11, 0.10), 0.5 * mechon + 0.5 * fbm(16, 3, 5840))
guarda(pelo, 'silueta_pelo.png')

# ---------- Huarache: cuero cafe con trenzado ----------
trenza = 0.5 + 0.5 * np.sin((xx + yy) * 2 * np.pi / 32) * np.sin((xx - yy) * 2 * np.pi / 32)
cuero = mezcla((0.30, 0.17, 0.08), (0.58, 0.38, 0.20), 0.5 * trenza + 0.5 * fbm(8, 4, 5850))
guarda(cuero, 'silueta_huarache.png')
