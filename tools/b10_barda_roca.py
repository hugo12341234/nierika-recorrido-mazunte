# ==============================================================
# B-10  Nierika Studio  -  Barda perimetral del panteón + Roca
#
# Instrucciones:
#   1. Abre Blender 5.2 con un archivo vacío
#   2. Ve a la pestaña Scripting y pega este archivo
#   3. Cambia BASE_PATH (línea 17) si es necesario
#   4. Presiona Run Script (▶)
# ==============================================================

import bpy, bmesh, os, shutil, random

# CAMBIA esta línea si tu repo está en otra ruta
import os as _os
BASE_PATH = _os.path.join(_os.path.expanduser("~"), "Documents", "CGeIHC", "nierika-recorrido-mazunte")

PIEDRA_SRC = os.path.join(BASE_PATH, "Models", "fachada", "piedra.jpg")

# ---- utilidades -----------------------------------------------

def limpiar():
    bpy.ops.object.select_all(action='SELECT')
    bpy.ops.object.delete()

def cubo(sx, sy, sz, x=0.0, y=0.0, z=None):
    """Cubo con escala (sx, sy, sz).
    Coordenadas Blender: X→juego X, Y→juego Z, Z→juego Y (arriba).
    z=None pone el pivote en la cara inferior."""
    if z is None:
        z = sz / 2.0
    bpy.ops.mesh.primitive_cube_add(size=1.0, location=(x, y, z))
    obj = bpy.context.active_object
    obj.scale = (sx, sy, sz)
    bpy.ops.object.transform_apply(scale=True)
    return obj

def asignar_material_piedra():
    mat = bpy.data.materials.get("piedra")
    if not mat:
        mat = bpy.data.materials.new("piedra")
        mat.use_nodes = True
        nodes = mat.node_tree.nodes
        for n in list(nodes):
            nodes.remove(n)
        tex  = nodes.new('ShaderNodeTexImage')
        bsdf = nodes.new('ShaderNodeBsdfPrincipled')
        out  = nodes.new('ShaderNodeOutputMaterial')
        if os.path.exists(PIEDRA_SRC):
            tex.image = bpy.data.images.load(PIEDRA_SRC)
        mat.node_tree.links.new(tex.outputs['Color'], bsdf.inputs['Base Color'])
        mat.node_tree.links.new(bsdf.outputs['BSDF'], out.inputs['Surface'])
    obj = bpy.context.active_object
    if obj.data.materials:
        obj.data.materials[0] = mat
    else:
        obj.data.materials.append(mat)

def guardar_exportar(nombre):
    bpy.ops.object.select_all(action='SELECT')
    objs = list(bpy.context.selected_objects)
    bpy.context.view_layer.objects.active = objs[0]
    if len(objs) > 1:
        bpy.ops.object.join()
    bpy.context.active_object.name = nombre
    asignar_material_piedra()

    carpeta = os.path.join(BASE_PATH, "Models", nombre)
    os.makedirs(carpeta, exist_ok=True)

    dst_tex = os.path.join(carpeta, "piedra.jpg")
    if os.path.exists(PIEDRA_SRC) and not os.path.exists(dst_tex):
        shutil.copy2(PIEDRA_SRC, dst_tex)

    ruta_obj = os.path.join(carpeta, nombre + ".obj")
    bpy.ops.wm.obj_export(
        filepath=ruta_obj,
        export_selected_objects=True,
        forward_axis='NEGATIVE_Z',
        up_axis='Y',
        apply_modifiers=True,
        export_materials=True,
        path_mode='COPY',
    )

    ruta_blend = os.path.join(carpeta, nombre + ".blend")
    bpy.ops.wm.save_as_mainfile(filepath=ruta_blend, copy=True)

    print(f"  ✓  {nombre}  →  {carpeta}")

# ============================================================
# BARDA — recinto rectangular del panteón
#   Dimensiones del recinto (coordenadas de juego):
#     X: -7 .. +7 (14 m de ancho)
#     Z: +3 .. +12 (9 m de profundidad)
#   Entradas (aberturas) de 2 m en el muro norte (z=12) y en el sur (z=3), centradas en x=0
#   Altura: 0.70 m  |  Espesor: 0.25 m
#
#   Mapeo Blender → juego (E-2, 12/11, corregido): Blender X = juego X
#                            Blender Y = juego -Z   (el exportador usa Forward -Z, igual que tools/bpy_comun.py)
#                            Blender Z = juego Y (vertical)
#   Antes (B-10) el script suponía Blender Y = juego +Z y la barda salía espejada en z (z = -12..-3 en el .obj):
#   se dibujaba DETRÁS del cuarto mientras sus colisiones estaban delante (z = +3..+12).
#   E-2 también abre una puerta de 2 m en el muro sur (x -1..+1), alineada con la del cuarto y con la entrada norte,
#   porque la silueta camina de la orilla al altar por x = 0 y el jugador debe poder pasar del panteón al cuarto.
# ============================================================
limpiar()

# Muro sur izquierdo (x=-7..-1, z=3..3.25 en el juego)
cubo( 6.00, 0.25, 0.70, x=-4.000, y=-3.125)   # sx=6, sy=0.25, sz=0.70

# Muro sur derecho (x=+1..+7, z=3..3.25): abertura central de 2 m (x -1..+1)
cubo( 6.00, 0.25, 0.70, x= 4.000, y=-3.125)

# Muro norte izquierdo (x=-7..-1, z=11.75..12)
cubo( 6.00, 0.25, 0.70, x=-4.000, y=-11.875)  # sx=6, sy=0.25

# Muro norte derecho (x=+1..+7, z=11.75..12)
cubo( 6.00, 0.25, 0.70, x= 4.000, y=-11.875)  # abertura central de 2 m (x -1..+1)

# Muro oeste (x=-7..-6.75, z=3.25..12)
cubo( 0.25, 8.75, 0.70, x=-6.875, y=-7.625)   # desde z=3.25 (tras el muro sur) a z=12

# Muro este (x=+6.75..+7, z=3.25..12)
cubo( 0.25, 8.75, 0.70, x= 6.875, y=-7.625)

guardar_exportar("barda")

# ============================================================
# ROCA — piedra irregular decorativa junto a la entrada
#   Dimensiones finales aprox. 0.45 × 0.35 × 0.30 m
#   Se genera deformando un cubo subdividido con ruido
# ============================================================
limpiar()

bpy.ops.mesh.primitive_cube_add(size=1.0, location=(0.0, 0.0, 0.15))
roca_obj = bpy.context.active_object
roca_obj.scale = (0.45, 0.35, 0.30)
bpy.ops.object.transform_apply(scale=True)

# Subdividir para más vértices antes de deformar
bpy.ops.object.mode_set(mode='EDIT')
bpy.ops.mesh.subdivide(number_cuts=2)
bpy.ops.object.mode_set(mode='OBJECT')

# Desplazar vértices con ruido controlado
random.seed(7)
for v in roca_obj.data.vertices:
    amp = 0.07
    v.co.x += random.uniform(-amp, amp)
    v.co.y += random.uniform(-amp, amp)
    v.co.z += random.uniform(-amp * 0.5, amp * 0.5)
    if v.co.z < 0.0:
        v.co.z = 0.0
roca_obj.data.update()

guardar_exportar("roca")

print("\n=== B-10 listo ===")
print("Models/barda/  y  Models/roca/  generados")
print("Cada carpeta tiene: .obj  .mtl  piedra.jpg  .blend")
