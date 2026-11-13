r"""X-18: colinas de fondo (malla de relieve) + textura de pasto + lista de posiciones de vegetacion (src/VegetacionColinas.h).
Las colinas son GENERICAS (costa verde del Pacifico oaxaqueno): NO representan la colina ni el mirador de Punta Cometa (fuera de alcance).
Uso (raiz del repo):
    python tools/x18_colinas.py texturas
    "C:\Program Files\Blender Foundation\Blender 5.2\blender.exe" -b --python tools/x18_colinas.py -- modelos
La altura h(x, z) se define UNA sola vez (alturaColinas) y la comparten la malla y las posiciones de la vegetacion."""
import math, os, sys
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
modo = sys.argv[-1]

# --- Parametros del escenario (ESTADO seccion 5, X-18) ---
X_MIN, X_MAX, Z_MIN, Z_MAX = -150.0, 150.0, -170.0, 45.0   # extension de la malla (m)
PLAYA_X, PLAYA_Z0 = 50.0, -20.0                            # la playa llega a |x| = 50 y a z = -20: ahi h = 0
ALTURA = 34.0                                              # altura maxima aproximada (m)
PASO = 4.0                                                 # separacion de la rejilla (m)
TILE = 10.0                                                # metros por repeticion de la textura


def ruido(x, z, semilla):
    """Ruido de valor suave (hash entero + interpolacion smoothstep), sin dependencias."""
    def h(i, j):
        n = (i * 374761393 + j * 668265263 + semilla * 1442695041) & 0xFFFFFFFF
        n = ((n ^ (n >> 13)) * 1274126177) & 0xFFFFFFFF
        return ((n ^ (n >> 16)) & 0xFFFF) / 65535.0
    i, j = math.floor(x), math.floor(z)
    fx, fz = x - i, z - j
    ux, uz = fx * fx * (3 - 2 * fx), fz * fz * (3 - 2 * fz)
    a, b, c, d = h(i, j), h(i + 1, j), h(i, j + 1), h(i + 1, j + 1)
    return (a * (1 - ux) + b * ux) * (1 - uz) + (c * (1 - ux) + d * ux) * uz


def fbm(x, z, semilla):
    t, amp, f, s = 0.0, 1.0, 1.0, 0.0
    for k in range(4):
        t += amp * ruido(x * f, z * f, semilla + k); s += amp; amp *= 0.5; f *= 2.0
    return t / s


def alturaColinas(x, z):
    """h(x, z) = ALTURA * m(d) * (0.35 + 0.65 * fbm) con m(d) = smoothstep(0, 70, d); d = distancia al borde de la playa."""
    dx = max(abs(x) - PLAYA_X, 0.0)
    dz = max(PLAYA_Z0 - z, 0.0)
    d = math.hypot(dx, dz) if (dx > 0 or dz > 0) else 0.0
    u = min(d / 70.0, 1.0)
    m = u * u * (3 - 2 * u)
    ondas = fbm(x / 55.0, z / 55.0, 7)
    return ALTURA * m * (0.35 + 0.65 * ondas)


def texturas():
    import numpy as np
    from PIL import Image
    N = 1024
    r = np.random.default_rng(18)

    def per(escala, sem):
        g = np.random.default_rng(sem).random((escala, escala))
        x = np.linspace(0, escala, N, endpoint=False); i = x.astype(int); f = x - i; u = f * f * (3 - 2 * f); i1 = (i + 1) % escala
        a = g[np.ix_(i, i)]; b = g[np.ix_(i, i1)]; c = g[np.ix_(i1, i)]; d = g[np.ix_(i1, i1)]
        ux = u[None, :]; uy = u[:, None]
        return (a * (1 - ux) + b * ux) * (1 - uy) + (c * (1 - ux) + d * ux) * uy
    tot = sum(per(8 * 2 ** k, 180 + k) / 2 ** k for k in range(5)); tot /= sum(1 / 2 ** k for k in range(5))
    hebras = r.random((N, N)) * 0.5 + 0.5 * per(256, 190)
    verde1 = np.array((0.07, 0.16, 0.06)); verde2 = np.array((0.22, 0.36, 0.12)); tierra = np.array((0.30, 0.24, 0.12))
    t = tot[..., None]
    base = verde1 * (1 - t) + verde2 * t
    seco = np.clip((per(5, 195) - 0.62) * 4, 0, 1)[..., None]
    base = base * (1 - 0.5 * seco) + tierra * 0.5 * seco
    base *= (0.8 + 0.4 * hebras[..., None])
    os.makedirs(os.path.join(RAIZ, "Models", "colinas"), exist_ok=True)
    Image.fromarray((np.clip(base, 0, 1) * 255).astype(np.uint8), "RGB").save(os.path.join(RAIZ, "Models", "colinas", "colinas_pasto.png"))
    print("textura OK")


def vegetacion():
    """Posiciones de palmas y arbustos sobre las colinas, escritas en src/VegetacionColinas.h (misma h que la malla)."""
    import random
    rnd = random.Random(18)
    palmas, arbustos = [], []
    intentos = 0
    while (len(palmas) < 70 or len(arbustos) < 140) and intentos < 20000:
        intentos += 1
        x = rnd.uniform(-135, 135); z = rnd.uniform(-125, 38)
        if abs(x) < PLAYA_X + 2 and z > PLAYA_Z0 - 2:
            continue                                     # no sobre la playa ni la barda
        h = alturaColinas(x, z)
        if h < 0.8 or h > 24:
            continue                                     # cerca de la playa o demasiado alto: solo laderas
        pend = max(abs(alturaColinas(x + 1, z) - h), abs(alturaColinas(x, z + 1) - h))
        if pend > 0.8:
            continue                                     # no en laderas muy empinadas
        reg = (round(x, 2), round(h, 2), round(z, 2), round(rnd.uniform(0, 360), 1), round(rnd.uniform(0.8, 1.5), 2))
        if rnd.random() < 0.35 and len(palmas) < 70:
            palmas.append(reg)
        elif len(arbustos) < 140:
            arbustos.append(reg)
    def arr(nombre, lst):
        return "    inline const Instancia %s[%d] = {\n%s\n    };\n" % (nombre, len(lst), ",\n".join("        { %.2ff, %.2ff, %.2ff, %.1ff, %.2ff }" % r for r in lst))
    txt = ("#ifndef VEGETACION_COLINAS_H\n#define VEGETACION_COLINAS_H\n// GENERADO por tools/x18_colinas.py (no editar a mano). Posiciones (x, y, z) de palmas y arbustos sobre las colinas,\n"
           "// con giro en Y (grados) y escala. y = altura de la malla en ese punto (misma funcion alturaColinas).\nnamespace VegetacionColinas\n{\n"
           "    struct Instancia { float x, y, z, giro, escala; };\n" + arr("PALMAS", palmas) + arr("ARBUSTOS", arbustos) +
           "    constexpr int N_PALMAS = %d;\n    constexpr int N_ARBUSTOS = %d;\n}\n#endif\n" % (len(palmas), len(arbustos)))
    open(os.path.join(RAIZ, "src", "VegetacionColinas.h"), "w", encoding="utf-8", newline="\n").write(txt)
    print("vegetacion:", len(palmas), "palmas,", len(arbustos), "arbustos")


def modelos():
    import bpy, bmesh
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from bpy_comun import limpiar, material, exportar
    limpiar()
    mat = material("colinas_pasto", os.path.join(RAIZ, "Models", "colinas", "colinas_pasto.png"))
    bm = bmesh.new()
    nx = int((X_MAX - X_MIN) / PASO) + 1; nz = int((Z_MAX - Z_MIN) / PASO) + 1
    fila = {}
    def vert(i, j):
        if (i, j) not in fila:
            x = X_MIN + i * PASO; z = Z_MIN + j * PASO
            # Blender: X = juego X, Y = juego -Z, Z = juego Y
            fila[(i, j)] = bm.verts.new((x, -z, alturaColinas(x, z)))
        return fila[(i, j)]
    uvl = bm.loops.layers.uv.verify()
    for i in range(nx - 1):
        for j in range(nz - 1):
            x0 = X_MIN + i * PASO; x1 = x0 + PASO; z0 = Z_MIN + j * PASO; z1 = z0 + PASO
            # hueco sobre la playa (|x| <= 50 y z >= -20): la playa ya existe ahi
            if x0 >= -PLAYA_X and x1 <= PLAYA_X and z1 > PLAYA_Z0:
                continue
            f = bm.faces.new((vert(i, j), vert(i + 1, j), vert(i + 1, j + 1), vert(i, j + 1)))
            for loop, (x, z) in zip(f.loops, ((x0, z0), (x1, z0), (x1, z1), (x0, z1))):
                loop[uvl].uv = (x / TILE, -z / TILE)
    bmesh.ops.recalc_face_normals(bm, faces=bm.faces)
    for f in bm.faces:
        if f.normal.z < 0:
            f.normal_flip()
    me = bpy.data.meshes.new("colinas"); bm.to_mesh(me); bm.free()
    for p in me.polygons:
        p.use_smooth = True
    ob = bpy.data.objects.new("colinas", me); bpy.context.collection.objects.link(ob)
    me.materials.append(mat)
    exportar(ob, "colinas")
    print("colinas OK:", len(me.polygons), "caras")


if modo == "texturas":
    texturas()
elif modo == "vegetacion":
    vegetacion()
elif modo == "modelos":
    modelos()
