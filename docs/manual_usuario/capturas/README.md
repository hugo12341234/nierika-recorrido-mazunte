# Capturas del manual de usuario

Todas son tomas reales del programa propio (ventana de 1280×720 o montajes de varias tomas), guardadas como JPG.

| Archivo | Qué muestra | Fecha y origen |
|---|---|---|
| `01_menu_inicio.jpg`, `01_menu_controles.jpg`, `01_menu_creditos.jpg`, `01_menu_creditos_fin.jpg` | Menú de inicio, controles y créditos | 17–18/11, modo de captura del programa |
| `01_menu_pausa.jpg` | Menú de pausa (Esc) | 18/11, modo de captura del programa |
| `01_ventana_inicio.jpg` | Cámara al empezar (ficha del papel picado) | 17/11, PC de D2 |
| `02_teclado_controles.png` | Esquema del teclado y el ratón (dibujado por `tools/x15_esquema_teclado.py`) | 17/11 |
| `02_velas_apagadas_V.jpg`, `02_ayuda_F1.jpg` | Efecto de las teclas V y F1 | 17/11, PC de D2 (`NIERIKA_UI=velas_apagadas` / `ayuda`) |
| `03_parada1_orilla.jpg` … `03_parada5_silueta_con_flor.jpg` (6) | Orilla, panteón, papel picado, altar y silueta | 17/11, PC de D2 |
| `03_vista_aerea_playa.jpg`, `03_banda_de_viento.jpg`, `03_camino_de_las_animas.jpg` | La playa desde arriba, la banda y el camino de las ánimas | 17/11, modo de captura |
| `03_recorrido_guiado_12_estaciones.jpg` | Montaje con una toma por cada estación del recorrido guiado | 18/11, modo de captura |
| `04_ficha_mariposas.jpg` | Ejemplo de ficha cultural | 17/11, modo de captura |

**Cómo se hicieron:** con el modo de captura de `src/main.cpp` (`NIERIKA_CAPTURAS="x,y,z,yaw,pitch,t,archivo.png;..."`, yaw −90 = mirar hacia −z, `t` = segundo de la caminata de la silueta; `NIERIKA_UI=velas_apagadas|ayuda|menu_*` para los menús y avisos). Las tomas colocan la cámara por código; no se grabó a una persona pulsando las teclas.
