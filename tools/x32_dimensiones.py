"""X-32: verificacion de dimensiones (1 unidad = 1 metro). Mide la caja envolvente de cada Models/<objeto>/<objeto>.obj, la multiplica por la escala con que
main.cpp lo dibuja y la compara con una medida esperada. Escribe docs/DIMENSIONES_VERIFICADAS.md.
Uso (raiz del repo): python tools/x32_dimensiones.py [etiqueta]   (etiqueta = 'antes' o 'despues', solo se escribe en el titulo)"""
import os, sys, glob
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
etiqueta = sys.argv[1] if len(sys.argv) > 1 else "despues"

# modelo: (escala con la que se dibuja en la escena, eje a medir, (minimo, maximo) esperado en metros, fuente de la medida esperada)
# Las medidas esperadas son referencias TIPICAS (indicadas por el equipo o de uso comun); no son mediciones en Mazunte.
REF = {
    "persona_pie_a": (1.0, "y", (1.55, 1.80), "estatura adulta tipica ~1.6-1.75 m (indicacion de D2: ~1.6 m)"),
    "persona_pie_b": (1.0, "y", (1.55, 1.80), "estatura adulta tipica ~1.6-1.75 m"),
    "persona_agachada_a": (1.0, "y", (0.9, 1.2), "persona de rodillas: ~60 % de la estatura"),
    "silueta/torso": (1.0, "y", (0.5, 0.7), "torso de adulto ~0.55-0.65 m"),
    "casa_a": (1.0, "y", (3.0, 4.2), "casa de un piso: muros ~2.6 m y techo a dos aguas hasta ~3.8 m (indicacion de D2: ~3 m de alto)"),
    "casa_b": (1.0, "y", (3.0, 4.2), "igual que casa_a"),
    "palapa": (1.0, "y", (3.5, 5.0), "palapa de playa: postes de ~2.8 m y techo hasta ~4.4 m"),
    "lancha": (1.0, "z", (5.5, 7.0), "panga de pescador ~6 m (indicacion de D2: ~6 m)"),
    "palma": (1.0, "y", (8.0, 12.0), "palma de coco adulta 8-12 m (indicacion de D2)"),
    "arbusto": (1.0, "y", (0.4, 1.0), "agave/arbusto bajo de ~0.5-0.8 m"),
    "tortuga": (0.85, "z", (0.55, 0.80), "tortuga golfina adulta: caparazon de ~0.6-0.7 m (referencia general, por verificar)"),
    "ave_ala": (1.2, "x", (0.9, 1.3), "una ala de pelicano/fragata ~1.0-1.2 m (envergadura 2-2.5 m, referencia general)"),
    "murcielago_ala": (1.0, "x", (0.35, 0.6), "un ala de murcielago ~0.2-0.3 m (se exagera para que se vea de lejos)"),
    "tabla_remo": (1.0, "z", (3.0, 3.6), "tabla de remo (SUP) 3.0-3.6 m"),
    "farol": (1.0, "y", (1.5, 1.9), "farol de calle o de jardin ~1.6-1.8 m"),
    "guitarra": (1.0, "y", (0.8, 1.05), "guitarra clasica ~1.0 m"),
    "tambor": (1.0, "x", (0.35, 0.5), "tambor de marcha ~0.4 m de diametro"),
    "trompeta": (1.0, "z", (0.45, 0.65), "trompeta ~0.5 m"),
    "hamaca": (1.0, "x", (3.0, 3.6), "hamaca con postes: ~3.2 m"),
    "fogata": (1.0, "x", (0.8, 1.2), "anillo de fogata ~1 m"),
    "vela": (1.0, "y", (0.12, 0.20), "vela de ofrenda ~0.15 m"),
    "altar3": (1.0, "y", (1.1, 1.3), "altar de tres niveles de ~1.2 m"),
    "cruz": (1.0, "y", (1.3, 1.7), "cruz de entrada ~1.5 m"),
    "barda": (1.0, "y", (0.6, 1.0), "barda baja de ~0.7 m"),
    "fachada": (1.0, "y", (2.5, 3.5), "cuarto: muros de ~2.6 m"),
    "arco": (1.0, "ymax", (2.0, 2.4), "cima del arco de entrada = alto de puerta ~2.1 m (+ grosor de las dovelas); indicacion de D2: puerta ~2.1 m"),
    "botella": (1.0, "y", (0.25, 0.35), "botella de ~0.3 m"),
    "marco": (1.0, "y", (0.25, 0.45), "marco de foto ~0.3 m"),
}


def caja(ruta):
    xs, ys, zs = [], [], []
    for l in open(ruta, encoding='utf-8', errors='ignore'):
        if l.startswith('v '):
            _, x, y, z = l.split()[:4]
            xs.append(float(x)); ys.append(float(y)); zs.append(float(z))
    return (min(xs), max(xs)), (min(ys), max(ys)), (min(zs), max(zs))


if etiqueta == "antes":    # escalas con las que se dibujaban antes de X-32
    REF["ave_ala"] = (2.4,) + REF["ave_ala"][1:]; REF["murcielago_ala"] = (2.2,) + REF["murcielago_ala"][1:]; REF["tortuga"] = (1.0,) + REF["tortuga"][1:]
filas = []
fallos = 0
for nombre, (esc, eje, (lo, hi), fuente) in REF.items():
    carpeta, archivo = (nombre.split('/') + [nombre.split('/')[0]])[:2] if '/' in nombre else (nombre, nombre)
    ruta = os.path.join(RAIZ, "Models", carpeta, archivo + ".obj")
    if not os.path.exists(ruta):
        filas.append((nombre, eje, lo, hi, None, esc, "no existe", fuente)); continue
    bx, by, bz = caja(ruta)
    ext = {"x": bx[1] - bx[0], "y": by[1] - by[0], "z": bz[1] - bz[0], "ymax": by[1]}[eje]
    real = ext * esc
    ok = lo <= real <= hi
    fallos += 0 if ok else 1
    filas.append((nombre, eje, lo, hi, real, esc, "OK" if ok else "FUERA DE RANGO", fuente))

md = ["# Dimensiones verificadas (1 unidad = 1 metro) — %s" % etiqueta, "",
      "> Generado por `tools/x32_dimensiones.py` el 13/11/2026: mide la caja envolvente de cada `.obj` y la multiplica por la escala con que `src/main.cpp` lo dibuja.",
      "> **Las medidas esperadas son referencias típicas o indicadas por el equipo, no mediciones hechas en Mazunte** (la columna «Fuente» lo dice). Los rangos son de aceptación, no verdades exactas.", "",
      "| Modelo | Eje | Esperado (m) | Medida real en escena (m) | Escala en escena | Resultado | Fuente de la medida esperada |", "|---|---|---|---|---|---|---|"]
for n, e, lo, hi, real, esc, res, f in filas:
    md.append("| `%s` | %s | %.2f – %.2f | %s | %.2f | %s | %s |" % (n, e, lo, hi, ("%.2f" % real) if real is not None else "—", esc, res, f))
md += ["", "**Resumen:** %d modelos revisados, %d fuera de rango." % (len(filas), fallos), ""]
open(os.path.join(RAIZ, "docs", "DIMENSIONES_VERIFICADAS.md" if etiqueta == "despues" else "DIMENSIONES_ANTES.md"), "w", encoding="utf-8", newline="\n").write("\n".join(md))
print(len(filas), "modelos;", fallos, "fuera de rango")
for n, e, lo, hi, real, esc, res, f in filas:
    if res != "OK":
        print("  ", n, e, "esperado %.2f-%.2f" % (lo, hi), "real", real, res)
