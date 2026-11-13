# D-6 · Audio ambiental propio de Nierika Studio (oleaje + música lejana), generado con código.
#
# Uso (desde la raíz del repo):  python3 tools/d6_audio.py
# Requiere: Python 3 y numpy. Escribe Audio/oleaje.wav y Audio/musica.wav (mono, 22 050 Hz, 16 bits).
#
# Por qué se genera con código y no se descarga:
#  - es material PROPIO del estudio (sin licencias que citar ni riesgo de CC BY-NC en un repositorio público);
#  - los dos archivos son periódicos por construcción: el último muestreo enlaza con el primero sin corte,
#    que es lo que necesita ma_sound_set_looping (repite el archivo tal cual).
# Todo es determinista (semilla fija): volver a correr el script da los mismos archivos.
import os
import wave
import numpy as np

FS = 22050                       # Hz
RAIZ = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
SALIDA = os.path.join(RAIZ, "Audio")


def guardar(nombre, x):
    """x en [-1, 1] -> WAV mono de 16 bits."""
    x = np.clip(x, -1.0, 1.0)
    datos = (x * 32767.0).astype("<i2")
    with wave.open(os.path.join(SALIDA, nombre), "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(FS)
        w.writeframes(datos.tobytes())


def ruido_periodico(n, rng, f_bajo, f_alto, pendiente):
    """Ruido de espectro  1/f^pendiente  entre f_bajo y f_alto (Hz), PERIÓDICO en n muestras (se arma en frecuencia)."""
    f = np.fft.rfftfreq(n, 1.0 / FS)
    amp = np.zeros_like(f)
    banda = (f >= f_bajo) & (f <= f_alto)
    amp[banda] = 1.0 / np.power(f[banda], pendiente)
    fase = rng.uniform(0.0, 2.0 * np.pi, len(f))
    x = np.fft.irfft(amp * np.exp(1j * fase), n)
    return x / np.max(np.abs(x))


def oleaje(segundos=40):
    """Mar: retumbo grave (ruido café) + espuma (ruido agudo) con la misma envolvente periódica de olas."""
    rng = np.random.default_rng(15)
    n = FS * segundos
    t = np.arange(n) / FS
    # Envolvente: suma de senos cuyos periodos dividen a `segundos` (8 s, 10 s y 20 s) -> periódica
    e = 0.50 + 0.30 * np.sin(2 * np.pi * t / 8.0) + 0.15 * np.sin(2 * np.pi * t / 10.0 + 1.0) + 0.05 * np.sin(2 * np.pi * t / 20.0 + 2.0)
    e = np.clip(e, 0.05, 1.0)
    grave = ruido_periodico(n, rng, 30, 900, 1.0)        # ~ ruido rosa/café: el rumor del agua
    espuma = ruido_periodico(n, rng, 1500, 7000, 0.3)    # casi blanco, solo agudos: la rompiente
    x = 0.55 * grave * e + 0.30 * espuma * (e ** 3)
    return 0.8 * x / np.max(np.abs(x))


def musica(segundos=48):
    """Música lejana: acordes de Am - F - C - G (12 s cada uno, pad suave) con 4 notas de campana de la pentatónica menor
    de La. Toda la mezcla pasa por un eco circular, así que su cola se enrolla con el inicio (sin corte al repetir)."""
    rng = np.random.default_rng(16)
    n = FS * segundos
    t = np.arange(n) / FS
    nota = lambda m: 440.0 * 2.0 ** ((m - 69) / 12.0)       # número MIDI -> Hz
    paso = 12.0
    acordes = [(57, 60, 64), (53, 57, 60), (48, 52, 55), (55, 59, 62)]   # MIDI: Am = A3-C4-E4, F = F3-A3-C4, C = C3-E3-G3, G = G3-B3-D4
    pad = np.zeros(n)
    for i, ac in enumerate(acordes):
        t0 = i * paso
        # Ventana periódica (seno^2) de 12 s con solape del 100 %: cada acorde crece y se apaga sin saltos
        u = (t - t0) / paso
        ventana = np.where((u >= 0) & (u < 1), np.sin(np.pi * u) ** 2, 0.0)
        for m in ac:
            f = nota(m)
            # tres osciladores ligeramente desafinados por nota: movimiento lento ("coro")
            for desf in (-0.9, 0.0, 0.9):
                pad += ventana * np.sin(2 * np.pi * (f + desf * 0.4) * t + rng.uniform(0, 2 * np.pi)) / 9.0
    # Campanas: seno con armónico y decaimiento exponencial (0.9 s) en tiempos fijos
    campanas = np.zeros(n)
    notas_campana = [(3.0, 76), (9.5, 72), (16.0, 79), (21.0, 76), (28.0, 74), (33.5, 72), (40.0, 79), (44.0, 76)]
    for tc, m in notas_campana:
        i0 = int(tc * FS)
        dur = int(3.0 * FS)
        tt = np.arange(dur) / FS
        env = np.exp(-tt / 0.9) * (1 - np.exp(-tt / 0.01))
        f = nota(m)
        s = (np.sin(2 * np.pi * f * tt) + 0.3 * np.sin(2 * np.pi * 2.76 * f * tt)) * env
        campanas[i0:i0 + dur] += 0.25 * s[: n - i0]
    seco = pad + campanas
    # Eco circular: convolución en frecuencia = el final se mezcla con el inicio
    ir = np.zeros(n)
    for k, g in enumerate([1.0, 0.45, 0.25, 0.12]):
        ir[int(k * 0.42 * FS)] = g
    h = np.fft.rfft(ir)
    x = np.fft.irfft(np.fft.rfft(seco) * h, n)
    # Filtro suave: quita agudos > ~3 kHz (suena "lejana")
    f = np.fft.rfftfreq(n, 1.0 / FS)
    x = np.fft.irfft(np.fft.rfft(x) / (1.0 + (f / 3000.0) ** 4), n)
    return 0.8 * x / np.max(np.abs(x))


if __name__ == "__main__":
    os.makedirs(SALIDA, exist_ok=True)
    guardar("oleaje.wav", oleaje())
    guardar("musica.wav", musica())
    for nom in ("oleaje.wav", "musica.wav"):
        ruta = os.path.join(SALIDA, nom)
        print(f"{nom}: {os.path.getsize(ruta) / 1e6:.2f} MB")
