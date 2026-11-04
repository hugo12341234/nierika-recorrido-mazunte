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

## Créditos (llenar al elegir cada archivo)

Cada pista debe quedar aquí con su referencia **APA 7** y se cita también en el manual y en los créditos finales.

| Pista | Autor | Título | Año | Sitio / URL | Licencia | Fecha de consulta |
|---|---|---|---|---|---|---|
| oleaje | *pendiente* | *pendiente* | *pendiente* | *pendiente* | *pendiente* | *pendiente* |
| musica | *pendiente* | *pendiente* | *pendiente* | *pendiente* | *pendiente* | *pendiente* |

Plantilla APA 7 para audio de un sitio web: Autor. (Año). *Título del audio* [Audio]. Sitio. URL

## Librería

miniaudio v0.11.26 (David Reid), dominio público / MIT-0, un solo archivo en `vendor/miniaudio/miniaudio.h`.
Fuente: <https://github.com/mackron/miniaudio> (etiqueta `0.11.26`).
