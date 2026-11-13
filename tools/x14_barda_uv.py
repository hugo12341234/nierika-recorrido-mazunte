# X-14: reasigna las UV de la barda con proyeccion de caja a 1 tile por metro (la textura ya no se estira) y reexporta.
# Uso: "C:\Program Files\Blender Foundation\Blender 5.2\blender.exe" -b --python tools/x14_barda_uv.py
import os, sys
import bpy
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from bpy_comun import RAIZ, limpiar, material, exportar

limpiar()
ruta = os.path.join(RAIZ, "Models", "barda", "barda.obj")
bpy.ops.wm.obj_import(filepath=ruta, forward_axis="NEGATIVE_Z", up_axis="Y")
objs = [o for o in bpy.context.scene.objects if o.type == "MESH"]
bpy.ops.object.select_all(action="DESELECT")
for o in objs:
    o.select_set(True)
bpy.context.view_layer.objects.active = objs[0]
if len(objs) > 1:
    bpy.ops.object.join()
ob = bpy.context.active_object
ob.name = "barda"
bpy.ops.object.mode_set(mode="EDIT")
bpy.ops.mesh.select_all(action="SELECT")
bpy.ops.uv.cube_project(cube_size=1.0, correct_aspect=False, scale_to_bounds=False)  # 1 m = 1 repeticion de la textura
bpy.ops.object.mode_set(mode="OBJECT")
ob.data.materials.clear()
ob.data.materials.append(material("piedra", os.path.join(RAIZ, "Models", "barda", "barda_arenisca.jpg")))
exportar(ob, "barda")
print("OK barda", len(ob.data.polygons), "caras")
