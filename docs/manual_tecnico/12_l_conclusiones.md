# l) Conclusiones

Las conclusiones se apoyan en datos del repositorio y en mediciones del equipo; donde no hay dato, se dice. Lo que no pudimos comprobar aparece como límite y no como logro.

## l.1 Técnica

El programa cumple lo central de la propuesta: un recorrido 3D en primera persona, en C++17 con OpenGL 3.3 Core, con iluminación Blinn-Phong, colisiones AABB, fichas culturales, audio y 10 animaciones (5 por KeyFrames y 5 simples, sección i). Son 9 177 líneas de C++ en 44 archivos y 574 líneas de shaders.

El resultado técnico más claro del cierre fue el rendimiento. En una laptop con Intel Core i5 de 11.ª generación y gráficos integrados, el programa pasó de unos 15 FPS a un mínimo de 45 y un promedio de unos 60 después del descarte por frustum y las cachés del PR #148 (medición de D2). El criterio del cliente es 30 FPS a 1280 × 720. Es una sola máquina con gráficos integrados: no se midió en otras.

Lo que queda como límite técnico: no se midió la memoria de GPU; las 23 piezas del inventario de modelos propios superan la meta de 18, pero parte de los modelos se generó con *scripts* de `bpy` hechos con ayuda de IA (anexo A); y no se puede comprobar en el repositorio que ninguna animación coincida con las de las prácticas del curso, porque esas prácticas no están en él.

## l.2 Social

El trabajo fue de dos personas y el repositorio no lo refleja por igual: 489 de los 491 commits de `develop` llevan el nombre de D2 (los otros 2, de la cuenta «Claude» de la plataforma), y D1 aparece sobre todo en las luces y los modelos de Maya que se integraron. La coordinación se hizo con Trello, GitHub y un asistente de IA (secciones e, f y h). La causa de ese desbalance no está documentada en el repositorio, por eso este manual no la afirma; cada integrante expone su propia versión en las secciones g y j.

La gestión mostró un patrón que conviene reconocer: la mayor parte del trabajo se concentró en las últimas dos semanas (más de la mitad de los commits cae del 11 al 18/11, sección d). Aprender de eso es la lección social principal: repartir el trabajo desde la semana 1 y registrar horas reales.

## l.3 Económica

La propuesta estimó **203 horas** de esfuerzo, un costo de mano de obra de **\$23 409.50 MXN** y un precio al cliente de **\$31 228.27 MXN**. **No se registraron horas reales**, así que no se puede decir si el estimado se cumplió, se excedió o sobró. El costo en dinero fue bajo porque se eligieron herramientas libres o gratuitas (OpenGL, GLFW, GLEW, GLM, Assimp, Dear ImGui, miniaudio, Blender, Poly Haven con licencia CC0). Hubo software comercial: Visual Studio (la propuesta presupuestó la edición Professional) y Maya en la parte de D1. No se anotó el plan de ElevenLabs con que se generó la voz, por lo que su costo no se puede reportar.

## l.4 Ambiental

No se midió ningún consumo de energía, ni de las computadoras ni del uso de servicios de IA y de síntesis de voz, así que no hay cifra que dar. Lo único que se puede afirmar con datos es indirecto: el trabajo de rendimiento redujo la carga de GPU para el mismo recorrido (de unos 15 a unos 60 FPS en gráficos integrados, sin necesidad de una tarjeta dedicada) y el proyecto corre en equipos de uso común. La escena misma tiene un tema ambiental real: la ficha de las tortugas de Mazunte cuenta el paso de la explotación de la tortuga a su conservación, según México Desconocido (2010).

## l.5 Cultural

El proyecto representa el atardecer y la noche del 1 de noviembre en Mazunte con 13 fichas que citan su fuente en APA 7 (secciones b y k) y una narración de 12 estaciones. Lo que el estudio adaptó o inventó se declara como adaptación, por ejemplo la banda de viento, las ofrendas de las familias y la comparsa: la comparsa que cita la ficha es la de la ciudad de Oaxaca, no la de Mazunte. Los límites: una de las fuentes es contenido de marca (México Desconocido, 2026), la afirmación «al atardecer» solo se apoya en una lectura automática de Baños (2025) y no hubo consulta con personas de la comunidad. La escena es una interpretación de estudiantes, no un registro etnográfico.

## l.6 Uso de inteligencia artificial

Claude (Anthropic) se usó para planear, programar, escribir *scripts*, revisar y redactar documentación, y la voz de la narración es sintética (ElevenLabs, Eleven v4). El equipo decide y revisa; el historial de commits y las reglas de `CLAUDE.md` muestran cómo se condujo ese trabajo. Declararlo es parte de la integridad del proyecto: quien lea este manual debe saber qué partes se asistieron.

## l.7 Lecciones aprendidas

1. Medir el rendimiento temprano: el salto de 15 a 60 FPS llegó el último día.
2. Registrar horas reales desde el inicio: sin ellas no hay comparación económica.
3. Cerrar las fuentes antes de escribir el texto: varias afirmaciones de las fichas se corrigieron después contra sus fuentes.
4. Repartir el trabajo y los commits entre ambos integrantes para que el historial refleje el trabajo real.
5. Probar el paquete de entrega en otra computadora antes de la fecha límite.
