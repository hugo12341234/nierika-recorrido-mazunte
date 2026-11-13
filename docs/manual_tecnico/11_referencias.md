# 11. Referencias (APA 7)

> Solo se listan fuentes **citadas en el manual**, tomadas de la bibliografía de la propuesta v2.0 (`docs/propuesta/Propuesta_Nierika_v2.0.pdf`, §12), del ESTADO o de `docs/fichas_culturales.md`. Las fechas «s. f.» de Poly Haven las definió D2 (ESTADO, A-6). **Falta que D2 compruebe cada enlace antes de entregar.**

## Tecnología

Assimp. (s. f.). *The Open Asset Import Library*. https://www.assimp.org/

Baglioni, C. (2023). *Sandy gravel* [Textura, CC0]. Poly Haven. https://polyhaven.com/a/sandy_gravel

Blender Foundation. (s. f.). *Features*. https://www.blender.org/features/

Cornut, O. (s. f.). *Dear ImGui*. GitHub. https://github.com/ocornut/imgui

de Vries, J. (2020). *Learn OpenGL: Learn modern OpenGL graphics programming in a step-by-step fashion*. Kendall & Welling. https://learnopengl.com/

GIMP. (s. f.). *GNU Image Manipulation Program*. https://www.gimp.org/

GLFW. (s. f.). *Documentation*. https://www.glfw.org/documentation

OBS Project. (s. f.). *OBS Studio*. https://obsproject.com/

Poly Haven. (s. f.). *License*. https://polyhaven.com/license

Reid, D. (s. f.). *miniaudio: Audio playback and capture library*. https://miniaud.io/

Segal, M., y Akeley, K. (2010). *The OpenGL graphics system: A specification* (Version 3.3, Core Profile). The Khronos Group. https://registry.khronos.org/OpenGL/specs/gl/glspec33.core.pdf

## Gestión

Anderson, D. J. (2010). *Kanban: Successful evolutionary change for your technology business*. Blue Hole Press.

## Fuentes culturales (de `docs/fichas_culturales.md`)

Baños, L. (2025, 27 de octubre). *Los pueblos mágicos de Oaxaca donde el Día de Muertos se vive con más alma*. Oaxaca Informa. https://www.oaxacainforma.com/es/cultura/2025/pueblos-magicos-oaxaca-dia-de-muertos-2025

Secretaría de Educación Pública. (2026, 22 de mayo). *Elementos básicos para la ofrenda de Día de Muertos*. Nueva Escuela Mexicana Digital. https://nuevaescuelamexicana.sep.gob.mx/contenido/recurso/elementos-basicos-para-la-ofrenda-de-dia-de-muerto

UNESCO. (s. f.). *Las fiestas indígenas dedicadas a los muertos*. https://ich.unesco.org/es/RL/las-fiestas-indigenas-dedicadas-a-los-muertos-00054

> Las fichas están marcadas «por verificar contra la fuente» (`fichas_culturales.md` §4): hasta que alguien lea cada fuente, estas tres referencias son las **previstas**, no verificadas por el equipo.

## Pendientes de formato

- 🔄 GLEW, GLM, vcpkg y SOIL2 ya tienen cita (X-15, 12/11, por verificar). stb_image también (X-16).
- 🔄 Slerp (Shoemake), Bézier (Farin) y Perlin ya tienen cita (X-15, por verificar); ver [§7.4](07_justificacion_matematica.md).

Savva, D., & Cilliers, R. (2021). *Clay floor 001* [Textura, CC0]. Poly Haven. https://polyhaven.com/a/clay_floor_001

Tuytel, R. (2020). *Large sandstone blocks 01* [Textura, CC0]. Poly Haven. https://polyhaven.com/a/large_sandstone_blocks_01

## Referencias de tecnología y matemática añadidas el 12/11 (X-15)

> Verificadas el 12/11/2026 (X-17) contra Crossref (DOI), GitHub, Poly Haven y las páginas oficiales; ver la tabla de verificación al final de este archivo.

Farin, G. (2002). *Curves and surfaces for CAGD: A practical guide* (5th ed.). Morgan Kaufmann.

g-truc. (s. f.). *OpenGL Mathematics (GLM)* [Software]. GitHub. https://github.com/g-truc/glm

SpartanJ. (s. f.). *SOIL2: Simple OpenGL Image Library 2* [Software]. GitHub. https://github.com/SpartanJ/SOIL2

Microsoft. (s. f.). *vcpkg: C++ library manager for Windows, Linux, and macOS* [Software]. GitHub. https://github.com/microsoft/vcpkg

Mikkelsen, M. S. (2010). Bump mapping unparametrized surfaces on the GPU. *Journal of Graphics, GPU, and Game Tools, 15*(1), 49–61.

Perlin, K. (1985). An image synthesizer. *ACM SIGGRAPH Computer Graphics, 19*(3), 287–296.

Perlin, K. (2002). Improving noise. *ACM Transactions on Graphics, 21*(3), 681–682.

Barrett, S. T. (s. f.). *stb: Single-file public domain libraries for C/C++* [Software]. GitHub. https://github.com/nothings/stb

Shoemake, K. (1985). Animating rotation with quaternion curves. *ACM SIGGRAPH Computer Graphics, 19*(3), 245–254.

The GLEW contributors. (s. f.). *The OpenGL Extension Wrangler Library (GLEW)* [Software]. https://glew.sourceforge.net/

## Verificación de referencias (X-17, 12/11/2026)

| Referencia | Contra qué se comprobó (acceso 12/11/2026) | Resultado |
|---|---|---|
| Perlin (1985) | Crossref, DOI 10.1145/325165.325247 | ✅ Ken Perlin; *An image synthesizer*; ACM SIGGRAPH Computer Graphics 19(3), 287–296 |
| Perlin (2002) | Crossref, DOI 10.1145/566654.566636 | ✅ *Improving noise*; ACM Transactions on Graphics 21(3), 681–682 |
| Shoemake (1985) | Crossref, DOI 10.1145/325334.325242 | ✅ Ken Shoemake; *Animating rotation with quaternion curves*; ACM SIGGRAPH Computer Graphics 19(3), 245–254 |
| Mikkelsen (2010) | Crossref | ✅ corregido a **Morten S. Mikkelsen**; Journal of Graphics, GPU, and Game Tools 15(1), 49–61 |
| Farin (2002) | Crossref (libro Elsevier/Morgan Kaufmann 2002, ISBN 9781558607378) y Open Library | 🔄 autor, año y editorial coinciden; **subtítulo y «5th ed.» no se pudieron confirmar** en esas fuentes |
| GLM | Página de GitHub `g-truc/glm` | 🔄 título correcto; **la página no nombra a una persona autora**: se cita al autor corporativo `g-truc` (antes decía «G-Truc Creation», sin respaldo) |
| SOIL2 | Página de GitHub `SpartanJ/SOIL2` | 🔄 título «Simple OpenGL Image Library 2»; **no nombra autor persona** (antes «Golini, M. L.», sin respaldo): se cita `SpartanJ`. Es un *fork* de SOIL de Jonathan Dummer |
| vcpkg | Página de GitHub `microsoft/vcpkg` (licencia MIT) | ✅ autor Microsoft; título corregido a «C++ library manager for Windows, Linux, and macOS» |
| stb_image | Página de GitHub `nothings/stb` | ✅ autor Sean T. Barrett (corregido «S. T.») |
| GLEW | Sitio `glew.sourceforge.net` | 🔄 título correcto; **el sitio no nombra autores**, se mantiene el autor colectivo |
| Textura Sandy Gravel | API de Poly Haven | ✅ Charlotte Baglioni; publicada en 2023 (antes «s. f.») |
| Textura Clay Floor 001 | API de Poly Haven | ✅ Dimitrios Savva (fotografía) y Rico Cilliers (procesamiento); 2021 (antes «s. f.») |
| Textura Large Sandstone Blocks 01 | API de Poly Haven | ✅ Rob Tuytel; 2020 (antes «s. f.») |

**Sin abrir:** las referencias de la propuesta v2.0 que no se tocan en X-17 (Assimp, GLFW, miniaudio, learnopengl, etc.) conservan su estado anterior.
