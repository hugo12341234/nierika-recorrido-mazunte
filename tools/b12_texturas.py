# B-12 · Texturas 1K propias de la ofrenda (altar3, vela, botella, marco, copalero) y mapa de relieve del pan.
#
# Uso (desde la raíz del repo):  python3 tools/b12_texturas.py
# Requiere: Python 3, numpy y Pillow. Escribe PNG de 1024×1024 dentro de Models/<objeto>/ y deja sus .mtl apuntando a ellos.
# Todo es procedural y determinista (semilla fija): es material PROPIO del estudio, sin fuentes externas que citar.
# Los PNG de las texturas se hacen **teselables** (el ruido se arma en frecuencia), así que no se ven costuras si la UV se repite.
import os
import re
import numpy as np
from PIL import Image

N = 1024
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))


def ruido(rng, octavas=5, f0=4, persistencia=0.5, estirado=(1.0, 1.0)):
    """Ruido fractal teselable en [0, 1]. `estirado` = (fx, fy) multiplica la frecuencia en cada eje (veta, hilo)."""
    fy = np.fft.fftfreq(N)[:, None]
    fx = np.fft.rfftfreq(N)[None, :]
    total = np.zeros((N, N))
    amp = 1.0
    for o in range(octavas):
        f = f0 * (2 ** o) / N
        # anillo de frecuencias alrededor de f (en el espacio de frecuencias anisótropo)
        r = np.sqrt((fx / estirado[0]) ** 2 + (fy / estirado[1]) ** 2)
        espectro = np.exp(-((r - f) ** 2) / (2 * (0.45 * f) ** 2)) * np.exp(1j * rng.uniform(0, 2 * np.pi, (N, N // 2 + 1)))
        capa = np.fft.irfft2(espectro, s=(N, N))
        capa /= np.abs(capa).max() + 1e-9
        total += amp * capa
        amp *= persistencia
    total -= total.min()
    return total / (total.max() + 1e-9)


def guardar(ruta, rgb):
    Image.fromarray(np.clip(rgb, 0, 255).astype(np.uint8), "RGB").save(os.path.join(RAIZ, ruta))


def mezcla(base, variacion, valor):
    """base (r,g,b) con variación multiplicativa: variacion en [-1,1] por píxel."""
    base = np.array(base, dtype=float)[None, None, :]
    return base * (1.0 + valor * variacion[..., None])


def altar3(rng):
    # Manta morada tejida: urdimbre y trama (seno 8 px) + variación de baja frecuencia
    y, x = np.mgrid[0:N, 0:N]
    tejido = np.sin(2 * np.pi * x / 8) * np.sin(2 * np.pi * y / 8)
    bajo = ruido(rng, 4, 3) * 2 - 1
    fino = ruido(rng, 3, 64) * 2 - 1
    v = 0.55 * tejido + 0.6 * bajo + 0.3 * fino
    return mezcla((88, 30, 66), v, 0.12)


def vela(rng):
    # Cera crema: veteado vertical (estirado en el eje de la altura) + motas oscuras de hollín
    veta = ruido(rng, 5, 3, 0.55, estirado=(1.0, 0.08)) * 2 - 1
    motas = (ruido(rng, 2, 96) > 0.86).astype(float)
    v = 0.8 * veta - 0.5 * motas
    return mezcla((226, 202, 149), v, 0.07)


def botella(rng):
    # Vidrio verde oscuro: vetas largas y burbujitas más claras
    veta = ruido(rng, 4, 2, 0.5, estirado=(1.0, 0.1)) * 2 - 1
    burb = (ruido(rng, 1, 110) > 0.93).astype(float)
    rgb = mezcla((14, 66, 36), veta, 0.35)
    return rgb + burb[..., None] * np.array([40, 70, 50])[None, None, :]


def marco(rng):
    # Madera oscura: veta horizontal fuerte + nudos suaves
    veta = ruido(rng, 5, 2, 0.6, estirado=(0.05, 1.0)) * 2 - 1
    anillos = np.sin(2 * np.pi * (np.arange(N)[:, None] / N * 14 + 1.5 * ruido(rng, 3, 2)))
    v = 0.8 * veta + 0.25 * anillos
    return mezcla((70, 36, 14), v, 0.28)


def copalero(rng):
    # Barro cocido: grano fino + bandas horizontales más oscuras (bruñido y humo del copal)
    grano = ruido(rng, 4, 48) * 2 - 1
    bajo = ruido(rng, 3, 3) * 2 - 1
    bandas = np.sin(2 * np.pi * np.arange(N)[:, None] / N * 6)[:, :] * np.ones((1, N))
    v = 0.5 * grano + 0.6 * bajo + 0.35 * bandas
    return mezcla((136, 62, 36), v, 0.22)


def relieve_pan(rng):
    # Mapa de altura (blanco = alto) del pan: grano de masa + pellizcos de azúcar. Gris en 3 canales.
    masa = ruido(rng, 6, 6, 0.55)
    azucar = (ruido(rng, 2, 80) > 0.62).astype(float) * 0.25
    h = np.clip(0.75 * masa + azucar, 0, 1)
    h = (h - h.min()) / (h.max() - h.min())
    g = h * 255
    return np.stack([g, g, g], axis=-1)


def escribir_mtl(objeto, reemplazos, extra_bump=None):
    """Cambia los map_Kd del .mtl (y agrega map_Bump si se pide)."""
    ruta = os.path.join(RAIZ, "Models", objeto, objeto + ".mtl")
    with open(ruta, encoding="utf-8") as f:
        txt = f.read()
    for viejo, nuevo in reemplazos.items():
        txt = txt.replace("map_Kd " + viejo, "map_Kd " + nuevo)
    if extra_bump and "map_Bump" not in txt:
        txt = txt.rstrip("\n") + "\nmap_Bump " + extra_bump + "\n"
    with open(ruta, "w", encoding="utf-8") as f:
        f.write(txt)


if __name__ == "__main__":
    rng = np.random.default_rng(12)
    trabajo = [
        ("altar3", "altar3_tela.png", altar3, {"altar3_provisional.png": "altar3_tela.png"}),
        ("vela", "vela_cera.png", vela, {"vela_provisional.png": "vela_cera.png"}),
        ("botella", "botella_vidrio.png", botella, {"botella_provisional.png": "botella_vidrio.png"}),
        ("marco", "marco_madera.png", marco, {"marco_provisional.png": "marco_madera.png"}),
        ("copalero", "copalero_barro.png", copalero, {"copalero_provisional.png": "copalero_barro.png"}),
    ]
    for objeto, archivo, f, repl in trabajo:
        guardar(os.path.join("Models", objeto, archivo), f(rng))
        escribir_mtl(objeto, repl)
        print(f"Models/{objeto}/{archivo}")
    guardar(os.path.join("Models", "pan_muerto", "pan_muerto_relieve.png"), relieve_pan(rng))
    escribir_mtl("pan_muerto", {}, extra_bump="pan_muerto_relieve.png")
    print("Models/pan_muerto/pan_muerto_relieve.png")
