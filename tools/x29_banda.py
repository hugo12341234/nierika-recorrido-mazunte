"""X-29: genera Audio/banda.wav (musica en vivo de los musicos de la playa, 32 s en bucle) por SINTESIS: guitarra pulsada (Karplus-Strong),
tambor (golpe grave con ruido) y trompeta (diente de sierra filtrado con vibrato). NO son grabaciones ni musicos reales; la melodia es propia del estudio.
Uso (raiz del repo): python tools/x29_banda.py"""
import wave, numpy as np
sr = 22050; bpm = 90; beat = 60.0 / bpm; compases = 16; dur = compases * 4 * beat
n = int(sr * dur)
r = np.random.default_rng(29)
out = np.zeros(n + sr)
def nota(m): return 440.0 * 2 ** ((m - 69) / 12)
def ks(f, largo, amp=0.5):                     # cuerda pulsada: Karplus-Strong
    N = int(sr / f); buf = r.uniform(-1, 1, N); res = np.zeros(int(sr * largo))
    for i in range(len(res)):
        res[i] = buf[i % N]; buf[i % N] = 0.5 * (buf[i % N] + buf[(i + 1) % N]) * 0.996
    return res * amp
def pon(sig, t0):
    i = int(t0 * sr); out[i:i + len(sig)] += sig[:len(out) - i]
acordes = [(57, 60, 64), (55, 59, 62), (53, 57, 60), (52, 56, 59)]       # La menor, Sol, Fa, Mi
for c in range(compases):
    ac = acordes[c % 4]
    for b in range(4):
        t0 = (c * 4 + b) * beat
        for k, m in enumerate(ac):                                       # rasgueo: las cuerdas entran con 15 ms de retraso
            pon(ks(nota(m), 1.2 * beat, 0.16), t0 + 0.015 * k)
        pon(ks(nota(ac[0] - 12), 1.5 * beat, 0.2), t0) if b % 2 == 0 else None
        # tambor: golpe grave en los tiempos 1 y 3, golpe seco en 2 y 4
        t = np.arange(int(0.25 * sr)) / sr
        f0 = 90 if b % 2 == 0 else 160
        golpe = np.sin(2 * np.pi * (f0 * np.exp(-t * 8)) * t) * np.exp(-t * 14) * 0.5 + r.standard_normal(len(t)) * np.exp(-t * 40) * (0.12 if b % 2 else 0.04)
        pon(golpe, t0)
# trompeta: frase de 4 compases que se repite (diente de sierra con vibrato y envolvente)
frase = [(69, 1), (72, 1), (71, 2), (67, 1), (71, 1), (69, 2), (65, 1), (69, 1), (67, 2), (64, 2), (66, 1), (68, 1)]
t0 = 0.0
for rep in range(4):
    t = 0.0
    for m, d in frase:
        ln = d * beat
        tt = np.arange(int(ln * sr)) / sr
        f = nota(m) * (1 + 0.006 * np.sin(2 * np.pi * 5.5 * tt))
        ph = np.cumsum(f) / sr
        saw = 2 * (ph % 1) - 1
        env = np.minimum(tt / 0.04, 1) * np.exp(-tt * 0.8) * np.minimum((ln - tt) / 0.05, 1)
        suave = np.convolve(saw, np.ones(6) / 6, mode='same')
        pon(suave * env * 0.14, t0 + t); t += ln
    t0 += sum(d for _, d in frase) * beat
out = out[:n]
fade = int(0.5 * sr); out[:fade] = out[:fade] * np.linspace(0, 1, fade) + out[-fade:] * np.linspace(1, 0, fade); out = out[:-fade]
out = out / np.max(np.abs(out)) * 0.7
with wave.open("Audio/banda.wav", "wb") as w:
    w.setnchannels(1); w.setsampwidth(2); w.setframerate(sr); w.writeframes((out * 32767).astype(np.int16).tobytes())
print("Audio/banda.wav", round(len(out) / sr, 1), "s")
