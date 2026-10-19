---
name: cierre
description: Cierra la sesión de trabajo actualizando docs/ESTADO_PROYECTO.md con lo que realmente se hizo. Úsalo cuando el usuario escriba /cierre o diga que termina por hoy.
disable-model-invocation: true
---

Actualiza `docs/ESTADO_PROYECTO.md` con lo ocurrido en **esta** sesión. Solo hechos reales; nada supuesto.

1. Revisa `git status` y `git diff --stat` (y `git log -5 --oneline`) para saber qué cambió.
2. En el archivo:
   - Cambia **Última actualización** a la fecha de hoy y la semana actual.
   - Tabla de actividades: actualiza el estado (✅ 🔄 👀 ⏳ ⚠️ ✂️) y la columna de evidencia (commit, PR, archivo).
   - **Registro de decisiones**: agrega cada cambio respecto a la propuesta (fecha, decisión, motivo, a qué afecta).
   - **Parámetros fijados**: agrega valores exactos elegidos hoy con su `archivo:línea`.
   - **Estructura del repositorio**: actualízala si se crearon carpetas o archivos importantes.
   - **Problemas abiertos** y **Siguiente paso concreto**.
3. Si la fecha actual va adelantada respecto al plan y hay actividades atrasadas, márcalas ⚠️ y dilo.
4. Muestra al usuario un resumen de 5 líneas máximo de lo que cambió en el ESTADO.
5. Recuérdale: hacer commit de `docs/ESTADO_PROYECTO.md` y subir la nueva versión al Proyecto de Claude.ai.
