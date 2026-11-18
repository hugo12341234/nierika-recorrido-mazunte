# B-4 · Pan de muerto con detalle de huesos (act. 27). Modelo propio de Nierika Studio.
# Cupula de masa (media esfera aplastada, 0.14 m de diametro, 0.06 m de alto) + 4 "huesos" (tubos con nudillos en
# los extremos) que suben por la cupula en cruz + bolita central. Un Decimate (collapse, razon 0.6) deja pocos poligonos.
# Pivote en el centro de la base (y = 0). Textura 1K procedural (masa dorada con granos de azucar).
# Uso: python3 tools/b4_pan_muerto.py   (necesita bpy, Pillow y numpy)
import math, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import numpy as np
from PIL import Image
import bpy, bmesh
from mathutils import Matrix, Vector
import bpy_comun as c

RX, RZ = 0.07, 0.06      # radio en planta y alto de la cupula (m)
RAZON = 0.6              # razon del Decimate (colapso)
NOMBRE = "pan_muerto"

def textura(ruta, n=1024):
    rng = np.random.default_rng(4)
    y, x = np.mgrid[0:n, 0:n]
    u, v = (x / n) * 2 - 1, (y / n) * 2 - 1
    r = np.hypot(u, v)
    base = np.array([196, 128, 52], np.float32)
    tostado = np.array([140, 80, 28], np.float32)
    t = np.clip(r, 0, 1)[..., None]
    # manchas suaves de horneado (suma de cosenos) + grano
    mancha = 0.5 + 0.25 * np.cos(9 * u + 2) * np.cos(7 * v + 1) + 0.25 * np.cos(5 * (u + v) + 3)
    col = base * (1 - 0.35 * t) + tostado * (0.35 * t) * mancha[..., None]
    col += rng.normal(0, 5, (n, n, 1))
    azucar = rng.random((n, n)) > 0.996                                    # granitos de azucar
    col[azucar] = [245, 235, 215]
    Image.fromarray(np.clip(col, 0, 255).astype(np.uint8), "RGB").save(ruta)

def tubo(bm, puntos, radio, lados=6):
    """Tubo de seccion circular que sigue `puntos`; devuelve nada (se agrega a bm)."""
    anillos = []
    for i, p in enumerate(puntos):
        p = Vector(p)
        t = (Vector(puntos[min(i + 1, len(puntos) - 1)]) - Vector(puntos[max(i - 1, 0)])).normalized()
        a = t.cross(Vector((0, 0, 1)))
        if a.length < 1e-6:
            a = t.cross(Vector((1, 0, 0)))
        a.normalize()
        b = t.cross(a).normalized()
        anillos.append([bm.verts.new(p + radio * (math.cos(2 * math.pi * k / lados) * a + math.sin(2 * math.pi * k / lados) * b)) for k in range(lados)])
    for i in range(len(anillos) - 1):
        for k in range(lados):
            bm.faces.new([anillos[i][k], anillos[i][(k + 1) % lados], anillos[i + 1][(k + 1) % lados], anillos[i + 1][k]])

def esfera(bm, centro, radio, seg=8, ani=5):
    m = Matrix.Translation(centro) @ Matrix.Scale(radio, 4)
    bmesh.ops.create_uvsphere(bm, u_segments=seg, v_segments=ani, radius=1.0, matrix=m)

carpeta = os.path.join(c.RAIZ, "Models", NOMBRE)
os.makedirs(carpeta, exist_ok=True)
png = os.path.join(carpeta, NOMBRE + ".png")
textura(png)

c.limpiar()
malla = bpy.data.meshes.new(NOMBRE)
obj = bpy.data.objects.new(NOMBRE, malla)
bpy.context.collection.objects.link(obj)
bm = bmesh.new()

# 1) cupula: esfera aplastada y cortada en z = 0 (se queda la mitad de arriba) con la base cerrada
bmesh.ops.create_uvsphere(bm, u_segments=20, v_segments=10, radius=1.0, matrix=Matrix.Diagonal((RX, RX, RZ, 1.0)))
corte = bmesh.ops.bisect_plane(bm, geom=bm.verts[:] + bm.edges[:] + bm.faces[:], plane_co=(0, 0, 0), plane_no=(0, 0, 1),
                               clear_inner=True, clear_outer=False)
borde = [e for e in corte["geom_cut"] if isinstance(e, bmesh.types.BMEdge)]
bmesh.ops.contextual_create(bm, geom=borde)                                   # tapa de la base
bmesh.ops.recalc_face_normals(bm, faces=bm.faces[:])

# 2) cuatro huesos: arco sobre la cupula desde la elevacion 15 grados hasta 70 grados, 0.006 m por encima
def sup(az, el, extra=0.006):
    x, y, z = RX * math.cos(el) * math.cos(az), RX * math.cos(el) * math.sin(az), RZ * math.sin(el)
    n = Vector((x / RX**2, y / RX**2, z / RZ**2)).normalized()
    return Vector((x, y, z)) + extra * n

for az in (0, math.pi / 2, math.pi, 3 * math.pi / 2):
    pts = [sup(az, math.radians(e)) for e in (15, 30, 45, 60, 72)]
    tubo(bm, pts, 0.0055)
    esfera(bm, pts[0], 0.0085)             # nudillo exterior
    esfera(bm, pts[-1], 0.0085)            # nudillo interior (cerca de la bolita)
# 3) bolita central (la "lagrima" de la tradicion)
esfera(bm, Vector((0, 0, RZ + 0.004)), 0.014, seg=10, ani=6)

# UV: proyeccion desde arriba (x, y) -> 0..1; la textura es de masa uniforme, sirve para cupula y huesos
uv = bm.loops.layers.uv.verify()
for f in bm.faces:
    for l in f.loops:
        p = l.vert.co
        l[uv].uv = (p.x / (2 * RX) + 0.5, p.y / (2 * RX) + 0.5)
bm.to_mesh(malla); bm.free()
for p in malla.polygons:
    p.use_smooth = True
malla.materials.append(c.material(NOMBRE, png))

antes = len(malla.polygons)
mod = obj.modifiers.new("Decimate", "DECIMATE")
mod.decimate_type = "COLLAPSE"
mod.ratio = RAZON
bpy.context.view_layer.objects.active = obj
dg = bpy.context.evaluated_depsgraph_get()
tris = sum(len(p.vertices) - 2 for p in obj.evaluated_get(dg).data.polygons)
c.exportar(obj, NOMBRE)
bb = [Vector(v) for v in obj.bound_box]
print(f"{NOMBRE}: caras antes={antes}, triangulos despues del Decimate={tris}; "
      f"caja x {min(v.x for v in bb):.3f}..{max(v.x for v in bb):.3f} z {min(v.z for v in bb):.3f}..{max(v.z for v in bb):.3f}")
