r"""X-19: figuras humanas ESTATICAS (poses fijas) para la ceremonia en la playa + petate de ofrenda.
No son personajes con IA: son mallas de pocas caras (~600) con una pose fija; su unica animacion es un balanceo senoidal en main.cpp.
Modelos (Models/<nombre>/, .obj + .mtl + 3 texturas 1K + .blend): persona_pie_a, persona_pie_b, persona_agachada_a, persona_agachada_b, petate.
Uso (raiz del repo):
    python tools/x19_ceremonia.py texturas
    "C:\Program Files\Blender Foundation\Blender 5.2\blender.exe" -b --python tools/x19_ceremonia.py -- modelos
Convenciones (como el resto del proyecto): 1 u = 1 m, pivote en el suelo bajo la persona (centro entre los pies o las rodillas),
el frente mira a -Z del juego con giro 0. Blender: X = juego X, Y = juego -Z (el frente mira a +Y de Blender), Z = arriba."""
import math, os, sys
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
modo = sys.argv[-1]
N = 1024
PERSONAS = ["persona_pie_a", "persona_pie_b", "persona_agachada_a", "persona_agachada_b"]
ROPA = {"a": ((0.15, 0.20, 0.45), (0.92, 0.88, 0.78)), "b": ((0.55, 0.10, 0.35), (0.95, 0.60, 0.20))}  # (color de trama, color base)


def texturas():
    import numpy as np
    from PIL import Image
    r = np.random.default_rng(19)
    yy, xx = np.mgrid[0:N, 0:N]
    for tag, (c1, c2) in ROPA.items():
        trama = 0.5 + 0.25 * np.sin(xx * 2 * math.pi / 10) * np.sin(yy * 2 * math.pi / 10)
        banda = (np.sin(yy * 2 * math.pi / 160) > 0.93).astype(float)[..., None]   # solo franjas horizontales finas (bordado)
        base = np.array(c2)[None, None, :] * (0.75 + 0.35 * trama[..., None])
        img = base * (1 - 0.7 * banda) + np.array(c1)[None, None, :] * 0.7 * banda
        img = np.clip(img * (0.9 + 0.1 * r.random((N, N, 1))), 0, 1)
        Image.fromarray((img * 255).astype(np.uint8), "RGB").save(os.path.join(RAIZ, "Models", "_tmp_ropa_%s.png" % tag))
    piel = np.array((0.55, 0.38, 0.27))[None, None, :] * (0.92 + 0.12 * r.random((N, N, 1)))
    Image.fromarray((np.clip(piel, 0, 1) * 255).astype(np.uint8), "RGB").save(os.path.join(RAIZ, "Models", "_tmp_piel.png"))
    pelo = np.array((0.06, 0.05, 0.05))[None, None, :] * (0.7 + 0.6 * r.random((N, N, 1)))
    Image.fromarray((np.clip(pelo, 0, 1) * 255).astype(np.uint8), "RGB").save(os.path.join(RAIZ, "Models", "_tmp_pelo.png"))
    # petate: trama de palma entrelazada
    cuad = ((xx // 32 + yy // 32) % 2).astype(float)
    hilo = 0.5 + 0.5 * np.where(cuad > 0, np.sin(xx * 2 * math.pi / 8), np.sin(yy * 2 * math.pi / 8))
    pet = np.array((0.62, 0.48, 0.25))[None, None, :] * (0.65 + 0.35 * hilo[..., None]) * (0.9 + 0.1 * r.random((N, N, 1)))
    os.makedirs(os.path.join(RAIZ, "Models", "petate"), exist_ok=True)
    Image.fromarray((np.clip(pet, 0, 1) * 255).astype(np.uint8), "RGB").save(os.path.join(RAIZ, "Models", "petate", "petate.png"))
    print("texturas OK")


def modelos():
    import bpy, bmesh, shutil
    from mathutils import Vector, Matrix
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from bpy_comun import limpiar, material, exportar

    def tubo(bm, p0, p1, r0, r1, mat, seg=10):
        p0, p1 = Vector(p0), Vector(p1)
        eje = (p1 - p0).normalized()
        a = Vector((1, 0, 0)) if abs(eje.x) < 0.9 else Vector((0, 1, 0))
        u = eje.cross(a).normalized(); v = eje.cross(u).normalized()
        anillos = []
        for p, r in ((p0, r0), (p1, r1)):
            anillos.append([bm.verts.new(p + (u * math.cos(2 * math.pi * k / seg) + v * math.sin(2 * math.pi * k / seg)) * r) for k in range(seg)])
        for k in range(seg):
            f = bm.faces.new((anillos[0][k], anillos[0][(k + 1) % seg], anillos[1][(k + 1) % seg], anillos[1][k])); f.material_index = mat
        for anillo, inv in ((anillos[0], True), (anillos[1], False)):
            f = bm.faces.new(anillo[::-1] if inv else anillo); f.material_index = mat

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

    # indices de material: 0 ropa, 1 piel, 2 pelo
    def persona_pie(bm):
        for s in (-1, 1):
            tubo(bm, (s * 0.09, 0.0, 0.04), (s * 0.09, 0.0, 0.88), 0.065, 0.085, 0)          # piernas (pantalon)
            caja(bm, (s * 0.09, 0.05, 0.03), (0.10, 0.26, 0.07), 1)                            # pies
            tubo(bm, (s * 0.20, 0.0, 1.38), (s * 0.22, 0.04, 0.88), 0.05, 0.045, 0)            # brazos
            tubo(bm, (s * 0.22, 0.04, 0.88), (s * 0.23, 0.08, 0.66), 0.042, 0.036, 1)          # antebrazos
            esfera(bm, (s * 0.23, 0.08, 0.62), 0.04, 1, (0.8, 1.0, 1.2))                       # manos
        caja(bm, (0, 0, 1.12), (0.36, 0.20, 0.58), 0)                                          # torso (huipil)
        caja(bm, (0, 0, 0.88), (0.32, 0.18, 0.16), 0)                                          # cadera
        tubo(bm, (0, 0, 1.40), (0, 0.01, 1.50), 0.05, 0.045, 1)                                # cuello
        esfera(bm, (0, 0.01, 1.60), 0.105, 1, (0.92, 1.0, 1.1))                                # cabeza
        esfera(bm, (0, -0.02, 1.64), 0.108, 2, (0.95, 1.0, 1.02))                              # pelo

    def persona_agachada(bm):
        # de rodillas: rodillas en el suelo, torso inclinado hacia adelante, brazos extendidos hacia la ofrenda (+Y)
        for s in (-1, 1):
            tubo(bm, (s * 0.10, -0.28, 0.08), (s * 0.10, 0.12, 0.07), 0.055, 0.07, 0)          # espinillas (en el suelo)
            tubo(bm, (s * 0.10, 0.12, 0.08), (s * 0.10, -0.06, 0.46), 0.08, 0.09, 0)           # muslos
            tubo(bm, (s * 0.19, 0.20, 0.82), (s * 0.20, 0.45, 0.50), 0.05, 0.045, 0)           # brazos
            tubo(bm, (s * 0.20, 0.45, 0.50), (s * 0.15, 0.68, 0.16), 0.042, 0.035, 1)          # antebrazos
            esfera(bm, (s * 0.15, 0.71, 0.12), 0.04, 1, (0.8, 1.2, 0.8))                       # manos
        rot = Matrix.Rotation(math.radians(-32), 3, "X")                                        # inclinacion del torso
        caja(bm, (0, 0.03, 0.62), (0.34, 0.19, 0.56), 0, rot)                                  # torso
        caja(bm, (0, -0.06, 0.40), (0.30, 0.18, 0.16), 0)                                      # cadera
        tubo(bm, (0, 0.21, 0.82), (0, 0.27, 0.90), 0.05, 0.045, 1)                             # cuello
        esfera(bm, (0, 0.30, 0.99), 0.105, 1, (0.92, 1.0, 1.1))                                # cabeza
        esfera(bm, (0, 0.27, 1.03), 0.108, 2, (0.95, 1.0, 1.02))                               # pelo

    def construir(nombre):
        limpiar()
        tag = nombre[-1]
        carpeta = os.path.join(RAIZ, "Models", nombre)
        os.makedirs(carpeta, exist_ok=True)
        for src, dst in (("_tmp_ropa_%s.png" % tag, "ropa.png"), ("_tmp_piel.png", "piel.png"), ("_tmp_pelo.png", "pelo.png")):
            shutil.copyfile(os.path.join(RAIZ, "Models", src), os.path.join(carpeta, dst))
        bm = bmesh.new()
        (persona_pie if "pie" in nombre else persona_agachada)(bm)
        me = bpy.data.meshes.new(nombre)
        # UV de caja (0.5 m por repeticion)
        bm.to_mesh(me); bm.free()
        ob = bpy.data.objects.new(nombre, me); bpy.context.collection.objects.link(ob)
        for nm, png in (("ropa", "ropa.png"), ("piel", "piel.png"), ("pelo", "pelo.png")):
            me.materials.append(material(nombre + "_" + nm, os.path.join(carpeta, png)))
        for p in me.polygons:
            p.use_smooth = True
        bpy.context.view_layer.objects.active = ob; ob.select_set(True)
        bpy.ops.object.mode_set(mode="EDIT"); bpy.ops.mesh.select_all(action="SELECT")
        bpy.ops.uv.cube_project(cube_size=0.5); bpy.ops.object.mode_set(mode="OBJECT")
        exportar(ob, nombre)
        print(nombre, len(me.polygons), "caras")

    for nom in PERSONAS:
        construir(nom)
    # petate: plano de 1.0 x 0.7 m con un grosor de 2 cm
    limpiar()
    pet_png = os.path.join(RAIZ, "Models", "petate", "petate.png")
    bm = bmesh.new()
    res = bmesh.ops.create_cube(bm, size=1.0)
    for v in res["verts"]:
        v.co = Vector((v.co.x * 1.0, v.co.y * 0.7, v.co.z * 0.02 + 0.01))
    me = bpy.data.meshes.new("petate"); bm.to_mesh(me); bm.free()
    ob = bpy.data.objects.new("petate", me); bpy.context.collection.objects.link(ob)
    me.materials.append(material("petate", pet_png))
    bpy.context.view_layer.objects.active = ob; ob.select_set(True)
    bpy.ops.object.mode_set(mode="EDIT"); bpy.ops.mesh.select_all(action="SELECT")
    bpy.ops.uv.cube_project(cube_size=0.7); bpy.ops.object.mode_set(mode="OBJECT")
    exportar(ob, "petate")
    for tmp in ("_tmp_ropa_a.png", "_tmp_ropa_b.png", "_tmp_piel.png", "_tmp_pelo.png"):
        try:
            os.remove(os.path.join(RAIZ, "Models", tmp))
        except OSError:
            pass
    print("modelos OK")


if modo == "texturas":
    texturas()
elif modo == "modelos":
    modelos()
