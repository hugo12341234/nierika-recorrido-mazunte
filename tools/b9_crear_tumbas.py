# ==============================================================
# B-9  Nierika Studio  -  Crear y exportar las 4 tumbas
#
# Instrucciones:
#   1. Abre Blender 5.2 con un archivo cualquiera (vacío ok)
#   2. Ve a la pestaña  Scripting  y abre este archivo
#      (o pega el contenido y guarda)
#   3. Cambia BASE_PATH  (línea 16)
#   4. Presiona  Run Script  (▶)
#      El script crea los .obj Y los .blend automáticamente.
# ==============================================================

import bpy, bmesh, os, shutil

BASE_PATH = r"C:\ruta\al\repo\nierika-recorrido-mazunte"  # <-- CAMBIA ESTO

# ruta de la textura que ya existe en el repo
PIEDRA_SRC = os.path.join(BASE_PATH, "Models", "fachada", "piedra.jpg")

# ---- utilidades -----------------------------------------------

def limpiar():
    bpy.ops.object.select_all(action='SELECT')
    bpy.ops.object.delete()

def cubo(sx, sy, sz, x=0.0, y=0.0, z=None):
    """Cubo con escala (sx,sy,sz). z es la posición del centro; si no se
    da, el pivote queda en la cara inferior (z = sz/2)."""
    if z is None:
        z = sz / 2.0
    bpy.ops.mesh.primitive_cube_add(size=1.0, location=(x, y, z))
    obj = bpy.context.active_object
    obj.scale = (sx, sy, sz)
    bpy.ops.object.transform_apply(scale=True)
    return obj

def esfera_top(sx, sy, sz, x=0.0, y=0.0, z_base=0.0):
    """Semiesfera: radios (sx, sy, sz), cara plana hacia abajo en z_base."""
    bpy.ops.mesh.primitive_uv_sphere_add(
        radius=1.0,
        location=(x, y, z_base + sz),  # centro de la esfera completa
        segments=16, ring_count=8
    )
    obj = bpy.context.active_object
    obj.scale = (sx, sy, sz)
    bpy.ops.object.transform_apply(scale=True)
    # Tras apply scale, local z va de -sz a +sz; borramos la mitad inferior
    bm = bmesh.new()
    bm.from_mesh(obj.data)
    bmesh.ops.delete(bm,
        geom=[v for v in bm.verts if v.co.z < 0],
        context='VERTS')
    bm.to_mesh(obj.data)
    bm.free()
    obj.data.update()
    return obj

def asignar_material_piedra():
    """Crea (o reutiliza) un material 'piedra' con piedra.jpg."""
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
    """Une todos los objetos, asigna material, exporta .obj y guarda .blend."""
    bpy.ops.object.select_all(action='SELECT')
    objs = list(bpy.context.selected_objects)
    bpy.context.view_layer.objects.active = objs[0]
    if len(objs) > 1:
        bpy.ops.object.join()
    bpy.context.active_object.name = nombre
    asignar_material_piedra()

    carpeta = os.path.join(BASE_PATH, "Models", nombre)
    os.makedirs(carpeta, exist_ok=True)

    # Copiar piedra.jpg a la carpeta de la tumba
    dst_tex = os.path.join(carpeta, "piedra.jpg")
    if os.path.exists(PIEDRA_SRC) and not os.path.exists(dst_tex):
        shutil.copy2(PIEDRA_SRC, dst_tex)

    # Exportar .obj
    ruta_obj = os.path.join(carpeta, nombre + ".obj")
    bpy.ops.wm.obj_export(
        filepath=ruta_obj,
        export_selected_objects=True,
        forward_axis='NEGATIVE_Z',
        up_axis='Y',
        apply_modifiers=True,
        export_materials=True,
        path_mode='COPY',       # copia texturas junto al .obj
    )

    # Guardar .blend (copy=True: no cambia el nombre del archivo actual)
    ruta_blend = os.path.join(carpeta, nombre + ".blend")
    bpy.ops.wm.save_as_mainfile(filepath=ruta_blend, copy=True)

    print(f"  ✓  {nombre}  →  {carpeta}")

# ============================================================
# TUMBA 1 — Capillita con domo y cruz
#   ancho 1.3  ·  prof 0.9  ·  alto ~1.9 m
# ============================================================
limpiar()
cubo(1.30, 0.90, 1.00)                       # cuerpo (z 0 → 1)
esfera_top(0.60, 0.45, 0.45, z_base=1.00)   # domo encima (z 1 → 1.45)
cubo(0.07, 0.07, 0.40, z=1.65)              # cruz: palo vertical
cubo(0.30, 0.07, 0.07, z=1.75)              # cruz: palo horizontal
guardar_exportar("tumba1")

# ============================================================
# TUMBA 2 — Lápida con cabecera redondeada
#   ancho 0.6  ·  prof 0.2  ·  alto ~1.05 m
# ============================================================
limpiar()
cubo(0.60, 0.20, 0.15)                       # zócalo
cubo(0.50, 0.15, 0.65, z=0.15 + 0.325)      # losa
esfera_top(0.25, 0.08, 0.25, z_base=0.80)   # cabecera redondeada
guardar_exportar("tumba2")

# ============================================================
# TUMBA 3 — Pirámide escalonada
#   base 1.0×1.0  ·  alto ~0.86 m
# ============================================================
limpiar()
cubo(1.00, 1.00, 0.20, z=0.10)              # escalón base
cubo(0.70, 0.70, 0.20, z=0.30)              # escalón medio
cubo(0.40, 0.40, 0.20, z=0.50)              # escalón cima
cubo(0.14, 0.06, 0.16, z=0.78)             # lápida encima
guardar_exportar("tumba3")

# ============================================================
# TUMBA 4 — Sepulcro tumbado con tapa y cruz
#   largo 1.6  ·  ancho 0.7  ·  alto ~0.63 m
# ============================================================
limpiar()
cubo(1.60, 0.70, 0.40, z=0.20)              # caja
cubo(1.62, 0.72, 0.18, z=0.49)              # tapa
cubo(0.06, 0.06, 0.26, x=0.70, z=0.75)     # cruz: vertical
cubo(0.22, 0.06, 0.06, x=0.70, z=0.84)     # cruz: horizontal
guardar_exportar("tumba4")

print("\n=== B-9 listo ===")
print("Archivos generados en Models/tumba1/ … tumba4/")
print("Cada carpeta tiene: .obj  .mtl  piedra.jpg  .blend")
