r"""X-25..X-28: objetos para llenar los lados de la escena (inspirados en la costa de Mazunte; ver docs/investigacion_mazunte.md).
Modelos propios hechos en Blender 5.2 (pocas caras, 1 u = 1 m, pivote en la base): roca_mar, lancha, red_pesca, ave_cuerpo, ave_ala,
tortuga, hamaca, fogata. Texturas de 1K generadas por codigo. NINGUN edificio: casas y palapas NO se modelan (decision pendiente, ver ESTADO).
Uso (raiz del repo):
    python tools/x25_lados.py texturas
    "C:\Program Files\Blender Foundation\Blender 5.2\blender.exe" -b --python tools/x25_lados.py -- modelos
Blender: X = juego X, Y = juego -Z (el frente mira a +Y), Z = arriba."""
import math, os, sys, random
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
modo = sys.argv[-1]
N = 1024
MODELOS = ["roca_mar", "lancha", "red_pesca", "ave_cuerpo", "ave_ala", "tortuga", "hamaca", "fogata"]


def texturas():
    import numpy as np
    from PIL import Image
    r = np.random.default_rng(25)
    yy, xx = np.mgrid[0:N, 0:N]

    def per(escala, sem):
        g = np.random.default_rng(sem).random((escala, escala))
        x = np.linspace(0, escala, N, endpoint=False); i = x.astype(int); f = x - i; u = f * f * (3 - 2 * f); i1 = (i + 1) % escala
        a = g[np.ix_(i, i)]; b = g[np.ix_(i, i1)]; c = g[np.ix_(i1, i)]; d = g[np.ix_(i1, i1)]
        ux = u[None, :]; uy = u[:, None]
        return (a * (1 - ux) + b * ux) * (1 - uy) + (c * (1 - ux) + d * ux) * uy

    def fbm(base, oct_, sem):
        t = sum(per(base * 2 ** k, sem + k) / 2 ** k for k in range(oct_)); return t / sum(1 / 2 ** k for k in range(oct_))

    def guarda(nombre, nombre_png, img, modo_="RGB"):
        d = os.path.join(RAIZ, "Models", nombre); os.makedirs(d, exist_ok=True)
        Image.fromarray((np.clip(img, 0, 1) * 255).astype(np.uint8), modo_).save(os.path.join(d, nombre_png))

    def mezcla(c1, c2, t):
        return np.array(c1)[None, None, :] * (1 - t[..., None]) + np.array(c2)[None, None, :] * t[..., None]

    # roca: gris-pardo con vetas y algo de verdin
    base = fbm(6, 6, 2500); veta = np.clip(1 - np.abs(fbm(3, 4, 2550) - 0.5) * 8, 0, 1) ** 2
    roca = mezcla((0.22, 0.20, 0.19), (0.52, 0.48, 0.44), base) * (1 - 0.3 * veta[..., None])
    musgo = np.clip((fbm(5, 5, 2580) - 0.6) * 6, 0, 1)
    roca = roca * (1 - 0.6 * musgo[..., None]) + np.array((0.16, 0.26, 0.10))[None, None, :] * 0.6 * musgo[..., None]
    guarda("roca_mar", "roca.png", roca)
    # lancha: casco azul con franja blanca y banda roja (tablas horizontales de madera)
    tabla = (0.8 + 0.2 * np.sin(yy * 2 * math.pi / 64))[..., None]
    azul = np.array((0.08, 0.30, 0.55))[None, None, :]; blanco = np.array((0.92, 0.92, 0.88))[None, None, :]; rojo = np.array((0.75, 0.18, 0.12))[None, None, :]
    zona = (yy / N)[..., None]
    lancha = np.where(zona < 0.15, rojo, np.where(zona < 0.45, blanco, azul)) * tabla * (0.9 + 0.1 * r.random((N, N, 1)))
    guarda("lancha", "lancha.png", lancha)
    # red de pesca: malla romboidal con alfa
    a = np.zeros((N, N, 4), np.uint8)
    d1 = np.abs(((xx + yy) % 64) - 32); d2 = np.abs(((xx - yy) % 64) - 32)
    hilo = (d1 < 3) | (d2 < 3)
    a[..., 0:3] = (170, 160, 120); a[..., 3] = np.where(hilo, 255, 0)
    d = os.path.join(RAIZ, "Models", "red_pesca"); os.makedirs(d, exist_ok=True)
    Image.fromarray(a, "RGBA").save(os.path.join(d, "red.png"))
    # ave (pelicano/fragata): gris claro con pecho blanco; ala con borde oscuro
    ave = mezcla((0.62, 0.62, 0.64), (0.92, 0.92, 0.94), fbm(4, 3, 2600)) * (0.9 + 0.1 * r.random((N, N, 1)))
    guarda("ave_cuerpo", "ave.png", ave); guarda("ave_ala", "ave.png", ave * np.where(yy[..., None] > N * 0.8, 0.45, 1.0))
    # tortuga: escudo oliva con placas (celdas) y piel mas clara
    celdas = (np.sin(xx * 2 * math.pi / 128) * np.sin(yy * 2 * math.pi / 128) > 0.2).astype(float)
    tort = mezcla((0.18, 0.24, 0.12), (0.36, 0.40, 0.22), 0.6 * celdas + 0.4 * fbm(8, 3, 2700)) * (0.9 + 0.1 * r.random((N, N, 1)))
    guarda("tortuga", "tortuga.png", tort)
    # hamaca: tela a rayas
    rayas = (np.sin(xx * 2 * math.pi / 96) > 0)[..., None]
    ham = np.where(rayas, np.array((0.90, 0.35, 0.25))[None, None, :], np.array((0.95, 0.88, 0.60))[None, None, :]) * (0.9 + 0.1 * r.random((N, N, 1)))
    guarda("hamaca", "hamaca.png", ham)
    # fogata: piedra oscura / madera carbonizada
    fog = mezcla((0.10, 0.08, 0.07), (0.38, 0.30, 0.22), fbm(8, 5, 2800)) * (0.85 + 0.3 * r.random((N, N, 1)))
    guarda("fogata", "fogata.png", fog)
    print("texturas OK")


def modelos():
    import bpy, bmesh
    from mathutils import Vector
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from bpy_comun import limpiar, material, exportar
    tex = {"roca_mar": "roca.png", "lancha": "lancha.png", "red_pesca": "red.png", "ave_cuerpo": "ave.png", "ave_ala": "ave.png",
           "tortuga": "tortuga.png", "hamaca": "hamaca.png", "fogata": "fogata.png"}

    def tubo(bm, p0, p1, r0, r1, seg=8):
        p0, p1 = Vector(p0), Vector(p1)
        eje = (p1 - p0).normalized(); a = Vector((1, 0, 0)) if abs(eje.x) < 0.9 else Vector((0, 1, 0))
        u = eje.cross(a).normalized(); v = eje.cross(u).normalized()
        an = [[bm.verts.new(p + (u * math.cos(2 * math.pi * k / seg) + v * math.sin(2 * math.pi * k / seg)) * r) for k in range(seg)] for p, r in ((p0, r0), (p1, r1))]
        for k in range(seg):
            bm.faces.new((an[0][k], an[0][(k + 1) % seg], an[1][(k + 1) % seg], an[1][k]))
        bm.faces.new(an[0][::-1]); bm.faces.new(an[1])

    def caja(bm, c, d, rot=None):
        res = bmesh.ops.create_cube(bm, size=1.0)
        for v in res["verts"]:
            p = Vector((v.co.x * d[0], v.co.y * d[1], v.co.z * d[2]))
            if rot is not None:
                p = rot @ p
            v.co = p + Vector(c)

    def esfera(bm, c, rd, esc=(1, 1, 1), seg=10):
        res = bmesh.ops.create_uvsphere(bm, u_segments=seg, v_segments=seg // 2, radius=rd)
        for v in res["verts"]:
            v.co = Vector((v.co.x * esc[0], v.co.y * esc[1], v.co.z * esc[2])) + Vector(c)

    def exporta(nombre, bm, uv_escala=(2.0, 2.0), plano_uv=None):
        limpiar()
        me = bpy.data.meshes.new(nombre)
        uvl = bm.loops.layers.uv.verify()
        for f in bm.faces:
            for loop in f.loops:
                co = loop.vert.co
                loop[uvl].uv = plano_uv(co) if plano_uv else (co.x * uv_escala[0] + co.y * 0.7, co.z * uv_escala[1] + co.y * 0.4)
        bm.to_mesh(me); bm.free()
        ob = bpy.data.objects.new(nombre, me); bpy.context.collection.objects.link(ob)
        me.materials.append(material(nombre, os.path.join(RAIZ, "Models", nombre, tex[nombre]), alfa=(nombre == "red_pesca")))
        for p in me.polygons:
            p.use_smooth = True
        bpy.context.view_layer.objects.active = ob; ob.select_set(True)
        exportar(ob, nombre)
        print(nombre, len(me.polygons), "caras")

    # --- roca: icosfera deformada con ruido, aplastada y con la base plana
    bm = bmesh.new()
    res = bmesh.ops.create_icosphere(bm, subdivisions=2, radius=1.0)
    rnd = random.Random(25)
    for v in res["verts"]:
        n = (math.sin(3.1 * v.co.x + 1.3) + math.sin(2.7 * v.co.y + 0.4) + math.sin(3.9 * v.co.z + 2.2)) / 3.0
        k = 1.0 + 0.28 * n + 0.08 * rnd.random()
        v.co = Vector((v.co.x * 1.15 * k, v.co.y * 0.95 * k, max(v.co.z * 0.75 * k, -0.05)))
    exporta("roca_mar", bm, plano_uv=lambda c: (c.x * 0.5 + c.y * 0.3, c.z * 0.5 + c.y * 0.2))
    # --- lancha de pescador: casco de 5.2 x 1.5 x 0.75 m con proa y popa, bancas y motor fuera de borda
    bm = bmesh.new()
    L, B, H, sec = 6.0, 1.7, 0.85, 12
    perfiles = []
    for i in range(sec + 1):
        s = i / sec
        y = (s - 0.5) * L
        w = (B / 2) * (math.sin(math.pi * (0.06 + 0.88 * s)) ** 0.6) * (1.0 if s < 0.8 else 1.0 - (s - 0.8) * 1.2)   # la proa (s -> 1, +Y) se afila
        zt = H + 0.25 * (s ** 3) + 0.1 * ((1 - s) ** 3)                                                           # el borde sube en la proa
        perfiles.append([bm.verts.new(p) for p in ((-w, y, zt), (-w * 0.85, y, H * 0.45), (-w * 0.35, y, 0.05), (w * 0.35, y, 0.05), (w * 0.85, y, H * 0.45), (w, y, zt))])
    for i in range(sec):
        a, b = perfiles[i], perfiles[i + 1]
        for k in range(5):
            bm.faces.new((a[k], a[k + 1], b[k + 1], b[k]))
    bm.faces.new(perfiles[0][::-1]); bm.faces.new(perfiles[-1])
    for yb in (-1.0, 0.1, 1.2):
        caja(bm, (0, yb, H * 0.62), (B * 0.82, 0.22, 0.05))          # bancas
    caja(bm, (0, -L / 2 - 0.12, H * 0.9), (0.28, 0.3, 0.45))          # motor fuera de borda
    tubo(bm, (0, -L / 2 - 0.12, 0.5), (0, -L / 2 - 0.12, 0.0), 0.04, 0.04, 6)
    exporta("lancha", bm, plano_uv=lambda c: (c.y / L + 0.5, c.z / 1.0))
    # --- red de pesca: panel de 2.4 x 1.2 m colgado de su borde superior (dos caras)
    bm = bmesh.new()
    for dy, inv in ((0.0, False), (0.004, True)):
        vs = [bm.verts.new(p) for p in ((-1.2, dy, 0.0), (1.2, dy, 0.0), (1.2, dy, -1.2), (-1.2, dy, -1.2))]
        bm.faces.new(vs[::-1] if inv else vs)
    exporta("red_pesca", bm, plano_uv=lambda c: (c.x / 2.4 * 3 + 0.5, -c.z / 1.2 * 1.5))
    # --- ave (pelicano/fragata simplificado): cuerpo con pico largo y cola
    bm = bmesh.new()
    esfera(bm, (0, 0, 0), 0.09, (0.8, 1.7, 0.8), 10)
    esfera(bm, (0, -0.2, 0.03), 0.045, (1, 1, 1), 8)                          # cabeza (frente en -Y de Blender... ver giro en main)
    tubo(bm, (0, -0.24, 0.03), (0, -0.46, -0.01), 0.022, 0.006, 6)            # pico
    tubo(bm, (0, 0.14, 0.0), (0, 0.34, 0.02), 0.03, 0.012, 6)                 # cola
    exporta("ave_cuerpo", bm)
    bm = bmesh.new()
    puntos = [(0.0, 0.04), (0.2, 0.10), (0.55, 0.12), (0.95, 0.0), (0.70, -0.06), (0.50, -0.02), (0.30, -0.10), (0.0, -0.12)]
    vs = [bm.verts.new((x, y, 0.0)) for x, y in puntos]
    for i in range(1, len(vs) - 1):
        bm.faces.new((vs[0], vs[i], vs[i + 1]))
    vs2 = [bm.verts.new((x, y, 0.003)) for x, y in puntos]
    for i in range(1, len(vs2) - 1):
        bm.faces.new((vs2[0], vs2[i + 1], vs2[i]))
    exporta("ave_ala", bm, plano_uv=lambda c: (c.x, c.y * 3 + 0.5))
    # --- tortuga marina (golfina simplificada): caparazon, cabeza, cuello y 4 aletas
    bm = bmesh.new()
    esfera(bm, (0, 0, 0.0), 0.32, (1.0, 1.25, 0.45), 12)
    esfera(bm, (0, 0.42, 0.02), 0.09, (1, 1.2, 0.9), 8)
    for sx in (-1, 1):
        caja(bm, (sx * 0.38, 0.16, -0.03), (0.34, 0.14, 0.03), None)
        caja(bm, (sx * 0.24, -0.34, -0.03), (0.2, 0.12, 0.03), None)
    exporta("tortuga", bm, plano_uv=lambda c: (c.x * 1.5 + 0.5, c.y * 1.2 + 0.5))
    # --- hamaca: dos postes de 1.5 m con una tela que se comba (3.2 m de largo)
    bm = bmesh.new()
    tubo(bm, (-1.6, 0, 0), (-1.6, 0, 1.5), 0.05, 0.04, 6)
    tubo(bm, (1.6, 0, 0), (1.6, 0, 1.5), 0.05, 0.04, 6)
    nseg = 10
    filas = []
    for i in range(nseg + 1):
        x = -1.5 + 3.0 * i / nseg
        z = 1.35 - 0.55 * (1 - (x / 1.5) ** 2)
        filas.append((bm.verts.new((x, -0.32, z)), bm.verts.new((x, 0.32, z + 0.04)), x))
    for i in range(nseg):
        bm.faces.new((filas[i][0], filas[i][1], filas[i + 1][1], filas[i + 1][0]))
    exporta("hamaca", bm, plano_uv=lambda c: (c.x * 0.6 + 0.5, c.y))
    # --- fogata SIN humo: anillo de 8 piedras y 4 troncos cruzados (la llama la pinta lighting.frag, igual que las velas)
    bm = bmesh.new()
    for k in range(8):
        a = 2 * math.pi * k / 8
        caja(bm, (0.42 * math.cos(a), 0.42 * math.sin(a), 0.07), (0.2, 0.16, 0.14))
    for k in range(4):
        a = math.pi * k / 4
        tubo(bm, (-0.3 * math.cos(a), -0.3 * math.sin(a), 0.1), (0.3 * math.cos(a), 0.3 * math.sin(a), 0.2), 0.045, 0.04, 6)
    exporta("fogata", bm)


if modo == "texturas":
    texturas()
elif modo == "modelos":
    modelos()
