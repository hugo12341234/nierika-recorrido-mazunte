## g.5 Integrante D2 (321332086)

**Herramienta o tecnología que aprendí:** Blender 5.2 controlado desde Python con la API `bpy`, usado como módulo y sin abrir la ventana del programa.

**Por qué fue necesaria para el proyecto:** el cliente pedía al menos 18 modelos propios y el calendario se comprimió en las últimas dos semanas. Modelar cada objeto a mano no era viable en ese tiempo y, además, cada vez que cambiaba una medida (por ejemplo, al corregir la escala a 1 unidad = 1 metro) había que rehacer el modelo. Con `bpy` cada modelo, textura o dato de la escena sale de un script: si algo cambia, se edita un número y se vuelve a generar, y cualquiera puede reproducir el mismo resultado desde el repositorio. Por eso el 11/11 decidí que los modelos y las texturas propias se generarían con scripts.

**Cómo la aprendí:** no conocía la API de Python de Blender; solo había usado Blender con la interfaz gráfica. Aprendí con la documentación oficial de la API (Blender Python API), con ejemplos de la comunidad y, sobre todo, probando: escribía un script corto, lo ejecutaba, abría el `.blend` para ver el resultado y corregía. Me apoyé en un asistente de IA (Claude) para escribir y depurar scripts; mi trabajo fue decidir qué debía generar cada uno, revisar el resultado en Blender y en el programa, y pedir correcciones. Lo más difícil fue entender el modelo de datos de Blender (la diferencia entre objeto, malla y material, y el contexto que exigen algunos operadores cuando no hay ventana), lograr que los `.obj` exportados conservaran la escala y las coordenadas UV correctas para OpenGL, y que las texturas no se vieran estiradas. Trabajé con ella durante la semana del 11 al 18/11.

**Qué construí con ella (evidencia):**

* `tools/bpy_comun.py`, un módulo con las funciones comunes (crear materiales, aplicar escala, exportar a `.obj` y guardar el `.blend`).
* 52 scripts de Python en `tools/`, 25 de ellos de Blender (`bpy`); el resto genera texturas, datos de la escena y documentos.
* Los 23 modelos del inventario, cada uno con su `.obj` y su `.blend` en `Models/` (Anexo A; tabla calculada con `tools/e5_tabla_modelos.py`).
* La silueta de piezas orgánicas generada con `tools/x7_silueta.py` (X7, 12/11).
* Las texturas propias `laminado1.jpg` y `concreto.jpg`, declaradas como propias en los Créditos del programa (X69 y X71).

**Qué haría distinto o qué no llegué a dominar:** empezaría a usar los scripts desde la primera semana, no al final, para tener tiempo de afinar los modelos. Todavía no domino el modelado orgánico por código: las formas complejas (personas, flores) me salen mejor a mano o con modelos externos, y aún dependo bastante de la documentación y del asistente para escribir scripts largos. También me faltó optimizar la geometría desde Blender: el diagnóstico de rendimiento del 18/11 mostró que había más triángulos de los necesarios.
