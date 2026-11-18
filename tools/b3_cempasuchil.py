# B-3 · Flor de cempasuchil con planos cruzados (act. 28). Modelo propio de Nierika Studio.
# Dos planos verticales cruzados a 90 grados + un plano horizontal (se ve desde cualquier lado y desde arriba),
# cada uno un cuadrado de 0.07 m con la misma textura RGBA (alfa recortado en la silueta de la flor).
# 6 triangulos en total: pensado para instancing (muchas copias). Pivote en el centro de la flor.
# Uso: python3 tools/b3_cempasuchil.py   (necesita bpy, Pillow y numpy)
import math, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import numpy as np
from PIL import Image
import bpy, bmesh
import bpy_comun as c

LADO = 0.07          # diametro de la flor en metros (cabeza real de ~5-8 cm)
N = 1024             # textura 1K

def textura(ruta):
    """Roseta de petalos: anillos naranja/amarillo, centro mas oscuro, borde festoneado; alfa 0 fuera de la flor."""
    rng = np.random.default_rng(3)
    y, x = np.mgrid[0:N, 0:N]
    u = (x + 0.5) / N * 2 - 1
    v = (y + 0.5) / N * 2 - 1
    r = np.hypot(u, v)
    th = np.arctan2(v, u)
    borde = 0.93 + 0.05 * np.cos(18 * th) + 0.02 * np.cos(31 * th + 1.0)   # festoneado de petalos
    alfa = (r < borde).astype(np.float32)
    # capas de petalos: franjas radiales + anillos
    petalo = 0.5 + 0.5 * np.cos(18 * th + 6 * r)
    t = np.clip(r / borde, 0, 1)
    naranja = np.array([240, 120, 10], np.float32)
    amarillo = np.array([255, 190, 30], np.float32)
    oscuro = np.array([150, 60, 5], np.float32)
    mezcla = (1 - t)[..., None] * oscuro + t[..., None] * naranja
    mezcla = mezcla * (0.75 + 0.25 * petalo)[..., None] + (petalo * (t > 0.55) * 0.35)[..., None] * (amarillo - naranja)
    mezcla += rng.normal(0, 4, (N, N, 1))                                   # grano
    rgb = np.clip(mezcla, 0, 255).astype(np.uint8)
    # fuera de la flor el color queda igual al borde (evita halos al filtrar con mipmaps)
    img = np.dstack([rgb, (alfa * 255).astype(np.uint8)])
    Image.fromarray(img, "RGBA").save(ruta)

carpeta = os.path.join(c.RAIZ, "Models", "cempasuchil")
os.makedirs(carpeta, exist_ok=True)
png = os.path.join(carpeta, "cempasuchil.png")
textura(png)

c.limpiar()
malla = bpy.data.meshes.new("cempasuchil")
obj = bpy.data.objects.new("cempasuchil", malla)
bpy.context.collection.objects.link(obj)
bm = bmesh.new()
h = LADO / 2

def plano(esquinas):
    vs = [bm.verts.new(p) for p in esquinas]
    f = bm.faces.new(vs)
    uv = bm.loops.layers.uv.verify()
    for loop, (a, b) in zip(f.loops, [(0, 0), (1, 0), (1, 1), (0, 1)]):
        loop[uv].uv = (a, b)

# Blender: X = juego X, Y = -juego Z, Z = juego Y. Normales hacia +Y/+X (cara visible; el shader invierte la normal por la parte de atras).
plano([(-h, 0, -h), (h, 0, -h), (h, 0, h), (-h, 0, h)])        # vertical (plano X-Z de Blender, frente al jugador)
plano([(0, h, -h), (0, -h, -h), (0, -h, h), (0, h, h)])        # vertical cruzado
plano([(-h, -h, 0), (h, -h, 0), (h, h, 0), (-h, h, 0)])        # horizontal
bm.to_mesh(malla); bm.free()
malla.materials.append(c.material("cempasuchil", png, alfa=True))
c.exportar(obj, "cempasuchil")
print("cempasuchil: caras =", len(malla.polygons), "vertices =", len(malla.vertices))
