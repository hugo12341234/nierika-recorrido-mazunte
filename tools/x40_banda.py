r"""X-40: banda de viento oaxaquena (instrumentos y musicos en pose de tocar). Modelos propios hechos en Blender 5.2 (modulo bpy).
Instrumentos: banda_trompeta, banda_clarinete, banda_saxofon, banda_trombon, banda_tuba, banda_tambora, banda_tarola, banda_platillos.
Musicos (persona de pie con los brazos en la pose de cada instrumento; mismos 3 materiales ropa/piel/pelo que persona_pie_*, asi Atuendos::dibujar
les pone ropa, piel y pelo): musico_viento, musico_trombon, musico_tuba, musico_percusion, musico_platillos.
Los instrumentos estan modelados en las COORDENADAS LOCALES del musico (pivote en el suelo, entre los pies, frente = +Y de Blender = -Z del juego),
y por eso se dibujan con la MISMA matriz que su musico. 1 u = 1 m.
Uso (raiz del repo; sirve el modulo `bpy` de pip o el Blender 5.2 de Windows):
    python tools/x40_banda.py texturas
    python tools/x40_banda.py modelos          (o:  blender.exe -b --python tools/x40_banda.py -- modelos)
    python tools/x40_banda.py vista            (renders de comprobacion en la carpeta que se pase como 3.er argumento, o ./_vista_banda)"""
import math, os, sys, random
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
modo = sys.argv[1] if len(sys.argv) > 1 and sys.argv[1] in ("texturas", "modelos", "vista") else sys.argv[-1]
N = 1024

INSTRUMENTOS = ["banda_trompeta", "banda_clarinete", "banda_saxofon", "banda_trombon", "banda_tuba", "banda_tambora", "banda_tarola", "banda_platillos"]
MUSICOS = ["musico_viento", "musico_trombon", "musico_tuba", "musico_percusion", "musico_platillos"]

# ---- POSES: (mano izquierda, mano derecha) en coordenadas locales del musico (x a la derecha del musico = +x de Blender) ----
# Se usan para modelar los brazos (IK de dos huesos) y para colocar los instrumentos justo en las manos.
POSES = {
    "musico_viento":     ((-0.03, 0.36, 1.43), (0.04, 0.28, 1.30)),    # trompeta/clarinete/saxofon: manos adelante del pecho
    "musico_trombon":    ((-0.07, 0.20, 1.47), (0.10, 0.70, 1.40)),    # izquierda junto a la boca, derecha estirada en la vara
    "musico_tuba":       ((-0.24, 0.30, 1.12), (0.24, 0.30, 1.12)),
    "musico_percusion":  ((-0.17, 0.36, 1.06), (0.20, 0.36, 1.10)),    # palillos sobre el parche
    "musico_platillos":  ((-0.30, 0.28, 1.18), (0.30, 0.28, 1.18)),
}


def texturas():
    import numpy as np
    from PIL import Image
    r = np.random.default_rng(40)
    yy, xx = np.mgrid[0:N, 0:N]

    def guarda(carpeta, nombre, arr):
        d = os.path.join(RAIZ, "Models", carpeta); os.makedirs(d, exist_ok=True)
        Image.fromarray((np.clip(arr, 0, 1) * 255).astype(np.uint8), "RGB").save(os.path.join(d, nombre))

    ruido = lambda f=0.08: (1 - f) + f * r.random((N, N, 1))
    # laton pulido: color base dorado con bandas verticales de reflejo suaves y rayitas finas
    refl = 0.80 + 0.20 * np.sin(xx * 2 * math.pi / 256.0)[..., None] * np.cos(yy * 2 * math.pi / 512.0 + 1.0)[..., None]
    laton = np.array((0.88, 0.64, 0.17))[None, None, :] * refl * ruido(0.08)
    # negro de ebonita / madera de grenadilla: casi negro con veta
    veta = 0.5 + 0.5 * np.sin(xx * 0.05 + 3.0 * np.sin(yy * 0.01))[..., None]
    negro = np.array((0.045, 0.035, 0.03))[None, None, :] * (0.8 + 0.5 * veta) * ruido(0.12)
    plata = np.array((0.74, 0.76, 0.80))[None, None, :] * (0.85 + 0.15 * np.sin(xx * 2 * math.pi / 180.0)[..., None]) * ruido(0.06)
    cuero = np.array((0.30, 0.17, 0.08))[None, None, :] * ruido(0.25)
    parche = np.array((0.90, 0.86, 0.74))[None, None, :] * (0.93 + 0.07 * np.sin(np.hypot(xx - 512, yy - 512) * 0.25)[..., None]) * ruido(0.06)
    # casco de la tambora/tarola: rojo oscuro lacado con dos franjas doradas y una cenefa de rombos blancos
    casco = np.array((0.50, 0.07, 0.08))[None, None, :] * np.ones((N, N, 1)) * ruido(0.10)
    for y0 in (0.12, 0.88):
        m = (np.abs(yy / N - y0) < 0.035)[..., None]
        casco = np.where(m, np.array((0.85, 0.65, 0.20))[None, None, :], casco)
    rombo = (np.abs(((xx % 64) - 32)) + np.abs(((yy % 64) - 32)) < 14) & (np.abs(yy / N - 0.5) < 0.1)
    casco = np.where(rombo[..., None], np.array((0.93, 0.88, 0.75))[None, None, :], casco)
    madera = np.array((0.80, 0.64, 0.42))[None, None, :] * (0.85 + 0.15 * np.sin(yy * 0.2 + 4 * np.sin(xx * 0.01))[..., None]) * ruido(0.10)
    mats = {"laton": laton, "negro": negro, "plata": plata, "cuero": cuero, "parche": parche, "casco": casco, "madera": madera}
    for inst, usa in (("banda_trompeta", ("laton", "plata")), ("banda_clarinete", ("negro", "plata")), ("banda_saxofon", ("laton", "plata", "cuero")),
                      ("banda_trombon", ("laton", "plata")), ("banda_tuba", ("laton", "plata")), ("banda_tambora", ("casco", "parche", "plata", "madera", "cuero")),
                      ("banda_tarola", ("casco", "parche", "plata", "madera", "cuero")), ("banda_platillos", ("laton", "cuero"))):
        for m in usa:
            guarda(inst, "%s_%s.png" % (inst, m), mats[m])
    # musicos: piel y pelo como persona_pie_a; la ropa de uniforme (ver ropa_14 mas abajo)
    for mu in MUSICOS:
        guarda(mu, "ropa.png", np.array((0.93, 0.92, 0.88))[None, None, :] * ruido(0.08))
        guarda(mu, "piel.png", np.array((0.55, 0.38, 0.27))[None, None, :] * ruido(0.12))
        guarda(mu, "pelo.png", np.array((0.06, 0.05, 0.05))[None, None, :] * (0.7 + 0.6 * r.random((N, N, 1))))
    # ropa_14: UNIFORME de banda. La malla de la persona usa UV de caja (0.5 m por repeticion, X-19): una repeticion de 512 px cubre 0.5 m.
    # Camisa blanca de manga larga con puno y botonadura (franja central rojo oscuro) y banda de galon dorado en el pecho.
    base = np.array((0.95, 0.94, 0.90))[None, None, :] * (0.93 + 0.07 * np.sin(xx * 2 * math.pi / 8)[..., None] * np.sin(yy * 2 * math.pi / 8)[..., None]) * ruido(0.05)
    m_rojo = ((yy % 512) > 60) & ((yy % 512) < 120)         # franja roja por cada 0.5 m de alto
    base = np.where(m_rojo[..., None], np.array((0.50, 0.08, 0.10))[None, None, :] * ruido(0.12), base)
    m_oro = (((yy % 512) > 40) & ((yy % 512) < 52)) | (((yy % 512) > 128) & ((yy % 512) < 140))
    base = np.where(m_oro[..., None], np.array((0.85, 0.66, 0.22))[None, None, :], base)
    base = np.where((((xx % 512) > 250) & ((xx % 512) < 262) & ((yy % 512) > 150))[..., None], np.array((0.78, 0.76, 0.70))[None, None, :], base)
    guarda("atuendos", "ropa_14.png", base)
    print("texturas OK")


def modelos(solo_vista=False):
    import bpy, bmesh, shutil
    from mathutils import Vector, Matrix
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from bpy_comun import limpiar, material, exportar

    # ----------------------------- geometria -----------------------------
    def marco(eje):
        a = Vector((1, 0, 0)) if abs(eje.x) < 0.9 else Vector((0, 1, 0))
        u = eje.cross(a).normalized(); v = eje.cross(u).normalized()
        return u, v

    def barrido(bm, pts, radios, mat, seg=12, tapas=True):
        """Tubo que sigue una polilinea con radio variable (anillos con marco transportado en paralelo)."""
        pts = [Vector(p) for p in pts]
        anillos = []
        u = v = None
        for i, p in enumerate(pts):
            t = (pts[min(i + 1, len(pts) - 1)] - pts[max(i - 1, 0)]).normalized()
            if u is None:
                u, v = marco(t)
            else:
                u = (u - t * u.dot(t)).normalized(); v = t.cross(u).normalized()
            anillos.append([bm.verts.new(p + (u * math.cos(2 * math.pi * k / seg) + v * math.sin(2 * math.pi * k / seg)) * radios[i]) for k in range(seg)])
        for i in range(len(anillos) - 1):
            for k in range(seg):
                f = bm.faces.new((anillos[i][k], anillos[i][(k + 1) % seg], anillos[i + 1][(k + 1) % seg], anillos[i + 1][k])); f.material_index = mat
        if tapas:
            for a, inv in ((anillos[0], True), (anillos[-1], False)):
                f = bm.faces.new(a[::-1] if inv else a); f.material_index = mat

    def recta(bm, p0, p1, r0, r1, mat, seg=12):
        barrido(bm, [p0, p1], [r0, r1], mat, seg)

    def arco(c, plano_u, plano_v, rad, a0, a1, n):
        return [Vector(c) + plano_u * (rad * math.cos(a0 + (a1 - a0) * i / n)) + plano_v * (rad * math.sin(a0 + (a1 - a0) * i / n)) for i in range(n + 1)]

    def campana(bm, p0, direccion, largo, r0, r1, mat, seg=20, n=14, curva=3.2):
        """Campana que se abre con perfil exponencial: r(s) = r0 + (r1 - r0) * s^curva."""
        d = Vector(direccion).normalized(); p0 = Vector(p0)
        pts = [p0 + d * (largo * i / n) for i in range(n + 1)]
        rad = [r0 + (r1 - r0) * (i / n) ** curva for i in range(n + 1)]
        barrido(bm, pts, rad, mat, seg, tapas=False)
        # el borde se cierra con un aro de 4 mm hacia dentro (para que no se vea el hueco como una cara abierta)
        barrido(bm, [pts[-1], pts[-1] + d * 0.004], [rad[-1], rad[-1] * 0.97], mat, seg, tapas=False)

    def disco(bm, c, eje, rad, grosor, mat, seg=24, cupula=0.0):
        e = Vector(eje).normalized(); u, v = marco(e); c = Vector(c)
        pts = [c, c + e * (cupula), c + e * (cupula * 0.9) + u * 0.0]
        centro_f = bm.verts.new(c + e * (grosor / 2 + cupula))
        borde_a = [bm.verts.new(c + (u * math.cos(2 * math.pi * k / seg) + v * math.sin(2 * math.pi * k / seg)) * rad + e * (grosor / 2)) for k in range(seg)]
        borde_b = [bm.verts.new(c + (u * math.cos(2 * math.pi * k / seg) + v * math.sin(2 * math.pi * k / seg)) * rad - e * (grosor / 2)) for k in range(seg)]
        centro_b = bm.verts.new(c - e * (grosor / 2))
        for k in range(seg):
            k1 = (k + 1) % seg
            f = bm.faces.new((centro_f, borde_a[k], borde_a[k1])); f.material_index = mat
            f = bm.faces.new((centro_b, borde_b[k1], borde_b[k])); f.material_index = mat
            f = bm.faces.new((borde_a[k], borde_b[k], borde_b[k1], borde_a[k1])); f.material_index = mat

    def esfera(bm, c, r, mat, esc=(1, 1, 1), seg=12):
        res = bmesh.ops.create_uvsphere(bm, u_segments=seg, v_segments=seg // 2, radius=r)
        for v in res["verts"]:
            v.co = Vector((v.co.x * esc[0], v.co.y * esc[1], v.co.z * esc[2])) + Vector(c)
        vs = set(res["verts"])
        for f in bm.faces:
            if all(v in vs for v in f.verts):
                f.material_index = mat

    def caja(bm, c, dim, mat, rot=None):
        res = bmesh.ops.create_cube(bm, size=1.0)
        for v in res["verts"]:
            p = Vector((v.co.x * dim[0], v.co.y * dim[1], v.co.z * dim[2]))
            if rot is not None:
                p = rot @ p
            v.co = p + Vector(c)
        vs = set(res["verts"])
        for f in bm.faces:
            if all(v in vs for v in f.verts):
                f.material_index = mat

    def cilindro_eje(bm, c, eje, rad, largo, mat, seg=24):
        e = Vector(eje).normalized(); c = Vector(c)
        recta(bm, c - e * (largo / 2), c + e * (largo / 2), rad, rad, mat, seg)

    # ------------------------------- persona -------------------------------
    def ik(S, H, pole, L1=0.40, L2=0.34):
        S, H = Vector(S), Vector(H)
        d = (H - S).length
        d = min(d, 0.985 * (L1 + L2))
        dr = (H - S).normalized()
        H = S + dr * d
        a = (L1 * L1 - L2 * L2 + d * d) / (2 * d)
        h = math.sqrt(max(L1 * L1 - a * a, 0.0))
        pv = Vector(pole); pv = (pv - dr * pv.dot(dr)).normalized()
        return S + dr * a + pv * h, H

    def persona(bm, mano_i, mano_d):
        for s in (-1, 1):
            barrido(bm, [(s * 0.09, 0, 0.04), (s * 0.09, 0, 0.88)], [0.065, 0.085], 0, 10)                 # piernas
            caja(bm, (s * 0.09, 0.05, 0.03), (0.10, 0.26, 0.07), 1)                                         # pies
        for s, mano in ((-1, mano_i), (1, mano_d)):
            S = Vector((s * 0.21, 0.0, 1.37))
            codo, mano = ik(S, mano, (s * 0.6, -0.4, -0.8))
            barrido(bm, [S, codo], [0.05, 0.045], 0, 10)                                                    # brazo (manga)
            barrido(bm, [codo, mano], [0.043, 0.035], 0, 10)                                                # antebrazo (manga, el uniforme es de manga larga)
            esfera(bm, mano, 0.042, 1, (1.0, 1.0, 1.0))                                                     # mano
        caja(bm, (0, 0, 1.12), (0.36, 0.20, 0.58), 0)                                                       # torso
        caja(bm, (0, 0, 0.88), (0.32, 0.18, 0.16), 0)                                                       # cadera
        barrido(bm, [(0, 0, 1.40), (0, 0.01, 1.50)], [0.05, 0.045], 1, 10)                                  # cuello
        esfera(bm, (0, 0.01, 1.60), 0.105, 1, (0.92, 1.0, 1.1))                                             # cabeza
        esfera(bm, (0, -0.02, 1.64), 0.108, 2, (0.95, 1.0, 1.02))                                           # pelo

    # ------------------------------ instrumentos ------------------------------
    # materiales: indices segun el orden de cada lista (ver MAT_*)
    def trompeta(bm):          # 0 laton, 1 plata
        boca = Vector((0.0, 0.115, 1.545))
        recta(bm, boca, boca + Vector((0, 0.05, 0.0)), 0.007, 0.011, 1, 12)                                # boquilla plateada
        # tubo de embocadura recto hasta la campana; eje 6 grados hacia abajo
        d = Vector((0, 1.0, -0.10)).normalized()
        p = boca + Vector((0, 0.05, 0)); tub = p + d * 0.22
        barrido(bm, [p, tub], [0.010, 0.0115], 0, 12)
        # lazo grande (tubo de retorno): dos tramos paralelos 7 cm mas abajo, unidos por una U de 7 cm de diametro
        u = Vector((0, 0, -1.0)); a0 = tub + u * 0.0
        for dz in (0.0,):
            pass
        cierre = tub + d * 0.14
        sup = [tub, tub + d * 0.14]
        U = arco(sup[1] + u * 0.035, d, u, 0.035, math.pi / 2, -math.pi / 2, 10)
        barrido(bm, sup + U[1:-1] + [sup[1] + u * 0.07 - d * 0.0], [0.0105] * (len(sup) + len(U) - 2 + 1), 0, 10, tapas=False)
        inf = [sup[1] + u * 0.07, sup[1] + u * 0.07 - d * 0.40]
        barrido(bm, inf, [0.0105, 0.0105], 0, 10, tapas=False)
        # segundo lazo hacia la campana
        base = inf[1]
        U2 = arco(base + u * (-0.035), -d, u, 0.035, -math.pi / 2, -3 * math.pi / 2, 10)
        barrido(bm, [base] + U2[1:], [0.0105] * len(U2), 0, 10, tapas=False)
        final = U2[-1]
        # tubo hacia la campana por arriba del lazo
        t2 = [final, final + d * 0.50]
        barrido(bm, t2, [0.0105, 0.012], 0, 10, tapas=False)
        # campana
        campana(bm, t2[1], d, 0.20, 0.014, 0.075, 0, 20)
        # 3 valvulas: cilindros verticales sobre el tubo con boton de plata
        for i, dy in enumerate((0.00, 0.045, 0.09)):
            c = tub + d * (0.20 + dy) + Vector((0, 0, -0.01))
            cilindro_eje(bm, c + Vector((0, 0, 0.0)), (0, 0, 1), 0.0105, 0.075, 1, 10)
            cilindro_eje(bm, c + Vector((0, 0, 0.05)), (0, 0, 1), 0.0125, 0.012, 1, 10)
        # bombas de afinacion (3 tubitos)
        for i, off in enumerate((-0.012, 0.0, 0.012)):
            pass

    def clarinete(bm):         # 0 negro, 1 plata
        boca = Vector((0.0, 0.112, 1.535))
        d = Vector((0, math.sin(math.radians(36)), -math.cos(math.radians(36)))).normalized()
        recta(bm, boca, boca + d * 0.05, 0.007, 0.013, 1, 12)                                              # boquilla
        pts = [boca + d * 0.05, boca + d * 0.12, boca + d * 0.55]
        barrido(bm, pts, [0.013, 0.0145, 0.0165], 0, 14)                                                   # cuerpo (barrilete, secciones)
        fin = boca + d * 0.55
        campana(bm, fin, d, 0.11, 0.0165, 0.040, 0, 18, 10, 2.2)                                           # campana
        for k in range(8):                                                                                 # anillos y llaves
            c = boca + d * (0.15 + 0.045 * k)
            recta(bm, c - d * 0.004, c + d * 0.004, 0.0185, 0.0185, 1, 12)
            esfera(bm, c + Vector((0, 0.018, 0.012)), 0.0085, 1, (1, 1, 0.5), 8)
        for k in (0, 1):
            recta(bm, boca + d * (0.52 + 0.0 * k), boca + d * 0.53, 0.0175, 0.0175, 1, 12)

    def saxofon(bm):           # 0 laton, 1 plata, 2 cuero
        boca = Vector((0.0, 0.113, 1.54))
        recta(bm, boca, boca + Vector((0, 0.06, -0.01)), 0.008, 0.012, 2, 10)                              # boquilla (cuero/ebonita)
        cuello = [boca + Vector((0, 0.06, -0.01)), Vector((0, 0.15, 1.55)), Vector((0.02, 0.22, 1.52)), Vector((0.04, 0.26, 1.44))]
        barrido(bm, cuello, [0.012, 0.014, 0.0165, 0.019], 0, 12, tapas=False)                             # cuello curvo
        cuerpo = [Vector((0.04, 0.26, 1.44)), Vector((0.05, 0.27, 1.05)), Vector((0.05, 0.28, 0.62))]
        barrido(bm, cuerpo, [0.019, 0.030, 0.050], 0, 16, tapas=False)                                      # cuerpo conico
        curva = arco(Vector((0.05, 0.34, 0.62)), Vector((0, -1, 0)), Vector((0, 0, 1)), 0.06, math.pi, 1.5 * math.pi, 8)   # codo inferior (U)
        barrido(bm, [Vector((0.05, 0.28, 0.62))] + [c for c in curva[1:]] + [Vector((0.05, 0.34, 0.56))], [0.050] * (len(curva) + 1), 0, 16, tapas=False)
        campana(bm, Vector((0.05, 0.40, 0.58)), (0, 0.15, 1.0), 0.16, 0.050, 0.115, 0, 20, 12, 2.6)       # campana hacia arriba
        for k in range(9):                                                                                 # llaves y almohadillas
            z = 1.36 - 0.075 * k
            esfera(bm, (0.05, 0.27 + 0.044 * (1 - k / 12.0), z), 0.011, 1, (1, 1, 0.6), 8)
        for z in (1.40, 1.16, 0.92):
            recta(bm, Vector((0.05, 0.295, z)), Vector((0.05, 0.31, z)), 0.007, 0.007, 1, 8)
        recta(bm, Vector((0.0, 0.113, 1.56)), Vector((0.0, 0.158, 1.58)), 0.0145, 0.0145, 2, 10)           # abrazadera de la caña

    def trombon(bm):           # 0 laton, 1 cuero
        boca = Vector((-0.07, 0.205, 1.47)) + Vector((0.07, -0.09, 0.0))        # (0, 0.115, 1.47)
        boca = Vector((0.0, 0.115, 1.48))
        y_far = 0.78
        # vara: dos tubos paralelos a x = +-0.03, del boquilla hasta el codo en U lejano
        for s in (-1, 1):
            recta(bm, Vector((s * 0.03, 0.17, 1.46)), Vector((s * 0.03, y_far, 1.44)), 0.0105, 0.0105, 0, 10)
            recta(bm, Vector((s * 0.03, 0.17, 1.46)), Vector((s * 0.03, y_far, 1.44)), 0.013, 0.013, 0, 10) if False else None
        U = arco(Vector((0, y_far, 1.44)), Vector((1, 0, 0)), Vector((0, 1, 0)), 0.03, math.pi, 0.0, 10)
        barrido(bm, [Vector((-0.03, y_far, 1.44))] + [Vector(p) for p in U[1:-1]] + [Vector((0.03, y_far, 1.44))], [0.0105] * (len(U)), 0, 10, tapas=False)
        recta(bm, boca, Vector((0.0, 0.17, 1.47)), 0.007, 0.011, 1, 10)                                  # boquilla
        recta(bm, Vector((-0.03, 0.17, 1.46)), Vector((0.03, 0.17, 1.46)), 0.011, 0.011, 0, 10)
        # traviesa (donde agarra la mano derecha) a y = 0.70
        recta(bm, Vector((-0.04, 0.70, 1.455)), Vector((0.04, 0.70, 1.455)), 0.008, 0.008, 1, 8)
        # tubo de la campana: de la vara por la izquierda hacia atras, sobre el hombro, y campana al frente
        cb = [Vector((-0.03, 0.30, 1.455)), Vector((-0.12, 0.30, 1.48)), Vector((-0.20, 0.24, 1.50)), Vector((-0.22, 0.10, 1.52)), Vector((-0.22, 0.0, 1.53))]
        barrido(bm, cb, [0.0115, 0.0115, 0.012, 0.0125, 0.0135], 0, 10, tapas=False)
        vuelta = arco(Vector((-0.22, 0.0, 1.53 - 0.0)), Vector((0, -1, 0)), Vector((0, 0, 0)), 0.0, 0, 0, 1)
        # codo hacia el frente por detras del hombro (la campana mira a +Y)
        codo = [Vector((-0.22, 0.0, 1.53)), Vector((-0.24, -0.08, 1.53)), Vector((-0.28, -0.08, 1.50)), Vector((-0.30, 0.00, 1.47))]
        barrido(bm, codo, [0.0135, 0.0135, 0.0135, 0.0135], 0, 10, tapas=False)
        campana(bm, Vector((-0.30, 0.0, 1.47)), (0.0, 1.0, 0.0), 0.34, 0.0135, 0.115, 0, 22, 14, 3.0)

    def tuba(bm):              # 0 laton, 1 plata
        # tubo de embocadura sube desde el cuerpo hasta la boca
        boca = Vector((0.0, 0.118, 1.52))
        recta(bm, boca, boca + Vector((0, 0.04, 0.0)), 0.009, 0.014, 1, 10)
        barrido(bm, [boca + Vector((0, 0.04, 0)), Vector((0.0, 0.20, 1.50)), Vector((0.0, 0.30, 1.30)), Vector((0.02, 0.34, 1.10))], [0.014, 0.016, 0.02, 0.026], 0, 12, tapas=False)
        # espiras del cuerpo: dos lazos (circulos verticales de 0.50 y 0.42 m)
        cx, cy, cz = 0.0, 0.40, 0.88
        for rad, dy in ((0.27, -0.05), (0.21, 0.0), (0.15, 0.05)):   # lazos en el plano XZ (de frente se ven como aros), apilados en Y
            ang = [(i / 26.0) * 2 * math.pi for i in range(27)]
            pts = [Vector((cx + rad * math.cos(a) * 0.9, cy + dy, cz + rad * math.sin(a))) for a in ang]
            barrido(bm, pts, [0.024] * len(pts), 0, 10, tapas=False)
        # campana grande que sube por el lado derecho del musico por encima de la cabeza
        campana(bm, Vector((0.17, 0.36, 1.00)), (0.0, 0.02, 1.0), 0.72, 0.040, 0.24, 0, 28, 18, 3.4)
        # pistones (3) al frente
        for i, dx in enumerate((-0.05, 0.0, 0.05)):
            cilindro_eje(bm, Vector((dx, 0.43, 1.24)), (0, 0, 1), 0.014, 0.12, 1, 10)
            cilindro_eje(bm, Vector((dx, 0.43, 1.31)), (0, 0, 1), 0.019, 0.012, 1, 10)

    def tambora(bm):           # 0 casco, 1 parche, 2 plata (aros y tensores), 3 madera (baqueta), 4 cuero (correa)
        c = Vector((0.0, 0.36, 0.96))           # centro del bombo; el eje es X (los parches miran a los lados)
        rad, prof = 0.31, 0.30
        e = Vector((1, 0, 0))
        cilindro_eje(bm, c, (1, 0, 0), rad, prof, 0, 28)                                                   # casco
        for s in (-1, 1):
            disco(bm, c + e * (s * (prof / 2 + 0.004)), (s, 0, 0), rad + 0.006, 0.008, 1, 28, 0.0)         # parche
            # aro metalico: anillo delgado en el borde
            ring = [c + e * (s * (prof / 2 + 0.010)) + Vector((0, math.cos(2 * math.pi * k / 28), math.sin(2 * math.pi * k / 28))) * (rad + 0.012) for k in range(29)]
            barrido(bm, ring, [0.009] * len(ring), 2, 8, tapas=False)
        for k in range(10):                                                                                # tensores
            a = 2 * math.pi * k / 10.0
            p = c + Vector((0, math.cos(a), math.sin(a))) * (rad + 0.012)
            recta(bm, p + e * (prof / 2 + 0.005), p - e * (prof / 2 + 0.005), 0.005, 0.005, 2, 6)
        # correa que pasa por el cuello
        barrido(bm, [Vector((0.15, 0.20, 0.98)), Vector((0.13, 0.10, 1.35)), Vector((0.0, 0.05, 1.47)), Vector((-0.13, 0.10, 1.35)), Vector((-0.15, 0.20, 0.98))],
                [0.012] * 5, 4, 6, tapas=False)
        # mazo con cabeza de fieltro en la mano derecha (0.20, 0.36, 1.10) y palillo en la izquierda
        recta(bm, Vector((0.20, 0.40, 1.08)), Vector((0.24, 0.30, 1.38)), 0.008, 0.008, 3, 8)
        esfera(bm, (0.2455, 0.2865, 1.43), 0.036, 3, (1, 1, 1), 10)
        recta(bm, Vector((-0.17, 0.40, 1.04)), Vector((-0.20, 0.36, 1.26)), 0.006, 0.006, 3, 8)

    def tarola(bm):            # 0 casco, 1 parche, 2 plata, 3 madera, 4 cuero
        c = Vector((0.0, 0.36, 0.98)); rad, prof = 0.17, 0.14
        n = Vector((0.0, -0.10, 1.0)).normalized()                       # parche hacia arriba, un poco inclinado hacia el musico
        cilindro_eje(bm, c, n, rad, prof, 0, 24)
        disco(bm, c + n * (prof / 2 + 0.003), n, rad + 0.004, 0.006, 1, 24)
        disco(bm, c - n * (prof / 2 + 0.003), -n, rad + 0.004, 0.006, 1, 24)
        u, v = marco(n)
        for sgn, off in ((1, prof / 2 + 0.008), (-1, -(prof / 2 + 0.008))):
            ring = [c + n * off + (u * math.cos(2 * math.pi * k / 24) + v * math.sin(2 * math.pi * k / 24)) * (rad + 0.008) for k in range(25)]
            barrido(bm, ring, [0.006] * len(ring), 2, 8, tapas=False)
        for k in range(8):
            a = 2 * math.pi * k / 8
            p = c + (u * math.cos(a) + v * math.sin(a)) * (rad + 0.008)
            recta(bm, p + n * (prof / 2 + 0.005), p - n * (prof / 2 + 0.005), 0.004, 0.004, 2, 6)
        # correa del cuello al lado izquierdo
        barrido(bm, [Vector((-0.12, 0.30, 0.95)), Vector((-0.12, 0.08, 1.30)), Vector((0.0, 0.05, 1.47)), Vector((0.12, 0.08, 1.30)), Vector((0.12, 0.30, 0.95))], [0.011] * 5, 4, 6, tapas=False)
        # palillos
        recta(bm, Vector((-0.17, 0.38, 1.07)), Vector((-0.12, 0.30, 1.03)), 0.005, 0.006, 3, 6)
        recta(bm, Vector((0.20, 0.38, 1.12)), Vector((0.12, 0.30, 1.05)), 0.005, 0.006, 3, 6)

    def platillos(bm):         # 0 laton, 1 cuero
        for s in (-1, 1):
            c = Vector((s * 0.27, 0.28, 1.18))
            disco(bm, c, (-s, 0.0, 0.0), 0.19, 0.004, 0, 36)
            esfera(bm, c + Vector((s * 0.0, 0, 0)), 0.04, 0, (0.4, 1.0, 1.0), 12)                          # cupula central
            # correas de cuero hacia la mano
            recta(bm, c + Vector((s * 0.03, 0.0, 0.0)), c + Vector((s * 0.05, -0.0, -0.02)), 0.005, 0.005, 1, 6)

    # ----------------------------- armado y exporte -----------------------------
    MAT_INST = {
        "banda_trompeta": ("laton", "plata"), "banda_clarinete": ("negro", "plata"), "banda_saxofon": ("laton", "plata", "cuero"),
        "banda_trombon": ("laton", "plata"), "banda_tuba": ("laton", "plata"), "banda_tambora": ("casco", "parche", "plata", "madera", "cuero"),
        "banda_tarola": ("casco", "parche", "plata", "madera", "cuero"), "banda_platillos": ("laton", "cuero"),
    }
    # trombon: 0 laton, 1 plata
    CONSTR = {"banda_trompeta": trompeta, "banda_clarinete": clarinete, "banda_saxofon": saxofon, "banda_trombon": trombon, "banda_tuba": tuba,
              "banda_tambora": tambora, "banda_tarola": tarola, "banda_platillos": platillos}

    def uv_caja(ob, tam=0.5):
        bpy.context.view_layer.objects.active = ob; ob.select_set(True)
        bpy.ops.object.mode_set(mode="EDIT"); bpy.ops.mesh.select_all(action="SELECT")
        bpy.ops.uv.cube_project(cube_size=tam); bpy.ops.object.mode_set(mode="OBJECT")

    def uv_cilindrica(ob, eje):
        # u = angulo alrededor del eje, v = posicion a lo largo del eje (para que las franjas del casco sigan el aro)
        me = ob.data
        uvl = me.uv_layers.new(name="UVMap") if not me.uv_layers else me.uv_layers[0]
        cn = [(sum((me.vertices[i].co for i in p.vertices), Vector()) / len(p.vertices)) for p in me.polygons]
        for p in me.polygons:
            for li in p.loop_indices:
                co = me.vertices[me.loops[li].vertex_index].co
                if eje == "x":
                    uvl.data[li].uv = (math.atan2(co.z - 0.96, co.y - 0.36) / (2 * math.pi) + 0.5, (co.x + 0.15) / 0.30)
                else:
                    uvl.data[li].uv = (co.x * 4, co.z * 4)

    def construir_instrumento(nombre):
        limpiar()
        carpeta = os.path.join(RAIZ, "Models", nombre)
        bm = bmesh.new()
        CONSTR[nombre](bm)
        me = bpy.data.meshes.new(nombre)
        bm.to_mesh(me); bm.free()
        ob = bpy.data.objects.new(nombre, me); bpy.context.collection.objects.link(ob)
        for m in MAT_INST[nombre]:
            me.materials.append(material(nombre + "_" + m, os.path.join(carpeta, "%s_%s.png" % (nombre, m))))
        for p in me.polygons:
            p.use_smooth = True
        uv_caja(ob)
        if nombre == "banda_tambora":
            # casco con UV cilindrica (franjas doradas alrededor): solo las caras del casco (material 0)
            uvl = me.uv_layers[0]
            for p in me.polygons:
                if p.material_index == 0:
                    for li in p.loop_indices:
                        co = me.vertices[me.loops[li].vertex_index].co
                        uvl.data[li].uv = (math.atan2(co.z - 0.96, co.y - 0.36) / (2 * math.pi) + 0.5, (co.x + 0.15) / 0.30)
        if nombre == "banda_tarola":
            pass
        exportar(ob, nombre)
        print(nombre, len(me.polygons), "caras")
        return ob

    def construir_musico(nombre):
        limpiar()
        carpeta = os.path.join(RAIZ, "Models", nombre)
        mi, md = POSES[nombre]
        bm = bmesh.new()
        persona(bm, mi, md)
        me = bpy.data.meshes.new(nombre)
        bm.to_mesh(me); bm.free()
        ob = bpy.data.objects.new(nombre, me); bpy.context.collection.objects.link(ob)
        for nm in ("ropa", "piel", "pelo"):
            me.materials.append(material(nombre + "_" + nm, os.path.join(carpeta, nm + ".png")))
        for p in me.polygons:
            p.use_smooth = True
        uv_caja(ob)
        exportar(ob, nombre)
        print(nombre, len(me.polygons), "caras")
        return ob

    if solo_vista:
        return
    for n in INSTRUMENTOS:
        construir_instrumento(n)
    for n in MUSICOS:
        construir_musico(n)


def vista(salida):
    """Renders de comprobacion (Workbench con textura): cada musico con su instrumento, de frente y de lado, y la banda completa."""
    import bpy
    from mathutils import Vector
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    os.makedirs(salida, exist_ok=True)
    pares = [("musico_viento", "banda_trompeta"), ("musico_viento", "banda_clarinete"), ("musico_viento", "banda_saxofon"), ("musico_trombon", "banda_trombon"),
             ("musico_tuba", "banda_tuba"), ("musico_percusion", "banda_tambora"), ("musico_percusion", "banda_tarola"), ("musico_platillos", "banda_platillos")]

    def importar(nombre, pos, giro=0.0):
        d = os.path.join(RAIZ, "Models", nombre)
        bpy.ops.wm.obj_import(filepath=os.path.join(d, nombre + ".obj"), forward_axis="NEGATIVE_Z", up_axis="Y")
        ob = bpy.context.selected_objects[0]
        ob.location = pos; ob.rotation_euler[2] = giro
        return ob

    def escena_base():
        bpy.ops.wm.read_factory_settings(use_empty=True)
        sc = bpy.context.scene
        sc.render.engine = "CYCLES"; sc.cycles.device = "CPU"; sc.cycles.samples = 24   # Cycles en CPU: no necesita GPU ni ventana
        sc.cycles.use_denoising = False
        sol = bpy.data.objects.new("sol", bpy.data.lights.new("sol", "SUN")); sc.collection.objects.link(sol)
        sol.data.energy = 7.0; sol.rotation_euler = (math.radians(50), 0, math.radians(35))
        sc.render.resolution_x, sc.render.resolution_y = 700, 700
        sc.view_settings.view_transform = "Standard"
        sc.world = bpy.data.worlds.new("w"); sc.world.color = (0.8, 0.82, 0.88)
        cam = bpy.data.objects.new("cam", bpy.data.cameras.new("cam")); sc.collection.objects.link(cam); sc.camera = cam
        return sc, cam

    def foto(sc, cam, centro, dist, az, el, ruta, lente=50):
        a, e = math.radians(az), math.radians(el)
        cam.location = Vector(centro) + Vector((dist * math.cos(e) * math.sin(a), -dist * math.cos(e) * math.cos(a), dist * math.sin(e)))
        d = Vector(centro) - cam.location
        cam.rotation_euler = d.to_track_quat("-Z", "Y").to_euler()
        cam.data.lens = lente
        sc.render.filepath = ruta
        bpy.ops.render.render(write_still=True)

    for i, (m, ins) in enumerate(pares):
        sc, cam = escena_base()
        importar(m, (0, 0, 0)); importar(ins, (0, 0, 0))
        foto(sc, cam, (0, 0.25, 1.05), 2.5, 30, 8, os.path.join(salida, "%d_%s_3q.png" % (i, ins)))
        foto(sc, cam, (0, 0.25, 1.05), 2.5, 90, 5, os.path.join(salida, "%d_%s_lado.png" % (i, ins)))
        # (la cara vista desde el frente: azimut 0 mira a +Y desde -Y; el musico mira a +Y, asi que -Y es su espalda; 180 = de frente)
        foto(sc, cam, (0, 0.25, 1.05), 2.5, 180, 6, os.path.join(salida, "%d_%s_frente.png" % (i, ins)))
    print("vista OK")


if modo == "texturas":
    texturas()
elif modo == "modelos":
    modelos()
elif modo == "vista":
    vista(sys.argv[2] if len(sys.argv) > 2 and sys.argv[1] == "vista" else os.path.join(RAIZ, "_vista_banda"))
