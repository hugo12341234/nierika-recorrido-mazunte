# Manual de usuario — Recorrido virtual: Día de Muertos en Mazunte

**Nierika Studio** · Equipo 15 · Cuentas: 319266894 · 321332086
**Versión:** 0.5 (12/11/2026, hora de México) · **Idiomas:** español (esta carpeta) e inglés ([`en/`](en/README.md), traducción completa de las 5 secciones; si difieren, manda el español)
**Formato final:** PDF, con capturas (actividad 59 del plan). Se genera con `tools/e5_exportar_pdf.sh` (PR de E-5); el PDF no se versiona.

> **Cómo leer este manual.** Cada sección dice si su contenido es **real** (coincide con el código de
> `src/main.cpp` de la rama `develop`) o está **PENDIENTE**. Nada pendiente se escribe como si ya funcionara.

## Índice

| # | Sección | Archivo | Estado |
|---|---|---|---|
| 1 | Qué es el recorrido y qué necesitas | [01_inicio.md](01_inicio.md) | Real, salvo §1.3 «cómo abrirlo» (espera el `.zip` de E-3/E-7) |
| 2 | Controles | [02_controles.md](02_controles.md) | **Real**; teclas de depuración sin decidir |
| 3 | El recorrido, parada por parada | [03_recorrido.md](03_recorrido.md) | Real (capturas de Linux/Mesa) |
| 4 | Fichas culturales y audio | [04_fichas_y_audio.md](04_fichas_y_audio.md) | Real en el código; fichas sin compilar en VS; audio sin oír |
| 5 | Problemas frecuentes | [05_problemas_frecuentes.md](05_problemas_frecuentes.md) | Parcial (filas «sin probar» marcadas) |
| — | Capturas | [capturas/](capturas/README.md) | 6 tomadas; 3 pendientes |

## Qué falta para cerrar E-6 (necesita a una persona)

1. **Repetir las 6 capturas en la PC de D2** con el aspecto final; tomar las 3 pendientes (esquema del teclado en GIMP, luces con la barra espaciadora y con L).
2. §1.3 «cómo abrirlo»: se escribe cuando exista el `.zip` probado en otra PC (E-3/E-7).
3. D2 decide si **L** y las teclas de depuración `T G Y H U J` / `1–6` salen antes de la entrega (siguen en `main.cpp`).
4. D2 confirma o elimina las filas «sin probar» de §5 tras las pruebas de E-1 y E-3.
5. Fichas: compilar con ImGui en VS y verificar textos y referencias contra la fuente (C-7a).
6. Revisar el PDF generado (sin logos de la escuela ni datos personales; solo números de cuenta) y su estilo final.
