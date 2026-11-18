# Reduce modelos de Maya (zip del altar): decimate + .mtl limpio + textura 1K.
import bpy, sys, os, re, shutil
from PIL import Image
SRC, OUT = sys.argv[-2], sys.argv[-1]
# carpeta zip, obj, destino, triangulos objetivo, textura(s) base (None = la del mtl)
JOBS = [
 ("Cuadro","cuadro","cuadro",0,"texture_diffuse.png"),
 ("Fuego","fuego","fuego",4000,"texture_diffuse.png"),
 ("Humo","humo","humo",0,None),
 ("Mariposa","alaDer","mariposa_ala_der",0,"texture_diffuse.png"),
 ("Mariposa","alaIzq","mariposa_ala_izq",0,"texture_diffuse.png"),
 ("Mariposa","cuerpo","mariposa_cuerpo",0,"texture_diffuse.png"),
 ("Mazeta","mazeta","mazeta",8000,"texture_diffuse.png"),
 ("Refresco","refresco","refresco",0,"texture_diffuse.png"),
 ("PapelFucsia","papelFucsia","papel_fucsia",6000,None),
 ("PapelMorado","papelMorado","papel_morado",6000,None),
 ("PapelNaranja","papelNaranja","papel_naranja",6000,None),
]
def mtl_map(path):
    d={};cur=None
    for l in open(path):
        l=l.strip()
        if l.startswith("newmtl "): cur=l[7:]
        elif l.startswith("map_Kd") and cur: d[cur]=l.split()[-1]
    return d
for folder,obj,dest,tris,tex in JOBS:
    d=os.path.join(SRC,folder); o=os.path.join(d,obj+".obj")
    used=[l.split(None,1)[1].strip() for l in open(o) if l.startswith("usemtl")]
    mm=mtl_map(os.path.join(d,obj+".mtl"))
    maps={u:(tex if (mm[u]=="shaded.png" and tex) else mm[u]) for u in used}
    bpy.ops.wm.read_factory_settings(use_empty=True)
    bpy.ops.wm.obj_import(filepath=o)
    objs=[x for x in bpy.context.scene.objects if x.type=="MESH"]
    before=sum(len(x.data.polygons) for x in objs)
    # triangular todo (el conteo del .obj original son quads+tris)
    bpy.ops.object.select_all(action='DESELECT')
    for x in objs:
        bpy.context.view_layer.objects.active=x
        bpy.ops.object.modifier_add(type='TRIANGULATE'); bpy.ops.object.modifier_apply(modifier=x.modifiers[-1].name)
    tb=sum(len(x.data.polygons) for x in objs)
    if tris and tb>tris:
        r=tris/tb
        for x in objs:
            bpy.context.view_layer.objects.active=x
            m=x.modifiers.new("d","DECIMATE"); m.ratio=r; bpy.ops.object.modifier_apply(modifier="d")
    ta=sum(len(x.data.polygons) for x in objs)
    dz=os.path.join(OUT,dest); os.makedirs(dz,exist_ok=True)
    bpy.ops.object.select_all(action='SELECT')
    bpy.ops.wm.obj_export(filepath=os.path.join(dz,dest+".obj"),export_selected_objects=True,
        export_materials=False,export_normals=True,export_uv=True,apply_modifiers=True,
        forward_axis='NEGATIVE_Z',up_axis='Y')
    # .obj: cambia mtllib; .mtl: solo materiales usados con su textura 1K
    p=os.path.join(dz,dest+".obj"); t=open(p).read()
    t=re.sub(r"^mtllib .*$","",t,flags=re.M)
    t=re.sub(r"^(o .*)$",r"\1",t,count=0,flags=re.M)
    t="mtllib %s.mtl\nusemtl %s\n"%(dest,dest)+t
    open(p,"w").write(t)
    # materiales: una sola entrada por textura; el obj exportado sin materiales -> usamos un unico usemtl
    texs=sorted(set(maps.values()))
    mtl=[]
    for tx in texs:
        nm=os.path.splitext(tx)[0]
        out_png=os.path.join(dz,(dest+".png") if len(texs)==1 else nm+".png")
        im=Image.open(os.path.join(d,tx)).convert("RGBA")
        if im.width>1024: im=im.resize((1024,1024),Image.LANCZOS)
        im.save(out_png,optimize=True)
        mtl.append("newmtl %s\nKa 1 1 1\nKd 1 1 1\nKs 0 0 0\nd 1\nillum 1\nmap_Kd %s\n"%(nm if len(texs)>1 else dest, os.path.basename(out_png)))
    open(os.path.join(dz,dest+".mtl"),"w").write("\n".join(mtl))
    print(dest,"tris",before,"->",tb,"->",ta,"texs",texs,"size",[round(v,3) for v in max(objs,key=lambda x:len(x.data.vertices)).dimensions])
