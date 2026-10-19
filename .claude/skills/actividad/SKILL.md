---
name: actividad
description: Arranca el trabajo de una tarea del plan vigente de Nierika Studio por su código (A-1, B-3…) o de una actividad del plan original por su número (1-60). Úsalo cuando el usuario escriba /actividad seguido de un código o número, o diga que va a empezar una tarea.
argument-hint: [código de tarea (A-1) o número de actividad (1-60)]
disable-model-invocation: true
---

Vamos a trabajar **$ARGUMENTS**.

1. **Identifica la tarea** en `docs/ESTADO_PROYECTO.md`:
   - Si `$ARGUMENTS` es un código (`A-1`, `B-12`, `X-2`…): búscalo en la sección 2 (*Plan vigente*).
   - Si es un número (1–60): búscalo en la sección 3 y toma el código de su columna **Tarea**.
     Si no tiene código (columna "—"), dilo y trabaja con la actividad tal cual.
   - Si no lo encuentras, **detente** y dilo; no adivines a qué se refiere.
2. Muestra en un bloque corto: código, tarea, estado actual, **criterio de terminado**, y las actividades
   originales que cubre (columna Act.). De `docs/PLAN_ACTIVIDADES.md` agrega, para esas actividades,
   el criterio de aceptación y el entregable originales.
3. Revisa en el ESTADO lo que afecta a la tarea:
   - Decisiones (sección 4), incluidas las *pendientes*: si alguna pendiente bloquea la tarea, avísalo antes de seguir.
   - Parámetros fijados (sección 5) y problemas abiertos (sección 6) relacionados.
   - Dependencias: ¿alguna tarea anterior de la que depende sigue sin terminar? Si sí, avísalo.
4. **Revisa la coherencia:** si el criterio del ESTADO, el del plan original, `CONTEXTO_PROYECTO.md` o el
   código existente se contradicen, señálalo y propone cómo resolverlo (registrando una decisión).
5. Revisa el código y los archivos existentes relacionados (clases, shaders o modelos con nombres afines).
6. Propón un plan en pasos numerados, cada uno pequeño y verificable, que termine cumpliendo el criterio de
   terminado. Si la tarea es de Blender/GIMP, el plan es una guía de menús y valores; el código solo cubre la integración.
7. Sugiere la rama: `feature/<código en minúsculas>-<nombre-corto>` (p. ej. `feature/a-10-dovelas`) y comprueba
   con `git branch --show-current` si ya estás en ella.
8. **Espera confirmación** del usuario antes de editar archivos. Después avanza paso por paso, indicando cómo
   comprobar cada uno (Claude no compila en Windows: pide al usuario que compile con F5 y te diga qué ve).

Recuerda las reglas de `CLAUDE.md` (integridad y coherencia, alcance, stack, KeyFrames, 30 FPS, autoría).
