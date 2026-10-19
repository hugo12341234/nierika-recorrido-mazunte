# PLAN DE ACTIVIDADES — 60 actividades (D1 = 319266894, D2 = 321332086)

> Resp. = quien ejecuta · Verif. = quien revisa y aprueba el PR · Verificación: 0.5 h por actividad.
> Estado real de cada actividad: ver `ESTADO_PROYECTO.md`.

| N.º | Sem | Fecha | Actividad | Resp | Verif | h | Criterio de aceptación | Entregable | Prior. | Recurso |
|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 1 | Lu 28/09 | Investigar fuentes confiables sobre el Día de Muertos en Mazunte | D1 | D2 | 4 | 5 fuentes en APA 7 | Documento con referencias | Alta | — |
| 2 | 1 | Lu 28/09 | Redactar y corregir propuesta técnica con alcance reducido | D2 | D1 | 5 | Incluye justificación de recortes | Propuesta en PDF | Alta | — |
| 3 | 1 | Ma 29/09 | Crear repo en GitHub y configurar .gitignore para C++ | D2 | D1 | 1.5 | README inicial con estructura | Enlace al repositorio | Alta | GitHub |
| 4 | 1 | Mi 30/09 | Configurar VS con OpenGL, GLFW, GLAD, GLM y Assimp | D1 | D2 | 4 | Carpeta vendor/ con dependencias | Commit inicial compilando | Alta | LearnOpenGL |
| 5 | 1 | Mi 30/09 | Configurar tablero Kanban en Trello | D2 | D1 | 1 | Columnas: Pendiente, En proceso, En revisión, Terminado | Enlace al tablero | Media | Trello |
| 6 | 1 | Ju 01/10 | Instalar Blender y GIMP en ambos equipos | D2 | D1 | 1.5 | Verificar misma versión | Captura del software | Media | — |
| 7 | 1 | Vi 02/10 | Documentar estructura del cuarto con bocetos a mano | D1 | D2 | 2 | Fachada, interior y distribución de la ofrenda | Doc. para manual técnico | Media | — |
| 8 | 2 | Lu 05/10 | Modelar bloque base de la playa (plano 100×50 m subdividido) | D2 | D1 | 3 | Escala: 1 unidad = 1 m | .blend y .obj | Alta | Blender |
| 9 | 2 | Lu 05/10 | Programar clase Camera (matrices View y Projection) | D1 | D2 | 4 | Soporte para primera persona | Commit en develop | Alta | LearnOpenGL |
| 10 | 2 | Ma 06/10 | Modelar superficie del mar (malla 100×100 m subdividida) | D2 | D1 | 2 | Preparada para vertex shader | .blend y .obj | Media | Blender |
| 11 | 2 | Ma 06/10 | Callbacks GLFW para WASD y rotación con ratón | D2 | D1 | 3 | Bloquear pitch en 89° | Commit en develop | Alta | LearnOpenGL |
| 12 | 2 | Mi 07/10 | Sistema de colisiones AABB básico | D1 | D2 | 4 | Evitar que la cámara atraviese el suelo | Commit en develop | Media | LearnOpenGL |
| 13 | 2 | Ju 08/10 | Textura de arena con mapeo UV | D1 | D2 | 2 | Resolución 1K | Modelo texturizado + textures/ | Media | GIMP, Poly Haven, ambientCG |
| 14 | 2 | Ju 08/10 | Modelar 3 o 4 tumbas simples | D2 | D1 | 3 | Lápida y base | .blend y .obj | Media | Blender |
| 15 | 2 | Vi 09/10 | Ciclo principal con deltaTime y render de la playa | D1 | D2 | 3 | Base del ciclo de render | Commit en develop | Alta | LearnOpenGL |
| 16 | 3 | Lu 12/10 | Muros frontales de fachada con apertura para arco de medio punto | D1 | D2 | 3 | Textura rústica de piedra | .blend y .obj | Alta | Blender |
| 17 | 3 | Lu 12/10 | Arco de medio punto con dovelas visibles | D2 | D1 | 3 | Detalle arquitectónico obligatorio | .blend y .obj | Alta | Blender |
| 18 | 3 | Ma 13/10 | Piso interior, paredes laterales y techo plano | D2 | D1 | 3 | Sin vigas ni detalles innecesarios | .blend y .obj | Media | Blender |
| 19 | 3 | Mi 14/10 | Cruz de piedra simple para la entrada | D1 | D2 | 1.5 | Una sola, sin variantes | .blend y .obj | Baja | Blender |
| 20 | 3 | Mi 14/10 | Delimitación perimetral del panteón con muros bajos | D2 | D1 | 3 | Variantes de rocas para las bardas | .blend y .obj | Media | Blender |
| 21 | 3 | Ju 15/10 | Texturas de piedra rústica a muros y fachada | D1 | D2 | 2 | Coherencia con materiales reales | Modelo texturizado + textures/ | Media | GIMP, Poly Haven, ambientCG |
| 22 | 3 | Vi 16/10 | Soportes de madera para papel picado en el arco | D2 | D1 | 1.5 | Postes simples | .blend y .obj | Baja | Blender |
| 23 | 4 | Lu 19/10 | Vela cilíndrica con base y pivote en la base | D1 | D2 | 1.5 | Pivote correcto para animación | .blend y .obj | Media | Blender |
| 24 | 4 | Lu 19/10 | Altar de 3 niveles escalonados | D2 | D1 | 3 | Debe caber dentro del cuarto | .blend y .obj | Alta | Blender |
| 25 | 4 | Ma 20/10 | Malla plana para papel picado con UV para canal alfa | D2 | D1 | 1.5 | Subdividida | .blend y .obj | Media | Blender |
| 26 | 4 | Ma 20/10 | Botella de refresco de vidrio con modificador Screw | D1 | D2 | 2 | Exportar aplicando modificadores | .blend y .obj | Media | Blender |
| 27 | 4 | Mi 21/10 | Pan de muerto con detalles de huesos | D1 | D2 | 3 | Mantener Decimate activo | .blend y .obj | Media | Blender |
| 28 | 4 | Ju 22/10 | Flor de cempasúchil con planos cruzados | D2 | D1 | 2.5 | Optimizar para instancing | .blend y .obj | Media | Blender |
| 29 | 4 | Ju 22/10 | Marco de madera con plano interno para fotografía | D2 | D1 | 2 | UV exactas para la foto | .blend y .obj | Baja | Blender |
| 30 | 4 | Vi 23/10 | Copalero de barro como extra (sin partículas) | D1 | D2 | 2.5 | Hueco interno para futura emisión | .blend y .obj | Baja | Blender |
| 31 | 5 | Lu 26/10 | Descargar texturas PBR (albedo y normal, 1K) | D1 | D2 | 2 | Poly Haven y ambientCG | textures/ organizada | Media | GIMP, Poly Haven, ambientCG |
| 32 | 5 | Lu 26/10 | Texturizar altar con madera y mantel bordado | D2 | D1 | 2 | Materiales coherentes | Modelo texturizado + textures/ | Media | GIMP, Poly Haven, ambientCG |
| 33 | 5 | Ma 27/10 | Texturizar vela, papel picado y botella | D2 | D1 | 2.5 | Descarte por alfa para el papel | Modelo texturizado + textures/ | Media | GIMP, Poly Haven, ambientCG |
| 34 | 5 | Ma 27/10 | Texturizar pan de muerto, cempasúchil y marco | D1 | D2 | 2.5 | Mapa de relieve para el pan | Modelo texturizado + textures/ | Media | GIMP, Poly Haven, ambientCG |
| 35 | 5 | Mi 28/10 | Clase Model con Assimp para cargar todos los .obj | D1 | D2 | 4 | Rutas relativas | Commit en develop | Alta | LearnOpenGL |
| 36 | 5 | Ju 29/10 | Colocar los 7 objetos en el cuarto según la ofrenda | D2 | D1 | 2.5 | Verificar que quepan y se vean | Commit en develop | Alta | — |
| 37 | 5 | Ju 29/10 | Iluminación: ambiental cálida y 3 luces clave | D2 | D1 | 3.5 | Sin múltiples luces puntuales | Commit en develop | Alta | LearnOpenGL |
| 38 | 5 | Vi 30/10 | Ajustar posiciones, escalas y rotaciones en el cuarto | D1 | D2 | 2.5 | Paso crítico antes de animar | Commit en develop | Alta | — |
| 39 | 6 | Lu 02/11 | Vertex shader de olas con suma de senos | D1 | D2 | 4 | Frecuencias y amplitudes definidas | Commit en develop | Alta | GLSL; LearnOpenGL |
| 40 | 6 | Lu 02/11 | Fragment shader de llamas con ruido | D2 | D1 | 3 | Ruido Perlin o similar | Commit en develop | Alta | GLSL; LearnOpenGL |
| 41 | 6 | Ma 03/11 | Velas flotando: trayectoria Bézier + oscilación senoidal | D2 | D1 | 3.5 | Desfase entre velas | Commit en develop | Media | C++ y GLM |
| 42 | 6 | Mi 04/11 | Flores flotando: movimiento circular + rotación continua | D1 | D2 | 3 | Velocidad variable con pausas | Commit en develop | Media | C++ y GLM |
| 43 | 6 | Mi 04/11 | Integrar las 3 animaciones simples en el render | D1 | D2 | 2 | Olas, velas y flores | Commit en develop | Media | LearnOpenGL |
| 44 | 6 | Ju 05/11 | Materiales con descarte por alfa para flores y velas | D2 | D1 | 2.5 | Evitar superposición de transparencias | Commit en develop | Alta | LearnOpenGL |
| 45 | 6 | Vi 06/11 | Documentar justificación matemática de las 3 simples | D2 | D1 | 2 | Fórmulas y parámetros | Doc. para manual técnico | Media | C++ y GLM |
| 46 | 7 | Lu 09/11 | Silueta low-poly con 4 articulaciones por lado | D2 | D1 | 5 | Jerarquía básica de huesos | .blend y .obj | Alta | Blender |
| 47 | 7 | Lu 09/11 | Caminata con KeyFrames y slerp | D1 | D2 | 6 | Curvas de aceleración no lineales | Commit en develop | Alta | C++ y GLM |
| 48 | 7 | Ma 10/11 | Curvas de aceleración (smoothstep) y desfases de piernas | D2 | D1 | 2.5 | Documentar justificación | Evidencia en el tablero | Media | — |
| 49 | 7 | Ma 10/11 | Silueta dejando flores con Bézier cuadrática | D1 | D2 | 5 | Caída libre de la flor | Commit en develop | Alta | C++ y GLM |
| 50 | 7 | Mi 11/11 | Integrar ambas animaciones complejas | D2 | D1 | 3 | Caminata y dejar flores | Commit en develop | Alta | C++ y GLM |
| 51 | 7 | Ju 12/11 | Dear ImGui y ventanas de información | D1 | D2 | 5 | Detección por proximidad | Commit en develop | Alta | Dear ImGui |
| 52 | 7 | Ju 12/11 | Audio ambiental con miniaudio | D2 | D1 | 2.5 | Un solo ciclo: mar y música lejana | Commit en develop | Media | miniaudio |
| 53 | 7 | Vi 13/11 | Documentar justificación matemática de las 2 complejas | D1 | D2 | 2 | Slerp, Bézier y smoothstep | Doc. para manual técnico | Media | C++ y GLM |
| 54 | 8 | Lu 16/11 | Pruebas de integración y fugas de memoria | D2 | D1 | 4 | Verificar VAO y VBO | Lista de errores priorizada | Alta | Visual Studio |
| 55 | 8 | Lu 16/11 | Corrección de errores en cámara y colisiones | D1 | D2 | 3 | Evitar atascos en bardas | Commit de correcciones | Alta | LearnOpenGL |
| 56 | 8 | Lu 16/11 | Dependencias .dll y Release x64 | D1 | D2 | 2 | .exe autónomo en dist/ | Carpeta dist/ | Alta | Visual Studio |
| 57 | 8 | Ma 17/11 | Capturar los videos demostrativos | D2 | D1 | 3 | Entorno; recorrido; animaciones e interactividad | 3 archivos MP4 | Alta | OBS Studio |
| 58 | 8 | Ma 17/11 | Manual técnico (español e inglés) | D1 | D2 | 5 | Arquitectura, cronograma, RACI y conclusiones | Manual en PDF | Alta | — |
| 59 | 8 | Mi 18/11 | Manual de usuario (español e inglés) | D2 | D1 | 4 | Capturas y explicación de interacciones | Manual en PDF | Alta | — |
| 60 | 8 | Mi 18/11 | Publicar Release en GitHub y entregar | D1 | D2 | 1.5 | Release final y enlaces | Release y enlace de entrega | Alta | GitHub |
