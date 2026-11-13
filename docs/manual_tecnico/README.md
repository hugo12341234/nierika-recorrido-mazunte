# Manual técnico — Nierika Studio (E-5)

> **Proyecto:** Recorrido virtual interactivo 3D — Día de Muertos en Mazunte, Oaxaca
> **Equipo:** Nierika Studio · Equipo 15 · 319266894 (D1) · 321332086 (D2)
> **Entrega final:** miércoles 18 de noviembre de 2026
> **Versión:** 0.5 (12/11/2026, hora de México). Base: `develop` con los PR #59 a #68 fusionados.
> **Idiomas:** español (esta carpeta) e inglés ([`en/`](en/README.md), resumen traducido; ver «Qué falta»).

## Qué es este documento

Manual técnico de la propuesta v2.0: arquitectura, cronograma, RACI, justificación matemática de las 5 animaciones, conclusiones y evidencias. **Todo está escrito con el estado real del repositorio**; lo que depende de una persona (compilar en la PC de D2, medir FPS, RACI real, horas) está marcado, no inventado.

## Cómo leer las marcas

| Marca | Significado |
|---|---|
| ✅ **[repo]** | Dato comprobado en el repositorio (ESTADO, código, archivos) |
| 🔄 | Hecho en código/archivos; **falta confirmar compilado en MSVC o visto en pantalla** |
| ⏳ **PENDIENTE** | Aún no existe |
| ❓ **POR DEFINIR** | Ningún documento lo fija; lo deciden D1 y D2 |
| 📷 **CAPTURA / FIGURA** | Falta una imagen que debe tomar un integrante |

## Índice y estado

| § | Archivo | Contenido | Estado |
|---|---|---|---|
| 1 | [`01_introduccion.md`](01_introduccion.md) | Objetivo, alcance, criterios y su estado | ✅ al 12/11 |
| 2 | [`02_arquitectura.md`](02_arquitectura.md) | Capas, módulos, ciclo de render, repositorio | ✅ al 12/11 (figuras 📷) |
| 3 | [`03_herramientas.md`](03_herramientas.md) | Herramientas y cambios respecto a la propuesta | ✅ al 12/11 |
| 4 | [`04_cronograma.md`](04_cronograma.md) | Plan vs. avance real por tarea | ✅ fechas; horas reales ❓ |
| 5 | [`05_raci.md`](05_raci.md) | RACI planeado y registro real | 🔄 el RACI real ❓ lo deciden D1 y D2 |
| 6 | [`06_modelos_y_recursos.md`](06_modelos_y_recursos.md) | Inventario calculado de los 23 modelos y créditos | ✅ al 12/11 |
| 7 | [`07_justificacion_matematica.md`](07_justificacion_matematica.md) + 2 documentos | Matemática de las 5 animaciones | ✅ (sin ver compilado) |
| 8 | [`08_pruebas_y_rendimiento.md`](08_pruebas_y_rendimiento.md) | Pruebas, FPS, GPU, problemas | 🔄 **FPS sin medir** (protocolo y tabla vacía) |
| 9 | [`09_conclusiones.md`](09_conclusiones.md) | Conclusiones | 🔄 borrador honesto |
| 10 | [`10_evidencias.md`](10_evidencias.md) | Índice de evidencias | 🔄 |
| 11 | [`11_referencias.md`](11_referencias.md) | Referencias APA 7 | 🔄 faltan GLEW, GLM, vcpkg, SOIL2 |

## Reglas de formato

- Formato **profesional/empresarial** «Nierika Studio». **Sin logos de la escuela ni datos personales**: solo los números de cuenta (D1 = 319266894, D2 = 321332086). [repo, `CONTEXTO_PROYECTO.md` §1]
- **Referencias en APA 7**.
- Los valores deben coincidir con el código y con `ESTADO_PROYECTO.md` §5; si cambian, se corrige aquí en el mismo PR.
- Fuente en Markdown; PDF generado con `tools/e5_exportar_pdf.sh` (ver «Cómo se genera el PDF»).

## Contradicciones detectadas al escribirlo (para decidir)

1. El encabezado del ESTADO dice «Última actualización 11/11, 7 días a la entrega», pero hay entradas del 12/11 (quedan 6 días): falta actualizarlo en el próximo `/cierre`.
2. **Dos excepciones de fusión** del ESTADO §4 cubren las mismas 10 tareas: la del **11/11** exigía una revisión crítica propia aprobada **escrita en la descripción de cada PR**, y la del **12/11** (PR #59–#68) dice que se fusionaron **sin veredicto**. No consta cuál condición se cumplió en cada PR: revisar las descripciones.
3. E-5 y E-6 aparecen como 🔄 en el ESTADO aunque sus PR (#67, #68) están fusionados; se cierran cuando D2 acepte el manual.
4. `Models/altar/` (capilla de Maya) y `Models/pan/` siguen en el repo pese a estar retirados de la escena; el PR #45 propone borrar la primera y sigue abierto.

## Qué falta para cerrar E-5 (necesita a una persona)

1. D2 compila `Release | x64`, revisa lo marcado 🔄 y mide los FPS (§8.2.1).
2. D1 y D2 deciden el RACI real, las horas (si se reportan) y la nota sobre IA ([§9.6](09_conclusiones.md)).
3. Capturas 📷 y figuras de arquitectura con el aspecto final, en la PC de D2.
4. Referencias que faltan ([§11](11_referencias.md)).
5. **Inglés:** `en/` traduce §1–§11 en resumen; las dos justificaciones matemáticas completas siguen en español (decisión de alcance de este PR: ver `en/README.md`).
6. Revisar el PDF generado.

## Cómo se genera el PDF

Ver `tools/e5_exportar_pdf.sh`: `pandoc` une los archivos en orden y LibreOffice lo convierte a PDF. El PDF **no se versiona** (se genera para la entrega).
