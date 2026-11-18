# C-2 · Malla de la llama de las velas (act. 40). Dos planos verticales cruzados a 90 grados de 0.04 x 0.07 m,
# pivote en el centro de la base; UV 0..1 con v = 0 en la base. La forma, el color y el movimiento de la llama
# los calcula el fragment shader (Shaders/lighting.frag, ruido de Perlin); la textura es solo un relleno blanco
# porque Model.h siempre carga un diffuse. NO cuenta como modelo propio (es un recurso de la animacion).
# Uso: python3 tools/c2_llama.py   (necesita bpy y Pillow)
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from PIL import Image
import bpy, bmesh
import bpy_comun as c

ANCHO, ALTO = 0.04, 0.07
carpeta = os.path.join(c.RAIZ, "Models", "llama")
os.makedirs(carpeta, exist_ok=True)
png = os.path.join(carpeta, "llama.png")
Image.new("RGB", (1024, 1024), (255, 255, 255)).save(png)

c.limpiar()
malla = bpy.data.meshes.new("llama")
obj = bpy.data.objects.new("llama", malla)
bpy.context.collection.objects.link(obj)
bm = bmesh.new()
uv = bm.loops.layers.uv.verify()
h = ANCHO / 2

def plano(p):
    f = bm.faces.new([bm.verts.new(q) for q in p])
    for l, a in zip(f.loops, [(0, 0), (1, 0), (1, 1), (0, 1)]):
        l[uv].uv = a

plano([(-h, 0, 0), (h, 0, 0), (h, 0, ALTO), (-h, 0, ALTO)])    # vertical, plano X-Z de Blender
plano([(0, h, 0), (0, -h, 0), (0, -h, ALTO), (0, h, ALTO)])    # vertical cruzado
bm.to_mesh(malla); bm.free()
malla.materials.append(c.material("llama", png))
c.exportar(obj, "llama")
print("llama: caras =", len(malla.polygons))
