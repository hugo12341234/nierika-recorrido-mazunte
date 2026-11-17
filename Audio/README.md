# Audio/ — sonido ambiental (tarea D-6)

Esta carpeta contiene **solo dos pistas con nombre fijo**, ambas `.mp3`, ambas en bucle:

| Archivo | Qué suena | Volumen en el código (`src/Audio.cpp`, `ActualizarZona`, cada cuadro) |
|---|---|---|
| `oleaje.mp3` | Mar | 0.02·15^s, con s = clamp(1 − max(0, 24 − z)/24, 0, 1) (D-6j): 0.020 en z ≤ 0, 0.039 en z = 6, 0.078 en z = 12, 0.153 en z = 18, 0.30 en la orilla (z = 24) |
| `musica.mp3` | Música de la banda (X-40): «Dios Nunca Muere», Banda Mixe de Oaxaca (D-6k) | 0.24·3/max(d, 3)·(1 − clamp((d − 30)/30, 0, 1)), d = distancia al centro de la banda (−12.0, 18.0): 0.24 a ≤ 3 m, 0.12 a 6 m, 0.06 a 12 m, 0 a 60 m. Además paneo estéreo hacia el lado donde está la banda |

El código busca únicamente `Audio/oleaje.mp3` y `Audio/musica.mp3` (rutas relativas a la raíz del repo; **no carga `.wav` ni `.flac`**, así un archivo viejo olvidado no tapa a los mp3).
Si falta una pista, la consola avisa (`[Audio] Falta Audio/...`) y el programa sigue sin ella. No hace falta recompilar para cambiar un mp3. `.ogg` no está soportado.

**Comprobar el volumen:** con la variable de entorno `NIERIKA_DEBUG_AUDIO=1` la consola imprime cada segundo `[Audio] x=… z=… | oleaje vol=… | musica vol=…` (el valor que tiene el motor, leído con `ma_sound_get_volume`). Al caminar hacia el mar `oleaje vol` debe subir de 0.02 a 0.30. Sin la variable no imprime nada.

## Medidas (ffmpeg, 14/11) — no escuchadas por Claude

| Pista | Duración | Peso | Formato | Sonoridad | Pico |
|---|---|---|---|---|---|
| `oleaje.mp3` | 2 min 00 s | 2.85 MB | mp3 VBR `-q:a 0` (≈190 kb/s), estéreo 48 kHz | −23.0 LUFS | −3.6 dBFS |
| `musica.mp3` | 4 min 20 s | 2.34 MB | mp3 original sin recodificar (71 kb/s), estéreo 48 kHz | −10.0 LUFS | +0.2 dBFS (así viene de origen; con volumen ≤ 0.24 en el juego no satura) |

**Importante sobre el balance:** con la curva actual el oleaje queda unos 20 dB por debajo de la música (oleaje −23.0 LUFS × 0.02…0.30 ≈ −57…−36 LUFS; música −10.0 × 0.048…0.24 ≈ −36…−22 LUFS). La música tapa al mar, por eso el cambio de volumen del mar casi no se nota. Subirlo es una decisión de D2 (valor a registrar en el ESTADO §5).

## Origen y créditos (externos: uso educativo no comercial, sin licencia libre confirmada, decisión de D2)

Ninguna de las dos es propia del estudio ni de Mazunte/Oaxaca; no cuentan como modelo propio.

### `oleaje.mp3`
- **Original:** «Relajarse Relax - Sonido Olas del Mar - Playa Atardecer HD» [Video], YouTube, https://www.youtube.com/watch?v=6TGHwQHdDlo (10 min 06 s, −26.0 LUFS, pico −3.4 dBFS).
- **Qué se hizo (reproducible con ffmpeg):** tramo de 123 s (de 2:00 a 4:03, niveles estables), **ganancia lineal de +3.2 dB sin limitador** (el pico del tramo era −6.7 dBFS; queda en −3.6 tras el mp3) y **bucle de 120 s con fundido cruzado de 3 s de potencia constante** (los últimos 3 s del tramo se mezclan con el inicio, así el final enlaza con el principio; salto en el empalme 0.0008 contra 0.0068 de paso típico entre muestras). El mp3 puede dejar un hueco de milisegundos al repetir (retardo del codificador); no medido.
- **No escuchado por Claude:** el tramo se eligió por nivel estable; si trae voces, aplausos o saltos, D2 dice el minuto y se cambia.
- **Referencia APA 7 (incompleta):** *[Canal — PENDIENTE (D2)].* ([Año — PENDIENTE (D2)]). *Relajarse Relax - Sonido Olas del Mar - Playa Atardecer HD* [Video]. YouTube. https://www.youtube.com/watch?v=6TGHwQHdDlo (fecha de consulta: PENDIENTE (D2)).

### `musica.mp3` (D-6k, 17/11: cambio de pista, decisión de D2)
- **Pista vigente en el código:** «Dios Nunca Muere», interpretada por la Banda Mixe de Oaxaca (según D2; es música de banda oaxaqueña, a diferencia de la pista anterior). El archivo conserva el nombre fijo `Audio/musica.mp3`: el código no cambia.
- **ESTADO DEL ARCHIVO:** el mp3 lo aporta D2. Mientras no se reemplace, `Audio/musica.mp3` sigue siendo la pista anterior (Xochipitzahuatl, náhuatl, sacada del juego en cuanto D2 suba la nueva). Al reemplazarlo: medir sonoridad con ffmpeg (`ffmpeg -i Audio/musica.mp3 -af ebur128 -f null -`) y, si difiere mucho de −10 LUFS, ajustar V0 en `Audio.cpp` (registrar el valor en el ESTADO §5); comprobar que empieza y termina sin clic al repetir (bucle).
- **Pista anterior (retirada):** arreglo de «Xochipitzahuatl (Flor Menudita)», canción tradicional náhuatl del centro de México; no era de Mazunte ni de Oaxaca.
- **Referencia APA 7 (incompleta):** *[Banda Mixe de Oaxaca — confirmar intérprete/canal (D2)].* ([Año — PENDIENTE (D2)]). *Dios Nunca Muere* [Canción/Video]. [Plataforma o canal — PENDIENTE (D2)]. [URL — PENDIENTE (D2)] (fecha de consulta: PENDIENTE (D2)). Uso educativo no comercial, sin licencia libre confirmada (decisión previa de D2).

Mientras falten esos campos, los créditos APA 7 de las dos pistas no están completos.
