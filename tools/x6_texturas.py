"""X-6: texturas propias de 1K (1024 px) generadas por codigo (ruido fractal con numpy). Sin fuentes externas.
Uso (desde la raiz del repo):  python tools/x6_texturas.py
Genera: silueta_tela, mar_agua, tumba1..4_piedra, cruz_piedra, soportes_madera (cada una en su carpeta de Models/)."""
import numpy as np
from PIL import Image

N = 1024
rng = np.random.default_rng(15)  # semilla fija: mismo resultado siempre


def ruido(escala, semilla):
    """Ruido de valor suave y PERIODICO (se repite sin costuras): rejilla aleatoria + interpolacion con curva suave."""
    r = np.random.default_rng(semilla)
    g = r.random((escala, escala))
    x = np.linspace(0, escala, N, endpoint=False)
    i = x.astype(int)
    f = x - i
    u = f * f * (3 - 2 * f)
    i1 = (i + 1) % escala
    a = g[np.ix_(i, i)]; b = g[np.ix_(i, i1)]; c = g[np.ix_(i1, i)]; d = g[np.ix_(i1, i1)]
    ux = u[None, :]; uy = u[:, None]
    return (a * (1 - ux) + b * ux) * (1 - uy) + (c * (1 - ux) + d * ux) * uy


def fbm(base, octavas, semilla):
    tot = np.zeros((N, N)); amp = 1.0; suma = 0.0
    for k in range(octavas):
        tot += amp * ruido(base * 2 ** k, semilla + k); suma += amp; amp *= 0.5
    return tot / suma


def guarda(rgb, ruta):
    Image.fromarray(np.clip(rgb * 255, 0, 255).astype(np.uint8), 'RGB').save(ruta)
    print('->', ruta)


def mezcla(c1, c2, t):
    return np.array(c1)[None, None, :] * (1 - t[..., None]) + np.array(c2)[None, None, :] * t[..., None]


# --- Silueta: tela de algodon crudo con trama visible y hilos finos (clara: se ve contra el cielo de noche)
yy, xx = np.mgrid[0:N, 0:N]
trama = 0.5 + 0.25 * np.sin(xx * 2 * np.pi / 8) * np.sin(yy * 2 * np.pi / 8)
tela = mezcla((0.80, 0.74, 0.62), (0.93, 0.89, 0.78), 0.6 * trama + 0.4 * fbm(8, 4, 100))
franja = (np.sin(yy * 2 * np.pi / 128) > 0.985).astype(float) * 0.25  # hilos finos y apenas visibles
tela = tela * (1 - franja[..., None]) + np.array((0.75, 0.15, 0.12))[None, None, :] * franja[..., None]
guarda(tela, 'Models/silueta/silueta_tela.png')

# --- Mar: agua azul-turquesa con crestas claras (la espuma se suma en el shader por la luz especular)
n1 = fbm(4, 5, 200); n2 = fbm(8, 4, 300)
cresta = np.clip(1 - np.abs(n1 - 0.5) * 7, 0, 1) ** 3
agua = mezcla((0.03, 0.17, 0.30), (0.08, 0.38, 0.50), n2)
agua = agua + cresta[..., None] * np.array((0.25, 0.35, 0.35))[None, None, :]
guarda(agua, 'Models/mar/mar_agua.png')

# --- Piedra de tumba: marmol/cantera con vetas y manchas de musgo; un tinte distinto por tumba
tintes = {1: ((0.55, 0.53, 0.50), (0.78, 0.76, 0.72)), 2: ((0.52, 0.55, 0.55), (0.76, 0.78, 0.78)),
          3: ((0.58, 0.52, 0.45), (0.82, 0.76, 0.66)), 4: ((0.50, 0.50, 0.55), (0.74, 0.74, 0.80))}
for k, (oscuro, claro) in tintes.items():
    base = fbm(6, 6, 400 + k * 10)
    veta = np.clip(1 - np.abs(fbm(3, 4, 450 + k) - 0.5) * 9, 0, 1) ** 2
    piedra = mezcla(oscuro, claro, base)
    piedra = piedra * (1 - 0.35 * veta[..., None])
    musgo = np.clip((fbm(5, 5, 480 + k) - 0.58) * 6, 0, 1) * np.clip((yy / N) ** 0.5, 0, 1)
    piedra = piedra * (1 - musgo[..., None]) + np.array((0.22, 0.34, 0.15))[None, None, :] * musgo[..., None]
    guarda(piedra, f'Models/tumba{k}/tumba{k}_piedra.png')
cantera = mezcla((0.45, 0.43, 0.40), (0.72, 0.70, 0.66), fbm(6, 6, 600))
guarda(cantera, 'Models/cruz/cruz_piedra.png')

# --- Madera de los soportes: vetas verticales
veta = fbm(2, 3, 700)
fibra = 0.5 + 0.5 * np.sin((xx / N * 18 + veta * 14) * np.pi)
madera = mezcla((0.28, 0.16, 0.08), (0.52, 0.32, 0.17), 0.6 * fibra + 0.4 * fbm(10, 4, 710))
guarda(madera, 'Models/soportes/soportes_madera.png')

# --- Marco: fotografia antigua generica (silueta de busto en sepia, sin rostro: no representa a ninguna persona real)
from PIL import ImageDraw, ImageFilter
foto = Image.new('RGB', (N, N), (196, 168, 120))
d = ImageDraw.Draw(foto)
d.ellipse((330, 190, 694, 600), fill=(86, 62, 40))            # cabeza
d.rounded_rectangle((440, 560, 584, 700), 40, fill=(86, 62, 40))   # cuello
d.pieslice((120, 640, 904, 1500), 180, 360, fill=(70, 50, 34))     # hombros
fotoa = np.asarray(foto.filter(ImageFilter.GaussianBlur(14))).astype(float) / 255
vig = 1 - 0.55 * ((xx - N / 2) ** 2 + (yy - N / 2) ** 2) / (N / 2) ** 2 / 1.4
grano = 0.06 * (fbm(256, 2, 800) - 0.5)
guarda(np.clip(fotoa * vig[..., None] + grano[..., None], 0, 1), 'Models/marco/foto_retrato.png')
