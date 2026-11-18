# X-43: separa las piezas de los modelos de Maya y las deja con pivote propio (1 u = 1 m).
# Uso: python3 -I piezas.py <Models> ; lee y REEMPLAZA los .obj de papel_*, mazeta, cuadro y refresco.
import bpy, sys, os, re, shutil
from mathutils import Vector
M = sys.argv[-1]
def cargar(n):
    bpy.ops.wm.read_factory_settings(use_empty=True)
    bpy.ops.wm.obj_import(filepath=f"{M}/_orig_{n}/{n}.obj")
    obs=[o for o in bpy.context.scene.objects if o.type=="MESH"]
    bpy.ops.object.select_all(action='DESELECT')
    for o in obs: o.select_set(True)
    bpy.context.view_layer.objects.active=obs[0]
    if len(obs)>1: bpy.ops.object.join()
    o=bpy.context.view_layer.objects.active
    bpy.ops.object.mode_set(mode='EDIT'); bpy.ops.mesh.select_all(action='SELECT')
    bpy.ops.mesh.separate(type='LOOSE'); bpy.ops.object.mode_set(mode='OBJECT')
    return [x for x in bpy.context.scene.objects if x.type=="MESH"]
def bb(o):
    c=[o.matrix_world@Vector(v) for v in o.bound_box]
    return Vector((min(p[i] for p in c) for i in range(3))),Vector((max(p[i] for p in c) for i in range(3)))
def guardar(n,partes,pivote,giro_y=0.0,tris=0):
    """Une las partes, mueve el pivote (px,py,pz) al origen, gira giro_y grados en Y (mundo OBJ) y exporta a Models/n."""
    bpy.ops.object.select_all(action='DESELECT')
    for o in partes: o.select_set(True)
    bpy.context.view_layer.objects.active=partes[0]
    if len(partes)>1: bpy.ops.object.join()
    o=bpy.context.view_layer.objects.active
    # OBJ (x,y,z) -> Blender (x,-z,y): el pivote en OBJ (px,py,pz) es Blender (px,-pz,py)
    o.location=(o.location.x-pivote[0], o.location.y+pivote[2], o.location.z-pivote[1])
    bpy.ops.object.transform_apply(location=True,rotation=False,scale=False)
    if giro_y:
        import math
        o.rotation_euler=(0,0,math.radians(giro_y)); bpy.ops.object.transform_apply(location=False,rotation=True,scale=False)
    o.name=n
    if tris:
        m=o.modifiers.new('t','TRIANGULATE'); bpy.ops.object.modifier_apply(modifier='t')
        if len(o.data.polygons)>tris:
            m=o.modifiers.new('d','DECIMATE'); m.ratio=tris/len(o.data.polygons); bpy.ops.object.modifier_apply(modifier='d')
        print('tris',n,len(o.data.polygons))
    bpy.ops.object.select_all(action='DESELECT'); o.select_set(True)
    d=f"{M}/{n}"
    bpy.ops.wm.obj_export(filepath=f"{d}/{n}.obj",export_selected_objects=True,export_materials=False,export_normals=True,export_uv=True,apply_modifiers=True,forward_axis='NEGATIVE_Z',up_axis='Y')
    p=f"{d}/{n}.obj"; t=open(p).read()
    t=re.sub(r"^mtllib .*\n?","",t,flags=re.M); t=re.sub(r"^usemtl .*\n?","",t,flags=re.M)
    open(p,"w").write(f"mtllib {n}.mtl\nusemtl {n.split('_x')[0]}\n"+t)
def mtl_de(n,usado):
    src=open(f"{M}/{usado}/{usado}.mtl").read().replace("newmtl "+usado,"newmtl "+usado)
    return src
def obj_nuevo(nombre,fuente,partes,pivote_fn,giro=0.0,tris=0):
    """Crea Models/nombre (obj, mtl, png) con 'partes' de la fuente."""
    d=f"{M}/{nombre}"; os.makedirs(d,exist_ok=True)
    png=[f for f in os.listdir(f"{M}/{fuente}") if f.endswith(".png")][0]
    shutil.copy(f"{M}/{fuente}/{png}",f"{d}/{nombre}.png")
    open(f"{d}/{nombre}.mtl","w").write(f"newmtl {nombre}\nKa 1 1 1\nKd 1 1 1\nKs 0 0 0\nd 1\nillum 1\nmap_Kd {nombre}.png\n")
    mn=Vector((1e9,)*3);mx=Vector((-1e9,)*3)
    for o in partes:
        a,b=bb(o)
        for i in range(3): mn[i]=min(mn[i],a[i]);mx[i]=max(mx[i],b[i])
    # bb() esta en coordenadas Blender (x, y=-z_obj, z=y_obj): pasar a OBJ
    omn=Vector((mn.x,mn.z,-mx.y));omx=Vector((mx.x,mx.z,-mn.y))
    guardar(nombre,partes,pivote_fn(omn,omx),giro,tris)
    # el obj ya usa usemtl <nombre> -> corregir
    p=f"{d}/{nombre}.obj";t=open(p).read().replace(f"usemtl {nombre.split('_x')[0]}",f"usemtl {nombre}");open(p,"w").write(t)
    print(nombre,"ok",[round(v,3) for v in (omx-omn)])
centro_sup=lambda a,b:((a.x+b.x)/2,b.y,(a.z+b.z)/2)    # centro del borde superior
centro_base=lambda a,b:((a.x+b.x)/2,a.y,(a.z+b.z)/2)   # centro de la base
for n in ["papel_fucsia","papel_morado","papel_naranja","mazeta","cuadro","refresco"]:
    shutil.copytree(f"{M}/{n}",f"{M}/_orig_{n}",dirs_exist_ok=True)
# papeles: pares en el plano YZ (x = const) -> giro 90 para que queden en el plano XY (ancho en x, cara hacia +z)
for n in ["papel_fucsia","papel_morado"]:
    P=cargar(n); obj_nuevo(n+"_x",n,P,centro_sup,0.0)
P=cargar("papel_naranja"); P=sorted(P,key=lambda o:bb(o)[0].x)
obj_nuevo("papel_naranja_x","papel_naranja",[P[1]],centro_sup,0.0)
# macetas: cada maceta = pieza grande + chicas cercanas; se exporta la primera (pivote en la base)
P=cargar("mazeta"); grandes=sorted([o for o in P if len(o.data.vertices)>500],key=lambda o:(bb(o)[0].x,bb(o)[0].y))
g=grandes[0];ca=(bb(g)[0]+bb(g)[1])/2
chicas=[o for o in P if o is not g and o not in grandes and (((bb(o)[0]+bb(o)[1])/2)-ca).length<0.3]
obj_nuevo("maceta_x","mazeta",[g]+chicas,centro_base,0.0,3500)
for n,f in [("cuadro","cuadro"),("refresco","refresco")]:
    P=cargar(f); P=[o for o in P if len(o.data.vertices)>50]; obj_nuevo(n+"_x",f,P,centro_base,0.0)
