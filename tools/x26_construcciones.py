r"""X-26: construcciones de la costa de Mazunte -- FUERA DEL ALCANCE ORIGINAL de la propuesta (calles y edificios del pueblo), agregadas por
decision del equipo del 13/11/2026 (ver ESTADO seccion 4). Se pueden quitar con la bandera MOSTRAR_FUERA_DE_ALCANCE de src/Extras.h (o la tecla F8).
Modelos propios en Blender 5.2: palapa (techo de palma sobre 4 postes), casa_a y casa_b (casa costera de 5 x 4 m con techo de teja a dos aguas;
dos colores de pared). Pocas caras, 1 u = 1 m, pivote en el centro del piso. Texturas 1K generadas por codigo.
Uso (raiz del repo):
    python tools/x26_construcciones.py texturas
    "C:\Program Files\Blender Foundation\Blender 5.2\blender.exe" -b --python tools/x26_construcciones.py -- modelos"""
import math, os, sys
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
modo = sys.argv[-1]
N = 1024


def texturas():
    import numpy as np
    from PIL import Image
    r = np.random.default_rng(26)
    yy, xx = np.mgrid[0:N, 0:N]

    def per(escala, sem):
        g = np.random.default_rng(sem).random((escala, escala))
        x = np.linspace(0, escala, N, endpoint=False); i = x.astype(int); f = x - i; u = f * f * (3 - 2 * f); i1 = (i + 1) % escala
        a = g[np.ix_(i, i)]; b = g[np.ix_(i, i1)]; c = g[np.ix_(i1, i)]; d = g[np.ix_(i1, i1)]
        ux = u[None, :]; uy = u[:, None]
        return (a * (1 - ux) + b * ux) * (1 - uy) + (c * (1 - ux) + d * ux) * uy

    def guarda(carpeta, nombre, img):
        d = os.path.join(RAIZ, "Models", carpeta); os.makedirs(d, exist_ok=True)
        Image.fromarray((np.clip(img, 0, 1) * 255).astype(np.uint8), "RGB").save(os.path.join(d, nombre))

    # paja de la palapa: hebras largas casi verticales, amarillo-pardo
    hebras = 0.5 + 0.5 * np.sin(xx * 2 * math.pi / 9 + 6 * per(64, 2600)[::1, ::1] + 0.4 * np.sin(yy * 2 * math.pi / 200))
    paja = np.array((0.55, 0.42, 0.20))[None, None, :] * (0.55 + 0.5 * hebras[..., None]) * (0.85 + 0.3 * r.random((N, N, 1)))
    guarda("palapa", "paja.png", paja)
    fibra = (0.75 + 0.25 * np.sin(yy * 2 * math.pi / 14 + 6 * r.random((N, 1))))[..., None]
    madera = np.array((0.34, 0.22, 0.12))[None, None, :] * fibra * (0.85 + 0.3 * r.random((N, N, 1)))
    guarda("palapa", "madera.png", madera)
    # casas: pared encalada con manchas, teja de barro, puertas y marcos de madera
    # v2 (X-31): estuco con brochazos, humedad en la base y grietas; madera con nudos; teja con tejas individuales y musgo; zocalo de piedra; vidrio
    def estuco(col, sem):
        base = (0.80 + 0.22 * per(8, sem)[..., None] + 0.10 * per(32, sem + 1)[..., None]) * np.array(col)[None, None, :]
        brocha = 0.97 + 0.03 * np.sin((xx + 40 * per(16, sem + 2)) * 2 * math.pi / 37)[..., None]
        hum = np.clip((yy - N * 0.78) / (N * 0.22), 0, 1)[..., None] * (0.35 + 0.4 * per(16, sem + 3)[..., None])
        grieta = (np.abs(np.sin(xx * 0.020 + 6 * per(8, sem + 4)) + np.sin(yy * 0.013 + 5 * per(8, sem + 5))) < 0.018)[..., None]
        img = base * brocha
        img = img * (1 - 0.45 * hum) + np.array((0.10, 0.085, 0.06))[None, None, :] * hum
        img = np.where(grieta, img * 0.6, img)
        return img * (0.94 + 0.08 * r.random((N, N, 1)))
    nudos = np.zeros((N, N))
    for _ in range(14):
        cx, cy = r.integers(0, N), r.integers(0, N)
        nudos += np.exp(-(((xx - cx) ** 2) / 1200.0 + ((yy - cy) ** 2) / 300.0))
    madera2 = np.array((0.32, 0.20, 0.11))[None, None, :] * (fibra * (0.85 + 0.3 * r.random((N, N, 1)))) * (1 - 0.35 * np.clip(nudos, 0, 1)[..., None])
    fila = yy // 64
    col_t = (xx + 32 * (fila % 2)) // 64
    var = np.random.default_rng(31).random((20, 20))[fila % 20, col_t % 20][..., None]
    borde = (np.abs((xx + 32 * (fila % 2)) % 64 - 32) > 29) | ((yy % 64) > 58)
    teja = np.array((0.60, 0.26, 0.14))[None, None, :] * (0.75 + 0.45 * var) * (0.85 + 0.15 * (0.5 + 0.5 * np.sin(yy * 2 * math.pi / 64))[..., None])
    teja = np.where(borde[..., None], teja * 0.55, teja)
    musgo = np.clip((per(16, 3300) - 0.62) * 5, 0, 1)[..., None]
    teja = teja * (1 - 0.5 * musgo) + np.array((0.16, 0.26, 0.10))[None, None, :] * 0.5 * musgo
    piedra = np.array((0.30, 0.28, 0.26))[None, None, :] * (0.7 + 0.6 * per(10, 3400)[..., None]) * (0.9 + 0.2 * r.random((N, N, 1)))
    junta = ((xx + 64 * ((yy // 64) % 2)) % 128 < 3) | (yy % 64 < 3)
    piedra = np.where(junta[..., None], piedra * 0.6, piedra)
    vidrio = np.array((0.08, 0.14, 0.20))[None, None, :] * (0.7 + 0.5 * np.clip(1 - (xx + yy) / (2.0 * N), 0, 1)[..., None]) + 0.05
    for tag, col in (("a", (0.20, 0.55, 0.60)), ("b", (0.88, 0.70, 0.35))):
        guarda("casa_" + tag, "pared.png", estuco(col, 2700 + ord(tag)))
        guarda("casa_" + tag, "madera.png", madera2)
        guarda("casa_" + tag, "teja.png", teja)
        guarda("casa_" + tag, "zocalo.png", piedra)
        guarda("casa_" + tag, "vidrio.png", vidrio)
    print("texturas OK")


def modelos():
    import bpy, bmesh
    from mathutils import Vector
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from bpy_comun import limpiar, material, exportar

    def tubo(bm, p0, p1, r0, r1, mi, seg=8):
        p0, p1 = Vector(p0), Vector(p1)
        eje = (p1 - p0).normalized(); a = Vector((1, 0, 0)) if abs(eje.x) < 0.9 else Vector((0, 1, 0))
        u = eje.cross(a).normalized(); v = eje.cross(u).normalized()
        an = [[bm.verts.new(p + (u * math.cos(2 * math.pi * k / seg) + v * math.sin(2 * math.pi * k / seg)) * r) for k in range(seg)] for p, r in ((p0, r0), (p1, r1))]
        for k in range(seg):
            bm.faces.new((an[0][k], an[0][(k + 1) % seg], an[1][(k + 1) % seg], an[1][k])).material_index = mi
        bm.faces.new(an[0][::-1]).material_index = mi; bm.faces.new(an[1]).material_index = mi

    def caja(bm, c, d, mi):
        res = bmesh.ops.create_cube(bm, size=1.0)
        vs = set(res["verts"])
        for v in res["verts"]:
            v.co = Vector((v.co.x * d[0], v.co.y * d[1], v.co.z * d[2])) + Vector(c)
        for f in bm.faces:
            if all(v in vs for v in f.verts):
                f.material_index = mi

    def piramide(bm, base, alto, mi, cx=0.0, cy=0.0, z0=0.0):
        a, b = base
        vs = [bm.verts.new(p) for p in ((cx - a, cy - b, z0), (cx + a, cy - b, z0), (cx + a, cy + b, z0), (cx - a, cy + b, z0))]
        pico = bm.verts.new((cx, cy, z0 + alto))
        for i in range(4):
            bm.faces.new((vs[i], vs[(i + 1) % 4], pico)).material_index = mi
        return vs

    def exporta(nombre, bm, mats, uvf):
        limpiar()
        me = bpy.data.meshes.new(nombre)
        uvl = bm.loops.layers.uv.verify()
        for f in bm.faces:
            for loop in f.loops:
                loop[uvl].uv = uvf(loop.vert.co, f.normal, f.material_index)
        bm.to_mesh(me); bm.free()
        ob = bpy.data.objects.new(nombre, me); bpy.context.collection.objects.link(ob)
        for m in mats:
            me.materials.append(material(nombre + "_" + m[:-4], os.path.join(RAIZ, "Models", nombre, m)))
        for p in me.polygons:
            p.use_smooth = False
        bpy.context.view_layer.objects.active = ob; ob.select_set(True)
        exportar(ob, nombre)
        print(nombre, len(me.polygons), "caras")

    # --- palapa: 4 postes de 2.8 m y techo piramidal de palma (6 x 6 m abajo, vertice a 4.4 m) con un segundo anillo para dar grosor
    bm = bmesh.new()
    for sx in (-1, 1):
        for sy in (-1, 1):
            tubo(bm, (sx * 2.2, sy * 2.2, 0.0), (sx * 2.2, sy * 2.2, 2.8), 0.11, 0.09, 1, 8)
    piramide(bm, (3.4, 3.4), 1.9, 0, z0=2.7)
    piramide(bm, (3.1, 3.1), 1.9, 0, z0=2.55)
    caja(bm, (0, 0, 0.45), (1.6, 0.9, 0.06), 1)            # mesa
    caja(bm, (0, -0.85, 0.28), (1.6, 0.35, 0.05), 1)      # banca
    exporta("palapa", bm, ["paja.png", "madera.png"], lambda c, n, mi: (c.x * 0.3 + c.y * 0.17, c.z * 0.4 + c.y * 0.12))
    # --- casa costera de 5 x 4 m: paredes de 2.6 m, techo de teja a dos aguas (cumbrera a 3.8 m), puerta y dos ventanas
    for tag in ("a", "b"):
        bm = bmesh.new()
        W, D, H, RH = 5.0, 4.0, 2.7, 3.8
        # v2 (X-31): materiales 0 pared, 1 teja, 2 madera, 3 zocalo de piedra, 4 vidrio
        caja(bm, (0, 0, H / 2 + 0.25), (W, D, H - 0.5), 0)
        caja(bm, (0, 0, 0.25), (W + 0.08, D + 0.08, 0.5), 3)
        e = 0.50
        v = [bm.verts.new(p) for p in ((-W / 2 - e, -D / 2 - e, H - 0.05), (W / 2 + e, -D / 2 - e, H - 0.05), (W / 2 + e, 0, RH), (-W / 2 - e, 0, RH),
                                        (-W / 2 - e, D / 2 + e, H - 0.05), (W / 2 + e, D / 2 + e, H - 0.05))]
        bm.faces.new((v[0], v[1], v[2], v[3])).material_index = 1
        bm.faces.new((v[3], v[2], v[5], v[4])).material_index = 1
        w2 = [bm.verts.new((q.co.x, q.co.y, q.co.z - 0.04)) for q in v]   # cara inferior (se ve desde el alero): vertices duplicados
        bm.faces.new((w2[3], w2[2], w2[1], w2[0])).material_index = 1
        bm.faces.new((w2[4], w2[5], w2[2], w2[3])).material_index = 1
        # frontal de la gablete (triangulos de pared en los lados +-X)
        for sx in (-1, 1):
            t = [bm.verts.new((sx * W / 2, -D / 2, H)), bm.verts.new((sx * W / 2, D / 2, H)), bm.verts.new((sx * W / 2, 0, RH - 0.05))]
            bm.faces.new(t if sx > 0 else t[::-1]).material_index = 0
        caja(bm, (0, D / 2 + 0.07, 2.17), (1.06, 0.14, 0.08), 2)           # cumbrera/tabla de alero frontal
        fy = D / 2 + 0.02
        # puerta de 1.0 x 2.1 m: hoja, marco y dos paneles
        caja(bm, (0, fy + 0.02, 1.05), (0.84, 0.05, 2.04), 2)
        for sx in (-1, 1):
            caja(bm, (sx * 0.52, fy + 0.03, 1.07), (0.08, 0.08, 2.14), 2)
        caja(bm, (0, fy + 0.03, 2.13), (1.12, 0.08, 0.08), 2)
        caja(bm, (0, fy + 0.05, 0.62), (0.62, 0.02, 0.62), 2)
        caja(bm, (0, fy + 0.05, 1.50), (0.62, 0.02, 0.72), 2)
        caja(bm, (0.33, fy + 0.07, 1.0), (0.05, 0.05, 0.05), 3)
        # ventanas de 1.0 x 1.1 m con marco, cruceta, repisa y postigos abiertos
        for sx in (-1, 1):
            cx = sx * 1.65
            caja(bm, (cx, fy + 0.02, 1.55), (0.94, 0.04, 1.04), 4)
            for dx, dz, ax, az in ((0, 0.57, 1.12, 0.08), (0, -0.57, 1.12, 0.08), (-0.56, 0, 0.08, 1.2), (0.56, 0, 0.08, 1.2), (0, 0, 0.05, 1.0), (0, 0, 1.0, 0.05)):
                caja(bm, (cx + dx, fy + 0.05, 1.55 + dz), (ax, 0.07, az), 2)
            caja(bm, (cx, fy + 0.10, 0.96), (1.24, 0.16, 0.06), 3)
            for sh in (-1, 1):
                caja(bm, (cx + sh * 0.78, fy + 0.07, 1.55), (0.42, 0.04, 1.06), 2)
        # porche: dos postes, techo de un agua, piso y escalon
        for sx in (-1, 1):
            tubo(bm, (sx * 1.15, fy + 0.95, 0.0), (sx * 1.15, fy + 0.95, 2.55), 0.07, 0.07, 2, 8)
        pv = [bm.verts.new(q) for q in ((-1.45, fy, 2.75), (1.45, fy, 2.75), (1.45, fy + 1.2, 2.40), (-1.45, fy + 1.2, 2.40))]
        bm.faces.new(pv).material_index = 1
        pv2 = [bm.verts.new((q.co.x, q.co.y, q.co.z - 0.05)) for q in pv]
        bm.faces.new(pv2[::-1]).material_index = 2
        caja(bm, (0, fy + 0.6, 0.06), (2.6, 1.2, 0.12), 3)
        caja(bm, (0, fy + 1.3, 0.03), (1.6, 0.35, 0.06), 3)
        exporta("casa_" + tag, bm, ["pared.png", "teja.png", "madera.png", "zocalo.png", "vidrio.png"], lambda c, n, mi: ((c.x + c.y) * 0.25, c.z * 0.25))


if modo == "texturas":
    texturas()
elif modo == "modelos":
    modelos()
