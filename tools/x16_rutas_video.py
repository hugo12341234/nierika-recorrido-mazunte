"""X-16: genera las rutas de camara de los 3 videos borrador (tools/video_rutas/*.txt) para el modo video de src/main.cpp.
Formato: 't x y z yaw pitch' por linea (interpolacion lineal) y 'silueta t0'. Coordenadas del juego (1 u = 1 m; yaw -90 mira a -z, yaw 90 mira a +z).
Uso: python tools/x16_rutas_video.py"""
import math, os
D = os.path.join(os.path.dirname(os.path.abspath(__file__)), "video_rutas"); os.makedirs(D, exist_ok=True)

def esc(nombre, puntos, silueta=0.0):
    with open(os.path.join(D, nombre), "w") as f:
        f.write(f"silueta {silueta}\n")
        for p in puntos: f.write(" ".join(f"{v:.3f}" for v in p) + "\n")

def zsil(t):  # z del torso de la silueta (ver Silueta.cpp: rampas de 2 s, crucero a 1.4 m/s)
    d = 1.4 * 2 / 1.5
    if t <= 2: u = t / 2; return 24 - d * (3 * u * u - 2 * u ** 3) if False else 24 - d * (u * u * (3 - 2 * u))
    if t <= 16.333: return 24 - d - 1.4 * (t - 2)
    u = min((t - 16.333) / 2, 1); return 24 - d - 1.4 * 14.333 - d * (1 - (1 - u) ** 2 * (3 - 2 * (1 - u)))

# Video 1 - entorno (65 s): A inicio 10 s, B playa 20 s, C panteon 15 s, D cuarto 20 s
esc("video1_entorno.txt", [
    (0, 0, 1.6, 6, -90, 0), (10, 0, 1.6, 6, -90, 0),                  # A: punto de inicio (ficha del papel picado)
    (14, 0, 1.6, 6, 90, -2), (28, 0, 1.6, 21, 90, -4), (30, 0, 1.6, 21, 90, -4),   # B: girar 180 y caminar a la orilla
    (35, 0, 1.6, 21, -90, -2), (45, 0, 1.6, 15, -90, -3),               # C: volver a mirar el panteon
    (57, 0, 1.6, 2.2, -90, -3), (61, -0.6, 1.5, 0.3, -10, -10), (65, -0.6, 1.5, 0.3, 0, -12),  # D: puerta, cuarto, altar
])
# Video 2 - recorrido (75 s): de la orilla al altar con las fichas
esc("video2_recorrido.txt", [
    (0, 0, 1.6, 23, -90, -2), (12, 0, 1.6, 13.5, -90, -2),           # orilla -> puerta norte de la barda
    (24, 0, 1.6, 6, -90, -3), (28, 3.5, 1.6, 6, -150, -6),            # panteon: papel picado y tumbas
    (36, -3.5, 1.6, 6, -30, -6), (42, 0, 1.6, 6, -90, -2),
    (52, 0, 1.6, 2.2, -90, -3), (56, -0.6, 1.5, 0.3, -10, -10),       # entrada al cuarto
    (66, 0.4, 1.5, 0.7, 0, -12), (75, 0.4, 1.5, 0.7, 0, -12),         # altar con su ficha (< 2 m)
])
# Video 3 - animaciones (30 s): caminata de lado (0-17 s), dejar flores desde el cuarto (17-30 s). Silueta desde t = 0
lado = [(t, -3.2, 0.95, zsil(t), 0, 0) for t in range(0, 17)]
esc("video3_animaciones.txt", lado + [(17.0, -1.4, 1.2, 0.9, -20, -12), (30, -1.4, 1.2, 0.9, -20, -12)])
print("rutas generadas en", D)
