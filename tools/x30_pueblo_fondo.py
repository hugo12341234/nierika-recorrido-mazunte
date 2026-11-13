r"""X-30: pueblo de FONDO sobre las colinas -- FUERA DEL ALCANCE ORIGINAL (calles y edificios del pueblo, Punta Cometa), agregado por decision del
equipo del 13/11/2026 (ver ESTADO seccion 4; se quita con MOSTRAR_FUERA_DE_ALCANCE o NIERIKA_SIN_FUERA_ALCANCE=1).
Genera: Models/calles/ (caminos de tierra que siguen el relieve), src/PuebloFondo.h (casas sobre los caminos, promontorio rocoso tipo Punta Cometa con
un mirador pequeno). El promontorio es una FORMACION ROCOSA GENERICA al fondo (rocas grandes + una palapa de mirador): no reproduce el sendero ni el mirador reales.
Uso (raiz del repo):
    python tools/x30_pueblo_fondo.py texturas
    python tools/x30_pueblo_fondo.py cabecera
    "C:\Program Files\Blender Foundation\Blender 5.2\blender.exe" -b --python tools/x30_pueblo_fondo.py -- modelos"""
import math, os, sys, importlib.util
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
modo = sys.argv[-1]
# La altura del relieve es la MISMA funcion que usa tools/x18_colinas.py
sys.argv = [sys.argv[0], "ninguno"]
spec = importlib.util.spec_from_file_location("x18", os.path.join(RAIZ, "tools", "x18_colinas.py"))
x18 = importlib.util.module_from_spec(spec); spec.loader.exec_module(x18)
h = x18.alturaColinas

CAMINOS = {
    "trasero": [(x, -55.0 + 8.0 * math.sin(x / 25.0)) for x in range(-120, 121, 4)],
    "oeste": [(-52.0, -8.0), (-58.0, -20.0), (-66.0, -30.0), (-78.0, -40.0), (-92.0, -48.0), (-104.0, -53.0)],
    "este": [(52.0, -6.0), (59.0, -18.0), (68.0, -28.0), (80.0, -38.0), (94.0, -47.0), (106.0, -52.0)],
}
ANCHO = 4.0


def remuestrea(pts, paso=4.0):
    out = [pts[0]]
    for (x0, z0), (x1, z1) in zip(pts, pts[1:]):
        n = max(1, int(math.hypot(x1 - x0, z1 - z0) / paso))
        for k in range(1, n + 1):
            out.append((x0 + (x1 - x0) * k / n, z0 + (z1 - z0) * k / n))
    return out


def texturas():
    import numpy as np
    from PIL import Image
    r = np.random.default_rng(30)
    N = 1024
    g = r.random((64, 64))
    def per(esc, sem):
        gg = np.random.default_rng(sem).random((esc, esc)); x = np.linspace(0, esc, N, endpoint=False); i = x.astype(int); f = x - i; u = f * f * (3 - 2 * f); i1 = (i + 1) % esc
        a = gg[np.ix_(i, i)]; b = gg[np.ix_(i, i1)]; c = gg[np.ix_(i1, i)]; d = gg[np.ix_(i1, i1)]
        return (a * (1 - u[None, :]) + b * u[None, :]) * (1 - u[:, None]) + (c * (1 - u[None, :]) + d * u[None, :]) * u[:, None]
    t = (per(8, 3000) + 0.5 * per(32, 3001)) / 1.5
    img = np.array((0.42, 0.33, 0.22))[None, None, :] * (0.7 + 0.5 * t[..., None]) * (0.9 + 0.2 * r.random((N, N, 1)))
    d = os.path.join(RAIZ, "Models", "calles"); os.makedirs(d, exist_ok=True)
    Image.fromarray((np.clip(img, 0, 1) * 255).astype(np.uint8), "RGB").save(os.path.join(d, "camino.png"))
    print("textura OK")


def cabecera():
    casas, rocas, = [], []
    paso_casa = 9.0
    for nombre, pts in CAMINOS.items():
        pts = remuestrea(pts)
        acum = 0.0
        for i in range(1, len(pts) - 1):
            (x0, z0), (x1, z1) = pts[i - 1], pts[i + 1]
            acum += math.hypot(pts[i][0] - pts[i - 1][0], pts[i][1] - pts[i - 1][1])
            if acum < paso_casa:
                continue
            acum = 0.0
            dx, dz = x1 - x0, z1 - z0
            L = math.hypot(dx, dz) or 1.0
            nx, nz = -dz / L, dx / L
            for lado in (-1, 1):
                off = 7.5 + (1.5 if (i + lado) % 3 == 0 else 0.0)
                cx, cz = pts[i][0] + nx * off * lado, pts[i][1] + nz * off * lado
                if abs(cx) < 52 and cz > -22:
                    continue                                   # no sobre la playa ni la barda
                alt = h(cx, cz)
                base = min(alt, h(cx + 2.5, cz), h(cx - 2.5, cz), h(cx, cz + 2.0), h(cx, cz - 2.0))   # la casa se apoya en el punto mas bajo de su huella
                pend = max(abs(h(cx + 2.5, cz) - alt), abs(h(cx, cz + 2.5) - alt), abs(h(cx - 2.5, cz) - alt), abs(h(cx, cz - 2.5) - alt)) / 2.5
                if alt < 0.5 or alt > 32 or pend > 0.5:
                    continue                                   # solo donde el terreno es casi plano
                ddx, ddz = -nx * lado, -nz * lado             # hacia el camino
                giro = math.degrees(math.atan2(-ddx, -ddz))
                casas.append((round(cx, 2), round(base - 0.05, 2), round(cz, 2), round(giro, 1), (i + lado) % 2))
    casas = casas[:46]
    # promontorio rocoso tipo Punta Cometa (formacion generica): rocas grandes en el extremo oeste, con un mirador pequeno
    cluster = [(-84.0, 30.0, 22.0, 20.0), (-92.0, 36.0, 30.0, 85.0), (-78.0, 38.0, 14.0, 150.0), (-98.0, 26.0, 26.0, 230.0), (-88.0, 24.0, 12.0, 300.0), (-72.0, 28.0, 9.0, 40.0)]
    for x, z, esc, giro in cluster:
        rocas.append((x, round(h(x, z) - esc * 0.18, 2), z, giro, esc))
    mx, mz = -84.0, 30.0
    mirador = (mx, round(h(mx, mz) + 22.0 * 0.75 - 0.5, 2), mz)       # sobre la roca mayor (esc 22, altura ~0.75 esc)
    def arr(nombre, lst, campos):
        return "    inline const %s %s[%d] = {\n%s\n    };\n" % (campos, nombre, len(lst), ",\n".join("        { " + ", ".join("%.2ff" % v if not isinstance(v, int) else "%d" % v for v in e) + " }" for e in lst))
    txt = ("#ifndef PUEBLO_FONDO_H\n#define PUEBLO_FONDO_H\n// GENERADO por tools/x30_pueblo_fondo.py (no editar a mano). FUERA DEL ALCANCE ORIGINAL: casas del pueblo y promontorio tipo Punta Cometa al fondo.\n"
           "namespace PuebloFondo\n{\n    struct Casa { float x, y, z, giro; int tipo; };\n    struct Roca { float x, y, z, giro, esc; };\n"
           + arr("CASAS", casas, "Casa") + arr("ROCAS", rocas, "Roca") +
           "    constexpr int N_CASAS = %d;\n    constexpr int N_ROCAS = %d;\n    constexpr float MIRADOR_X = %.2ff, MIRADOR_Y = %.2ff, MIRADOR_Z = %.2ff;\n}\n#endif\n" % (len(casas), len(rocas), mirador[0], mirador[1], mirador[2]))
    open(os.path.join(RAIZ, "src", "PuebloFondo.h"), "w", encoding="utf-8", newline="\n").write(txt)
    print("PuebloFondo.h:", len(casas), "casas,", len(rocas), "rocas; mirador en", mirador)


def modelos():
    import bpy, bmesh
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from bpy_comun import limpiar, material, exportar
    limpiar()
    bm = bmesh.new(); uvl = bm.loops.layers.uv.verify()
    for nombre, pts in CAMINOS.items():
        pts = remuestrea(pts, 4.0)
        izq, der = [], []
        for i, (x, z) in enumerate(pts):
            x0, z0 = pts[max(i - 1, 0)]; x1, z1 = pts[min(i + 1, len(pts) - 1)]
            dx, dz = x1 - x0, z1 - z0
            L = math.hypot(dx, dz) or 1.0
            nx, nz = -dz / L, dx / L
            for lado, lista in ((-1, izq), (1, der)):
                px, pz = x + nx * ANCHO / 2 * lado, z + nz * ANCHO / 2 * lado
                lista.append(bm.verts.new((px, -pz, h(px, pz) + 0.30)))       # Blender: Y = -z del juego; +30 cm para que no se hunda en la malla de las colinas
        d = 0.0
        for i in range(len(pts) - 1):
            f = bm.faces.new((izq[i], der[i], der[i + 1], izq[i + 1]))
            l0 = d / 6.0; l1 = (d + 4.0) / 6.0
            for loop, uv in zip(f.loops, ((0, l0), (1, l0), (1, l1), (0, l1))):
                loop[uvl].uv = uv
            d += 4.0
    me = bpy.data.meshes.new("calles"); bm.to_mesh(me); bm.free()
    ob = bpy.data.objects.new("calles", me); bpy.context.collection.objects.link(ob)
    me.materials.append(material("camino", os.path.join(RAIZ, "Models", "calles", "camino.png")))
    for p in me.polygons:
        p.use_smooth = True
        if p.normal.z < 0:
            pass
    bpy.context.view_layer.objects.active = ob; ob.select_set(True)
    exportar(ob, "calles")
    print("calles", len(me.polygons), "caras")


if modo == "texturas":
    texturas()
elif modo == "cabecera":
    cabecera()
elif modo == "modelos":
    modelos()
