r"""X-55: personas ARTICULADAS, perro y pelota para las 5 animaciones nuevas del 17/11 (modelos propios hechos en Blender con el modulo bpy).
Modelos (todos propios; cada uno es una carpeta con varias piezas .obj que comparten textura):
  Models/persona_art/   torso (cadera, pecho, cuello, cabeza y pelo), brazo, antebrazo, muslo, pierna  (+ ropa.png, piel.png, pelo.png)
  Models/perro/         cuerpo, cabeza, cola, pata   (+ pelaje.png, oscuro.png)
  Models/pelota/        pelota                        (+ pelota.png)
Cada pieza tiene el PIVOTE EN SU ARTICULACION (hombro, codo, cadera, rodilla, cuello, base de la cola): asi PersonasAnim.h solo compone
mundo(hijo) = mundo(padre) * T(offset) * R(rot), igual que la silueta. Las piezas son SIMETRICAS respecto de su eje: la misma pieza sirve para el lado
izquierdo y el derecho (solo cambia el offset). Medidas del adulto (iguales a Silueta.cpp): cadera 0.90 m, torso 0.60, hombro a 0.54 sobre la cadera,
brazo 0.30, antebrazo 0.26 + mano, muslo 0.45, pierna 0.45 (con pie) -> 1.72 m. Los materiales se llaman *_ropa, *_piel y *_pelo (Atuendos los viste).
Uso (raiz del repo):
    python3 tools/x55_personas_articuladas.py texturas
    python3 tools/x55_personas_articuladas.py modelos
    python3 tools/x55_personas_articuladas.py vista [carpeta]      (renders de comprobacion con Cycles)
Blender: X = juego X, Y = juego -Z (el frente mira a +Y), Z = juego Y (arriba); se exporta con Forward -Z / Up Y como el resto del repo."""
import math, os, shutil, sys
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
modo = sys.argv[1] if len(sys.argv) > 1 and sys.argv[1] in ("texturas", "modelos", "vista") else sys.argv[-1]
N = 1024


def texturas():
    import numpy as np
    from PIL import Image
    r = np.random.default_rng(55)
    # persona_art: las mismas telas de persona_pie_a (Atuendos::dibujar las sustituye al dibujar; el .mtl necesita alguna)
    d = os.path.join(RAIZ, "Models", "persona_art"); os.makedirs(d, exist_ok=True)
    for f in ("ropa.png", "piel.png", "pelo.png"):
        shutil.copyfile(os.path.join(RAIZ, "Models", "persona_pie_a", f), os.path.join(d, f))
    # perro criollo: pelaje beige con manchas cafe y suave ruido; "oscuro" para nariz, orejas y ojos
    yy, xx = np.mgrid[0:N, 0:N]
    manchas = 0.5 + 0.5 * np.sin(xx * 2 * math.pi / 210 + 2 * np.sin(yy * 2 * math.pi / 330)) * np.sin(yy * 2 * math.pi / 260 + 1.3 * np.sin(xx * 2 * math.pi / 400))
    base = np.array((0.78, 0.60, 0.38))[None, None, :]
    cafe = np.array((0.45, 0.28, 0.14))[None, None, :]
    img = base * (1 - 0.55 * (manchas[..., None] > 0.62)) + cafe * 0.55 * (manchas[..., None] > 0.62)
    img = np.clip(img * (0.88 + 0.12 * r.random((N, N, 1))), 0, 1)
    d = os.path.join(RAIZ, "Models", "perro"); os.makedirs(d, exist_ok=True)
    Image.fromarray((img * 255).astype(np.uint8), "RGB").save(os.path.join(d, "pelaje.png"))
    osc = np.array((0.08, 0.06, 0.05))[None, None, :] * (0.8 + 0.4 * r.random((N, N, 1)))
    Image.fromarray((np.clip(osc, 0, 1) * 255).astype(np.uint8), "RGB").save(os.path.join(d, "oscuro.png"))
    # pelota: gajos rojo y crema (de goma, de las que se venden en la playa)
    gajos = ((xx // 128 + yy // 512) % 2).astype(float)[..., None]
    pel = np.array((0.80, 0.12, 0.10))[None, None, :] * gajos + np.array((0.95, 0.88, 0.70))[None, None, :] * (1 - gajos)
    d = os.path.join(RAIZ, "Models", "pelota"); os.makedirs(d, exist_ok=True)
    Image.fromarray((np.clip(pel * (0.92 + 0.08 * r.random((N, N, 1))), 0, 1) * 255).astype(np.uint8), "RGB").save(os.path.join(d, "pelota.png"))
    print("texturas OK")


def modelos():
    import bpy
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from bpy_comun import limpiar, material

    def prim_cono(r_arriba, r_abajo, z_ini, z_fin, mat, desde=(0, 0), hasta=None, seg=14):
        """Cilindro/cono entre (x, y, z_ini) y (x, y, z_fin); si 'hasta' no es None, el extremo de abajo se desplaza a (hasta[0], hasta[1])."""
        import mathutils
        p0 = mathutils.Vector((desde[0], desde[1], z_ini))
        p1 = mathutils.Vector((desde[0] if hasta is None else hasta[0], desde[1] if hasta is None else hasta[1], z_fin))
        eje = p1 - p0
        bpy.ops.mesh.primitive_cone_add(vertices=seg, radius1=r_abajo, radius2=r_arriba, depth=eje.length, location=(p0 + p1) / 2)
        o = bpy.context.active_object
        o.rotation_mode = "QUATERNION"
        # el cono nace con la base grande (radius1) abajo y la chica arriba (+Z local): se gira para que +Z apunte de p1 hacia p0
        o.rotation_quaternion = mathutils.Vector((0, 0, 1)).rotation_difference(p0 - p1)
        o.data.materials.append(mat)
        return o

    def prim_esfera(r, centro, mat, escala=(1, 1, 1), seg=16):
        bpy.ops.mesh.primitive_uv_sphere_add(segments=seg, ring_count=max(8, seg // 2), radius=r, location=centro)
        o = bpy.context.active_object
        o.scale = escala
        o.data.materials.append(mat)
        return o

    def prim_caja(dim, centro, mat, bisel=0.0, giro_x=0.0):
        bpy.ops.mesh.primitive_cube_add(size=1, location=centro)
        o = bpy.context.active_object
        o.scale = dim
        o.rotation_euler = (math.radians(giro_x), 0, 0)
        o.data.materials.append(mat)
        if bisel > 0:
            bpy.ops.object.transform_apply(scale=True, rotation=True)
            m = o.modifiers.new("bisel", "BEVEL"); m.width = bisel; m.segments = 3
        return o

    def pieza(carpeta, nombre, partes, mats):
        """Une las partes, aplica modificadores, UV por caja y exporta Models/<carpeta>/<nombre>.obj (pivote en el origen)."""
        bpy.ops.object.select_all(action="DESELECT")
        for p in partes:
            p.select_set(True)
        bpy.context.view_layer.objects.active = partes[0]
        bpy.ops.object.transform_apply(location=False, rotation=True, scale=True)
        bpy.ops.object.join()
        o = bpy.context.active_object
        o.name = nombre
        for m in list(o.modifiers):
            bpy.ops.object.modifier_apply(modifier=m.name)
        bpy.ops.object.shade_smooth()
        bpy.ops.object.mode_set(mode="EDIT"); bpy.ops.mesh.select_all(action="SELECT")
        bpy.ops.uv.cube_project(cube_size=0.5); bpy.ops.object.mode_set(mode="OBJECT")
        bpy.ops.object.select_all(action="DESELECT"); o.select_set(True)
        bpy.context.view_layer.objects.active = o
        d = os.path.join(RAIZ, "Models", carpeta); os.makedirs(d, exist_ok=True)
        bpy.ops.wm.obj_export(filepath=os.path.join(d, nombre + ".obj"), export_selected_objects=True, forward_axis="NEGATIVE_Z", up_axis="Y",
                              apply_modifiers=True, export_materials=True, path_mode="RELATIVE")
        print(carpeta, nombre, len(o.data.polygons), "caras")
        return o

    # ============ persona_art ============
    limpiar()
    d = os.path.join(RAIZ, "Models", "persona_art")
    M = [material("persona_art_ropa", os.path.join(d, "ropa.png")), material("persona_art_piel", os.path.join(d, "piel.png")),
         material("persona_art_pelo", os.path.join(d, "pelo.png"))]
    ropa, piel, pelo = M
    # torso: pivote en el centro de la cadera. Pecho 0.22..0.58, cintura/cadera 0..0.26, cuello, cabeza (centro a 0.71 m) y pelo
    pieza("persona_art", "torso", [
        prim_caja((0.36, 0.20, 0.36), (0, 0, 0.40), ropa, 0.05), prim_caja((0.30, 0.18, 0.26), (0, 0, 0.13), ropa, 0.04),
        prim_cono(0.045, 0.05, 0.56, 0.62, piel, (0, 0.01)),
        prim_esfera(0.105, (0, 0.01, 0.72), piel, (0.92, 1.0, 1.1), 24), prim_esfera(0.108, (0, -0.02, 0.76), pelo, (0.95, 1.0, 1.02), 24)], M)
    pieza("persona_art", "brazo", [prim_cono(0.050, 0.040, 0.0, -0.30, ropa), prim_esfera(0.055, (0, 0, 0), ropa)], M)
    pieza("persona_art", "antebrazo", [prim_cono(0.038, 0.030, 0.0, -0.25, piel), prim_esfera(0.04, (0, 0, -0.27), piel, (0.8, 1.0, 1.2))], M)
    pieza("persona_art", "muslo", [prim_cono(0.082, 0.060, 0.0, -0.45, ropa), prim_esfera(0.082, (0, 0, 0), ropa)], M)
    pieza("persona_art", "pierna", [prim_cono(0.060, 0.042, 0.0, -0.40, ropa), prim_esfera(0.060, (0, 0, 0), ropa),
                                    prim_caja((0.09, 0.25, 0.06), (0, 0.05, -0.42), piel, 0.015)], M)
    bpy.ops.wm.save_as_mainfile(filepath=os.path.join(d, "persona_art.blend"), copy=True)

    # ============ perro ============
    limpiar()
    d = os.path.join(RAIZ, "Models", "perro")
    pel_m, osc_m = material("perro_pelaje", os.path.join(d, "pelaje.png")), material("perro_oscuro", os.path.join(d, "oscuro.png"))
    # cuerpo: pivote en el centro del tronco (a 0.45 m del suelo). Tronco elipsoidal 0.76 m de largo, pecho mas alto adelante, cuello hasta (0, 0.34, 0.12)
    pieza("perro", "cuerpo", [prim_esfera(0.5, (0, 0, 0), pel_m, (0.26, 0.76, 0.30), 24), prim_esfera(0.5, (0, 0.20, 0.02), pel_m, (0.28, 0.40, 0.34), 20),
                              prim_cono(0.07, 0.10, 0.12, 0.05, pel_m, (0, 0.34), (0, 0.22))], [pel_m, osc_m])
    # cabeza: pivote en la base del cuello. Craneo, hocico, nariz, dos orejas caidas y ojos
    pieza("perro", "cabeza", [prim_esfera(0.095, (0, 0.08, 0.08), pel_m, (1.0, 1.1, 1.0), 20), prim_caja((0.10, 0.17, 0.08), (0, 0.22, 0.06), pel_m, 0.02),
                              prim_esfera(0.03, (0, 0.31, 0.075), osc_m),
                              prim_caja((0.03, 0.07, 0.12), (-0.098, 0.07, 0.035), osc_m, 0.01), prim_caja((0.03, 0.07, 0.12), (0.098, 0.07, 0.035), osc_m, 0.01),
                              prim_esfera(0.014, (-0.05, 0.15, 0.13), osc_m, seg=8), prim_esfera(0.014, (0.05, 0.15, 0.13), osc_m, seg=8)], [pel_m, osc_m])
    # cola: pivote en la grupa; apunta atras y arriba
    pieza("perro", "cola", [prim_cono(0.032, 0.014, 0.0, 0.17, pel_m, (0, 0), (0, -0.20)), prim_esfera(0.032, (0, 0, 0), pel_m)], [pel_m, osc_m])
    # pata: pivote en el hombro/cadera (0.35 m hasta el suelo): muslo grueso, canilla y pata con garra oscura
    pieza("perro", "pata", [prim_cono(0.050, 0.030, 0.0, -0.31, pel_m), prim_esfera(0.052, (0, 0, 0), pel_m),
                            prim_caja((0.06, 0.11, 0.04), (0, 0.025, -0.33), pel_m, 0.01)], [pel_m, osc_m])
    bpy.ops.wm.save_as_mainfile(filepath=os.path.join(d, "perro.blend"), copy=True)

    # ============ pelota ============
    limpiar()
    d = os.path.join(RAIZ, "Models", "pelota")
    pm = material("pelota_goma", os.path.join(d, "pelota.png"))
    pieza("pelota", "pelota", [prim_esfera(0.055, (0, 0, 0), pm, seg=20)], [pm])
    bpy.ops.wm.save_as_mainfile(filepath=os.path.join(d, "pelota.blend"), copy=True)
    print("modelos OK")


def vista():
    """Renders de comprobacion (Cycles, 64 muestras): la persona y el perro armados con las mismas medidas que PersonasAnim.h."""
    import bpy, mathutils
    salida = sys.argv[2] if len(sys.argv) > 2 and sys.argv[1] == "vista" else os.path.join(RAIZ, "_vista_x55")
    os.makedirs(salida, exist_ok=True)
    bpy.ops.wm.read_factory_settings(use_empty=True)

    def importa(carpeta, nombre, loc, rot=(0, 0, 0)):
        """loc en coordenadas del JUEGO (x, y arriba, z); en Blender es (x, -z, y). rot = giro en grados alrededor de X (+ = hacia delante)."""
        bpy.ops.wm.obj_import(filepath=os.path.join(RAIZ, "Models", carpeta, nombre + ".obj"), forward_axis="NEGATIVE_Z", up_axis="Y")
        o = bpy.context.selected_objects[0]
        bpy.context.view_layer.objects.active = o
        bpy.ops.object.transform_apply(location=True, rotation=True, scale=True)   # el importador deja un giro de 90 grados en el objeto: se hornea en la malla
        o.location = (loc[0], -loc[2], loc[1]); o.rotation_euler = [math.radians(a) for a in rot]
        return o
    ax = -0.6   # persona de pie en x = -0.6: brazo derecho levantado hacia delante, pierna izquierda adelantada
    importa("persona_art", "torso", (ax, 0.90, 0))
    importa("persona_art", "brazo", (ax + 0.22, 1.44, 0), (150, 0, 0)); importa("persona_art", "brazo", (ax - 0.22, 1.44, 0))
    importa("persona_art", "antebrazo", (ax - 0.22, 1.14, 0)); importa("persona_art", "antebrazo", (ax + 0.22 , 1.44 + 0.15, -0.26), (150, 0, 0))
    importa("persona_art", "muslo", (ax + 0.09, 0.90, 0)); importa("persona_art", "muslo", (ax - 0.09, 0.90, 0), (30, 0, 0))
    importa("persona_art", "pierna", (ax + 0.09, 0.45, 0)); importa("persona_art", "pierna", (ax - 0.09, 0.45 + 0.04, -0.22), (-30, 0, 0))
    px = 0.8    # perro a la derecha (el hocico mira a -Z del juego)
    importa("perro", "cuerpo", (px, 0.45, 0)); importa("perro", "cabeza", (px, 0.45 + 0.12, -0.34)); importa("perro", "cola", (px, 0.45 + 0.08, 0.38), (-35, 0, 0))
    for sx in (-1, 1):
        for sz in (-0.25, 0.25):
            importa("perro", "pata", (px + sx * 0.09, 0.35, sz))
    importa("pelota", "pelota", (px + 0.6, 0.055, -0.4))
    bpy.ops.mesh.primitive_plane_add(size=6, location=(0, 0, 0))
    bpy.ops.object.light_add(type="SUN", location=(2, 4, 3)); bpy.context.active_object.data.energy = 4
    sc = bpy.context.scene
    sc.render.engine = "CYCLES"; sc.cycles.samples = 32; sc.cycles.device = "CPU"
    sc.render.resolution_x, sc.render.resolution_y = 1000, 560
    sc.world = bpy.data.worlds.new("w"); sc.world.color = (0.5, 0.55, 0.6)
    cam_d = bpy.data.cameras.new("c"); cam = bpy.data.objects.new("c", cam_d); sc.collection.objects.link(cam); sc.camera = cam
    # camara de frente (desde +Z del juego, mirando a -Z) y de lado
    for nombre, loc, rot in (("frente", (0.1, 4.2, 1.0), (90, 0, 180)), ("lado", (-4.0, 0.0, 0.9), (90, 0, -90))):
        cam.location = loc; cam.rotation_euler = [math.radians(a) for a in rot]
        sc.render.filepath = os.path.join(salida, "x55_%s.png" % nombre)
        bpy.ops.render.render(write_still=True)
    print("vista OK ->", salida)


{"texturas": texturas, "modelos": modelos, "vista": vista}[modo]()
