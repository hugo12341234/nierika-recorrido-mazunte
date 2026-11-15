# X-44 · Piedra rugosa oscura (como el video de referencia de la capilla) para muros, arco y piso. 1K, teselable, procedural y determinista.
# Uso (desde la raiz del repo):  python3 tools/x44_piedra.py     Requiere: numpy y Pillow.
# 1 unidad UV = 1 m en los muros: las celdas miden ~ 9 cm (muro) y ~ 20 cm (piso). La luminancia alimenta el relieve del shader (relieveLuma).
import os
import numpy as np
from PIL import Image

N = 1024
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))


def voronoi(rng, celdas):
    """Voronoi teselable: devuelve (distancia al centro mas cercano, id de celda aleatorio, distancia al borde) en [0,1]."""
    pts = (np.stack(np.meshgrid(np.arange(celdas), np.arange(celdas), indexing="ij"), -1) + rng.uniform(0.15, 0.85, (celdas, celdas, 2))) / celdas
    y, x = (np.arange(N) + 0.5) / N, (np.arange(N) + 0.5) / N
    X, Y = np.meshgrid(x, y)
    d1 = np.full((N, N), 9.0); d2 = np.full((N, N), 9.0); ident = np.zeros((N, N))
    ids = rng.uniform(0, 1, (celdas, celdas))
    # version vectorizada con envoltura toroidal
    for i in range(-1, celdas + 1):
        for j in range(-1, celdas + 1):
            ii, jj = i % celdas, j % celdas
            cx = pts[ii, jj, 1] + (j - jj) / celdas
            cy = pts[ii, jj, 0] + (i - ii) / celdas
            d = np.sqrt((X - cx) ** 2 + (Y - cy) ** 2)
            mejor = d < d1
            d2 = np.where(mejor, d1, np.minimum(d2, d))
            ident = np.where(mejor, ids[ii, jj], ident)
            d1 = np.where(mejor, d, d1)
    return d1 * celdas, ident, (d2 - d1) * celdas


def ruido(rng, octavas, f0, pers=0.55):
    fy = np.fft.fftfreq(N)[:, None]; fx = np.fft.rfftfreq(N)[None, :]
    tot = np.zeros((N, N)); amp = 1.0
    for o in range(octavas):
        f = f0 * (2 ** o) / N
        r = np.sqrt(fx ** 2 + fy ** 2)
        esp = np.exp(-((r - f) ** 2) / (2 * (0.45 * f) ** 2)) * np.exp(1j * rng.uniform(0, 2 * np.pi, (N, N // 2 + 1)))
        c = np.fft.irfft2(esp, s=(N, N)); c /= np.abs(c).max() + 1e-9
        tot += amp * c; amp *= pers
    tot -= tot.min()
    return tot / (tot.max() + 1e-9)


def piedra(semilla, celdas, base, claro, junta, contraste, f0=12):
    """Roca rugosa y picada: ruido fractal + guijarros (Voronoi chico). `junta` oscurece los huecos entre guijarros."""
    rng = np.random.default_rng(semilla)
    d, ident, borde = voronoi(rng, celdas)
    fbm = ruido(rng, 7, f0, 0.62)
    macro = ruido(rng, 3, 2)
    cima = np.clip(1.0 - d * 1.1, 0, 1)
    hueco = np.clip(borde * 2.2, 0, 1)
    h = 0.62 * fbm + 0.38 * cima * (0.55 + 0.45 * hueco)
    h = (h - h.min()) / (h.max() - h.min())
    h = np.clip(0.5 + (h - 0.5) * contraste, 0, 1)
    luz = h ** 1.25 * (1.0 + (ident - 0.5) * 0.30 + (macro - 0.5) * 0.30)
    rgb = np.zeros((N, N, 3))
    for k in range(3):
        rgb[..., k] = base[k] + (claro[k] - base[k]) * luz
    oscuro = np.array(junta, dtype=float)
    peso = np.clip((0.42 - h) * 3.0, 0, 1)[..., None]            # los huecos profundos tiran a la junta oscura
    rgb = rgb * (1 - peso) + oscuro * peso * (0.7 + 0.3 * luz[..., None])
    return np.clip(rgb, 0, 255).astype(np.uint8)


def guardar(ruta, a):
    Image.fromarray(a, "RGB").save(os.path.join(RAIZ, ruta), quality=90)


if __name__ == "__main__":
    muro = piedra(4401, 15, (60, 54, 50), (152, 136, 120), (28, 25, 24), 1.5, 6)     # basalto oscuro con piedras tostadas
    guardar("Models/fachada/piedra.jpg", muro)
    guardar("Models/arco/piedra.jpg", muro)
    piso = piedra(4402, 9, (88, 86, 84), (200, 196, 188), (48, 46, 46), 1.25, 6)       # lajas claras grises, rugosas
    guardar("Models/piso/piso_piedra.jpg", piso)
    print("ok")
