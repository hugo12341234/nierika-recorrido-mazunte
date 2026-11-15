# Audio/ — sonido ambiental (tarea D-6)

Desde D-6h (14/11) esta carpeta solo debe contener **dos pistas con nombre fijo**, en formato `.mp3`:

| Archivo | Qué suena |
|---|---|
| `oleaje.mp3` | Mar (bucle) |
| `musica.mp3` | Música de fondo (bucle) |

`src/Audio.cpp` busca únicamente `Audio/oleaje.mp3` y `Audio/musica.mp3` (rutas relativas a la raíz del repo).
Ya **no** carga `.wav` ni `.flac`, así que un archivo viejo olvidado no puede tapar a los mp3.
Si falta una pista, la consola lo avisa (`[Audio] Falta Audio/...`) y el programa sigue sin ella; no se cae.
No hace falta recompilar al reemplazar un mp3. `.ogg` no está soportado.

**Estado de este commit:** carpeta vacía a propósito (limpieza D-6h); las dos pistas se agregan en el PR siguiente.
