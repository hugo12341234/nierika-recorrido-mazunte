r"""X-53: alturas del terreno VISIBLE (la malla de Models/colinas/colinas.obj) para los scripts de verificacion y ajuste.
La funcion lisa h(x, z) de x18_colinas.py (alturaColinas) solo se muestrea cada 4 m; entre vertices la malla es plana por triangulos,
y esa diferencia llega a +-0.5 m. Todo lo que se coloca sobre el suelo debe apoyarse en la SUPERFICIE QUE SE VE, no en la formula.
Esta funcion replica exactamente Terreno::altura de src/Terreno.h (misma rejilla, misma diagonal).
Rejilla: x -150..150 y z -170..42 cada 4 m (x18 pide hasta 45, pero 215 m no es multiplo de 4: la ultima fila de vertices cae en z = 42). Cada celda (i, j) es el cuadrilatero (i,j) -> (i,j+1) -> (i+1,j+1) -> (i+1,j) (visto desde arriba,
antihorario); el triangulado es en abanico desde el primer vertice, o sea la diagonal va de (i,j) a (i+1,j+1).
Dentro de la playa (|x| <= 50, z >= -20) el suelo es la playa (A-5); la franja z -22..-20 (X-40) une la malla con la playa (h = 0 en z = -20)."""
import functools, importlib.util, math, os, sys

RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
PLAYA_X, PLAYA_Z0 = 50.0, -20.0


def _cargar_x18():
    guardado = sys.argv
    sys.argv = [guardado[0], "ninguno"]            # x18_colinas.py lee el modo de sys.argv[-1]
    try:
        spec = importlib.util.spec_from_file_location("x18", os.path.join(RAIZ, "tools", "x18_colinas.py"))
        mod = importlib.util.module_from_spec(spec); spec.loader.exec_module(mod)
    finally:
        sys.argv = guardado
    return mod


x18 = _cargar_x18()
h_formula = x18.alturaColinas
X_MIN, X_MAX, Z_MIN, PASO = -150.0, 150.0, -170.0, 4.0
Z_MAX = Z_MIN + int((x18.Z_MAX - Z_MIN) / PASO) * PASO   # 42.0


def playa(z):
    """Perfil de la playa (A-5): y = 0 hasta z = +20 y baja 0.05 m por metro."""
    return 0.0 if z <= 20.0 else -0.05 * (z - 20.0)


def h_visible(x, z):
    """Altura de un VERTICE de la malla: la formula lisa, salvo en el borde de la playa (x = +-50, z > 20): ahi la playa baja 0.05 m por metro
    (hasta -0.5 en z = 30, donde termina playa.obj) y la malla se queda en 0 -> un escalon de hasta 0.5 m junto a la orilla ("suelo levantado").
    Se baja el vertice del borde al nivel de la playa (con tope -0.5) para que el suelo sea continuo."""
    if abs(abs(x) - PLAYA_X) < 1e-9 and z > 20.0:
        return max(playa(z), -0.5)
    return h_formula(x, z)


@functools.lru_cache(maxsize=None)
def vertice(i, j):
    return h_visible(X_MIN + i * PASO, Z_MIN + j * PASO)


def malla(x, z):
    """Altura de la malla de colinas en (x, z); None fuera de la rejilla."""
    if x < X_MIN or x > X_MAX or z < Z_MIN or z > Z_MAX:
        return None
    i = min(int((x - X_MIN) // PASO), int((X_MAX - X_MIN) / PASO) - 1)
    j = min(int((z - Z_MIN) // PASO), int((Z_MAX - Z_MIN) / PASO) - 1)
    fx = (x - X_MIN) / PASO - i
    fz = (z - Z_MIN) / PASO - j
    x0 = X_MIN + i * PASO; z0 = Z_MIN + j * PASO
    y00, y10 = vertice(i, j), vertice(i + 1, j)
    y01, y11 = vertice(i, j + 1), vertice(i + 1, j + 1)
    if x0 >= -PLAYA_X - 1e-9 and x0 + PASO <= PLAYA_X + 1e-9 and z0 < PLAYA_Z0 < z0 + PASO:
        # celda recortada (X-40): llega solo hasta z = -20, donde h = 0 (la playa)
        if z > PLAYA_Z0:
            return None                              # ya es playa
        fz = (z - z0) / (PLAYA_Z0 - z0)
        y01 = y11 = 0.0
    if fz >= fx:                                     # triangulo (i,j) (i,j+1) (i+1,j+1)
        return y00 + fx * (y11 - y01) + fz * (y01 - y00)
    return y00 + fx * (y10 - y00) + fz * (y11 - y10)  # triangulo (i,j) (i+1,j+1) (i+1,j)


def altura(x, z):
    """Altura del suelo visible en (x, z): playa dentro de |x| <= 50 y z >= -20; malla de colinas en el resto; None fuera de todo."""
    if abs(x) <= PLAYA_X and z >= PLAYA_Z0:
        return playa(z)
    return malla(x, z)


def inclinacion(x, z, esc):
    """Pendiente (gx, gz) del suelo para apoyar una roca; MISMA formula que Terreno::inclinacion de src/Terreno.h."""
    w = max(1.0, 0.8 * esc)
    def a(px, pz):                       # fuera de la malla, Terreno::alturaMalla devuelve la formula lisa
        v = altura(px, pz)
        return h_formula(px, pz) if v is None else v
    gx = (a(x + w, z) - a(x - w, z)) / (2 * w)
    gz = (a(x, z + w) - a(x, z - w)) / (2 * w)
    m = math.hypot(gx, gz)
    if m > 0.5:
        gx *= 0.5 / m; gz *= 0.5 / m
    return gx, gz
