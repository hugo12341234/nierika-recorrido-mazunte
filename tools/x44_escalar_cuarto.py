# X-44 · Agranda el cuarto (fachada, arco, techo y piso) por K alrededor del origen, igual que A-2 pero en Blender/obj.
# Los vertices y las UV se multiplican por K (la UV es 1 u = 1 m, asi la textura conserva su tamano); las normales no cambian.
# Uso: python3 tools/x44_escalar_cuarto.py        (una sola vez; K = 1.4, decision del 15/11)
import os
K = 1.4
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
for n in ("fachada", "arco", "techo", "piso"):
    p = os.path.join(RAIZ, "Models", n, n + ".obj")
    salida = []
    for l in open(p, encoding="utf-8"):
        a = l.split()
        if l.startswith("v "):
            l = "v %.6f %.6f %.6f\n" % tuple(K * float(c) for c in a[1:4])
        elif l.startswith("vt "):
            l = "vt %.6f %.6f\n" % tuple(K * float(c) for c in a[1:3])
        salida.append(l)
    open(p, "w", encoding="utf-8", newline="").write("".join(salida))
    print(n, "escalado x", K)
