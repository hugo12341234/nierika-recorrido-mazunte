r"""X-55: cuatro modelos propios nuevos para las ofrendas familiares de la playa (opcion C de Hugo, 17/11/2026).
Blender 5.x con el modulo bpy. Uso (raiz del repo):   python tools/x55_ofrendas_modelos.py
Genera en Models/<nombre>/ el .obj, .mtl, .blend y las texturas PNG de 1024x1024 (hechas aqui, sin fuentes externas):
  mesa_ofrenda  mesa con mantel claro (se tine en el juego por familia) y patas de madera; tablero 1.20 x 0.70 m a 0.78 m de alto
  veladora      vela en vaso de vidrio (0.10 m de diametro, 0.20 m de alto); el vidrio se tine por familia
  calabaza      calabaza acostillada de 0.22 m de diametro (pivote en la base)
  naranjas      monton de 5 naranjas de 8 cm (pivote en la base; se tine a limon o mandarina por familia)
  patio_tierra  plano de tierra apisonada de 13.6 x 9.0 m (x -6.8..6.8, z 3.2..12.2, y 0.015) bajo el patio de la capilla: las 4 tumbas dejan de estar sobre arena
Medidas reales (1 u = 1 m). Las texturas son casi neutras a proposito: el juego las multiplica por un color por familia (uniform tinteObjeto)."""
import math, os
import numpy as np
from PIL import Image
import bpy, bmesh
from mathutils import Matrix, Vector

RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
rng = np.random.default_rng(55)


def guardar(carpeta, nombre, arr):
    os.makedirs(carpeta, exist_ok=True)
    Image.fromarray(np.clip(arr, 0, 255).astype(np.uint8), "RGB").save(os.path.join(carpeta, nombre))


def hexa(h): return np.array([int(h[i:i + 2], 16) for i in (0, 2, 4)], dtype=float)


def pb(p): return (p[0], -p[2], p[1])   # juego -> Blender


N = 1024
yy, xx = np.mgrid[0:N, 0:N]


def tex_mantel():
    """Mantel de algodon casi blanco: trama fina y 3 franjas bordadas grises (el color final lo pone el tinte)."""
    img = np.zeros((N, N, 3)); img[:] = hexa("e9e6df")
    trama = ((xx // 4 + yy // 4) % 2) * 9.0
    img -= trama[..., None]
    for y0 in (96, 160, 928):
        img[y0:y0 + 24, :, :] = hexa("b9b4a8")
    rombos = (np.abs((xx % 64) - 32) + np.abs((yy % 64) - 32)) < 10
    img[(yy > 256) & (yy < 320) & rombos] = hexa("c9c3b5")
    img += rng.normal(0, 3, img.shape)
    return img


def tex_madera():
    img = np.zeros((N, N, 3)); img[:] = hexa("7a5233")
    veta = np.sin(yy / 9.0 + 3 * np.sin(xx / 150.0)) * 10 + rng.normal(0, 5, (N, N))
    return img + veta[..., None]


def tex_vidrio():
    """Vaso de veladora: vidrio liso casi blanco con una etiqueta ancha al centro (se tine por familia)."""
    img = np.zeros((N, N, 3)); img[:] = hexa("dfe6e4")
    img[(yy > 300) & (yy < 760)] = hexa("f2f0ea")
    img[(yy > 300) & (yy < 330)] = hexa("c8c4b8")
    img[(yy > 730) & (yy < 760)] = hexa("c8c4b8")
    cruz = (np.abs(xx % 512 - 256) < 18) & (yy > 400) & (yy < 640) | (np.abs(yy - 520) < 18) & (np.abs(xx % 512 - 256) < 90)
    img[cruz] = hexa("b6b1a4")
    img += rng.normal(0, 2, img.shape)
    return img


def tex_calabaza():
    """Calabaza: naranja con costillas verticales mas oscuras (cada 64 px) y manchas de ruido."""
    base = hexa("e8861b")
    costilla = 0.5 + 0.5 * np.cos(xx / 64.0 * 2 * math.pi)
    img = base[None, None, :] * (0.78 + 0.22 * costilla[..., None])
    img += rng.normal(0, 7, img.shape)
    img[yy < 70] = hexa("5d6b2a")           # tallo (parte alta de la UV)
    return img


def tex_naranja():
    img = np.zeros((N, N, 3)); img[:] = hexa("ef8a16")
    hoyuelo = ((xx % 32 - 16) ** 2 + (yy % 32 - 16) ** 2) < 14
    img[hoyuelo] = hexa("d97410")
    img += rng.normal(0, 5, img.shape)
    return img


def material(nombre, png_abs):
    mat = bpy.data.materials.new(nombre)
    mat.use_nodes = True
    nt = mat.node_tree
    p = next(n for n in nt.nodes if n.type == "BSDF_PRINCIPLED")
    t = nt.nodes.new("ShaderNodeTexImage")
    t.image = bpy.data.images.load(png_abs)
    nt.links.new(t.outputs["Color"], p.inputs["Base Color"])
    p.inputs["Roughness"].default_value = 0.8
    return mat


def uv_caja(bm, m_por_tile):
    uv = bm.loops.layers.uv.verify()
    for f in bm.faces:
        n = f.normal
        ax = max(range(3), key=lambda i: abs(n[i]))
        a, b = [(1, 2), (0, 2), (0, 1)][ax]
        for l in f.loops:
            c = l.vert.co
            l[uv].uv = (c[a] / m_por_tile, c[b] / m_por_tile)


def caja(bm, x0, x1, y0, y1, z0, z1):
    c = ((x0 + x1) / 2, (y0 + y1) / 2, (z0 + z1) / 2)
    d = (x1 - x0, y1 - y0, z1 - z0)
    bmesh.ops.create_cube(bm, size=1.0, matrix=Matrix.Translation(Vector(pb(c))) @ Matrix.Diagonal((d[0], d[2], d[1], 1.0)))


def objeto(nombre, bm, carpeta, tex):
    me = bpy.data.meshes.new(nombre); bm.to_mesh(me); bm.free()
    ob = bpy.data.objects.new(nombre, me); bpy.context.scene.collection.objects.link(ob)
    me.materials.append(material(nombre + "_mat", os.path.join(carpeta, tex)))
    return ob


def unir_y_exportar(nombre, carpeta, partes):
    bpy.ops.object.select_all(action="DESELECT")
    for o in partes: o.select_set(True)
    bpy.context.view_layer.objects.active = partes[0]
    if len(partes) > 1: bpy.ops.object.join()
    ob = bpy.context.view_layer.objects.active
    ob.name = nombre
    for p in ob.data.polygons: p.use_smooth = False
    bpy.ops.object.select_all(action="DESELECT"); ob.select_set(True)
    bpy.ops.wm.obj_export(filepath=os.path.join(carpeta, nombre + ".obj"), export_selected_objects=True, apply_modifiers=True,
                          export_materials=True, path_mode="STRIP", forward_axis="NEGATIVE_Z", up_axis="Y", global_scale=1.0)
    bpy.ops.wm.save_as_mainfile(filepath=os.path.join(carpeta, nombre + ".blend"), copy=True)
    print(nombre, "->", carpeta, "triangulos:", sum(len(p.vertices) - 2 for p in ob.data.polygons))


# ------------------------------------------------------------------ mesa_ofrenda
def mesa():
    c = os.path.join(RAIZ, "Models", "mesa_ofrenda")
    guardar(c, "mesa_mantel.png", tex_mantel()); guardar(c, "mesa_madera.png", tex_madera())
    bpy.ops.wm.read_factory_settings(use_empty=True)
    bm = bmesh.new()
    caja(bm, -0.60, 0.60, 0.70, 0.78, -0.35, 0.35)            # tablero
    caja(bm, -0.64, 0.64, 0.46, 0.70, -0.39, 0.39)            # mantel que cuelga por los 4 lados (4 cm mas ancho)
    uv_caja(bm, 0.5)
    tela = objeto("mesa_tela", bm, c, "mesa_mantel.png")
    bw = bmesh.new()
    for sx in (-0.56, 0.56):
        for sz in (-0.31, 0.31):
            caja(bw, sx - 0.03, sx + 0.03, 0.0, 0.46, sz - 0.03, sz + 0.03)   # patas (asoman bajo el mantel)
    uv_caja(bw, 0.5)
    pat = objeto("mesa_patas", bw, c, "mesa_madera.png")
    unir_y_exportar("mesa_ofrenda", c, [tela, pat])


# ------------------------------------------------------------------ veladora
def veladora():
    c = os.path.join(RAIZ, "Models", "veladora")
    guardar(c, "veladora_vidrio.png", tex_vidrio())
    bpy.ops.wm.read_factory_settings(use_empty=True)
    bm = bmesh.new()
    bmesh.ops.create_cone(bm, cap_ends=True, cap_tris=False, segments=12, radius1=0.045, radius2=0.052, depth=0.20,
                          matrix=Matrix.Translation(Vector(pb((0, 0.10, 0)))), calc_uvs=True)
    ob = objeto("veladora", bm, c, "veladora_vidrio.png")
    unir_y_exportar("veladora", c, [ob])


# ------------------------------------------------------------------ calabaza
def esfera(bm, centro, rx, ry, rz, seg=14, anillos=8):
    m = Matrix.Translation(Vector(pb(centro))) @ Matrix.Diagonal((rx, rz, ry, 1.0))
    bmesh.ops.create_uvsphere(bm, u_segments=seg, v_segments=anillos, radius=1.0, matrix=m, calc_uvs=True)


def calabaza():
    c = os.path.join(RAIZ, "Models", "calabaza")
    guardar(c, "calabaza.png", tex_calabaza())
    bpy.ops.wm.read_factory_settings(use_empty=True)
    bm = bmesh.new()
    esfera(bm, (0, 0.075, 0), 0.11, 0.075, 0.11, 16, 8)
    bmesh.ops.create_cone(bm, cap_ends=True, cap_tris=False, segments=6, radius1=0.016, radius2=0.011, depth=0.04,
                          matrix=Matrix.Translation(Vector(pb((0, 0.165, 0)))), calc_uvs=True)
    # el tallo usa la franja verde alta de la textura: mueve sus UV a v ~ 0.97
    uv = bm.loops.layers.uv.verify()
    for f in bm.faces:
        if min(l.vert.co.z for l in f.loops) > 0.14:
            for l in f.loops: l[uv].uv = (l[uv].uv.x, 0.97)
    ob = objeto("calabaza", bm, c, "calabaza.png")
    unir_y_exportar("calabaza", c, [ob])


# ------------------------------------------------------------------ naranjas
def naranjas():
    c = os.path.join(RAIZ, "Models", "naranjas")
    guardar(c, "naranja.png", tex_naranja())
    bpy.ops.wm.read_factory_settings(use_empty=True)
    bm = bmesh.new()
    for (x, y, z) in [(-0.042, 0.040, -0.042), (0.042, 0.040, -0.042), (-0.042, 0.040, 0.042), (0.042, 0.040, 0.042), (0.0, 0.112, 0.0)]:
        esfera(bm, (x, y, z), 0.04, 0.04, 0.04, 8, 6)
    ob = objeto("naranjas", bm, c, "naranja.png")
    unir_y_exportar("naranjas", c, [ob])


# ------------------------------------------------------------------ patio_tierra
def patio():
    c = os.path.join(RAIZ, "Models", "patio_tierra")
    os.makedirs(c, exist_ok=True)
    import shutil
    shutil.copyfile(os.path.join(RAIZ, "Models", "sendero", "tierra_plaza.png"), os.path.join(c, "tierra_plaza.png"))   # la misma tierra de la plaza (X-36)
    bpy.ops.wm.read_factory_settings(use_empty=True)
    bm = bmesh.new()
    x0, x1, z0, z1, y = -6.8, 6.8, 3.2, 12.2, 0.015
    vs = [bm.verts.new(pb(p)) for p in [(x0, y, z0), (x1, y, z0), (x1, y, z1), (x0, y, z1)]]
    f = bm.faces.new([vs[0], vs[3], vs[2], vs[1]])          # normal hacia arriba (+y juego)
    uv = bm.loops.layers.uv.verify()
    for l in f.loops:
        gx, gz = l.vert.co.x, -l.vert.co.y                   # Blender (x, -z) -> juego (x, z)
        l[uv].uv = (gx / 3.0, gz / 3.0)                      # 1 repeticion cada 3 m
    ob = objeto("patio_tierra", bm, c, "tierra_plaza.png")
    unir_y_exportar("patio_tierra", c, [ob])


mesa(); veladora(); calabaza(); naranjas(); patio()
