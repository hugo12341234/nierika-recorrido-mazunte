---
name: cierre
description: Cierra la sesión de trabajo actualizando docs/ESTADO_PROYECTO.md con lo que realmente se hizo. Úsalo cuando el usuario escriba /cierre o diga que termina por hoy.
disable-model-invocation: true
---

Actualiza `docs/ESTADO_PROYECTO.md` con lo ocurrido en **esta** sesión. Solo hechos reales; nada supuesto.

1. Revisa `git status`, `git diff --stat` y `git log -5 --oneline` para saber qué cambió.
2. En el archivo:
   - **Última actualización:** fecha de hoy, semana/bloque actual y días a la entrega (18/11/2026).
   - **Sección 2 (Plan vigente):** estado de la tarea trabajada (✅ 🔄 ⏳ ⚠️ ✂️). Márcala ✅ si en esta rama compiló y cumple su criterio de terminado: el ESTADO viaja en el PR, así que en `develop` solo aparece ✅ cuando el otro integrante aprueba y hace el *merge*. Si queda a medias, 🔄 con lo que falta.
   - **Sección 3 (por actividad):** el mismo estado en las actividades que cubre la tarea (columna Act.), con la evidencia (rama, PR o archivo).
   - **Sección 4 (decisiones):** cada cambio respecto a la propuesta o a una regla (fecha, decisión, motivo, a qué afecta). Cierra las *pendientes* que se hayan decidido.
   - **Sección 5 (parámetros):** valores exactos elegidos hoy, con su archivo.
   - **Secciones 6 y 7:** problemas abiertos (con la tarea que los resuelve) y siguiente paso concreto.
3. **Coherencia:** si hoy cambió algo que otro documento describe (carpetas, stack, convenciones, flujo),
   corrige también ese documento (`CONTEXTO_PROYECTO.md`, `FLUJO_DE_TRABAJO.md` → mapa del repositorio,
   `README.md`, `CLAUDE.md`). Si encuentras una contradicción que no puedes resolver, anótala en problemas abiertos.
4. Si la fecha va adelantada respecto al plan vigente y hay tareas atrasadas, márcalas ⚠️ y dilo.
5. Muestra al usuario un resumen de 5 líneas máximo de lo que cambió en el ESTADO.
6. Recuérdale: hacer commit de los documentos en la rama de la tarea (a su nombre: revisa `git config user.name`)
   y abrir el PR. El Proyecto de Claude (web) lee el repo directamente, así que no hay que copiarle nada.
