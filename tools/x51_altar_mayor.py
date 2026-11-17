r"""X-51: modelo propio `altar_mayor` (ofrenda mayor del patio del panteon). Blender 5.x con el modulo bpy.
Uso (raiz del repo):   python tools/x51_altar_mayor.py
Genera Models/altar_mayor/{altar_mayor.obj,.mtl,.blend} y 4 texturas PNG de 1024x1024 (hechas aqui, sin fuentes externas).
Medidas reales (1 u = 1 m). Se define en coordenadas del JUEGO (x derecha, y arriba, z hacia el frente +z) y se pasa a Blender con
(x, y, z) -> (x, -z, y); el exportador usa Forward -Z / Up Y, asi que el .obj queda de nuevo en coordenadas del juego.
  Huella 3.6 x 2.0 m (x -1.8..1.8, z -1.0..1.0), 4 niveles escalonados (alturas de tope 0.40, 0.80, 1.20, 1.55 m),
  retablo trasero con cresta curva (hasta 2.9 m), dos columnas con travesano (2.75 m) y faldones de color en cada nivel."""
import math, os, sys
import numpy as np
from PIL import Image
import bpy, bmesh
from mathutils import Matrix, Vector

RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
CARPETA = os.path.join(RAIZ, "Models", "altar_mayor")
os.makedirs(CARPETA, exist_ok=True)
rng = np.random.default_rng(50)


# ----------------------------------------------------------------- texturas 1K (1 tile = 1 m)
def guardar(nombre, arr):
    Image.fromarray(arr.astype(np.uint8), "RGB").save(os.path.join(CARPETA, nombre))


def hexa(h): return np.array([int(h[i:i + 2], 16) for i in (0, 2, 4)], dtype=float)


def tex_tela():
    """Mantel bordado: magenta con hileras de rombos (punto de cruz) naranja, turquesa y blanco."""
    N = 1024; celda = 64
    img = np.zeros((N, N, 3)); img[:] = hexa("8e1b63")
    yy, xx = np.mgrid[0:N, 0:N]
    cx = (xx % celda) - celda / 2 + 0.5; cy = (yy % celda) - celda / 2 + 0.5
    fila = (yy // celda); col = (xx // celda)
    rombo = (np.abs(cx) + np.abs(cy)) < 22
    rombo_in = (np.abs(cx) + np.abs(cy)) < 11
    punto = (np.abs(cx) + np.abs(cy)) < 4
    # filas alternas de colores
    naranja = (fila % 2 == 0); turq = ~naranja
    img[rombo & naranja] = hexa("f08a1c"); img[rombo & turq] = hexa("19a3a0")
    img[rombo_in & naranja] = hexa("b3122e"); img[rombo_in & turq] = hexa("f6e7c1")
    img[punto] = hexa("fff3d0")
    # franjas horizontales cada 4 filas
    banda = (fila % 4 == 3)
    img[banda & ~rombo] = hexa("5c0f45")
    img += rng.normal(0, 4, img.shape)       # grano de hilo
    return np.clip(img, 0, 255)


def tex_borde():
    """Faldon: naranja cempasuchil con dientes de sierra magenta y blancos."""
    N = 1024; diente = 64
    img = np.zeros((N, N, 3)); img[:] = hexa("f08a1c")
    yy, xx = np.mgrid[0:N, 0:N]
    t = (xx % diente) / diente
    tri = 1 - np.abs(t * 2 - 1)                 # 0..1..0
    altura = (yy / N)                           # 0 arriba .. 1 abajo
    img[(altura > 0.55 - 0.30 * tri) & (altura < 0.80 - 0.30 * tri)] = hexa("b3122e")
    img[(altura > 0.80 - 0.30 * tri) & (altura < 0.88 - 0.30 * tri)] = hexa("fff3d0")
    img[altura < 0.12] = hexa("19a3a0")
    img += rng.normal(0, 4, img.shape)
    return np.clip(img, 0, 255)


def tex_madera():
    N = 1024
    base = hexa("6b3f1d")
    veta = np.sin((np.arange(N)[:, None] * 0.07) + np.cumsum(rng.normal(0, 0.15, N))[:, None]) * 12
    ruido = rng.normal(0, 6, (N, N))
    img = base[None, None, :] + veta[:, :, None] + ruido[:, :, None]
    return np.clip(img, 0, 255)


def tex_retablo():
    """Retablo: violeta oscuro con flores de cempasuchil (radiales) y estrellas doradas."""
    N = 1024
    img = np.zeros((N, N, 3)); img[:] = hexa("33124a")
    yy, xx = np.mgrid[0:N, 0:N]
    celda = 128
    cx = (xx % celda) - celda / 2; cy = (yy % celda) - celda / 2
    r = np.hypot(cx, cy); ang = np.arctan2(cy, cx)
    # el desfase alterna por fila para un tresbolillo
    desf = ((yy // celda) % 2) * (celda // 2)
    cx = ((xx + desf) % celda) - celda / 2
    r = np.hypot(cx, cy); ang = np.arctan2(cy, cx)
    petalos = r < (30 + 9 * np.cos(ang * 12))
    img[petalos] = hexa("f59a12")
    img[r < 20] = hexa("c2410c")
    img[r < 8] = hexa("fde68a")
    img += rng.normal(0, 3, img.shape)
    return np.clip(img, 0, 255)


guardar("altar_mayor_tela.png", tex_tela())
guardar("altar_mayor_borde.png", tex_borde())
guardar("altar_mayor_madera.png", tex_madera())
guardar("altar_mayor_retablo.png", tex_retablo())


# ----------------------------------------------------------------- geometria
def pb(p):  # juego -> Blender
    return (p[0], -p[2], p[1])


def caja(bm, x0, x1, y0, y1, z0, z1):
    c = ((x0 + x1) / 2, (y0 + y1) / 2, (z0 + z1) / 2)
    d = (x1 - x0, y1 - y0, z1 - z0)
    m = Matrix.Translation(Vector(pb(c))) @ Matrix.Diagonal((d[0], d[2], d[1], 1.0))   # ejes Blender: (x, -z, y)
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

# niveles: (x_semi, z0, z1, y0, y1)
NIV = [(1.80, -1.00, 1.00, 0.00, 0.40), (1.50, -0.85, 0.55, 0.40, 0.80), (1.20, -0.70, 0.10, 0.80, 1.20), (0.85, -0.55, -0.25, 1.20, 1.55)]
bm = bmesh.new(); bf = bmesh.new()
for (xs, z0, z1, y0, y1) in NIV:
    caja(bm, -xs, xs, y0, y1 - 0.12 if y0 > 0 else y1 - 0.12, z0, z1)         # cuerpo (el faldon cubre los 0.12 m de arriba... ver abajo)
    caja(bm, -xs, xs, y1 - 0.12, y1, z0, z1)                                  # tapa del mismo mantel
    # faldon que cuelga por el frente y los lados (0.18 m de alto, 2 cm mas grande)
    caja(bf, -xs - 0.02, xs + 0.02, y1 - 0.30, y1 - 0.12, z0 - 0.02, z1 + 0.02)
tela = objeto("altar_mayor_tela", bm, "altar_mayor_tela.png", 1.0)
borde = objeto("altar_mayor_borde", bf, "altar_mayor_borde.png", 0.6)

# retablo: cresta curva extruida (espesor 0.10 m) detras del nivel inferior
bmr = bmesh.new()
pts = []
N = 40
for i in range(N + 1):
    x = -1.5 + 3.0 * i / N
    y = 2.20 + 0.70 * max(0.0, 1 - (x / 1.5) ** 2) ** 0.8
    pts.append((x, y))
zr0, zr1 = -0.98, -0.88
base = [(1.5, 0.0), (-1.5, 0.0)]
perfil = [(-1.5, 0.0)] + [p for p in pts] + [(1.5, 0.0)]
# frente y atras como poligonos y laterales como quads
def cara(z):
    return [bmr.verts.new(pb((x, y, z))) for (x, y) in perfil]
vf = cara(zr1); vb = cara(zr0)
bmr.faces.new(vf)
bmr.faces.new(list(reversed(vb)))
for i in range(len(perfil)):
    j = (i + 1) % len(perfil)
    bmr.faces.new([vf[i], vb[i], vb[j], vf[j]])
bmesh.ops.recalc_face_normals(bmr, faces=bmr.faces[:])
retablo = objeto("altar_mayor_retablo", bmr, "altar_mayor_retablo.png", 1.5)

# estructura de madera: 2 columnas, travesano y repisa sobre el retablo
bw = bmesh.new()
for sx in (-1.70, 1.70):
    columna(bw, sx, -0.90, 0.07, 0.0, 2.75)
    caja(bw, sx - 0.12, sx + 0.12, 0.0, 0.10, -1.02, -0.78)       # base
    caja(bw, sx - 0.10, sx + 0.10, 2.65, 2.75, -1.00, -0.80)      # capitel
caja(bw, -1.80, 1.80, 2.68, 2.78, -0.96, -0.84)                   # travesano
# peldano de madera al frente
caja(bw, -1.90, 1.90, 0.0, 0.06, 1.00, 1.30)
madera = objeto("altar_mayor_madera", bw, "altar_mayor_madera.png", 1.0)

for o in (borde, retablo, madera):
    o.select_set(True)
tela.select_set(True)
bpy.context.view_layer.objects.active = tela
bpy.ops.object.join()
ob = bpy.context.view_layer.objects.active
ob.name = "altar_mayor"
bpy.ops.object.select_all(action="DESELECT"); ob.select_set(True)
bpy.ops.wm.obj_export(filepath=os.path.join(CARPETA, "altar_mayor.obj"), export_selected_objects=True, apply_modifiers=True,
                      export_materials=True, path_mode="STRIP", forward_axis="NEGATIVE_Z", up_axis="Y", global_scale=1.0,
                      export_uv=True, export_normals=True)
bpy.ops.wm.save_as_mainfile(filepath=os.path.join(CARPETA, "altar_mayor.blend"), copy=True)
xs = [v.co.x for v in ob.data.vertices]; ys = [v.co.z for v in ob.data.vertices]; zs = [-v.co.y for v in ob.data.vertices]
print("altar_mayor: x %.2f..%.2f  y %.2f..%.2f  z %.2f..%.2f  caras %d" % (min(xs), max(xs), min(ys), max(ys), min(zs), max(zs), len(ob.data.polygons)))
