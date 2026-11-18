# Utilidades comunes de los scripts de Blender de Nierika Studio.
# Se ejecutan sin ventana con el modulo `bpy` de Python (python3 tools/<script>.py desde cualquier carpeta)
# o dentro de Blender (pestana Scripting). Coordenadas de Blender: X = juego X, Y = juego -Z, Z = juego Y (arriba);
# el exportador usa Forward -Z / Up Y, como en el resto de los modelos del repo.
import os
import bpy

RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))


def limpiar():
    """Escena vacia (mallas, materiales e imagenes huerfanas incluidas)."""
    bpy.ops.wm.read_factory_settings(use_empty=True)


def material(nombre, png_abs, alfa=False):
    """Material con una sola textura (Principled + Image Texture). Si alfa, conecta el alfa de la imagen."""
    mat = bpy.data.materials.new(nombre)
    mat.use_nodes = True
    nodos = mat.node_tree.nodes
    bsdf = nodos["Principled BSDF"]
    tex = nodos.new("ShaderNodeTexImage")
    tex.image = bpy.data.images.load(png_abs)
    enlaces = mat.node_tree.links
    enlaces.new(tex.outputs["Color"], bsdf.inputs["Base Color"])
    if alfa:
        enlaces.new(tex.outputs["Alpha"], bsdf.inputs["Alpha"])
    return mat


def exportar(objeto, nombre):
    """Guarda Models/<nombre>/<nombre>.obj (+ .mtl) y .blend. Aplica modificadores."""
    carpeta = os.path.join(RAIZ, "Models", nombre)
    os.makedirs(carpeta, exist_ok=True)
    bpy.ops.object.select_all(action="DESELECT")
    objeto.select_set(True)
    bpy.context.view_layer.objects.active = objeto
    bpy.ops.wm.obj_export(
        filepath=os.path.join(carpeta, nombre + ".obj"),
        export_selected_objects=True,
        forward_axis="NEGATIVE_Z", up_axis="Y",
        apply_modifiers=True, export_materials=True, path_mode="RELATIVE",
    )
    bpy.ops.wm.save_as_mainfile(filepath=os.path.join(carpeta, nombre + ".blend"), copy=True)
    return carpeta
