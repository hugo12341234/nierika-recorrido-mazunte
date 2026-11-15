r"""X-41: textura del tronco de la palma (Models/palma/palma_tronco.png), 1024 x 1024, teselable en u y v.
Sustituye a la de X-9, cuyas bandas onduladas se leian como un zigzag (chevron) repetido sobre el tronco. Aqui: anillos horizontales
casi rectos (las cicatrices de las hojas viejas, con una ligera irregularidad), fibras verticales y grano fino. Solo numpy + PIL.
Uso (raiz del repo):  python tools/x41_tronco_palma.py
El mapeo UV no cambia (u 0..2 alrededor del tronco, v 0..3 a lo largo: ~2.8 m de tronco por repeticion; 16 anillos por repeticion = 1 cada 17 cm)."""
import math, os
import numpy as np
from PIL import Image

RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
N = 1024
rng = np.random.default_rng(41)


def ruido_periodico(celdas_y, celdas_x, semilla):
    """Ruido de valor suave (smoothstep), periodico en ambos ejes, de N x N."""
    g = np.random.default_rng(semilla).random((celdas_y, celdas_x))
    y = np.linspace(0, celdas_y, N, endpoint=False); x = np.linspace(0, celdas_x, N, endpoint=False)
    iy = y.astype(int); ix = x.astype(int); fy = y - iy; fx = x - ix
    uy = fy * fy * (3 - 2 * fy); ux = fx * fx * (3 - 2 * fx)
    iy1 = (iy + 1) % celdas_y; ix1 = (ix + 1) % celdas_x
    a = g[np.ix_(iy, ix)]; b = g[np.ix_(iy, ix1)]; c = g[np.ix_(iy1, ix)]; d = g[np.ix_(iy1, ix1)]
    return (a * (1 - ux[None, :]) + b * ux[None, :]) * (1 - uy[:, None]) + (c * (1 - ux[None, :]) + d * ux[None, :]) * uy[:, None]


y = np.arange(N)[:, None] * np.ones((1, N)); x = np.arange(N)[None, :] * np.ones((N, 1))
# anillos casi horizontales: 16 por tile; la fase se desplaza un poco con un ruido suave (irregularidad, no zigzag)
fase = y * 2 * math.pi / 64 + 1.1 * (ruido_periodico(4, 8, 410) - 0.5)
anillo = (0.5 + 0.5 * np.sin(fase)) ** 1.6                      # franja clara ancha y cicatriz oscura angosta
# fibras verticales: franjas finas (128 columnas) que varian despacio a lo largo del tronco
fibra = 0.55 * ruido_periodico(4, 128, 411) + 0.45 * ruido_periodico(16, 256, 412)
grano = ruido_periodico(256, 256, 413)
base = np.clip(0.50 * anillo + 0.38 * fibra + 0.12 * grano, 0, 1)
base = (base - base.min()) / (base.max() - base.min())
c1 = np.array((0.19, 0.13, 0.08)); c2 = np.array((0.50, 0.40, 0.29))
img = c1[None, None, :] * (1 - base[..., None]) + c2[None, None, :] * base[..., None]
Image.fromarray((np.clip(img, 0, 1) * 255).astype(np.uint8), "RGB").save(os.path.join(RAIZ, "Models", "palma", "palma_tronco.png"))
print("palma_tronco.png OK")
