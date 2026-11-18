r"""X-24: objetos de la narrativa de la ruta: farol (poste con linterna abierta; la luz es una esfera emisiva aparte), guitarra, cesta,
y las dos piezas del murcielago (cuerpo y ala). Pocas caras, 1 u = 1 m, pivote en la base (guitarra y cesta) o en el centro del cuerpo.
Uso (raiz del repo):
    python tools/x24_narrativa.py texturas
    "C:\Program Files\Blender Foundation\Blender 5.2\blender.exe" -b --python tools/x24_narrativa.py -- modelos
Blender: X = juego X, Y = juego -Z, Z = arriba."""
import math, os, sys
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
modo = sys.argv[-1]
N = 1024
OBJETOS = {"farol": "madera_oscura", "guitarra": "madera_clara", "cesta": "mimbre", "murcielago_cuerpo": "pelaje", "murcielago_ala": "membrana"}


def texturas():
    import numpy as np
    from PIL import Image
    r = np.random.default_rng(24)
    yy, xx = np.mgrid[0:N, 0:N]
    def guarda(nombre, tipo, img):
        d = os.path.join(RAIZ, "Models", nombre); os.makedirs(d, exist_ok=True)
        Image.fromarray((np.clip(img, 0, 1) * 255).astype(np.uint8), "RGB").save(os.path.join(d, tipo + ".png"))
    fibra = (0.75 + 0.25 * np.sin(yy * 2 * math.pi / 14 + 6 * r.random((N, 1))))[..., None]
    for nombre, tipo, col in (("farol", "madera_oscura", (0.30, 0.18, 0.09)), ("guitarra", "madera_clara", (0.72, 0.48, 0.24))):
        guarda(nombre, tipo, np.array(col)[None, None, :] * fibra * (0.85 + 0.3 * r.random((N, N, 1))))
    cuad = ((xx // 24 + yy // 24) % 2).astype(float)
    hilo = 0.5 + 0.5 * np.where(cuad > 0, np.sin(xx * 2 * math.pi / 6), np.sin(yy * 2 * math.pi / 6))
    guarda("cesta", "mimbre", np.array((0.66, 0.50, 0.26))[None, None, :] * (0.6 + 0.4 * hilo[..., None]) * (0.9 + 0.1 * r.random((N, N, 1))))
    guarda("murcielago_cuerpo", "pelaje", np.array((0.10, 0.08, 0.08))[None, None, :] * (0.7 + 0.6 * r.random((N, N, 1))))
    nerv = (np.abs(np.sin((xx + yy * 0.3) * 2 * math.pi / 150)) < 0.04).astype(float)[..., None]
    guarda("murcielago_ala", "membrana", np.array((0.16, 0.11, 0.14))[None, None, :] * (0.8 + 0.4 * r.random((N, N, 1))) * (1 - 0.5 * nerv))
    print("texturas OK")


def modelos():
    import bpy, bmesh
    from mathutils import Vector
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from bpy_comun import limpiar, material, exportar

    def tubo(bm, p0, p1, r0, r1, seg=8):
        p0, p1 = Vector(p0), Vector(p1)
        eje = (p1 - p0).normalized(); a = Vector((1, 0, 0)) if abs(eje.x) < 0.9 else Vector((0, 1, 0))
        u = eje.cross(a).normalized(); v = eje.cross(u).normalized()
        an = [[bm.verts.new(p + (u * math.cos(2 * math.pi * k / seg) + v * math.sin(2 * math.pi * k / seg)) * r) for k in range(seg)] for p, r in ((p0, r0), (p1, r1))]
        for k in range(seg):
            bm.faces.new((an[0][k], an[0][(k + 1) % seg], an[1][(k + 1) % seg], an[1][k]))
        bm.faces.new(an[0][::-1]); bm.faces.new(an[1])

    def caja(bm, c, d):
        res = bmesh.ops.create_cube(bm, size=1.0)
        for v in res["verts"]:
            v.co = Vector((v.co.x * d[0], v.co.y * d[1], v.co.z * d[2])) + Vector(c)

    def esfera(bm, c, r, esc=(1, 1, 1), seg=10):
        res = bmesh.ops.create_uvsphere(bm, u_segments=seg, v_segments=seg // 2, radius=r)
        for v in res["verts"]:
            v.co = Vector((v.co.x * esc[0], v.co.y * esc[1], v.co.z * esc[2])) + Vector(c)

    def exporta(nombre, bm):
        limpiar()
        tipo = OBJETOS[nombre]
        me = bpy.data.meshes.new(nombre)
        uvl = bm.loops.layers.uv.verify()
        for f in bm.faces:
            for loop in f.loops:
                co = loop.vert.co
                loop[uvl].uv = (co.x * 2.0 + co.y * 1.3, co.z * 2.0 + co.y * 0.7)
        bm.to_mesh(me); bm.free()
        ob = bpy.data.objects.new(nombre, me); bpy.context.collection.objects.link(ob)
        me.materials.append(material(nombre, os.path.join(RAIZ, "Models", nombre, tipo + ".png")))
        for p in me.polygons:
            p.use_smooth = True
        bpy.context.view_layer.objects.active = ob; ob.select_set(True)
        exportar(ob, nombre)
        print(nombre, len(me.polygons), "caras")

    # --- farol: poste de 1.6 m, brazo, linterna abierta (4 columnas, techo y base) para ver la esfera emisiva
    bm = bmesh.new()
    tubo(bm, (0, 0, 0), (0, 0, 1.62), 0.028, 0.022)
    tubo(bm, (0, 0, 1.58), (0.14, 0, 1.58), 0.014, 0.014)
    caja(bm, (0.14, 0, 1.40), (0.18, 0.18, 0.03))                                  # base de la linterna
    for sx in (-1, 1):
        for sy in (-1, 1):
            tubo(bm, (0.14 + sx * 0.08, sy * 0.08, 1.40), (0.14 + sx * 0.08, sy * 0.08, 1.64), 0.008, 0.008, 5)
    caja(bm, (0.14, 0, 1.665), (0.22, 0.22, 0.03))                                 # techo
    caja(bm, (0.14, 0, 1.70), (0.10, 0.10, 0.04))
    caja(bm, (0, 0, 0.02), (0.14, 0.14, 0.04))                                     # base del poste
    exporta("farol", bm)
    # --- guitarra (eje largo en +Z, pivote en el centro del cuerpo): dos lobulos, brazo y cabeza
    bm = bmesh.new()
    esfera(bm, (0, 0, -0.08), 0.15, (1, 0.28, 1.0), 14)
    esfera(bm, (0, 0, 0.10), 0.11, (1, 0.28, 1.0), 14)
    caja(bm, (0, 0, 0.45), (0.045, 0.03, 0.55))
    caja(bm, (0, 0, 0.76), (0.065, 0.035, 0.12))
    tubo(bm, (0, -0.045, -0.05), (0, -0.045, 0.02), 0.035, 0.035, 8)              # boca
    exporta("guitarra", bm)
    # --- cesta: cono truncado de 0.18 m de alto y asa
    bm = bmesh.new()
    tubo(bm, (0, 0, 0), (0, 0, 0.18), 0.12, 0.19, 12)
    tubo(bm, (0, 0, 0.18), (0, 0, 0.20), 0.20, 0.20, 12)
    for k in range(9):
        a0 = math.pi * k / 9; a1 = math.pi * (k + 1) / 9
        tubo(bm, (0.19 * math.cos(a0), 0, 0.20 + 0.17 * math.sin(a0)), (0.19 * math.cos(a1), 0, 0.20 + 0.17 * math.sin(a1)), 0.012, 0.012, 5)
    exporta("cesta", bm)
    # --- murcielago: cuerpo (frente en -Z) y ala derecha (extendida a +X; la izquierda es su espejo)
    bm = bmesh.new()
    esfera(bm, (0, 0, 0), 0.05, (0.8, 1.0, 1.6), 8)
    esfera(bm, (0, 0.0, -0.08), 0.032, (1, 1, 1), 8)
    for sx in (-1, 1):
        caja(bm, (sx * 0.02, 0.02, -0.11), (0.012, 0.01, 0.045))
    exporta("murcielago_cuerpo", bm)
    bm = bmesh.new()
    puntos = [(0.0, 0.0), (0.12, 0.06), (0.30, 0.03), (0.44, -0.04), (0.36, -0.07), (0.28, -0.04), (0.20, -0.09), (0.12, -0.05), (0.0, -0.08)]
    vs = [bm.verts.new((x, 0.0, z)) for x, z in puntos]
    for i in range(1, len(vs) - 1):
        bm.faces.new((vs[0], vs[i], vs[i + 1]))
    vs2 = [bm.verts.new((x, 0.003, z)) for x, z in puntos]          # cara trasera (vertices duplicados, sentido inverso)
    for i in range(1, len(vs2) - 1):
        bm.faces.new((vs2[0], vs2[i + 1], vs2[i]))
    exporta("murcielago_ala", bm)


if modo == "texturas":
    texturas()
elif modo == "modelos":
    modelos()
