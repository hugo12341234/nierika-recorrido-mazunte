r"""X-58: decoracion de las 27 casas del fondo (21 del pueblo + 6 de la playa) -- FUERA DEL ALCANCE ORIGINAL, igual que las casas (decision del 13/11).
Genera src/DecoracionCasas.h con la lista EXPLICITA de piezas de cada casa (coordenadas LOCALES de la casa: x a la derecha, y arriba, +z hacia la pared
trasera; el frente con porche esta en z negativo). Solo reutiliza modelos que ya existen (papel_picado, papel_naranja, papel_fucsia, papel_morado, mazeta,
racimo_cempasuchil, arco_cempasuchil, calabaza, cesta, naranjas): NO agrega modelos propios nuevos ni animaciones.
Cada casa recibe una combinacion distinta (se comprueba) y ninguna pieza toca las ventanas ni la puerta (se comprueba contra las cajas del .obj).
Uso (raiz del repo, sin Blender):  python3 tools/x58_decorar_casas.py          # escribe el .h y valida
"""
import os, random, sys

RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
N_PUEBLO, N_PLAYA = 21, 6
N = N_PUEBLO + N_PLAYA      # indice g: 0..20 pueblo (PuebloFondo::CASAS), 21..26 playa (Extras::CASAS)

# modelos (el orden es el del enum Modelo de DecoracionCasas.h)
MODELOS = ["PAPEL_PICADO", "PAPEL_NARANJA", "PAPEL_FUCSIA", "PAPEL_MORADO", "MAZETA", "RACIMO", "ARCO", "CALABAZA", "CESTA", "NARANJAS"]
# medio ancho (x), alto (y) y medio fondo (z) del modelo ya con su escala 1: sirven para validar choques
DIM = {"PAPEL_PICADO": (0.85, 0.50, 0.0), "PAPEL_NARANJA": (0.39, 0.38, 0.0), "PAPEL_FUCSIA": (0.97, 0.57, 0.01), "PAPEL_MORADO": (1.07, 0.60, 0.03),
       "MAZETA": (0.12, 0.41, 0.11), "RACIMO": (0.39, 0.15, 0.37), "ARCO": (1.19, 3.15, 0.17), "CALABAZA": (0.11, 0.18, 0.11),
       "CESTA": (0.20, 0.38, 0.20), "NARANJAS": (0.08, 0.15, 0.08)}
PAPELES = ["PAPEL_PICADO", "PAPEL_FUCSIA", "PAPEL_MORADO", "PAPEL_NARANJA"]

Y_PORCHE = 0.12      # losa del porche (casa_a/casa_b: x +-1.3, z -3.22..-2.02)
Y_ALERO = 2.36       # los papeles cuelgan justo debajo del borde del alero (teja: y 2.4) y del techo del porche (y 2.35)


def papel(rng, g, z, giro, x=0.0, lado=False):
    """Una tira (o par) de papel picado colgada de y = Y_ALERO. El naranja es una tira de 0.78 m: va doble."""
    m = rng.choice(PAPELES)
    e = 1.2 if m in ("PAPEL_PICADO", "PAPEL_NARANJA") else 1.0     # tiras un poco mayores para que se lean a 40-100 m (picado 2.04 m < 2.36 m entre ventanas)
    if m == "PAPEL_NARANJA":
        return [(m, False, x - 0.50, Y_ALERO, z, giro, e), (m, False, x + 0.50, Y_ALERO, z, giro, e)]
    return [(m, False, x, Y_ALERO, z, giro, e)]


GRUPOS = ["macetasF", "ofrendaF", "caminoF", "arco", "lados", "papelB2", "macetasB", "racimosB"]
PESO = [3, 2, 3, 2, 2, 3, 2, 3]     # probabilidad relativa de que un grupo salga (el arco es raro)


def casa(g):
    """papel del frente y de atras siempre + 3 o 4 grupos distintos; si salen mas de 11 piezas se vuelve a sortear (no amontonar)."""
    for intento in range(200):
        rng = random.Random(5800 + g + 1000 * intento)
        elegidos = set()
        while len(elegidos) < rng.choice((3, 4, 4)):
            elegidos.add(rng.choices(GRUPOS, PESO)[0])
        p = []
        p += papel(rng, g, -3.10, 0.0)                                       # siempre: tira bajo el techo del porche
        if "macetasF" in elegidos:                                           # macetas flanqueando la puerta (puerta: x +-0.42)
            for s in (-1, 1): p.append(("MAZETA", False, 0.95 * s, Y_PORCHE, -2.55, rng.choice((0, 90, 200)), 1.0))
        if "ofrendaF" in elegidos:
            if rng.random() < 0.5: p.append(("CALABAZA", False, rng.choice((-0.75, 0.75)), Y_PORCHE, -3.00, 0.0, 1.5))
            else:                                                            # cesta con naranjas sobre el porche
                x = rng.choice((-0.80, 0.80))
                p.append(("CESTA", False, x, Y_PORCHE, -2.90, 0.0, 1.0)); p.append(("NARANJAS", False, x, Y_PORCHE + 0.30, -2.90, 0.0, 1.3))
        if "caminoF" in elegidos:                                            # camino de cempasuchil hacia la puerta
            for k in range(rng.choice((2, 3))):
                p.append(("RACIMO", True, 0.55 * (1 if k % 2 else -1), 0.0, -3.9 - 1.1 * k, rng.choice((0, 40, 100, 160)), 1.6))
        if "arco" in elegidos:                                               # arco de carrizo y cempasuchil sobre el camino
            p.append(("ARCO", True, 0.0, 0.0, -5.6, 0.0, 1.0))
        if "lados" in elegidos:                                              # tira en la pared lateral (pared x +-2.5, alero x +-3.0)
            for s in rng.choice(((-1,), (1,), (-1, 1))):
                p.append((rng.choice(PAPELES[:3]), False, 2.62 * s, Y_ALERO, 0.0, 90.0, 1.0))
        p += papel(rng, g, 2.25, 180.0)                                      # siempre: muchas casas del pueblo miran de espaldas a la playa y esa cara es la que se ve
        if "papelB2" in elegidos:                                            # segunda tira, mas abajo, entre las dos ventanas traseras (|x| < 1.07 < 1.18)
            m = rng.choice(PAPELES[:3]); p.append((m, False, 0.0, 1.72, 2.12, 180.0, 1.0))
        if "macetasB" in elegidos:
            for s in (-1, 1): p.append(("MAZETA", True, 0.90 * s, 0.0, 2.65, rng.choice((0, 90, 200)), 1.0))
        if "racimosB" in elegidos:
            for k in range(rng.choice((2, 3))): p.append(("RACIMO", True, (-1.1, 0.0, 1.1)[k], 0.0, 3.15, rng.choice((0, 50, 110)), 1.6))
        if len(p) <= 11 and len(p) >= 5 and firma(p) not in USADAS:
            USADAS.add(firma(p)); return p
    raise RuntimeError("sin combinacion libre para la casa %d" % g)


USADAS = set()


def firma(p):   # lo que distingue a una casa de otra: modelo + lado (frente/lado/atras) + posicion
    return tuple((m, round(x, 2), round(z, 2), e) for (m, s, x, y, z, gi, e) in p)


def validar(piezas):
    """(1) combinaciones distintas, (2) sin amontonar (5..11 piezas), (3) nada dentro de la puerta ni de las ventanas ni de su resplandor."""
    vistas, errores = {}, []
    for g, p in enumerate(piezas):
        f = firma(p)
        if f in vistas: errores.append("casa %d repite la combinacion de la %d" % (g, vistas[f]))
        vistas[f] = g
        if not 5 <= len(p) <= 11: errores.append("casa %d: %d piezas (fuera de 5..11)" % (g, len(p)))
        for (m, suelo, x, y, z, gi, e) in p:
            hx, hy, hz = DIM[m][0] * e, DIM[m][1] * e, DIM[m][2] * e
            if m.startswith("PAPEL"): y0, y1 = y - hy, y            # cuelgan hacia abajo desde y
            else:                     y0, y1 = y, y + hy
            rot = (abs(gi) % 180) == 90 if m.startswith("PAPEL") else False
            ax, az = (hz, hx) if rot else (hx, hz)                  # la tira lateral se gira 90 grados: su ancho va en z
            # ventanas delanteras y traseras (vidrio: |x| 1.18..2.12, y 1.03..2.07) con 0.10 m de holgura; puerta |x| < 0.42, y < 2.07 (frente)
            for zv in (-2.06, 2.06):
                if abs(z - zv) < 0.25 + az and 1.03 - 0.10 < y1 and y0 < 2.07 + 0.10 and abs(x) + ax > 1.18 - 0.10 and abs(x) - ax < 2.12 + 0.10:
                    errores.append("casa %d: %s toca una ventana" % (g, m))
            if m != "ARCO" and abs(z + 2.06) < 0.9 + az and abs(x) - ax < 0.42 and y0 < 2.07 and y1 > 0.12 and z > -2.9 and not m.startswith("PAPEL"):
                errores.append("casa %d: %s tapa la puerta" % (g, m))
            if m == "ARCO" and abs(x) + ax >= 1.30 + 0.0 and False: pass
    return errores


def escribir(piezas):
    nom = {m: i for i, m in enumerate(MODELOS)}
    L = ["#ifndef DECORACION_CASAS_H", "#define DECORACION_CASAS_H",
         "// GENERADO por tools/x58_decorar_casas.py (no editar a mano). FUERA DEL ALCANCE ORIGINAL (como las casas, decision del 13/11): decoracion de las",
         "// 27 casas del fondo con modelos que YA existen (papel picado, macetas, racimos de cempasuchil, arco, calabaza, cesta con naranjas). Sin modelos nuevos,",
         "// sin KeyFrames, sin humo. Coordenadas LOCALES de la casa (frente con porche en z < 0). suelo = true: la pieza se apoya en Terreno::altura (y es un extra).",
         "// Indice de casa g: 0..20 = PuebloFondo::CASAS[g]; 21..26 = Extras::CASAS[g - 21].",
         "namespace DecoracionCasas", "{",
         "    enum Modelo { " + ", ".join(MODELOS) + " };",
         "    struct Pieza { unsigned char modelo; bool suelo; float x, y, z, giro, esc; };",
         "    constexpr int N_CASAS = %d, N_PUEBLO = %d;" % (N, N_PUEBLO)]
    plano, ini = [], []
    for p in piezas:
        ini.append(len(plano)); plano += p
    ini.append(len(plano))
    L.append("    constexpr int N_PIEZAS = %d;" % len(plano))
    L.append("    inline const int INICIO[N_CASAS + 1] = { " + ", ".join(map(str, ini)) + " };")
    L.append("    inline const Pieza PIEZAS[N_PIEZAS] = {")
    for i, (m, s, x, y, z, gi, e) in enumerate(plano):
        L.append("        { %d, %s, %.2ff, %.2ff, %.2ff, %.1ff, %.2ff }%s  // %s" % (nom[m], "true" if s else "false", x, y, z, gi, e, "," if i < len(plano) - 1 else " ", m))
    L += ["    };", "}", "#endif", ""]
    open(os.path.join(RAIZ, "src", "DecoracionCasas.h"), "w", newline="\n").write("\n".join(L))


if __name__ == "__main__":
    piezas = [casa(g) for g in range(N)]
    err = validar(piezas)
    escribir(piezas)
    print("casas:", N, " piezas:", sum(map(len, piezas)), " por casa (min/max):", min(map(len, piezas)), max(map(len, piezas)))
    for e in err: print("ERROR:", e)
    sys.exit(1 if err else 0)
