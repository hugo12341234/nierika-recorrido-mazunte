# B-7 · Papel picado: malla plana subdividida con UV y textura RGBA con alfa (acts. 25 y 33). Modelo propio.
# Una tira de 1.70 m de ancho (los postes de `soportes` estan separados 1.80 m) y 0.50 m de alto, con 5 banderines de
# colores, orilla inferior en dientes y perforaciones; el alfa de la textura recorta todo (el shader descarta alfa < 0.1).
# Malla de 34 x 10 cuadros de 0.05 m (se puede ondear con un vertex shader sin pedir mas vertices).
# Pivote en el centro del borde superior (el papel cuelga de ahi, y = 0 hacia abajo).
# La textura es de 1024 x 1024 pero se estira a 1.70 x 0.50 m: el dibujo se calcula en metros para que se vea sin deformar.
# Uso: python3 tools/b7_papel_picado.py   (necesita bpy, Pillow y numpy)
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import numpy as np
from PIL import Image
import bpy, bmesh
import bpy_comun as c

ANCHO, ALTO = 1.70, 0.50
NX, NY = 34, 10
NOMBRE = "papel_picado"
COLORES = [(214, 40, 120), (245, 130, 20), (250, 200, 30), (40, 170, 90), (30, 120, 200)]

def textura(ruta, n=1024):
    j, i = np.mgrid[0:n, 0:n]
    x = (i + 0.5) / n * ANCHO            # metros desde el borde izquierdo
    y = (j + 0.5) / n * ALTO             # metros desde el borde superior (la fila 0 de la imagen es el borde superior)
    rgba = np.zeros((n, n, 4), np.uint8)
    paso = ANCHO / 5
    k = np.minimum((x // paso).astype(int), 4)
    xl = x - (k + 0.5) * paso            # distancia al centro de su banderin
    ancho_b = 0.30
    en_tira = y < 0.05
    # orilla inferior en dientes: profundidad 0.035 m cada 0.06 m
    diente = 0.035 * np.abs(((xl + 0.15) % 0.06) / 0.06 * 2 - 1)
    fondo = 0.5 - diente
    en_banderin = (np.abs(xl) < ancho_b / 2) & (y >= 0.05) & (y < fondo)
    cuerpo = en_tira | en_banderin
    # perforaciones: malla de rombos de 0.06 m + medallon central con anillo
    yy = y - 0.05
    hx = (xl + 0.03) % 0.06 - 0.03
    hy = (yy + 0.03) % 0.06 - 0.03
    rombos = (np.abs(hx) + np.abs(hy) < 0.017) & (np.abs(xl) < ancho_b / 2 - 0.035) & (yy > 0.035) & (y < fondo - 0.05)
    cy = 0.24
    r = np.hypot(xl, y - cy)
    medallon = (r < 0.085) & ~((r > 0.045) & (r < 0.062))        # circulo grande con anillo que queda de papel
    medallon_hueco = (r < 0.045) & (np.hypot(xl, y - cy) > 0.02)   # corona perforada, centro de papel
    perforado = (rombos & (r > 0.095)) | ((r > 0.062) & (r < 0.085)) | medallon_hueco
    visible = cuerpo & ~perforado
    paleta = np.array(COLORES, np.uint8)
    rgba[..., :3] = paleta[k]
    rgba[..., 3] = (visible * 255).astype(np.uint8)
    Image.fromarray(rgba, "RGBA").save(ruta)

carpeta = os.path.join(c.RAIZ, "Models", NOMBRE)
os.makedirs(carpeta, exist_ok=True)
png = os.path.join(carpeta, NOMBRE + ".png")
textura(png)

c.limpiar()
malla = bpy.data.meshes.new(NOMBRE)
obj = bpy.data.objects.new(NOMBRE, malla)
bpy.context.collection.objects.link(obj)
bm = bmesh.new()
# Blender: X = juego X, Z = juego Y (arriba), Y = -juego Z. Fila 0 = borde superior (z = 0), baja hasta z = -ALTO.
v = [[bm.verts.new((-ANCHO / 2 + ANCHO * a / NX, 0.0, -ALTO * b / NY)) for a in range(NX + 1)] for b in range(NY + 1)]
uv = bm.loops.layers.uv.verify()
for b in range(NY):
    for a in range(NX):
        # orden que deja la normal hacia -Y de Blender (= +Z del juego, hacia donde mira la entrada del panteon)
        f = bm.faces.new([v[b][a], v[b][a + 1], v[b + 1][a + 1], v[b + 1][a]])
        f.normal_flip()
        for loop in f.loops:
            p = loop.vert.co
            loop[uv].uv = ((p.x + ANCHO / 2) / ANCHO, 1.0 + p.z / ALTO)
bmesh.ops.recalc_face_normals(bm, faces=bm.faces[:])
bm.to_mesh(malla); bm.free()
malla.materials.append(c.material(NOMBRE, png, alfa=True))
c.exportar(obj, NOMBRE)
print(f"{NOMBRE}: caras={len(malla.polygons)} vertices={len(malla.vertices)}")
