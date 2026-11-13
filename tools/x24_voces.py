"""X-24: genera Audio/voces.wav (murmullo de voces lejanas, 24 s en bucle) por SINTESIS: ruido filtrado en las bandas de las vocales
(formantes ~ 500, 1500 y 2500 Hz) y modulado por envolventes silabicas aleatorias (3-5 Hz). No usa grabaciones ni voces reales.
Uso (raiz del repo): python tools/x24_voces.py"""
import wave, numpy as np
sr = 22050; dur = 24.0; n = int(sr * dur)
r = np.random.default_rng(24)
def banda(ruido, f0, ancho):
    F = np.fft.rfft(ruido); fr = np.fft.rfftfreq(len(ruido), 1 / sr)
    F *= np.exp(-0.5 * ((fr - f0) / ancho) ** 2)
    return np.fft.irfft(F, len(ruido))
mezcla = np.zeros(n)
for k in range(6):                                        # 6 "hablantes" con formantes algo distintos
    base = r.standard_normal(n)
    v = sum(banda(base, f * (0.9 + 0.2 * r.random()), a) * g for f, a, g in ((500, 120, 1.0), (1500, 250, 0.6), (2500, 350, 0.3)))
    t = np.arange(n) / sr
    env = np.zeros(n)
    for j in range(40):                                   # frases: tramos de silabas con silencios
        t0 = r.random() * dur; ln = 0.8 + 2.2 * r.random(); f_sil = 3 + 2 * r.random()
        m = (t > t0) & (t < t0 + ln)
        env += m * (0.5 + 0.5 * np.sin(2 * np.pi * f_sil * (t - t0))) ** 2 * np.hanning(max(m.sum(), 2)).take(np.arange(m.sum()) if m.sum() > 1 else [0], mode='clip').mean() if False else m * (0.5 + 0.5 * np.sin(2 * np.pi * f_sil * (t - t0))) ** 2
    mezcla += v * np.clip(env, 0, 1)
mezcla = mezcla / np.max(np.abs(mezcla)) * 0.5
fade = int(sr * 1.0)                                      # bucle sin saltos: el final se funde con el principio
mezcla[:fade] = mezcla[:fade] * np.linspace(0, 1, fade) + mezcla[-fade:] * np.linspace(1, 0, fade)
mezcla = mezcla[:-fade]
with wave.open("Audio/voces.wav", "wb") as w:
    w.setnchannels(1); w.setsampwidth(2); w.setframerate(sr)
    w.writeframes((mezcla * 32767).astype(np.int16).tobytes())
print("Audio/voces.wav", round(len(mezcla) / sr, 1), "s")
