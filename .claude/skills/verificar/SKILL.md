---
name: verificar
description: Revisa una actividad como verificador antes de aprobar su pull request. Úsalo cuando el usuario escriba /verificar N.
argument-hint: [número de actividad]
disable-model-invocation: true
---

Usa el subagente `verificador` para revisar la **actividad $ARGUMENTS** contra su criterio de aceptación en `docs/PLAN_ACTIVIDADES.md`.

Cuando regrese, muestra su veredicto tal cual y, si es APROBAR, recuerda al usuario:
- aprobar el PR en GitHub hacia `develop`,
- mover la tarjeta en Trello a "Terminado",
- marcar la actividad ✅ con `/cierre`.
