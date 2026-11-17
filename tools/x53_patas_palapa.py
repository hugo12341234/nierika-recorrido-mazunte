r"""X-53: patas para la mesa y la banca de la palapa (Models/palapa/palapa.obj) SIN Blender. Idempotente (python3 tools/x53_patas_palapa.py).
Por que: tools/x26_construcciones.py modelo la mesa (1.6 x 0.9 m, tablon de 6 cm a 0.45 m) y la banca (1.6 x 0.35 m, tablon de 5 cm a 0.28 m) como tablones
SUELTOS: sin patas se veian como dos planchas que flotan junto a la arena (auditoria del 17/11, "las bancas estan mal").
Se agregan 4 patas de 8 x 8 cm a la mesa y 4 de 6 x 6 cm a la banca, inscritas 6 cm desde las esquinas y apoyadas en y = 0.
Coordenadas del .obj (juego): x, y arriba, z = -(y de Blender); la mesa va en (0, 0.45, 0) y la banca en (0, 0.28, +0.85). El mismo cambio esta en
tools/x26_construcciones.py (funcion modelos) para quien regenere la palapa con Blender."""
import os
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
RUTA = os.path.join(RAIZ, "Models", "palapa", "palapa.obj")
MARCA = "# X-53: patas de la mesa y la banca"


def patas():
    # (centro x, z), semilado, y inferior, y superior
    esp = []
    for cx, cz, ax, az, ytop, h in ((0.0, 0.0, 0.80, 0.45, 0.45 - 0.03, 0.04), (0.0, 0.85, 0.80, 0.175, 0.28 - 0.025, 0.03)):
        for sx in (-1, 1):
            for sz in (-1, 1):
                esp.append((cx + sx * (ax - 0.06 - h), cz + sz * (az - 0.06 - h), h, 0.0, ytop))
    return esp


def main():
    with open(RUTA, encoding="utf-8", newline="") as f:
        crudo = f.read()
    if MARCA in crudo:
        print("palapa.obj ya tiene patas")
        return
    lin = crudo.replace("\r\n", "\n").rstrip("\n").split("\n")
    nv = sum(1 for l in lin if l.startswith("v ")); nvt = sum(1 for l in lin if l.startswith("vt ")); nvn = sum(1 for l in lin if l.startswith("vn "))
    # insertar los v / vt / vn nuevos junto a los demas es opcional en OBJ: se agregan al final y las caras los nombran por indice absoluto
    nuevos_v, nuevos_vt, nuevos_vn, caras = [], [], [], []
    normales = {"+x": (1, 0, 0), "-x": (-1, 0, 0), "+z": (0, 0, 1), "-z": (0, 0, -1), "-y": (0, -1, 0)}
    base_n = nvn
    for k, (name, n) in enumerate(normales.items()):
        nuevos_vn.append("vn %.4f %.4f %.4f" % n)
    idx_n = {name: base_n + 1 + k for k, name in enumerate(normales)}
    for (cx, cz, h, y0, y1) in patas():
        def v(dx, y, dz): 
            nuevos_v.append("v %.6f %.6f %.6f" % (cx + dx, y, cz + dz)); return nv + len(nuevos_v)
        def vt(px, py, pz):
            nuevos_vt.append("vt %.6f %.6f" % (px * 0.3 + (-pz) * 0.17, py * 0.4 + (-pz) * 0.12)); return nvt + len(nuevos_vt)
        # cada cara con sus 4 vertices propios (uv segun la misma formula de la palapa)
        def cara(ps, nombre):
            ids = []
            for (dx, y, dz) in ps:
                a = v(dx, y, dz); b = vt(cx + dx, y, cz + dz); ids.append("%d/%d/%d" % (a, b, idx_n[nombre]))
            caras.append("f " + " ".join(ids))
        # lados, vistos desde afuera (antihorario)
        cara([(h, y0, -h), (h, y1, -h), (h, y1, h), (h, y0, h)], "+x")
        cara([(-h, y0, h), (-h, y1, h), (-h, y1, -h), (-h, y0, -h)], "-x")
        cara([(h, y0, h), (h, y1, h), (-h, y1, h), (-h, y0, h)], "+z")
        cara([(-h, y0, -h), (-h, y1, -h), (h, y1, -h), (h, y0, -h)], "-z")
    salida = lin + [MARCA] + nuevos_v + nuevos_vt + nuevos_vn + ["usemtl palapa_madera"] + caras
    with open(RUTA, "w", encoding="utf-8", newline="") as f:
        f.write(("\r\n" if "\r\n" in crudo else "\n").join(salida) + ("\r\n" if "\r\n" in crudo else "\n"))
    print("palapa.obj: %d patas, %d caras nuevas" % (len(patas()), len(caras)))


main()
