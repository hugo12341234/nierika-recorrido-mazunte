---
name: actividad
description: Arranca el trabajo de una actividad del plan de Nierika Studio por su número (1-60). Úsalo cuando el usuario escriba /actividad N o diga que va a empezar una actividad.
argument-hint: [número de actividad]
disable-model-invocation: true
---

Vamos a trabajar la **actividad $ARGUMENTS** del plan.

1. Lee `docs/PLAN_ACTIVIDADES.md` y muestra la fila $ARGUMENTS: actividad, responsable, verificador, horas, criterio de aceptación, entregable, recurso.
2. Lee `docs/ESTADO_PROYECTO.md` (el *Plan vigente* de la sección 2 manda sobre las fechas y responsables originales):
   - ¿La actividad ya tiene avance? ¿Hay parámetros fijados o decisiones que la afecten?
   - ¿Alguna actividad de la que depende sigue sin terminar? Si sí, avísalo antes de seguir.
3. Revisa el código/archivos existentes relacionados (busca clases, shaders o modelos con nombres afines).
4. Propón un plan en pasos numerados, cada uno pequeño y verificable, que termine cumpliendo el criterio de aceptación. Si la actividad es de Blender/GIMP, el plan es una guía de menús y valores; el código solo cubre la integración.
5. Sugiere el nombre de rama: `feature/act-$ARGUMENTS-<nombre-corto>`.
6. **Espera confirmación** del usuario antes de editar archivos. Después avanza paso por paso, compilando o indicando cómo comprobar cada uno.

Recuerda las restricciones de `CLAUDE.md` (alcance, stack, KeyFrames, 30 FPS).
