# tools/ — scripts de apoyo

> Índice creado el 18/11/2026 (X-71). Los scripts **no se necesitan para compilar ni para ejecutar el juego**: se usaron para generar modelos, texturas
> y datos, para probar fórmulas y para armar la entrega. Cada archivo explica en su encabezado qué hace. El prefijo es el código de la tarea del ESTADO
> (`b3_` = B-3, `x55_` = X-55…).
> **Aviso (ESTADO §6, problema 16):** `x33_vida_colinas.py` ya no reproduce `src/VidaColinasDatos.h`; no se debe volver a correr sin revisar el resultado.

| Qué | Archivos |
|---|---|
| **Entrega** | `empaquetar_release.ps1` (compila Release x64 y arma `dist/` + `.zip`, E-3) · `e5_exportar_pdf.py` y `e5_figuras.py` + `pdf/` (PDF de los manuales, E-5/E-6) · `e5_tabla_modelos.py` (tabla del inventario) · `x16_generar_videos.ps1`, `x16_rutas_video.py` y `video_rutas/` (modo de video, X-16) |
| **Modelos con Blender (`bpy`)** | `bpy_comun.py` (utilidades) · `b3_`, `b4_`, `b7_`, `b9_`, `b10_`, `c2_` (ofrenda, tumbas, barda, llama) · `x7_silueta.py`, `x36_`, `x40_banda.py`, `x41_`, `x49_mariposas.py`, `x51_`, `x52_`, `x53_patas_palapa.py`, `x55_ofrendas_modelos.py`, `x55_personas_articuladas.py` |
| **Texturas** | `b12_texturas.py`, `x6_texturas.py`, `x14_barda_uv.py`, `x35_atuendos.py`, `x44_piedra.py`, `x58_silueta_texturas.py`, `x43_reducir_maya.py`, `x43_piezas.py` (modelos de Maya de D1) |
| **Datos de la escena (generan `src/*Datos.h` y similares)** | `x18_colinas.py`, `x19_ceremonia.py`, `x21_tablas.py`, `x24_narrativa.py`, `x25_lados.py`, `x26_construcciones.py`, `x29_comparsa.py`, `x30_pueblo_fondo.py`, `x33_vida_colinas.py`, `x42_redistribuir_playa.py`, `x48_corregir_solapes.py`, `x49_normalizar.py`, `x53_terreno.py`, `x53_colinas_obj.py`, `x53_ajustar_alturas.py`, `x58_decorar_casas.py`, `x9_vegetacion.py`, `x69_generar_creditos.py` (genera `src/Creditos.h` desde `docs/REFERENCIAS.md`) |
| **Verificadores y pruebas numéricas** | `d7_pruebas_matematicas.cpp`, `x19_pruebas_extras.cpp`, `x49_pruebas.cpp`, `x53_pruebas.cpp` + `x53_comparar_cpp.py`, `x55_pruebas.cpp`, `x55_verificar_ofrendas.cpp`, `x55_buscar_sitios.cpp`, `x65_pruebas_peregrinos.cpp`, `x48_verificar_solapes.py`, `x53_verificar_alturas.py`, `x32_dimensiones.py`, `x15_esquema_teclado.py` |
