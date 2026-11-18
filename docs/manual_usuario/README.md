# Manual de usuario — Recorrido virtual: Día de Muertos en Mazunte

**Nierika Studio** · Equipo 15 · Cuentas: 319266894 · 321332086
**Versión:** 1.0 (18/11/2026, hora de México). Fuente en Markdown; el PDF de entrega es `docs/entrega/ManualDeUsuario_Equipo15_319266894_321332086.pdf`.

## Índice

1. [Qué es el recorrido y cómo abrirlo](01_inicio.md)
2. [Controles](02_controles.md)
3. [El recorrido (libre y guiado)](03_recorrido.md)
4. [Fichas culturales y audio](04_fichas_y_audio.md)
5. [Problemas frecuentes](05_problemas_frecuentes.md)
6. [Código fuente y documentación](06_codigo_fuente.md)

Las capturas están en [`capturas/`](capturas/README.md). La traducción anterior al inglés (v0.6 del 17/11) se movió a `docs/archivo/manual_usuario_en_v0.6/`: **no forma parte de la entrega** y puede diferir del español.

## Cómo generar el PDF

`python3 tools/e5_exportar_pdf.py usuario` (necesita pandoc, Node.js con Playwright y poppler; ver el encabezado del script). La salida va a `docs/entrega/`.
