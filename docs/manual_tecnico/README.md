# Manual técnico — Nierika Studio (esqueleto, tarea E-5 parcial)

> **Proyecto:** Recorrido virtual interactivo 3D — Día de Muertos en Mazunte, Oaxaca
> **Equipo:** Nierika Studio · Equipo 15 · 319266894 (D1) · 321332086 (D2)
> **Entrega final:** miércoles 18 de noviembre de 2026
> **Versión del esqueleto:** 0.1 (borrador). Base: `develop` con los PR #16 (A-8) y #17 (X-3) fusionados; datos del `docs/ESTADO_PROYECTO.md` del 04/11/2026.
> **Idioma:** este esqueleto está en español. La versión en inglés (ES + EN, tarea E-5) **no existe todavía** y se traduce al final, cuando el contenido esté cerrado.

## Qué es este documento

Es el manual técnico que pide la propuesta v2.0 (arquitectura, cronograma, RACI, justificación matemática de las 5 animaciones, conclusiones y evidencias). Hoy es un **esqueleto**: cada sección tiene su estructura final y **solo contiene lo que ya es verdad según el ESTADO y el código**. Lo que falta está marcado, no inventado.

## Cómo leer las marcas

| Marca | Significado |
|---|---|
| ✅ **[repo]** | Dato comprobado en el repositorio (ESTADO, código o PDFs de la propuesta). Se cita de dónde sale |
| ⏳ **PENDIENTE** | La tarea que lo produce aún no se hace; el texto se escribe cuando exista |
| ❓ **POR DEFINIR** | Ningún documento lo fija; necesita una decisión registrada en el ESTADO (§4 o §5) |
| 📷 **CAPTURA** | Falta una imagen que debe tomar un integrante |

## Índice y estado

| § | Archivo | Contenido | Estado hoy |
|---|---|---|---|
| 1 | [`01_introduccion.md`](01_introduccion.md) | Objetivo, alcance, restricciones del cliente, cómo está organizado el equipo | 🔄 base escrita |
| 2 | [`02_arquitectura.md`](02_arquitectura.md) | Capas, módulos, ciclo de render, estructura del repositorio | 🔄 lo existente escrito; módulos futuros marcados |
| 3 | [`03_herramientas.md`](03_herramientas.md) | Herramientas y librerías, **con los cambios respecto a la propuesta** | ✅ completa a la fecha |
| 4 | [`04_cronograma.md`](04_cronograma.md) | Plan original vs. avance real por tarea y actividad | 🔄 real hasta el 04/11; horas reales ❓ |
| 5 | [`05_raci.md`](05_raci.md) | RACI planeado y registro real de quién hizo qué | 🔄 plan original copiado; real ❓ |
| 6 | [`06_modelos_y_recursos.md`](06_modelos_y_recursos.md) | Inventario de modelos propios, extras, texturas y créditos | 🔄 conteo real al 04/11 |
| 7 | [`07_justificacion_matematica.md`](07_justificacion_matematica.md) | Fórmulas y parámetros de las 5 animaciones | ⏳ (C-8, D-7) |
| 8 | [`08_pruebas_y_rendimiento.md`](08_pruebas_y_rendimiento.md) | Pruebas, FPS, fugas de GPU, problemas abiertos | 🔄 problemas escritos; FPS ❓ sin medir |
| 9 | [`09_conclusiones.md`](09_conclusiones.md) | Conclusiones y lecciones | ⏳ (al final) |
| 10 | [`10_evidencias.md`](10_evidencias.md) | Índice de evidencias (Trello, commits, minutas, capturas) | 🔄 estructura; `evidencias/` hoy solo tiene su README |
| 11 | [`11_referencias.md`](11_referencias.md) | Referencias APA 7 | 🔄 técnicas tomadas de la propuesta; culturales con C-7a |

## Reglas de formato (del profesor y del repo)

- Formato **profesional/empresarial** «Nierika Studio». **Sin logos de la escuela ni datos personales**: solo los números de cuenta (D1 = 319266894, D2 = 321332086). [repo, `CONTEXTO_PROYECTO.md` §1]
- **Referencias en APA 7** de todo lo citado (texturas, librerías, fuentes culturales). [repo]
- Los valores de este manual deben coincidir con el código y con `ESTADO_PROYECTO.md` §5. Si cambian, se corrige aquí en el mismo PR.
- Archivos fuente en Markdown. **Cómo se convierte al formato de entrega** (.docx o PDF) ❓ **POR DEFINIR**: ningún documento del repo lo fija; propuesta: convertir con una herramienta al cerrar E-5 y registrar la decisión en el ESTADO §4.

## Qué falta para cerrar E-5 (resumen)

1. Cada bloque deja sus secciones con capturas y parámetros (regla del ESTADO §2).
2. Escribir §7 con los valores del código cuando existan C-1, C-3, C-4, D-2, D-3, D-4 (tareas C-8 y D-7).
3. Registrar el RACI **real** (§5) con D1 y D2; hoy nadie lo ha confirmado.
4. Medir FPS en la PC de pruebas (§8).
5. Tomar capturas (📷) y guardar el Trello y las minutas en `evidencias/` (§10).
6. Traducir al inglés.
