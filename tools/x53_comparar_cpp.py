r"""X-53: compara Terreno::altura / Terreno::inclinacion (src/Terreno.h, C++) con tools/x53_terreno.py (Python) en 20 000 puntos al azar.
Sirve para que las herramientas midan EXACTAMENTE el mismo suelo que pisa el visitante. Necesita g++.  Uso: python3 tools/x53_comparar_cpp.py"""
import os, random, subprocess, sys, tempfile
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import x53_terreno as T

random.seed(50)
pts = [(random.uniform(-148, 148), random.uniform(-168, 41)) for _ in range(18000)]
pts += [(random.uniform(-52, 52), random.uniform(-26, 36)) for _ in range(2000)]          # alrededor de la playa y su borde
with tempfile.TemporaryDirectory() as d:
    exe = os.path.join(d, "x53")
    subprocess.check_call(["g++", "-std=c++17", "-O1", os.path.join(T.RAIZ, "tools", "x53_pruebas.cpp"), "-o", exe])
    salida = subprocess.run([exe], input="\n".join("%.6f %.6f" % p for p in pts), capture_output=True, text=True, check=True).stdout.split("\n")
peor_h = peor_g = 0.0; n = 0
for linea in salida:
    if not linea: continue
    x, z, h, gx, gz = map(float, linea.split())
    ph = T.altura(x, z)
    if ph is None: continue
    pgx, pgz = T.inclinacion(x, z, 1.5)
    peor_h = max(peor_h, abs(h - ph)); peor_g = max(peor_g, abs(gx - pgx), abs(gz - pgz)); n += 1
print("puntos comparados: %d  |  mayor diferencia de altura: %.5f m  |  de inclinacion: %.5f" % (n, peor_h, peor_g))
sys.exit(0 if peor_h < 0.01 and peor_g < 0.01 else 1)
