# Audio/ — sonido ambiental (tarea D-6)

El código (`src/Audio.h/.cpp`, miniaudio) busca **dos pistas** aquí, con estos nombres exactos
(sin importar si es `.wav`, `.mp3` o `.flac`; **desde D-6e se prueban en el orden `.mp3`, `.wav`, `.flac`**). Si falta una, el programa
lo avisa en la consola y sigue sin ella. No hace falta recompilar al añadirlas.

| Archivo | Qué debe sonar | Volumen en el código |
|---|---|---|
| `oleaje.wav` / `.mp3` / `.flac` | Oleaje de mar, sin voces ni música | 0.60 |
| `musica.wav` / `.mp3` / `.flac` | Música lejana tranquila (de fondo, sin letra) | 0.25 |

Ambas suenan **en bucle**. Conviene que el final enlace con el inicio sin silencio ni corte brusco
(`ma_sound_set_looping` repite el archivo tal cual). `.ogg` **no** está soportado (necesitaría otra librería).

## Reglas para elegir los archivos

- Licencia que permita uso y redistribución en un proyecto escolar: **CC0** o **CC BY** (con crédito).
  Evitar CC BY-NC si el repositorio se hace público; no usar música comercial.
- Peso razonable: el repositorio ya es grande. Sugerencia (valor propuesto, no medido): oleaje ≤ 3 MB y música ≤ 5 MB
  (p. ej. `.mp3` de 30–60 s a 128 kbps).
- La música lejana va en el **ESTADO §4** como "externo": no cuenta como modelo propio ni como contenido original del estudio.

## Créditos (D-6, 12/11)

Las dos pistas son **propias del estudio**: las genera `tools/d6_audio.py` (Python + numpy, semilla fija; correrlo de nuevo da los mismos archivos).
No hay licencia de terceros que citar. Si algún día se cambian por archivos externos, cada uno debe quedar aquí con su referencia **APA 7**.

| Pista | Autor | Título | Año | Sitio / URL | Licencia | Fecha de consulta |
|---|---|---|---|---|---|---|
| oleaje | Nierika Studio (Equipo 15) | Oleaje (síntesis de ruido con envolvente de olas de 8, 10 y 20 s; 40 s en bucle) | 2026 | `tools/d6_audio.py` en el repositorio | Propia | — |
| musica | Nierika Studio (Equipo 15) | Música lejana (pad Am–F–C–G y notas de campana de la pentatónica menor de La; 48 s en bucle) | 2026 | `tools/d6_audio.py` en el repositorio | Propia | — |

Peso: `oleaje.wav` 1.76 MB y `musica.wav` 2.12 MB (WAV mono, 22 050 Hz, 16 bits), dentro de lo sugerido arriba.

## X-24: tercera pista y volumen por zona

| Archivo | Qué suena | Volumen (según la posición del visitante) |
|---|---|---|
| `oleaje.mp3` | Oleaje | 0.015 + 0.075·s^2.5, con s = clamp(1 − max(0, 24 − z)/24, 0, 1) (D-6g, 14/11; antes 0.03 + 0.15·s^2.5): 0.015 lejos, 0.09 en la orilla (z = 24) |
| `musica.mp3` | Música de fondo | 0.48·(0.10 + 0.40·clamp(1 − d/10, 0, 1)), con d la distancia al altar (1.35, 0): más fuerte cerca del altar |
| `voces.wav` | Murmullo de voces lejanas | 0.55·clamp(1 − d/14, 0, 1), con d la distancia a la ceremonia (11, 19.5) o al panteón (0, 7): suena cerca de la gente |

`voces.wav` es **propia**: la genera `tools/x24_voces.py` por síntesis (ruido filtrado en las bandas de las vocales con envolventes silábicas; no usa grabaciones ni voces reales).

## D-6b (14/11): música de fondo `musica.mp3`

`Audio/musica.mp3` (4 min 20 s, 2.3 MB, estéreo 48 kHz) sustituye a la pista sintetizada (`musica.wav` se quitó del repo; `tools/d6_audio.py` la sigue generando). Empieza y termina en silencio, así que el bucle empalma sin clic.

**Es externa y no es de Mazunte ni de Oaxaca:** es un arreglo de «Xochipitzahuatl (Flor Menudita)», canción tradicional náhuatl del centro de México. Uso **educativo no comercial, sin licencia libre**, aportado por D2.

| Pista | Autor / intérprete | Título | Año | Sitio / URL | Licencia | Consulta |
|---|---|---|---|---|---|---|
| musica | **PENDIENTE (D2)** | Xochipitzahuatl (Flor Menudita) | **PENDIENTE (D2)** | **PENDIENTE: canal y URL (D2)** | Sin licencia libre; uso educativo no comercial | **PENDIENTE (D2)** |

Mientras falten esos campos, el crédito APA 7 no está completo.

Volumen: el mp3 mide −10.0 LUFS (medido con ffmpeg `loudnorm`) y la pista anterior −16.4, por eso la música lleva el factor 0.48 (= 10^(−6.4/20)).

## D-6c (14/11): oleaje nuevo y se quita la banda

- `oleaje.wav` se **regeneró** con `tools/d6_audio.py` (48 s, 2.1 MB, mismo nombre y mismo bucle sin corte): ahora son 7 olas individuales e irregulares (marejada grave + rompiente + espuma que se apaga despacio) en vez de ruido con envolvente suave, que sonaba a motor. Propia del estudio, sin licencia de terceros. RMS ≈ 0.106 contra 0.114 de la anterior, así que la curva de volumen no cambia. **No escuchada por Claude** (solo se midió el empalme y el nivel): se ajusta al oído de D2.
- Se **quitó la pista `banda`** (`Audio/banda.wav` y `tools/x29_banda.py`): la música en vivo sintetizada de los músicos de la playa se encimaba con el oleaje y con la música nueva. Los músicos siguen en pantalla, sin sonido propio.

## D-6d (14/11): oleaje = grabación real de D2 (externa)

`Audio/oleaje.mp3` sustituye al oleaje sintetizado (`oleaje.wav` se quitó del repo; desde D-6e el código ya prueba `.mp3` primero, pero conviene que no quede ningún `.wav` viejo en la carpeta). `tools/d6_audio.py` **ya no se usa para el oleaje** (si se corre, recrea `Audio/oleaje.wav` y ese archivo tendría prioridad: no correrlo o borrar el resultado).

- **Origen:** pista de un video de YouTube que D2 aportó como `oleaje_hugo.mp3` (10 min 06 s, 8.7 MB, estéreo 48 kHz, −26.0 LUFS). **Externa, uso educativo no comercial, sin licencia libre confirmada** (misma decisión de D2 que la música).
- **Qué se hizo:** tramo de 123 s (de 2:00 a 4:03; niveles estables entre −24.5 y −28.8 dB por 10 s), ganancia +5.5 dB con limitador a 0.89 y **bucle de 120 s con fundido cruzado de 3 s de potencia constante** (el final se mezcla con el inicio: salto en el empalme 0.008 contra 0.005 de diferencia típica entre muestras). Resultado: **1.92 MB**, estéreo 48 kHz, mp3 128 kb/s, **−21.2 LUFS** (la pista sintetizada anterior medía −20.5, por eso la curva de volumen no cambia). El mp3 puede dejar un hueco de milisegundos al repetir (retardo del codificador); no medido.
- **No escuchado por Claude:** el tramo se eligió por estabilidad de nivel, no al oído; si trae voces, aplausos o saltos, D2 dice el minuto y se cambia el tramo.

| Pista | Autor / canal | Título | Año | Sitio / URL | Licencia | Consulta |
|---|---|---|---|---|---|---|
| oleaje | **PENDIENTE (D2): canal de YouTube** | Relajarse Relax - Sonido Olas del Mar - Playa Atardecer HD [Video] | **PENDIENTE (D2)** | YouTube, https://www.youtube.com/watch?v=6TGHwQHdDlo | Sin licencia libre confirmada; uso educativo no comercial | **PENDIENTE (D2)** |

Referencia APA 7 (incompleta hasta tener el canal y el año): *Canal desconocido.* (Año). *Relajarse Relax - Sonido Olas del Mar - Playa Atardecer HD* [Video]. YouTube. https://www.youtube.com/watch?v=6TGHwQHdDlo

## D-6e (14/11): único sonido de mar

En `develop` ya no queda ningún otro audio de mar: solo `Audio/oleaje.mp3`. Si en una PC se sigue oyendo el oleaje sintético, es un `Audio/oleaje.wav` viejo que quedó en esa carpeta (no está en el repo): borrarlo. El código ahora prefiere el `.mp3` de todos modos. La consola imprime `[Audio] Suena en bucle: Audio/oleaje.mp3` al arrancar.

## D-6f (14/11): oleaje.mp3 recodificado con más calidad (D2: «se oye muy saturada el agua»)

- **Medido en el `oleaje.mp3` anterior (128 kb/s):** sin clipping (pico −1.8 dBFS, ninguna muestra ≥ 0.85, cresta 22.1 dB igual que la del original: sin compresión). Pero al compararlo con el original alineado, el error de codificación es de solo **19–20 dB bajo la señal** (SNR): en un ruido de olas eso puede oírse granulado o «saturado».
- **Cambio:** mismo tramo (empieza en 123 s del original, 120 s en bucle con fundido cruzado de 3 s de potencia constante) y misma ganancia (+5.1 dB, sin limitador; pico −1.6 dBFS), pero codificado con LAME **VBR -q:a 0** (antes 128 kb/s). SNR contra la señal sin codificar: **28.4 dB** (antes 20.6). Peso: **2.76 MB** (antes 1.92 MB; un poco por debajo de los 3 MB sugeridos).
- **La curva de volumen y el nivel (−21 LUFS) no cambian.** **No escuchado por Claude**: si sigue sonando mal, la causa puede estar en la grabación original de YouTube (ya viene comprimida) y no en este archivo.

## D-6g (14/11): oleaje 6 dB más bajo

D2: «bájale aún más al oleaje, se oye demasiado». Se multiplicó toda la curva del mar por 0.5 (−6 dB): de 0.03 + 0.15·s^2.5 a **0.015 + 0.075·s^2.5** (0.015 lejos, 0.09 en la orilla). El archivo `oleaje.mp3` no cambia. Valor propuesto por Claude a partir de «más bajo»; **no escuchado**: D2 lo ajusta al oído (cambiar los dos números de `Audio::ActualizarZona` en `src/Audio.cpp`).
