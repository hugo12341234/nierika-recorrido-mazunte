# 9. Conclusiones

> **Estado (12/11/2026): borrador honesto.** Se escribe lo que ya es cierto. Las conclusiones finales (FPS, errores, pruebas en otra PC, videos) se completan tras las pruebas de E-1/E-3; **no se afirma nada sin medir**.

## 9.1 Qué se logró frente a los criterios de aceptación

Ver la tabla de [§1.4](01_introduccion.md). En resumen, a la fecha:

- **Escenario completo en código:** playa y mar con olas, panteón con 4 tumbas, cruz, roca, barda y papel picado, y el cuarto con altar de 3 niveles y ofrenda ✅ **[repo]**.
- **5 animaciones** implementadas (2 por KeyFrames, 3 simples) con su justificación matemática y valores del código ([§7](07_justificacion_matematica.md)).
- **23 modelos con `.obj` y `.blend`** en el repositorio; la meta es ≥ 18 con la definición de «propio» del 19/10 ([§6](06_modelos_y_recursos.md)).
- **Colisiones, fichas (5), audio propio, iluminación Blinn-Phong, `cleanup()` y contador de FPS** en el código.

**Lo que ningún documento puede afirmar todavía:** que se cumplan los 30 FPS (sin medir), que el programa corra en otra PC sin Visual Studio (E-3 sin probar), que el audio suene bien, ni que todo lo marcado 🔄 compile sin errores en MSVC.

## 9.2 Qué quedó fuera o se recortó

Nada se ha recortado formalmente al 12/11. Hay **concesiones registradas** en el ESTADO: textura del mar y de 8 modelos provisionales de un solo color, `laminado1.jpg` de 512 px, y la animación de las luces aceptada por palabra de D2. El orden de recortes del ESTADO §2 (marco B-6 → variantes de rocas B-10 → pausas de flores C-4) **no se ha aplicado**.

## 9.3 Desviaciones de la propuesta (todas en el ESTADO §4)

GLEW en vez de GLAD · SOIL2 además de stb_image · vcpkg · techo a dos aguas · iluminación de D1 en lugar de «ambiental + 3 luces» · extras de Maya (X-3) · caminata de 18.33 s en lugar de 6 s · audio generado con código · modelos de B-3…B-12 generados con `bpy` y scripts.

## 9.4 Lecciones aprendidas (propuesta; D1 y D2 las confirman o cambian) ❓

Estas lecciones salen de hechos registrados en el repositorio, no de opinión:

1. **Una revisión rápida no basta para detectar todo:** la barda (B-10) tuvo `/verificar` APROBAR y se vio compilada en la PC de D2, y aun así estaba **espejada en z** hasta que E-2 lo halló el 12/11. Los PR #1–#7 y #59–#68 además entraron sin veredicto de otra persona.
2. **Heredar el proyecto base con sus errores:** la solución recibida estaba en 32 bits y con escalas fuera de 1 u = 1 m (decisiones del 19/10 y A-2).
3. **Un entorno sin compilador de Windows obliga a probar fuera de OpenGL:** las matemáticas se comprobaron con programas aparte (23 comprobaciones en D-7), pero **no sustituyen** ver la escena.
4. **Definir pronto los valores:** varios valores (colisiones, luces, tiempos) no estaban en los documentos y se propusieron sobre la marcha.
5. **El calendario se comprimió:** buena parte de los cierres se concentró del 03/11 al 12/11.

## 9.5 Trabajo futuro

Lo que el alcance excluyó: Punta Cometa, calles y edificios del pueblo, VR/web/móvil, multijugador, personajes con IA, humo de copal con partículas, tapete de arena. ✅ **[repo, CONTEXTO]**

## 9.6 Nota sobre herramientas de IA (borrador para decidir) ❓

Propuesta de redacción: «El equipo usó Claude (Anthropic) como asistente de planeación, programación, generación de scripts y redacción; las decisiones de alcance, valores y aceptación las tomaron los integrantes, y cada cambio quedó registrado en `docs/ESTADO_PROYECTO.md` y en el historial de Git». **No es texto definitivo:** D1 y D2 deciden si se incluye.
