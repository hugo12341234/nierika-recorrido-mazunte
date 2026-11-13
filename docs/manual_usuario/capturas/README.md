# Capturas del manual de usuario

Carpeta para las imágenes del manual (PNG, ventana a 1280×720). Nombre sugerido: `<sección>_<tema>.png`.
Todas las capturas son del programa propio; **la capilla provisional no sale en las capturas finales**
(decisión del 19/10 en `ESTADO_PROYECTO.md`).

| Archivo | Qué muestra | Sección | Estado |
|---|---|---|---|
| `01_ventana_inicio.jpg` | Cámara al abrir el programa (con la ficha del papel picado) | 1.4 | **Repetida 12/11 en la PC de D2** (X-4, aspecto nocturno final) |
| `03_parada1_orilla.jpg` | Orilla con velas, flores y la silueta | 3 | **Repetida 12/11 en la PC de D2** (X-4) |
| `03_parada4_panteon_desde_la_orilla.jpg` | Panteón con barda, tumbas y papel picado | 3 | **Repetida 12/11 en la PC de D2** (X-4) |
| `03_parada4_panteon_papel_picado.jpg` | Papel picado entre los postes | 3 | **Repetida 12/11 en la PC de D2** (X-4) |
| `03_parada5_cuarto_altar.jpg` | Altar con su ficha | 3, 4.1 | **Repetida 12/11 en la PC de D2** (X-4) |
| `03_parada5_silueta_con_flor.jpg` | La silueta con la flor frente al altar | 3 | **Repetida 12/11 en la PC de D2** (X-4) |
| `02_teclado_controles.png` | Esquema del teclado con W A S D, flechas, espacio, R, L y Esc | 2.1 | **Hecho 12/11** (`tools/x15_esquema_teclado.py`, Pillow) |
| `02_luces_espacio.jpg` | Escena con las luces de colores encendidas | 2.1 | **Tomada 12/11** (`NIERIKA_LUCES=1`) |
| `02_luces_L.jpg` | Escena con los cubos de las luces visibles | 2.1 | **Tomada 12/11** (`NIERIKA_LUCES=2`) |

Las capturas de 12/11 se hicieron en Linux con un renderizador por software y una cámara colocada por código; **conviene repetirlas en la PC de D2** con el aspecto final (texturas, hora del día) antes de entregar.

**Cómo se hicieron (12/11):** con el modo de captura de `src/main.cpp` (variable `NIERIKA_CAPTURAS="x,y,z,yaw,pitch,t,archivo.png;..."`; `NIERIKA_LUCES=1|2` para las luces). Las seis capturas del 12/11 hechas en Linux se sustituyeron por estas, tomadas en la PC de D2 con glReadPixels.
