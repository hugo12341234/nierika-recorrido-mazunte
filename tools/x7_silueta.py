# X-7: piezas organicas de la silueta (reemplazan los 10 cubos de D1). Mismos pivotes y largos que Silueta.cpp.
# Uso (desde la raiz del repo):  "C:\Program Files\Blender Foundation\Blender 5.2\blender.exe" -b --python tools/x7_silueta.py
# Coordenadas de Blender: X = juego X, Y = juego -Z (el frente de la silueta mira a +Y), Z = juego Y (arriba).
import os, sys
import bpy
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from bpy_comun import RAIZ, limpiar, material

CARPETA = os.path.join(RAIZ, "Models", "silueta")
limpiar()
mat = material("silueta_tela", os.path.join(CARPETA, "silueta_tela.png"))


def objeto_de(op_resultado_nombre):
    return bpy.context.active_object


def cono(r_arriba, r_abajo, z_ini, z_fin, seg=16):
    """Cilindro/cono entre dos alturas (z_ini arriba, z_fin abajo)."""
    alto = z_ini - z_fin
    bpy.ops.mesh.primitive_cone_add(vertices=seg, radius1=r_abajo, radius2=r_arriba, depth=alto, location=(0, 0, (z_ini + z_fin) / 2))
    return bpy.context.active_object


def esfera(r, centro, escala=(1, 1, 1), seg=16):
    bpy.ops.mesh.primitive_uv_sphere_add(segments=seg, ring_count=max(8, seg // 2), radius=r, location=centro)
    o = bpy.context.active_object
    o.scale = escala
    return o


def caja(dim, centro):
    bpy.ops.mesh.primitive_cube_add(size=1, location=centro)
    o = bpy.context.active_object
    o.scale = dim
    return o


def biselar(o, ancho, seg=3):
    bpy.ops.object.transform_apply(scale=True)
    m = o.modifiers.new("bisel", "BEVEL")
    m.width = ancho
    m.segments = seg


def pieza(nombre, partes):
    """Une las partes en un solo objeto con el pivote en el origen, aplica transformaciones/modificadores y exporta."""
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
    o.data.materials.clear()
    o.data.materials.append(mat)
    # UV: proyeccion por caja (la tela es continua, se repite cada 0.5 m)
    bpy.ops.object.mode_set(mode="EDIT")
    bpy.ops.mesh.select_all(action="SELECT")
    bpy.ops.uv.cube_project(cube_size=0.5)
    bpy.ops.object.mode_set(mode="OBJECT")
    bpy.ops.object.select_all(action="DESELECT")
    o.select_set(True)
    bpy.context.view_layer.objects.active = o
    bpy.ops.wm.obj_export(filepath=os.path.join(CARPETA, nombre + ".obj"), export_selected_objects=True,
                          forward_axis="NEGATIVE_Z", up_axis="Y", apply_modifiers=True, export_materials=True, path_mode="RELATIVE")
    return o


def hacer_todo(lado):
    s = "izq" if lado < 0 else "der"
    # Brazo: hombro redondo + cono hasta el codo (0.30 m)
    pieza("brazo_" + s, [cono(0.045, 0.036, 0.0, -0.30), esfera(0.05, (0, 0, 0))])
    # Antebrazo: cono hasta la muneca (0.26 m) + mano (0.28 m)
    pieza("antebrazo_" + s, [cono(0.036, 0.028, 0.0, -0.25), esfera(0.04, (0, 0, -0.265), (0.8, 1.0, 1.15))])
    # Muslo (0.45 m) con cadera redondeada
    pieza("muslo_" + s, [cono(0.07, 0.052, 0.0, -0.45), esfera(0.07, (0, 0, 0))])
    # Pierna (0.45 m): pantorrilla + pie hacia adelante (+Y)
    pieza("pierna_" + s, [cono(0.052, 0.036, 0.0, -0.42), esfera(0.052, (0, 0, 0)),
                          caja((0.085, 0.22, 0.06), (0, 0.04, -0.42))])


# Torso (0.58 m, pivote en la cadera): pecho ancho tipo huipil, cintura mas angosta
t1 = caja((0.36, 0.20, 0.36), (0, 0, 0.40)); biselar(t1, 0.05)    # pecho y hombros (0.22 a 0.58)
t2 = caja((0.30, 0.18, 0.26), (0, 0, 0.13)); biselar(t2, 0.04)    # cintura y cadera (0 a 0.26)
pieza("torso", [t1, t2])
# Cabeza (centro a 0.12 m sobre la articulacion) con cuello
pieza("cabeza", [esfera(0.10, (0, 0, 0.12), (0.95, 1.0, 1.1), seg=24), cono(0.04, 0.045, 0.06, -0.0, 12)])
for lado in (-1, 1):
    hacer_todo(lado)

bpy.ops.wm.save_as_mainfile(filepath=os.path.join(CARPETA, "silueta.blend"))
print("OK piezas de la silueta")
