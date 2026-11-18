r"""X-49: elige las posiciones de las mariposas de las colinas, las verifica y escribe src/MariposasDatos.h (17/11/2026).
Reglas: grupos de 3 mariposas alrededor de racimos de cempasuchil (VidaColinasDatos::RACIMOS) de las colinas (|x| > 55 o z < -25), repartidos
en 6 zonas; un grupo se acepta si la TRAYECTORIA COMPLETA (misma formula en x-z que src/Mariposas.h, muestreada 0..900 s cada 0.45 s):
  - queda a >= 0.8 m de la huella de cualquier casa, palapa o roca (usa los cargadores de tools/x48_verificar_solapes.py),
  - queda a >= radio + 0.5 m del tronco de cualquier palma, a >= 0.6 m de faroles, cruces, fogatas, personas y de los troncos de arboles (0.5 m),
  - queda dentro de la zona caminable (Terreno::LIM_*) y a >= 6 m de la playa.
No usa colisiones: las mariposas no chocan con nada ni cuentan para las colisiones AABB del visitante.
Uso (raiz del repo):  python tools/x49_mariposas.py          (reescribe src/MariposasDatos.h y avisa si algo no pasa)"""
import math, os, random, sys
AQUI = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, AQUI)
import x48_verificar_solapes as x48
RAIZ = x48.RAIZ
N_GRUPOS, POR_GRUPO = 6, 3


def trayectoria(i, cx, cz, A, B, f):
    k = 1.0 + 0.08 * (i % 5)
    for n in range(2000):
        t = n * 0.45
        yield cx + A * math.sin(0.36 * k * t + f), cz + B * math.sin(0.53 * k * t + 2.0 * f)


def ok(i, cx, cz, A, B, f, E, arboles):
    for x, z in trayectoria(i, cx, cz, A, B, f):
        if abs(x) > 135 or z < -155 or (abs(x) <= 56 and z > -26): return "fuera de zona"
        p = (x, z)
        for c in E["casas"]:
            if x48.dist_poly(p, c[2]) < 0.8: return "casa/palapa " + str(c[:2])
        for r in E["rocas"]:
            if x48.dist_poly(p, r[2]) < 0.8: return "roca " + str(r[:2])
        for q in E["palmas"]:
            if math.hypot(x - q[2], z - q[3]) < q[4] + 0.5: return "palma " + str(q[:2])
        for q in arboles:
            if math.hypot(x - q[0], z - q[1]) < 0.5 + 0.5: return "arbol"
        for nombre in ("faroles", "cruces", "fogatas"):
            for q in E[nombre]:
                if math.hypot(x - q[2], z - q[3]) < (1.0 if nombre == "fogatas" else 0.6): return nombre
        for q in E["personas"]:
            if math.hypot(x - q[2], z - q[3]) < q[4] + 0.6: return "persona"
    return None


def main():
    E = x48.cargar()
    arboles = [(p[0], p[2]) for p in x48.leer_h("VidaColinasDatos.h", "ARBOLES")]
    racimos = [(p[0], p[2]) for p in x48.leer_h("VidaColinasDatos.h", "RACIMOS") if (abs(p[0]) > 55 or p[2] < -25)]
    rnd = random.Random(49)
    # grupos repartidos: cada uno a >= 25 m de los ya aceptados; se recorren los racimos en orden aleatorio (semilla fija)
    datos, centros = [], []
    cand = racimos[:]; rnd.shuffle(cand)
    for (rx, rz) in cand:
        if len(centros) >= N_GRUPOS: break
        if any(math.hypot(rx - c[0], rz - c[1]) < 25.0 for c in centros): continue
        cx, cz = rx, rz
        if any(math.hypot(cx - c[0], cz - c[1]) < 25.0 for c in centros): continue
        grupo = []
        for intento in range(40):               # cada mariposa se prueba por separado alrededor del racimo (+-6 m); se descartan las que chocan
            i = len(datos) + len(grupo)
            A, B = 2.0 + 0.8 * (i % 3), 1.6 + 0.7 * ((i + 1) % 3)
            f = rnd.uniform(0, 2 * math.pi)
            px, pz = cx + rnd.uniform(-6.0, 6.0), cz + rnd.uniform(-6.0, 6.0)
            if not ok(i, px, pz, A, B, f, E, arboles): grupo.append((px, pz, f, A, B))
            if len(grupo) == POR_GRUPO: break
        if len(grupo) == POR_GRUPO:
            datos += grupo; centros.append((cx, cz)); print("grupo %d cerca del racimo (%.1f, %.1f)" % (len(centros), cx, cz))
    if len(centros) < N_GRUPOS: print("AVISO: solo %d grupos validos" % len(centros))
    with open(os.path.join(RAIZ, "src", "MariposasDatos.h"), "w", encoding="utf-8", newline="\n") as o:
        o.write("#pragma once\n\n// X-49: GENERADO por tools/x49_mariposas.py (no editar a mano). Centro (cx, cz) del vuelo, semiejes A (x) y B (z) en metros y fase en radianes.\n"
                "// Las formulas estan en Mariposas.h. Todas las trayectorias verificadas contra casas, rocas, palapas, palmas, arboles, faroles, cruces, fogatas y personas.\n\n"
                "namespace MariposasDatos\n{\n    struct Mariposa { float cx, cz, A, B, fase; };\n")
        o.write("    constexpr int N_MARIPOSAS = %d;\n    inline const Mariposa MARIPOSAS[%d] = {\n" % (len(datos), len(datos)))
        for (cx, cz, f, A, B) in datos: o.write("        { %.2ff, %.2ff, %.1ff, %.1ff, %.3ff },\n" % (cx, cz, A, B, f))
        o.write("    };\n}\n")
    print("mariposas escritas:", len(datos))


if __name__ == "__main__":
    main()
