r"""X-9: texturas (numpy + PIL) y modelos (Blender 5.2) de vegetacion decorativa: palma y arbusto (extras, no cuentan entre los 18 obligatorios).
Uso (desde la raiz del repo):
    python tools/x9_vegetacion.py texturas
    "C:\Program Files\Blender Foundation\Blender 5.2\blender.exe" -b --python tools/x9_vegetacion.py -- modelos"""
import os, sys, math
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
N = 1024
modo = sys.argv[-1]


def texturas():
    import numpy as np
    from PIL import Image, ImageDraw
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    rng = np.random.default_rng(9)
    os.makedirs(os.path.join(RAIZ, "Models", "palma"), exist_ok=True)
    os.makedirs(os.path.join(RAIZ, "Models", "arbusto"), exist_ok=True)

    # Tronco: corteza con anillos horizontales y fibras verticales (tonos cafe/gris)
    y = np.arange(N)[:, None]; x = np.arange(N)[None, :]
    anillos = 0.5 + 0.5 * np.sin(y * 2 * math.pi / 64 + 2 * np.sin(x * 2 * math.pi / 256))
    fibra = rng.random((N, 1)) * 0 + rng.random((1, N))
    base = 0.55 * anillos + 0.45 * fibra
    c1 = np.array((0.22, 0.15, 0.09)); c2 = np.array((0.50, 0.40, 0.28))
    tronco = c1[None, None, :] * (1 - base[..., None]) + c2[None, None, :] * base[..., None]
    Image.fromarray((tronco * 255).astype(np.uint8), "RGB").save(os.path.join(RAIZ, "Models", "palma", "palma_tronco.png"))

    def hoja(ruta, n_foliolos, verde1, verde2):
        im = Image.new("RGBA", (N, N), (0, 0, 0, 0))
        d = ImageDraw.Draw(im)
        cx = N // 2
        # v = 0 en la base del tallo (abajo de la imagen) -> la imagen se dibuja con la base abajo
        d.line([(cx, N), (cx, 0)], fill=(60, 80, 30, 255), width=14)
        for k in range(n_foliolos):
            t = (k + 0.5) / n_foliolos
            yy = N - t * N * 0.97
            largo = (1 - 0.55 * t) * N * 0.46
            for lado in (-1, 1):
                x1 = cx + lado * largo
                y1 = yy - largo * 0.35
                g = (np.array(verde1) * (1 - t) + np.array(verde2) * t).astype(int)
                d.line([(cx, yy), (x1, y1)], fill=(int(g[0]), int(g[1]), int(g[2]), 255), width=22)
        im.save(ruta)

    hoja(os.path.join(RAIZ, "Models", "palma", "palma_hoja.png"), 26, (90, 170, 70), (170, 225, 100))
    # Arbusto: hoja de agave/pasto: forma de lanza con nervadura clara
    im = Image.new("RGBA", (N, N), (0, 0, 0, 0))
    a = np.zeros((N, N, 4), np.uint8)
    yy = np.arange(N)[:, None]; xx = np.arange(N)[None, :]
    t = 1 - yy / N                      # 0 base, 1 punta
    ancho = (np.sin(np.pi * np.clip(t, 0, 1) ** 0.6) * 0.46) * N
    dentro = np.abs(xx - N / 2) < ancho
    nerv = np.exp(-((xx - N / 2) / 14.0) ** 2)
    ruido = rng.random((N, N)) * 0.1
    r = 0.12 + 0.25 * t + 0.25 * nerv + ruido
    g = 0.38 + 0.25 * t + 0.20 * nerv + ruido
    b = 0.12 + 0.05 * t + ruido
    a[..., 0] = np.clip(r * 255, 0, 255); a[..., 1] = np.clip(g * 255, 0, 255); a[..., 2] = np.clip(b * 255, 0, 255)
    a[..., 3] = np.where(dentro, 255, 0)
    Image.fromarray(a, "RGBA").save(os.path.join(RAIZ, "Models", "arbusto", "arbusto_hoja.png"))
    print("texturas OK")


def modelos():
    import bpy, bmesh
    from mathutils import Vector
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from bpy_comun import limpiar, material, exportar

    def tira(bm, largo, ancho, caida, ang, base, seg=8):
        """Hoja curva: tira que sale de `base` en la direccion `ang` (rad, plano XY de Blender), se levanta y cae. UV: u ancho, v largo."""
        filas = []
        d = Vector((math.cos(ang), math.sin(ang), 0))
        lado = Vector((-d.y, d.x, 0))
        for i in range(seg + 1):
            s = i / seg
            w = ancho * (1 - 0.15 * s) * (0.35 + 0.65 * math.sin(math.pi * min(1, 0.15 + 0.85 * (1 - s)) )) if ancho else 0
            pos = base + d * (s * largo) + Vector((0, 0, caida[0] * s - caida[1] * s * s))
            filas.append((bm.verts.new(pos - lado * w / 2), bm.verts.new(pos + lado * w / 2), s))
        for i in range(seg):
            a, b, _ = filas[i]; c, e, _ = filas[i + 1]
            f = bm.faces.new((a, b, e, c))
            uvl = bm.loops.layers.uv.verify()
            for loop, uv in zip(f.loops, ((0, filas[i][2]), (1, filas[i][2]), (1, filas[i + 1][2]), (0, filas[i + 1][2]))):
                loop[uvl].uv = uv

    # --- Palma: tronco curvo de 8.5 m (X-32) + 9 hojas de 3 m
    limpiar()
    m_tronco = material("palma_tronco", os.path.join(RAIZ, "Models", "palma", "palma_tronco.png"))
    m_hoja = material("palma_hoja", os.path.join(RAIZ, "Models", "palma", "palma_hoja.png"), alfa=True)
    m_hoja.blend_method = "HASHED" if hasattr(m_hoja, "blend_method") else None
    bm = bmesh.new()
    seg, alto, cur = 10, 8.5, 0.8
    anillos = []
    for i in range(seg + 1):
        s = i / seg
        r = 0.16 - 0.06 * s + (0.03 if i == 0 else 0)
        centro = Vector((cur * s * s, 0, alto * s))
        anillo = [bm.verts.new(centro + Vector((r * math.cos(2 * math.pi * k / 12), r * math.sin(2 * math.pi * k / 12), 0))) for k in range(12)]
        anillos.append(anillo)
    uvl = bm.loops.layers.uv.verify()
    for i in range(seg):
        for k in range(12):
            f = bm.faces.new((anillos[i][k], anillos[i][(k + 1) % 12], anillos[i + 1][(k + 1) % 12], anillos[i + 1][k]))
            f.material_index = 0
            for loop, uv in zip(f.loops, ((k / 12, i / seg), ((k + 1) / 12, i / seg), ((k + 1) / 12, (i + 1) / seg), (k / 12, (i + 1) / seg))):
                loop[uvl].uv = (uv[0] * 2, uv[1] * 3)
    n_antes = len(bm.faces)
    cima = Vector((cur, 0, alto))
    for h in range(9):
        tira(bm, 3.0, 1.0, (0.9, 1.6), 2 * math.pi * h / 9 + 0.2 * (h % 2), cima + Vector((0, 0, 0.05 * (h % 3))))
    for i, f in enumerate(bm.faces):
        if i >= n_antes:
            f.material_index = 1
    me = bpy.data.meshes.new("palma"); bm.to_mesh(me); bm.free()
    ob = bpy.data.objects.new("palma", me); bpy.context.collection.objects.link(ob)
    me.materials.append(m_tronco); me.materials.append(m_hoja)
    for p in me.polygons: p.use_smooth = True
    exportar(ob, "palma")

    # --- Arbusto (agave): 14 hojas rigidas de 0.7 m
    limpiar()
    m_hoja = material("arbusto_hoja", os.path.join(RAIZ, "Models", "arbusto", "arbusto_hoja.png"), alfa=True)
    bm = bmesh.new()
    for h in range(14):
        ang = 2 * math.pi * h / 14 + 0.15 * (h % 3)
        L = 0.55 + 0.2 * ((h * 7) % 5) / 4
        tira(bm, L, 0.18, (0.35 + 0.2 * (h % 2), 0.1), ang, Vector((0, 0, 0.0)), seg=6)
    me = bpy.data.meshes.new("arbusto"); bm.to_mesh(me); bm.free()
    ob = bpy.data.objects.new("arbusto", me); bpy.context.collection.objects.link(ob)
    me.materials.append(m_hoja)
    for p in me.polygons: p.use_smooth = True
    exportar(ob, "arbusto")
    print("modelos OK")


if modo == "texturas":
    texturas()
elif modo == "modelos":
    modelos()
