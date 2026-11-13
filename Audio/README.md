# Audio/ — sonido ambiental (tarea D-6)

El código (`src/Audio.h/.cpp`, miniaudio) busca **dos pistas** aquí, con estos nombres exactos
(sin importar si es `.wav`, `.mp3` o `.flac`; se prueban en ese orden). Si falta una, el programa
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
| `oleaje.wav` | Oleaje | 0.20 + 0.45·s, con s = clamp((z − 2)/22, 0, 1): más fuerte hacia el mar |
| `musica.wav` | Música lejana | 0.10 + 0.40·clamp(1 − d/10, 0, 1), con d la distancia al altar (1.35, 0): más fuerte cerca del altar |
| `voces.wav` | Murmullo de voces lejanas | 0.55·clamp(1 − d/14, 0, 1), con d la distancia a la ceremonia (11, 19.5) o al panteón (0, 7): suena cerca de la gente |

`voces.wav` es **propia**: la genera `tools/x24_voces.py` por síntesis (ruido filtrado en las bandas de las vocales con envolventes silábicas; no usa grabaciones ni voces reales).
