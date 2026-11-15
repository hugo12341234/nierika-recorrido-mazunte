r"""X-42: redistribucion de la playa (pedido de D2, 15/11): casi todo estaba pegado a la orilla (z 15..24) o al pie de las colinas (z -20..-5)
y el centro de la playa (x +-12..+-45, z -4..+14) estaba vacio. Este script es una MIGRACION de una sola vez sobre src/VidaColinasDatos.h
(no se vuelve a correr x33_vida_colinas.py porque el archivo versionado ya no coincide con lo que esa semilla produce hoy: otras casas,
otros conteos). Mueve, sin agregar ni quitar nada (mismas llamadas de dibujo):
  - 2 campamentos del pie de las colinas (fogata + su gente) hacia las alas de la playa: (-16, -9) -> (-21, 2) y (18, -11) -> (22, 1)
  - 3 grupitos con vela que caminaban por z -5 hacia el centro: (-27, -5) -> (-13, -2), (29, -6) -> (13, -3), (-42, -5) -> (-24, -8)
  - la roca de playa que caia dentro de la palapa de (26, -17) pasa a (29.5, -13.5)
  - las 30 luciernagas de la franja norte (z -19..-3) se reparten en z -19..+14
Uso (raiz del repo):  python tools/x42_redistribuir_playa.py     (falla si ya se aplico: busca las posiciones originales)"""
import math, os, re, sys

RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
RUTA = os.path.join(RAIZ, "src", "VidaColinasDatos.h")
txt = open(RUTA, encoding="utf-8").read()
CRLF = "\r\n" in txt
txt = txt.replace("\r\n", "\n")

FORMATOS = {"FOGATAS": "%.2ff, %.3ff, %.2ff", "PERSONAS": "%.2ff, %.3ff, %.2ff, %.1ff, %d, %.2ff, %.2ff, %d", "LUCIERNAGAS": "%.2ff, %.2ff, %.2ff, %.3ff", "ROCAS": "%.2ff, %.3ff, %.2ff, %.1ff, %.2ff"}
TIPOS = {"FOGATAS": "Fuego", "PERSONAS": "Persona", "LUCIERNAGAS": "Lucier", "ROCAS": "Planta"}


def leer(nombre):
    m = re.search(r"    inline const %s %s\[(\d+)\] = \{\n(.*?)\n    \};\n" % (TIPOS[nombre], nombre), txt, re.S)
    filas = []
    for l in m.group(2).split("\n"):
        nums = re.findall(r"-?\d+\.?\d*f?", l.strip().strip("{},"))
        filas.append([int(n) if "." not in n and not n.endswith("f") else float(n.rstrip("f")) for n in nums])
    return m, filas


def escribir(nombre, filas):
    global txt
    m, _ = leer(nombre)
    cuerpo = ",\n".join("        { " + FORMATOS[nombre] % tuple(r) + " }" for r in filas)
    nuevo = "    inline const %s %s[%d] = {\n%s\n    };\n" % (TIPOS[nombre], nombre, len(filas), cuerpo)
    txt = txt[:m.start()] + nuevo + txt[m.end():]


_, fogatas = leer("FOGATAS")
_, personas = leer("PERSONAS")
_, luci = leer("LUCIERNAGAS")
_, rocas = leer("ROCAS")


def cerca(lista, x, z, tol):
    idx = [i for i, r in enumerate(lista) if abs(r[0] - x) < tol and abs(r[2] - z) < tol]
    return idx


# --- campamentos: la fogata y la gente que la rodea (a <= 3.5 m) se trasladan juntas ---
for (ox, oz), (nx, nz) in (((-16.0, -9.0), (-21.0, 2.0)), ((18.0, -11.0), (22.0, 1.0))):
    f = cerca(fogatas, ox, oz, 0.1)
    assert len(f) == 1, "no se encontro la fogata (%s, %s): ya se aplico?" % (ox, oz)
    fogatas[f[0]][0], fogatas[f[0]][2] = nx, nz
    gente = [i for i, r in enumerate(personas) if math.hypot(r[0] - ox, r[2] - oz) <= 3.5]
    assert len(gente) >= 4, (ox, oz, len(gente))
    for i in gente:       # traslacion rigida: conserva el giro (mira al fuego) y la distancia
        personas[i][0] += nx - ox
        personas[i][2] += nz - oz
    print("campamento", (ox, oz), "->", (nx, nz), "con", len(gente), "personas")

# --- grupitos con vela: 3 personas en fila (x, x+1.2, x+2.4) cerca de (cx, cz) ---
for (ox, oz), (nx, nz) in (((-27.0, -5.0), (-13.0, -2.0)), ((29.0, -6.0), (13.0, -3.0)), ((-42.0, -5.0), (-24.0, -8.0))):
    gente = [i for i, r in enumerate(personas) if ox - 0.1 <= r[0] <= ox + 2.5 and abs(r[2] - oz) <= 0.7 and r[7] == 1]
    assert len(gente) == 3, ("grupito", ox, oz, len(gente))
    for i in gente:
        personas[i][0] += nx - ox
        personas[i][2] += nz - oz
    print("grupito", (ox, oz), "->", (nx, nz))

# --- luciernagas de la franja norte: z de [-19, -3] a [-19, 14] ---
n = 0
for r in luci:
    if abs(r[0]) < 40 and -20 < r[2] < -2 and r[1] < 3.0:
        r[2] = -19.0 + (r[2] + 19.0) * (33.0 / 16.0)
        n += 1
print("luciernagas reubicadas:", n)

# --- una roca de playa caia en el centro de la palapa de (26, -17), pegada al banco (hallazgo H3 de X-41) ---
r = [i for i, f in enumerate(rocas) if abs(f[0] - 26.0) < 0.2 and abs(f[2] + 17.2) < 0.3]
assert len(r) == 1, "no se encontro la roca de la palapa"
rocas[r[0]][0], rocas[r[0]][2] = 29.5, -13.5
print("roca de la palapa movida a (29.5, -13.5)")

escribir("FOGATAS", fogatas)
escribir("ROCAS", rocas)
escribir("PERSONAS", personas)
escribir("LUCIERNAGAS", luci)
open(RUTA, "w", encoding="utf-8", newline="").write(txt.replace("\n", "\r\n") if CRLF else txt)
print("VidaColinasDatos.h actualizado")
