---
name: verificar
description: Revisa una tarea (código A-1, B-3…) o una actividad (número 1-60) como verificador antes de aprobar su pull request. Úsalo cuando el usuario escriba /verificar seguido de un código o número.
argument-hint: [código de tarea (A-1) o número de actividad (1-60)]
disable-model-invocation: true
---

Usa el subagente `verificador` para revisar **$ARGUMENTS** contra su criterio de terminado
(sección 2 de `docs/ESTADO_PROYECTO.md`) y el criterio de aceptación original (`docs/PLAN_ACTIVIDADES.md`).

Cuando regrese, muestra su veredicto tal cual y, si es APROBAR, recuerda al usuario, en este orden:
1. Confirmar que el ESTADO de la rama marca la tarea ✅ (si no, pedir a quien la hizo que corra `/cierre`).
2. Aprobar el PR en GitHub hacia `develop` y hacer el *merge*.
3. Mover la tarjeta en Trello a "Terminado".
4. Los dos: `git checkout develop` y `git pull`.
