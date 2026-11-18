# m) Créditos y licencias

## m.1 Equipo

Nierika Studio, Equipo 15: D1 (319266894) y D2 (321332086). Proyecto final de Computación Gráfica e Interacción Humano-Computadora, Facultad de Ingeniería, UNAM, semestre 2027-1.

## m.2 Código

El código propio (`src/`, `Shaders/`, `tools/`) tiene licencia **MIT** (archivo `LICENSE`, «Nierika Studio (Equipo 15)»). D2 confirmó el 18/11/2026 que se usa la licencia MIT. Los modelos, texturas y audio **no** están cubiertos por esa licencia.

## m.3 Bibliotecas

| Biblioteca | Licencia según su página | Cómo se obtiene |
|---|---|---|
| GLFW | zlib/libpng | vcpkg |
| GLEW | no aparece en su sitio | vcpkg |
| GLM | Happy Bunny License (MIT modificada) o MIT | vcpkg |
| Assimp | BSD de 3 cláusulas modificada | vcpkg |
| stb_image 2.15 | Dominio público o MIT | `vendor/SOIL2` |
| SOIL2 | MIT-0 | `vendor/SOIL2` |
| Dear ImGui 1.91.9 | MIT | `vendor/imgui` |
| miniaudio 0.11.26 | Dominio público o MIT No Attribution | `vendor/` |

Las versiones de GLFW, GLEW, GLM y Assimp dependen de la línea base de vcpkg (`664933b8…`) y no se anotaron individualmente. Cada biblioteca conserva su propia licencia.

## m.4 Recursos propios y de terceros

| Recurso | Autoría | Licencia o condición |
|---|---|---|
| Modelos de Blender (inventario del anexo A) | El equipo; parte generada con *scripts* de `bpy` hechos con ayuda de IA | Propios. Sin licencia abierta |
| Modelos de Maya (calavera, pan, flor, altar provisional, objetos del altar) | D1 (según D2) | Propios. Sin licencia abierta; el permiso del retrato del cuadro, si se parece a una persona real, queda por confirmar |
| `laminado1.jpg`, `concreto.jpg` | El equipo, creadas en Blender (según D2) | Propias |
| Texturas procedurales (`piedra.jpg`, `mar_agua.png`, etc.) | El equipo, generadas con scripts de `tools/` | Propias |
| Arena de la playa (*Sandy gravel*) | Charlotte Baglioni, Poly Haven | CC0 |
| Piso de barro (*Clay floor 001*) | Dimitrios Savva y Rico Cilliers, Poly Haven | CC0 |
| Barda (*Large sandstone blocks 01*) | Rob Tuytel, Poly Haven | CC0 |
| Oleaje (`Audio/oleaje.mp3`, tramo de 123 s) | Canal «Quiet», YouTube (2020) | Uso educativo no comercial; no se confirmó una licencia libre. D2 debe confirmar canal y fecha |
| Música (`Audio/musica.mp3`) | «Dios Nuca Muere con la Banda Mixe de Oaxaca», canal «Ricardo Tizo», YouTube (2012) | Uso educativo no comercial; no se confirmó una licencia libre; no se identificó al compositor |
| Voz de la narración (12 mp3) | Síntesis con ElevenLabs, modelo Eleven v4 (2026) | No se encontró exigencia de atribución en las condiciones de uso del 9/10/2026; se nombra la herramienta en texto, sin logotipo; el plan contratado no está registrado |

## m.5 Límites de uso

El oleaje y la música son de terceros y se usan con fines educativos; **no se deben redistribuir comercialmente**. Los modelos, texturas y audio propios tampoco llevan licencia abierta. Las referencias completas están en la sección k y en la pantalla de Créditos del programa.

## m.6 Pendientes que solo el equipo puede cerrar

- ~~Texto «Pendiente: origen de laminado1.jpg» en Créditos~~: **cerrado**. La pantalla de Créditos ya declara `laminado1.jpg` y `concreto.jpg` como texturas propias hechas en Blender (X-69); si se ve el texto viejo, es un ejecutable anterior al 18/11 y hay que recompilar.
- ~~Licencia del código~~: **cerrado**. D2 confirmó por escrito el 18/11/2026 la licencia MIT (m.2).
- ~~Referencias APA de voz, música y texturas~~: **cerradas**. Están en la sección k y en `docs/REFERENCIAS.md` (68 fuentes con su estado de verificación) y en la pantalla de Créditos.
- Confirmar canal y fecha del oleaje: D2 debe abrir el video una vez (la lectura salió de metadatos). **Sigue abierto.**
- Fuente APA de la chilena (ver `justificacion_personas.md`): **no existe en el repositorio**; la coreografía se declara adaptación del estudio. **Sigue abierto.**
- Registrar las versiones de las bibliotecas de vcpkg, de Blender, Visual Studio y Maya.
