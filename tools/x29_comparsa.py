r"""X-29: comparsa, musica en vivo y alfombras de flores (FUERA DEL ALCANCE ORIGINAL de la propuesta; decision del 13/11/2026, ver ESTADO seccion 4).
Modelos propios (Blender 5.2): tambor, trompeta, alfombra_flores (rectangular), alfombra_cruz (en forma de cruz). Las alfombras son de PETALOS DE CEMPASUCHIL
(flores en relieve de color, con alfa y hojas verdes), NO de arena de colores: no son el tapete de arena que sigue fuera de alcance.
Uso (raiz del repo):
    python tools/x29_comparsa.py texturas
    "C:\Program Files\Blender Foundation\Blender 5.2\blender.exe" -b --python tools/x29_comparsa.py -- modelos"""
import math, os, sys, random
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
modo = sys.argv[-1]
N = 1024


def texturas():
    import numpy as np
    from PIL import Image, ImageDraw, ImageFilter
    r = np.random.default_rng(29)
    rnd = random.Random(29)

    def guarda_rgb(carpeta, nombre, arr):
        d = os.path.join(RAIZ, "Models", carpeta); os.makedirs(d, exist_ok=True)
        Image.fromarray((np.clip(arr, 0, 1) * 255).astype(np.uint8), "RGB").save(os.path.join(d, nombre))

    yy, xx = np.mgrid[0:N, 0:N]
    fibra = (0.75 + 0.25 * np.sin(yy * 2 * math.pi / 14 + 6 * r.random((N, 1))))[..., None]
    guarda_rgb("tambor", "tambor.png", np.where((yy < N * 0.12)[..., None] | (yy > N * 0.88)[..., None], np.array((0.55, 0.55, 0.58))[None, None, :],
                                                  np.array((0.62, 0.22, 0.14))[None, None, :] * fibra) * (0.9 + 0.1 * r.random((N, N, 1))))
    guarda_rgb("trompeta", "trompeta.png", np.array((0.85, 0.65, 0.20))[None, None, :] * (0.8 + 0.2 * np.sin(xx * 2 * math.pi / 200))[..., None] * (0.9 + 0.1 * r.random((N, N, 1))))

    def petalos(ancho, alto, forma):
        """Alfombra de petalos de cempasuchil: miles de petalos ovalados naranjas, amarillos y magenta, con hojas verdes; la forma da el alfa."""
        im = Image.new("RGBA", (N, N), (0, 0, 0, 0))
        d = ImageDraw.Draw(im)
        for k in range(14000):
            x, y = rnd.random() * N, rnd.random() * N
            if not forma(x / N, y / N):
                continue
            tono = rnd.random()
            if tono < 0.55:
                col = (235 + rnd.randint(-15, 15), 125 + rnd.randint(-25, 25), 20 + rnd.randint(0, 20))      # naranja cempasuchil
            elif tono < 0.85:
                col = (250, 190 + rnd.randint(-20, 20), 40 + rnd.randint(0, 30))                              # amarillo
            elif tono < 0.93:
                col = (150 + rnd.randint(0, 40), 40, 120 + rnd.randint(0, 40))                                # magenta (cresta de gallo)
            else:
                col = (40 + rnd.randint(0, 30), 110 + rnd.randint(0, 40), 40)                                # hojas verdes
            rx, ry = 6 + rnd.random() * 9, 3 + rnd.random() * 5
            ang = rnd.random() * math.pi
            pts = [(x + rx * math.cos(a) * math.cos(ang) - ry * math.sin(a) * math.sin(ang), y + rx * math.cos(a) * math.sin(ang) + ry * math.sin(a) * math.cos(ang))
                   for a in [i * math.pi / 8 for i in range(16)]]
            sombra = tuple(int(c * 0.6) for c in col)
            d.polygon(pts, fill=col + (255,), outline=sombra + (255,))
        a = np.array(im)
        # el alfa se completa dentro de la forma para que no queden huecos (el fondo es hojarasca verde oscura)
        mapa = np.fromfunction(lambda j, i: np.vectorize(forma)(i / N, j / N), (N, N))
        base = a[..., 3] == 0
        rel = mapa & base
        a[rel, 0] = 45; a[rel, 1] = 70; a[rel, 2] = 30; a[rel, 3] = 255
        return Image.fromarray(a, "RGBA")

    def rect(u, v):
        return 0.03 < u < 0.97 and 0.06 < v < 0.94

    def cruz(u, v):
        return (0.34 < u < 0.66 and 0.03 < v < 0.97) or (0.03 < u < 0.97 and 0.34 < v < 0.66)

    for carpeta, forma in (("alfombra_flores", rect), ("alfombra_cruz", cruz)):
        d = os.path.join(RAIZ, "Models", carpeta); os.makedirs(d, exist_ok=True)
        petalos(N, N, forma).save(os.path.join(d, "petalos.png"))
    print("texturas OK")


def modelos():
    import bpy, bmesh
    from mathutils import Vector
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from bpy_comun import limpiar, material, exportar

    def tubo(bm, p0, p1, r0, r1, seg=12):
        p0, p1 = Vector(p0), Vector(p1)
        eje = (p1 - p0).normalized(); a = Vector((1, 0, 0)) if abs(eje.x) < 0.9 else Vector((0, 1, 0))
        u = eje.cross(a).normalized(); v = eje.cross(u).normalized()
        an = [[bm.verts.new(p + (u * math.cos(2 * math.pi * k / seg) + v * math.sin(2 * math.pi * k / seg)) * r) for k in range(seg)] for p, r in ((p0, r0), (p1, r1))]
        for k in range(seg):
            bm.faces.new((an[0][k], an[0][(k + 1) % seg], an[1][(k + 1) % seg], an[1][k]))
        bm.faces.new(an[0][::-1]); bm.faces.new(an[1])

    def exporta(nombre, bm, png, alfa=False, uvf=None):
        limpiar()
        me = bpy.data.meshes.new(nombre)
        uvl = bm.loops.layers.uv.verify()
        for f in bm.faces:
            for loop in f.loops:
                co = loop.vert.co
                loop[uvl].uv = uvf(co) if uvf else (math.atan2(co.y, co.x) / (2 * math.pi) + 0.5, co.z * 2.0)
        bm.to_mesh(me); bm.free()
        ob = bpy.data.objects.new(nombre, me); bpy.context.collection.objects.link(ob)
        me.materials.append(material(nombre, os.path.join(RAIZ, "Models", nombre, png), alfa=alfa))
        for p in me.polygons:
            p.use_smooth = not alfa
        bpy.context.view_layer.objects.active = ob; ob.select_set(True)
        exportar(ob, nombre)
        print(nombre, len(me.polygons), "caras")

    # tambor (pivote en la base): cilindro de 0.44 m de diametro y 0.38 m de alto
    bm = bmesh.new(); tubo(bm, (0, 0, 0), (0, 0, 0.38), 0.22, 0.22, 16)
    exporta("tambor", bm, "tambor.png", uvf=lambda c: (math.atan2(c.y, c.x) / (2 * math.pi) + 0.5, c.z / 0.38))
    # trompeta (eje largo en +Y, pivote en la boquilla): tubo de 0.55 m que se abre en campana
    bm = bmesh.new()
    tubo(bm, (0, 0, 0), (0, 0.35, 0), 0.012, 0.012, 8)
    tubo(bm, (0, 0.35, 0), (0, 0.58, 0), 0.012, 0.085, 12)
    tubo(bm, (0.06, 0.05, 0), (0.06, 0.33, 0), 0.01, 0.01, 6)
    exporta("trompeta", bm, "trompeta.png", uvf=lambda c: (c.x * 4 + c.y, c.z * 4 + c.y))
    # alfombras: plano de 1 x 1 m a 2 cm del suelo (se escala en la escena); mismo pivote en el centro
    for nombre in ("alfombra_flores", "alfombra_cruz"):
        bm = bmesh.new()
        vs = [bm.verts.new(p) for p in ((-0.5, -0.5, 0.02), (0.5, -0.5, 0.02), (0.5, 0.5, 0.02), (-0.5, 0.5, 0.02))]
        bm.faces.new(vs)
        exporta(nombre, bm, "petalos.png", alfa=True, uvf=lambda c: (c.x + 0.5, 0.5 - c.y))


if modo == "texturas":
    texturas()
elif modo == "modelos":
    modelos()
