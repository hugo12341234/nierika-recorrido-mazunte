r"""X-49: normaliza los modelos `fuego` y `mariposa_*` (X-43, hechos por D1) para poder usarlos (17/11/2026). Solo Python estandar (sin bpy).
  fuego:     el .obj trae 9 llamas de vela (de la escena de D1) repartidas en 5.6 m, cada una de ~6.7 cm. Se deja UNA (la 3.a por orden de
             posicion: la primera que no esta en una esquina), con el pivote en el centro de su base (y = 0) y escalada x4.5 para que
             sirva de llama de fogata (0.30 m de alto, ~9 cm de ancho). Textura y .mtl no cambian.
  mariposa_*: el juego de 3 piezas (cuerpo, ala_der, ala_izq) mide 1.0 x 1.93 m. Se escala x0.075 (cuerpo 7.5 cm, envergadura 14.5 cm) en
             las 3 por igual, sin mover el pivote (el origen sigue siendo el de las 3 piezas: bisagra de las alas en (0, 0.26, 0) x 0.075).
Idempotente: si el modelo ya esta normalizado (fuego < 0.5 m, mariposa < 0.5 m) no hace nada.
Uso (raiz del repo):  python tools/x49_normalizar.py"""
import collections, os, sys
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
ESC_FUEGO, ESC_MARIPOSA = 4.5, 0.075


def leer(nombre):
    ruta = os.path.join(RAIZ, "Models", nombre, nombre + ".obj")
    return ruta, open(ruta, encoding="utf-8").read().splitlines()


def mariposa(nombre):
    ruta, L = leer(nombre)
    v = [tuple(map(float, l.split()[1:4])) for l in L if l.startswith("v ")]
    if max(max(abs(c) for c in p) for p in v) < 0.5:
        print(nombre, "ya normalizado"); return
    out = []
    for l in L:
        if l.startswith("v "):
            x, y, z = map(float, l.split()[1:4]); out.append("v %.6f %.6f %.6f" % (x * ESC_MARIPOSA, y * ESC_MARIPOSA, z * ESC_MARIPOSA))
        else:
            out.append(l)    # las normales son unitarias: no cambian con una escala uniforme
    open(ruta, "w", encoding="utf-8").write("\n".join(out) + "\n"); print(nombre, "escalado x", ESC_MARIPOSA)


def fuego():
    ruta, L = leer("fuego")
    v = [l for l in L if l.startswith("v ")]; vt = [l for l in L if l.startswith("vt ")]; vn = [l for l in L if l.startswith("vn ")]
    V = [tuple(map(float, l.split()[1:4])) for l in v]
    if len(V) < 1000:
        print("fuego ya normalizado"); return
    f = [[tuple(int(i) - 1 for i in t.split("/")) for t in l.split()[1:]] for l in L if l.startswith("f ")]
    p = list(range(len(V)))
    def fd(a):
        while p[a] != a: p[a] = p[p[a]]; a = p[a]
        return a
    for t in f:
        for q in t[1:]: p[fd(q[0])] = fd(t[0][0])
    comps = collections.defaultdict(list)
    for t in f: comps[fd(t[0][0])].append(t)
    orden = sorted(comps.values(), key=lambda c: min(V[q[0]][0] for t in c for q in t))
    fs = orden[2]
    usados = sorted({q[0] for t in fs for q in t})
    xs = [V[i][0] for i in usados]; ys = [V[i][1] for i in usados]; zs = [V[i][2] for i in usados]
    cx, cz, by = (min(xs) + max(xs)) / 2, (min(zs) + max(zs)) / 2, min(ys)
    out = ["mtllib fuego.mtl", "usemtl fuego", "o fuego"]
    m = {i: n + 1 for n, i in enumerate(usados)}
    for i in usados:
        x, y, z = V[i]; out.append("v %.6f %.6f %.6f" % ((x - cx) * ESC_FUEGO, (y - by) * ESC_FUEGO, (z - cz) * ESC_FUEGO))
    tu = sorted({q[1] for t in fs for q in t}); tm = {i: n + 1 for n, i in enumerate(tu)}
    out += [vt[i] for i in tu]
    nu = sorted({q[2] for t in fs for q in t}); nm = {i: n + 1 for n, i in enumerate(nu)}
    out += [vn[i] for i in nu]
    for t in fs: out.append("f " + " ".join("%d/%d/%d" % (m[q[0]], tm[q[1]], nm[q[2]]) for q in t))
    open(ruta, "w", encoding="utf-8").write("\n".join(out) + "\n")
    print("fuego: 1 llama de %d triangulos, alto %.3f m" % (len(fs), (max(ys) - by) * ESC_FUEGO))


if __name__ == "__main__":
    fuego()
    for n in ("mariposa_cuerpo", "mariposa_ala_der", "mariposa_ala_izq"): mariposa(n)
