r"""X-52: modelo propio `altar_piramide` (altar de 5 niveles con dosel, para la plaza detras de la capilla). Blender 5.x con el modulo bpy.
Uso (raiz del repo):   python tools/x52_altar_piramide.py
Genera Models/altar_piramide/{altar_piramide.obj,.mtl,.blend} y 4 texturas PNG de 1024x1024 (hechas aqui, sin fuentes externas).
Medidas reales (1 u = 1 m). Se define en coordenadas del JUEGO (x derecha, y arriba, z hacia el frente +z) y se pasa a Blender con
(x, y, z) -> (x, -z, y); el exportador usa Forward -Z / Up Y, asi que el .obj queda de nuevo en coordenadas del juego.
  Huella 3.2 x 3.2 m, 5 niveles (topes 0.30..1.50 m) con faldones de color, 4 columnas de carrizo de 2.6 m y travesanos de dosel (2.5-2.6 m)."""
import math, os, sys
import numpy as np
from PIL import Image
import bpy, bmesh
from mathutils import Matrix, Vector

RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
CARPETA = os.path.join(RAIZ, "Models", "altar_piramide")
os.makedirs(CARPETA, exist_ok=True)
rng = np.random.default_rng(50)


# ----------------------------------------------------------------- texturas 1K (1 tile = 1 m)
def guardar(nombre, arr):
    Image.fromarray(arr.astype(np.uint8), "RGB").save(os.path.join(CARPETA, nombre))


def hexa(h): return np.array([int(h[i:i + 2], 16) for i in (0, 2, 4)], dtype=float)


def tex_tela():
    """Mantel bordado: magenta con hileras de rombos (punto de cruz) naranja, turquesa y blanco."""
    N = 1024; celda = 64
    img = np.zeros((N, N, 3)); img[:] = hexa("0f6b73")
    yy, xx = np.mgrid[0:N, 0:N]
    cx = (xx % celda) - celda / 2 + 0.5; cy = (yy % celda) - celda / 2 + 0.5
    fila = (yy // celda); col = (xx // celda)
    rombo = (np.abs(cx) + np.abs(cy)) < 22
    rombo_in = (np.abs(cx) + np.abs(cy)) < 11
    punto = (np.abs(cx) + np.abs(cy)) < 4
    # filas alternas de colores
    naranja = (fila % 2 == 0); turq = ~naranja
    img[rombo & naranja] = hexa("fcd34d"); img[rombo & turq] = hexa("e11d74")
    img[rombo_in & naranja] = hexa("7c1d6f"); img[rombo_in & turq] = hexa("f6e7c1")
    img[punto] = hexa("fff3d0")
    # franjas horizontales cada 4 filas
    banda = (fila % 4 == 3)
    img[banda & ~rombo] = hexa("0a4a50")
    img += rng.normal(0, 4, img.shape)       # grano de hilo
    return np.clip(img, 0, 255)


def tex_borde():
    """Faldon: naranja cempasuchil con dientes de sierra magenta y blancos."""
    N = 1024; diente = 64
    img = np.zeros((N, N, 3)); img[:] = hexa("fcd34d")
    yy, xx = np.mgrid[0:N, 0:N]
    t = (xx % diente) / diente
    tri = 1 - np.abs(t * 2 - 1)                 # 0..1..0
    altura = (yy / N)                           # 0 arriba .. 1 abajo
    img[(altura > 0.55 - 0.30 * tri) & (altura < 0.80 - 0.30 * tri)] = hexa("7c1d6f")
    img[(altura > 0.80 - 0.30 * tri) & (altura < 0.88 - 0.30 * tri)] = hexa("fff3d0")
    img[altura < 0.12] = hexa("e11d74")
    img += rng.normal(0, 4, img.shape)
    return np.clip(img, 0, 255)


def tex_madera():
    N = 1024
    base = hexa("6b3f1d")
    veta = np.sin((np.arange(N)[:, None] * 0.07) + np.cumsum(rng.normal(0, 0.15, N))[:, None]) * 12
    ruido = rng.normal(0, 6, (N, N))
    img = base[None, None, :] + veta[:, :, None] + ruido[:, :, None]
    return np.clip(img, 0, 255)


guardar("altar_piramide_tela.png", tex_tela())
guardar("altar_piramide_borde.png", tex_borde())
guardar("altar_piramide_madera.png", tex_madera())


# ----------------------------------------------------------------- geometria
def pb(p):
    return (p[0], -p[2], p[1])


def caja(bm, x0, x1, y0, y1, z0, z1):
    c = ((x0 + x1) / 2, (y0 + y1) / 2, (z0 + z1) / 2)
    d = (x1 - x0, y1 - y0, z1 - z0)
    m = Matrix.Translation(Vector(pb(c))) @ Matrix.Diagonal((d[0], d[2], d[1], 1.0))
    bmesh.ops.create_cube(bm, size=1.0, matrix=m)


def columna(bm, x, z, radio, y0, y1, seg=14):
    m = Matrix.Translation(Vector(pb((x, (y0 + y1) / 2, z))))
    bmesh.ops.create_cone(bm, cap_ends=True, cap_tris=False, segments=seg, radius1=radio, radius2=radio, depth=y1 - y0, matrix=m)


def uv_caja(bm, m_por_tile):
    uv = bm.loops.layers.uv.verify()
    for f in bm.faces:
        n = f.normal
        ax = max(range(3), key=lambda i: abs(n[i]))
        a, b = [(1, 2), (0, 2), (0, 1)][ax]
        for l in f.loops:
            c = l.vert.co
            l[uv].uv = (c[a] / m_por_tile, c[b] / m_por_tile)


def objeto(nombre, bm, tex, m_por_tile):
    uv_caja(bm, m_por_tile)
    me = bpy.data.meshes.new(nombre); bm.to_mesh(me); bm.free()
    ob = bpy.data.objects.new(nombre, me); bpy.context.scene.collection.objects.link(ob)
    mat = bpy.data.materials.new(nombre)
    if mat.node_tree is None: mat.use_nodes = True
    nt = mat.node_tree
    p = next(n for n in nt.nodes if n.type == "BSDF_PRINCIPLED")
    t = nt.nodes.new("ShaderNodeTexImage")
    t.image = bpy.data.images.load(os.path.join(CARPETA, tex))
    nt.links.new(t.outputs["Color"], p.inputs["Base Color"])
    p.inputs["Roughness"].default_value = 0.85
    me.materials.append(mat)
    return ob


bpy.ops.wm.read_factory_settings(use_empty=True)

# 5 niveles cuadrados de 0.30 m de alto: semilado 1.60, 1.30, 1.00, 0.70, 0.40 (topes 0.30 .. 1.50)
SEMI = [1.60, 1.30, 1.00, 0.70, 0.40]
bm = bmesh.new(); bf = bmesh.new()
for k, s in enumerate(SEMI):
    y0 = 0.30 * k; y1 = y0 + 0.30
    caja(bm, -s, s, y0, y1 - 0.10, -s, s)
    caja(bm, -s, s, y1 - 0.10, y1, -s, s)
    caja(bf, -s - 0.02, s + 0.02, y1 - 0.26, y1 - 0.10, -s - 0.02, s + 0.02)   # faldon
tela = objeto("altar_piramide_tela", bm, "altar_piramide_tela.png", 1.0)
borde = objeto("altar_piramide_borde", bf, "altar_piramide_borde.png", 0.6)

# dosel de carrizo: 4 columnas en las esquinas (semilado 1.50) de 2.60 m, travesanos a 2.50-2.60 m y 4 peldanos bajos
bw = bmesh.new()
for sx in (-1.50, 1.50):
    for sz in (-1.50, 1.50):
        columna(bw, sx, sz, 0.06, 0.0, 2.60)
        caja(bw, sx - 0.10, sx + 0.10, 0.0, 0.08, sz - 0.10, sz + 0.10)
for sz in (-1.50, 1.50):
    caja(bw, -1.56, 1.56, 2.50, 2.60, sz - 0.04, sz + 0.04)
for sx in (-1.50, 1.50):
    caja(bw, sx - 0.04, sx + 0.04, 2.50, 2.60, -1.50, 1.50)
madera = objeto("altar_piramide_madera", bw, "altar_piramide_madera.png", 1.0)

for o in (borde, madera):
    o.select_set(True)
tela.select_set(True)
bpy.context.view_layer.objects.active = tela
bpy.ops.object.join()
ob = bpy.context.view_layer.objects.active
ob.name = "altar_piramide"
bpy.ops.object.select_all(action="DESELECT"); ob.select_set(True)
bpy.ops.wm.obj_export(filepath=os.path.join(CARPETA, "altar_piramide.obj"), export_selected_objects=True, apply_modifiers=True,
                      export_materials=True, path_mode="STRIP", forward_axis="NEGATIVE_Z", up_axis="Y", global_scale=1.0,
                      export_uv=True, export_normals=True)
bpy.ops.wm.save_as_mainfile(filepath=os.path.join(CARPETA, "altar_piramide.blend"), copy=True)
xs = [v.co.x for v in ob.data.vertices]; ys = [v.co.z for v in ob.data.vertices]; zs = [-v.co.y for v in ob.data.vertices]
print("altar_piramide: x %.2f..%.2f  y %.2f..%.2f  z %.2f..%.2f  caras %d" % (min(xs), max(xs), min(ys), max(ys), min(zs), max(zs), len(ob.data.polygons)))
