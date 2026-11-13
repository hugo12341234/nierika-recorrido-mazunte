r"""X-21: tabla de remo (SUP) con su remo, para las personas sobre las olas.
La tabla mide 3.2 x 0.76 x 0.12 m (pivote en el centro de la cara inferior, proa hacia -Z del juego); el remo va vertical a la derecha
(x = +0.23, y = 0.08 respecto al centro) a la altura de las manos de persona_pie_a/b, que se dibujan encima con su pivote en y = 0.12.
Uso: python tools/x21_tablas.py texturas
     "C:\Program Files\Blender Foundation\Blender 5.2\blender.exe" -b --python tools/x21_tablas.py -- modelos"""
import math, os, sys
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
modo = sys.argv[-1]
N = 1024
LARGO, ANCHO, GRUESO = 3.2, 0.76, 0.12


def texturas():
    import numpy as np
    from PIL import Image
    r = np.random.default_rng(21)
    yy, xx = np.mgrid[0:N, 0:N]
    franja = (np.sin(xx * 2 * math.pi / 256) > 0.55).astype(float)[..., None]
    blanco = np.array((0.92, 0.92, 0.88)); turq = np.array((0.05, 0.55, 0.62)); coral = np.array((0.95, 0.40, 0.25))
    base = blanco[None, None, :] * (1 - franja) + turq[None, None, :] * franja
    centro = (np.abs(yy - N / 2) < 22).astype(float)[..., None]
    base = base * (1 - centro) + coral[None, None, :] * centro
    base *= (0.94 + 0.08 * r.random((N, N, 1)))
    os.makedirs(os.path.join(RAIZ, "Models", "tabla_remo"), exist_ok=True)
    Image.fromarray((np.clip(base, 0, 1) * 255).astype(np.uint8), "RGB").save(os.path.join(RAIZ, "Models", "tabla_remo", "tabla_remo.png"))
    fibra = (0.75 + 0.25 * np.sin(yy * 2 * math.pi / 14 + 6 * r.random((N, 1))))[..., None]
    madera = np.array((0.45, 0.28, 0.14))[None, None, :] * fibra * (0.85 + 0.3 * r.random((N, N, 1)))
    Image.fromarray((np.clip(madera, 0, 1) * 255).astype(np.uint8), "RGB").save(os.path.join(RAIZ, "Models", "tabla_remo", "remo.png"))
    print("texturas OK")


def modelos():
    import bpy, bmesh
    from mathutils import Vector
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from bpy_comun import limpiar, material, exportar
    limpiar()
    carpeta = os.path.join(RAIZ, "Models", "tabla_remo")
    mat_t = material("tabla_remo", os.path.join(carpeta, "tabla_remo.png"))
    mat_r = material("remo", os.path.join(carpeta, "remo.png"))
    bm = bmesh.new()
    uvl = bm.loops.layers.uv.verify()
    # contorno de la tabla: semiancho w(s) = ANCHO/2 * sen(pi * s^0.85), s = 0 (popa) .. 1 (proa); la proa mira a +Y de Blender (-Z del juego)
    seg = 24
    arriba, abajo = [], []
    for i in range(seg + 1):
        s = i / seg
        y = (s - 0.5) * LARGO
        w = max(ANCHO / 2 * math.sin(math.pi * (0.04 + 0.92 * s) ** 0.85), 0.01)
        zt = GRUESO - 0.04 * (s - 0.5) ** 2 * 4                       # la cubierta se curva hacia las puntas
        arriba.append((bm.verts.new((-w, y, zt)), bm.verts.new((w, y, zt))))
        abajo.append((bm.verts.new((-w, y, 0.0)), bm.verts.new((w, y, 0.0))))
    for i in range(seg):
        for (a, b, c, d, mi) in ((arriba[i][0], arriba[i][1], arriba[i + 1][1], arriba[i + 1][0], 0),
                                 (abajo[i][1], abajo[i][0], abajo[i + 1][0], abajo[i + 1][1], 0),
                                 (abajo[i][0], arriba[i][0], arriba[i + 1][0], abajo[i + 1][0], 0),
                                 (arriba[i][1], abajo[i][1], abajo[i + 1][1], arriba[i + 1][1], 0)):
            f = bm.faces.new((a, b, c, d))
            f.material_index = mi
            for loop in f.loops:
                loop[uvl].uv = (loop.vert.co.x / ANCHO + 0.5, loop.vert.co.y / LARGO + 0.5)
    # remo: caña (tubo de 6 caras) + pala + empuñadura en T, vertical en (0.23, 0.08), de z = 0.12 a z = 1.95
    def tubo(p0, p1, r, mi):
        p0, p1 = Vector(p0), Vector(p1)
        eje = (p1 - p0).normalized(); a = Vector((1, 0, 0)) if abs(eje.x) < 0.9 else Vector((0, 1, 0))
        u = eje.cross(a).normalized(); v = eje.cross(u).normalized()
        anillos = [[bm.verts.new(p + (u * math.cos(2 * math.pi * k / 6) + v * math.sin(2 * math.pi * k / 6)) * r) for k in range(6)] for p in (p0, p1)]
        for k in range(6):
            f = bm.faces.new((anillos[0][k], anillos[0][(k + 1) % 6], anillos[1][(k + 1) % 6], anillos[1][k])); f.material_index = mi
            for loop in f.loops:
                loop[uvl].uv = (k / 6, loop.vert.co.z)
    tubo((0.23, 0.08, 0.14), (0.23, 0.08, 1.85), 0.018, 1)
    tubo((0.13, 0.08, 1.85), (0.33, 0.08, 1.85), 0.016, 1)           # empunadura en T
    pala = [bm.verts.new(v) for v in ((0.17, 0.075, 0.12), (0.29, 0.075, 0.12), (0.29, 0.075, 0.55), (0.17, 0.075, 0.55),
                                       (0.17, 0.085, 0.12), (0.29, 0.085, 0.12), (0.29, 0.085, 0.55), (0.17, 0.085, 0.55))]
    for idx in ((0, 1, 2, 3), (4, 7, 6, 5), (0, 4, 5, 1), (1, 5, 6, 2), (2, 6, 7, 3), (3, 7, 4, 0)):
        f = bm.faces.new([pala[i] for i in idx]); f.material_index = 1
        for loop in f.loops:
            loop[uvl].uv = (loop.vert.co.x * 4, loop.vert.co.z)
    me = bpy.data.meshes.new("tabla_remo"); bm.to_mesh(me); bm.free()
    me.materials.append(mat_t); me.materials.append(mat_r)
    bmesh_obj = bpy.data.objects.new("tabla_remo", me); bpy.context.collection.objects.link(bmesh_obj)
    bpy.context.view_layer.objects.active = bmesh_obj; bmesh_obj.select_set(True)
    bpy.ops.object.shade_smooth()
    exportar(bmesh_obj, "tabla_remo")
    print("tabla_remo", len(me.polygons), "caras")


if modo == "texturas":
    texturas()
elif modo == "modelos":
    modelos()
