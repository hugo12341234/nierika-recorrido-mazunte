r"""X-50: escribe Models/colinas/colinas.obj SIN Blender (python3 tools/x50_colinas_obj.py), con la misma malla de x18_colinas.py
(rejilla de 4 m, hueco sobre la playa, franja recortada z -22..-20 de X-40) pero con las UV planas correctas.
Por que: el .obj exportado por Blender traia UV equivocadas en 51 caras (la franja z -22..-20 de 100 m y la columna x = 146..150: dos de sus
vertices tomaban la UV de otro lado y la textura del pasto se estiraba en rayas, junto a la arena; se veia como "tramo de suelo levantado").
UV = (x / 10, -z / 10), una por vertice (mapeo plano, sin costuras). Normales suaves (promedio de las caras vecinas).
Formato = el del resto de los .obj del repo (v / vt / vn, caras v/vt/vn, `usemtl colinas_pasto`). x18_colinas.py (modo `modelos`) sigue
siendo la fuente de la malla; este script es el camino sin Blender y las dos salidas deben ser la misma malla (ver tools/x50_verificar_alturas.py, R12)."""
import math, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import x50_terreno as T

TILE = 10.0


def construir():
    nx = int((T.X_MAX - T.X_MIN) / T.PASO) + 1
    nz = int((T.Z_MAX - T.Z_MIN) / T.PASO) + 1
    verts, idx = [], {}

    def vert(clave, x, z, y):
        if clave not in idx:
            idx[clave] = len(verts); verts.append((x, y, z))
        return idx[clave]

    caras = []
    for i in range(nx - 1):
        for j in range(nz - 1):
            x0 = T.X_MIN + i * T.PASO; x1 = x0 + T.PASO; z0 = T.Z_MIN + j * T.PASO; z1 = z0 + T.PASO
            dentro_x = x0 >= -T.PLAYA_X and x1 <= T.PLAYA_X
            if dentro_x and z1 > T.PLAYA_Z0:
                if z0 >= T.PLAYA_Z0:
                    continue                                     # sobre la playa: ya existe
                z1 = T.PLAYA_Z0                                  # X-40: celda recortada hasta z = -20 (h = 0)
            def v(ii, x, z):
                y = 0.0 if z == T.PLAYA_Z0 and dentro_x else T.h_visible(x, z)
                return vert((ii, z), x, z, y)
            # (i,j) -> (i,j+1) -> (i+1,j+1) -> (i+1,j): antihorario visto desde arriba (normal +y); diagonal (i,j)-(i+1,j+1)
            caras.append((v(i, x0, z0), v(i, x0, z1), v(i + 1, x1, z1), v(i + 1, x1, z0)))
    # Faldon bajo la playa (X-50): la playa (playa.obj) y la malla se tocan en x = +-50 y en z = -20 con vertices que no coinciden (la playa los tiene cada 2 m,
    # la malla cada 4 m): en las uniones en T se cuela un hilo de color del cielo (la "costura" azul junto a la arena). Una tira de 0.5 m de la malla, 3 cm
    # POR DEBAJO de la playa y metida bajo ella, tapa esa rendija con pasto. No se ve (la playa queda encima) y no cambia la altura del suelo.
    FALDON, BAJA = 0.5, 0.03
    xs_n = [T.X_MIN + 4.0 * k for k in range(int((-T.PLAYA_X - T.X_MIN) / 4.0), int((T.PLAYA_X - T.X_MIN) / 4.0) + 1)]
    for a, b in zip(xs_n, xs_n[1:]):                                  # lado norte (z = -20): de x = a a x = b
        caras.append((vert(("e", a, -20.0), a, -20.0, 0.0), vert(("fn", a), a, -20.0 + FALDON, -BAJA), vert(("fn", b), b, -20.0 + FALDON, -BAJA), vert(("e", b, -20.0), b, -20.0, 0.0)))
    zs_l = [-20.0] + [T.Z_MIN + 4.0 * k for k in range(int((-18.0 - T.Z_MIN) / 4.0), int((T.Z_MAX - T.Z_MIN) / 4.0) + 1)]
    for lado in (-1, 1):                                               # lados este y oeste (x = +-50)
        xe, xi = lado * T.PLAYA_X, lado * (T.PLAYA_X - FALDON)
        for za, zb in zip(zs_l, zs_l[1:]):
            ya, yb = (0.0 if za == -20.0 else T.h_visible(xe, za)), T.h_visible(xe, zb)
            cs = (vert(("e", xe, za), xe, za, ya), vert(("fl", lado, za), xi, za, ya - BAJA), vert(("fl", lado, zb), xi, zb, yb - BAJA), vert(("e", xe, zb), xe, zb, yb))
            caras.append(cs if lado > 0 else cs[::-1])                 # que la normal mire hacia arriba en los dos lados
    normales = [[0.0, 0.0, 0.0] for _ in verts]
    for c in caras:
        for a, b, d in ((c[0], c[1], c[2]), (c[0], c[2], c[3])):
            p, q, r = verts[a], verts[b], verts[d]
            e1 = [q[k] - p[k] for k in range(3)]; e2 = [r[k] - p[k] for k in range(3)]
            n = [e1[1] * e2[2] - e1[2] * e2[1], e1[2] * e2[0] - e1[0] * e2[2], e1[0] * e2[1] - e1[1] * e2[0]]
            for w in (a, b, d):
                for k in range(3): normales[w][k] += n[k]
    for n in normales:
        m = math.sqrt(sum(c * c for c in n)) or 1.0
        n[:] = [c / m for c in n]
    return verts, normales, caras


def escribir(ruta):
    verts, normales, caras = construir()
    lin = ["# X-50: generado por tools/x50_colinas_obj.py (no editar a mano)", "mtllib colinas.mtl", "o colinas"]
    lin += ["v %.6f %.6f %.6f" % v for v in verts]
    lin += ["vt %.6f %.6f" % (v[0] / TILE, -v[2] / TILE) for v in verts]
    lin += ["vn %.4f %.4f %.4f" % tuple(n) for n in normales]
    lin += ["s 1", "usemtl colinas_pasto"]
    for c in caras:
        lin.append("f " + " ".join("%d/%d/%d" % (k + 1, k + 1, k + 1) for k in c))
    with open(ruta, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lin) + "\n")
    return len(verts), len(caras)


if __name__ == "__main__":
    destino = os.path.join(T.RAIZ, "Models", "colinas", "colinas.obj")
    print("colinas.obj: %d vertices, %d caras" % escribir(destino))
