# Capturas del manual de usuario

Carpeta para las imágenes del manual (PNG, ventana a 1280×720). Nombre sugerido: `<sección>_<tema>.png`.
Todas las capturas son del programa propio; **la capilla provisional no sale en las capturas finales**
(decisión del 19/10 en `ESTADO_PROYECTO.md`).

| Archivo | Qué muestra | Sección | Estado |
|---|---|---|---|
| `01_ventana_inicio.jpg` | Cámara al abrir el programa (con la ficha del papel picado) | 1.4 | **Repetida 17/11 en la PC de D2** |
| `02_teclado_controles.png` | Esquema del teclado: W A S D, flechas, ratón, V, M, F1, R y Esc | 2.1 | **Regenerado 17/11, sin L** (`tools/x15_esquema_teclado.py`, Pillow) |
| `02_velas_apagadas_V.jpg` | Altar con las velas apagadas y el aviso de V | 2.4 | **Nueva 17/11** (`NIERIKA_UI=velas_apagadas`) |
| `02_ayuda_F1.jpg` | Lista de controles de F1 | 2.4 | **Nueva 17/11, recapturada con X-61 (sin L)** (`NIERIKA_UI=ayuda`) |
| `03_parada1_orilla.jpg` | Orilla con velas, flores, lanchas y la silueta | 3 | **Repetida 17/11 en la PC de D2** |
| `03_parada4_panteon_desde_la_orilla.jpg` | Panteón con barda, tumbas, arco de cempasúchil y papel picado | 3 | **Repetida 17/11 en la PC de D2** |
| `03_parada4_panteon_papel_picado.jpg` | Papel picado entre los postes y una persona de rodillas junto a una tumba | 3 | **Repetida 17/11 en la PC de D2** |
| `03_parada5_cuarto_altar.jpg` | Altar con la ficha del copal | 3, 4.1 | **Repetida 17/11 en la PC de D2** |
| `03_parada5_altar_sin_ficha.jpg` | Altar visto desde la puerta del cuarto | 3 | **Nueva 17/11** |
| `03_parada5_silueta_con_flor.jpg` | La silueta frente al altar (t = 22 s) | 3 | **Repetida 17/11 en la PC de D2** |

Las `02_luces_espacio.jpg` (luces con la barra espaciadora) y `02_luces_L.jpg` (cubos de luces, tecla L) se **eliminaron** el 17/11: esas funciones ya no existen.


**Cómo se hicieron (17/11):** en la PC de D2, con el *Release x64* de `develop` y el modo de captura de `src/main.cpp`:
`NIERIKA_CAPTURAS="x,y,z,yaw,pitch,t,archivo.png;..."` (yaw −90 = mirar hacia −z; `t` = segundo de la caminata de la silueta) y, para V y F1, `NIERIKA_UI=velas_apagadas` o `NIERIKA_UI=ayuda`. Se guardan con `glReadPixels` (1280×720) y se convirtieron a JPG (calidad 88). Son tomas reales del programa, pero con la cámara colocada por código; **no** se vio a una persona pulsar las teclas.

| Toma | Pose `x,y,z,yaw,pitch,t` |
|---|---|
| inicio | `0,1.6,6,-90,0,0` |
| orilla | `0,1.6,21,90,-8,0` |
| panteón desde la orilla | `0,1.6,20,-90,0,0` |
| papel picado | `0,1.6,10,-90,0,0` |
| altar | `0,1.6,1.3,-90,-20,0` |
| altar sin ficha | `0,1.6,3.5,-90,-3,0` |
| silueta con flor | `0,1.6,1.3,-90,-20,22` |
| V (velas apagadas) | `0,1.6,1.8,-90,-15,0` |
| F1 | `0,1.6,6,-90,0,0` |
