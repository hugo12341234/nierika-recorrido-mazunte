## g.4 Integrante D1 (319266894)

**Herramienta o tecnología que aprendí:** Autodesk Maya, para modelar y texturizar objetos 3D y exportarlos en formato `.obj` para OpenGL.

**Por qué fue necesaria para el proyecto:** la ofrenda y el altar necesitaban objetos con forma propia que dieran identidad cultural a la escena: la calavera, el pan de muerto, la flor, el fuego, el humo, las mariposas, las macetas, el cuadro, el refresco y los papeles. Modelarlos nosotros permitía que el recorrido tuviera piezas hechas por el equipo y no solo modelos descargados de internet.

**Cómo la aprendí:** antes del proyecto no había modelado objetos completos en Maya. Aprendí con tutoriales en video y con la documentación de Autodesk, practicando primero con piezas sencillas antes de pasar a los objetos del altar. Lo más difícil fue la exportación: conseguir que los `.obj` conservaran las normales y las coordenadas UV para que las texturas se vieran bien al cargarlos con Assimp en el programa, y que el tamaño y la orientación coincidieran con la escala de la escena. Otro problema fue el tamaño de las texturas: las hice a 2048 px y después hubo que reducirlas a 1K para que el programa las cargara sin problemas.

**Qué construí con ella (evidencia):**

* Pan, flor y calavera: `Models/pan`, `Models/flor`, `Models/calavera` (PR #17, X3).
* Modelos del altar: cuadro, fuego, humo, mariposa, maceta, refresco y papeles (subidos en X43, PR #111; el humo se ajustó en X47, PR #115; el fuego y las mariposas se usan en la escena desde X49). El fuego, el humo, los papeles, las macetas y el cuadro se usan en la escena final.
* El primer modelo de la silueta, de 10 piezas (PR #39, 04/11), que después se sustituyó por una versión generada por script.
* Además adapté la iluminación de mi Práctica 8 (una luz direccional, cuatro puntuales animadas, un reflector en la cruz y seis en los escalones), que se usó como base de la iluminación Blinn-Phong final (PR #16, A8).

**Qué haría distinto o qué no llegué a dominar:** subiría mi trabajo directamente al repositorio con mis propios commits, en lugar de enviarlo por fuera de Git, para que quedara registrado y se integrara a tiempo. Prepararía las texturas desde el inicio en la resolución adecuada. Todavía no domino el mapeo UV de objetos orgánicos ni la optimización de la cantidad de polígonos, que es importante para que la escena corra bien en computadoras con gráficos integrados.
