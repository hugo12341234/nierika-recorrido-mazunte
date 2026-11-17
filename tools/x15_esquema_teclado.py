"""X-15: esquema del teclado del manual de usuario (PNG 1280x720), dibujado con Pillow. Sin imagenes externas.
Uso (raiz del repo): python tools/x15_esquema_teclado.py"""
from PIL import Image, ImageDraw, ImageFont
W, H = 1280, 720
im = Image.new("RGB", (W, H), (22, 28, 44))
d = ImageDraw.Draw(im)
def fuente(t, b=False):
    for n in (("arialbd.ttf" if b else "arial.ttf"), "segoeui.ttf", "DejaVuSans.ttf"):
        try: return ImageFont.truetype(n, t)
        except OSError: pass
    return ImageFont.load_default()
F, FB, FS = fuente(28), fuente(28, True), fuente(20)
d.text((40, 28), "Controles — Recorrido virtual: Día de Muertos en Mazunte", font=fuente(34, True), fill=(240, 232, 210))
def tecla(x, y, w, h, txt, activa=True):
    d.rounded_rectangle((x, y, x + w, y + h), 12, fill=(238, 148, 40) if activa else (60, 68, 88), outline=(250, 235, 200), width=3)
    tw = d.textlength(txt, font=FB)
    d.text((x + (w - tw) / 2, y + h / 2 - 18), txt, font=FB, fill=(30, 20, 10) if activa else (170, 175, 190))
def nota(x, y, txt, color=(235, 235, 240)):
    d.text((x, y), txt, font=FS, fill=color)
S = 84
# Bloque WASD
x0, y0 = 90, 150
tecla(x0 + S + 8, y0, S, S, "W"); tecla(x0, y0 + S + 8, S, S, "A"); tecla(x0 + S + 8, y0 + S + 8, S, S, "S"); tecla(x0 + 2 * (S + 8), y0 + S + 8, S, S, "D")
nota(x0, y0 + 2 * S + 28, "W / S: avanzar / retroceder")
nota(x0, y0 + 2 * S + 56, "A / D: moverse a los lados")
# Flechas
x1, y1 = 520, 150
tecla(x1 + S + 8, y1, S, S, "↑"); tecla(x1, y1 + S + 8, S, S, "←"); tecla(x1 + S + 8, y1 + S + 8, S, S, "↓"); tecla(x1 + 2 * (S + 8), y1 + S + 8, S, S, "→")
nota(x1, y1 + 2 * S + 28, "Flechas: lo mismo que W A S D")
# Mouse
d.rounded_rectangle((930, 140, 1090, 330), 70, fill=(60, 68, 88), outline=(250, 235, 200), width=3)
d.line((930, 235, 1090, 235), fill=(250, 235, 200), width=3); d.line((1010, 140, 1010, 235), fill=(250, 235, 200), width=3)
d.polygon([(1010, 175), (1030, 190), (1010, 205), (990, 190)], fill=(238, 148, 40))
nota(900, 345, "Mover el ratón: mirar alrededor")
nota(900, 372, "(el cursor queda oculto; vista ±89°)")
# Otras teclas
y2 = 470
def tecla_nota(cx, w, txt, nt):
    tecla(cx - w // 2, y2, w, 70, txt)
    nota(cx - d.textlength(nt, font=FS) / 2, y2 + 82, nt)
tecla_nota(130, 90, "V", "Velas del altar (cerca)")
tecla_nota(370, 90, "M", "Sonido: mar / música")
tecla_nota(610, 90, "F1", "Ayuda de controles")
tecla_nota(850, 90, "R", "Reiniciar la caminata")
tecla_nota(1060, 90, "L", "Cubos de luces")
tecla_nota(1200, 90, "ESC", "Salir")
nota(40, 665, "Las fichas culturales aparecen solas al acercarte a menos de 2 m del objeto.", (200, 205, 220))
im.save("docs/manual_usuario/capturas/02_teclado_controles.png")
print("ok")
