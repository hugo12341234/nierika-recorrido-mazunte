# E-5: tabla de los modelos propios del inventario (ESTADO §2) calculada de los archivos del repo.
# Uso (desde la raíz):  python3 tools/e5_tabla_modelos.py   (necesita Pillow para leer el tamaño de las texturas)
# Imprime una tabla Markdown: caras triangulables, vértices, dimensiones (m), .blend presente y texturas con su tamaño.
import glob
import os
import re
from PIL import Image

RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
# (n.º del inventario, carpeta, archivo .obj; None = todos los .obj de la carpeta)
INVENTARIO = [
    (1, "fachada", "fachada.obj"), (2, "techo", "techo.obj"), (3, "arco", "arco.obj"), (4, "piso", "piso.obj"),
    (5, "playa", "playa.obj"), (6, "mar", "mar.obj"), (7, "tumba1", "tumba1.obj"), (8, "tumba2", "tumba2.obj"),
    (9, "tumba3", "tumba3.obj"), (10, "tumba4", "tumba4.obj"), (11, "barda", "barda.obj"), (12, "roca", "roca.obj"),
    (13, "cruz", "cruz.obj"), (14, "soportes", "soportes.obj"), (15, "papel_picado", "papel_picado.obj"),
    (16, "altar3", "altar3.obj"), (17, "vela", "vela.obj"), (18, "cempasuchil", "cempasuchil.obj"),
    (19, "pan_muerto", "pan_muerto.obj"), (20, "botella", "botella.obj"), (21, "marco", "marco.obj"),
    (22, "silueta", None), (23, "copalero", "copalero.obj"),
]


def medir(rutas):
    caras = verts = 0
    mn = [1e9] * 3
    mx = [-1e9] * 3
    for r in rutas:
        for linea in open(r, encoding="utf-8", errors="ignore"):
            if linea.startswith("v "):
                v = [float(x) for x in linea.split()[1:4]]
                verts += 1
                for i in range(3):
                    mn[i] = min(mn[i], v[i]); mx[i] = max(mx[i], v[i])
            elif linea.startswith("f "):
                caras += len(linea.split()) - 3   # triángulos por cara (n - 2)
    return caras, verts, [mx[i] - mn[i] for i in range(3)]


def texturas(carpeta):
    out = set()
    for mtl in glob.glob(os.path.join(carpeta, "*.mtl")):
        for linea in open(mtl, encoding="utf-8", errors="ignore"):
            m = re.match(r"\s*map_(Kd|Bump)\s+(\S+)", linea)
            if m:
                ruta = os.path.join(carpeta, m.group(2))
                if os.path.exists(ruta):
                    out.add(f"`{m.group(2)}` {Image.open(ruta).size[0]}×{Image.open(ruta).size[1]}")
                else:
                    out.add(f"`{m.group(2)}` (no existe)")
    return ", ".join(sorted(out)) or "—"


if __name__ == "__main__":
    print("| # | Modelo | Triángulos | Vértices | Dimensiones x · y · z (m) | `.blend` | Texturas (px) |")
    print("|---|---|---|---|---|---|---|")
    for n, carpeta, archivo in INVENTARIO:
        base = os.path.join(RAIZ, "Models", carpeta)
        rutas = [os.path.join(base, archivo)] if archivo else [r for r in sorted(glob.glob(os.path.join(base, "*.obj"))) if not r.endswith("silueta.obj")]
        c, v, d = medir(rutas)
        blend = "sí" if glob.glob(os.path.join(base, "*.blend")) else "**no**"
        print(f"| {n} | `{carpeta}` | {c} | {v} | {d[0]:.2f} · {d[1]:.2f} · {d[2]:.2f} | {blend} | {texturas(base)} |")
