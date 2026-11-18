// Creditos.h - X-69: lista de creditos del menu (generada por tools/x69_generar_creditos.py a partir de docs/REFERENCIAS.md).
// NO editar a mano: cambia el script y vuelve a generarlo. Solo caracteres Latin-1 (fuente por defecto de ImGui). Una linea por referencia APA 7.
#pragma once

namespace Creditos
{
    struct Entrada { const char* texto; const char* detalle; };   // detalle puede ser nullptr
    struct Seccion { const char* titulo; const Entrada* e; int n; };

    inline const Entrada SEC0[] = {
        { "Números de cuenta: 321332086 · 319266894", nullptr },
        { "Proyecto final de Computación Gráfica e Interacción Humano-Computadora, semestre 2027-1.", nullptr },
        { "Escena, modelos (Blender y Maya), texturas propias (incluidas laminado1.jpg y concreto.jpg, hechas en Blender) y animaciones: hechos por el equipo con Blender, Maya y código propio (C++17 y OpenGL 3.3).", "Cada ficha cultural cita su fuente en APA 7 y dice cuándo algo es adaptación del estudio. La lista completa, con el estado de verificación de cada referencia, está en docs/REFERENCIAS.md." },
    };
    inline const Entrada SEC1[] = {
        { "Secretaría de Educación Pública. (2026, 22 de mayo). Elementos básicos para la ofrenda de Día de Muertos. Nueva Escuela Mexicana Digital. https://nuevaescuelamexicana.sep.gob.mx/contenido/recurso/elementos-basicos-para-la-ofrenda-de-dia-de-muertos/", nullptr },
        { "Baños, L. (2025, 27 de octubre). Los pueblos mágicos de Oaxaca donde el Día de Muertos se vive con más alma. Oaxaca Informa. https://www.oaxacainforma.com/es/cultura/2025/pueblos-magicos-oaxaca-dia-de-muertos-2025", nullptr },
        { "UNESCO. (s. f.). Las fiestas indígenas dedicadas a los muertos. https://ich.unesco.org/es/RL/las-fiestas-indigenas-dedicadas-a-los-muertos-00054", nullptr },
        { "México Desconocido. (2023, 19 de diciembre). Bandas de viento de Oaxaca, música popular que engalana las fiestas comunales. https://www.mexicodesconocido.com.mx/bandas-de-viento-de-oaxaca.html", nullptr },
        { "Instituto Nacional de Antropología e Historia. (2025, 27 de octubre). Tepehuas celebran a sus fieles difuntos en el Museo Nacional de Antropología (Boletín No. 562). https://inah.gob.mx/images/boletines/2025/562/Boletin%20562.pdf", nullptr },
        { "México Desconocido. (2026, 28 de octubre). Alumbradas de Oaxaca: una noche para reencontrarse con los difuntos. https://www.mexicodesconocido.com.mx/alumbradas-de-oaxaca.html", nullptr },
        { "La Razón Online. (2025, 1 de noviembre). Calles de Oaxaca se llenan de vida con la Gran Comparsa del Día de Muertos. La Razón. https://www.razon.com.mx/estados/2025/10/31/calles-de-oaxaca-se-llenan-de-vida-con-la-gran-comparsa-del-dia-de-muertos/", nullptr },
        { "Expansión Digital. (2025, 20 de octubre). Mariposa monarca: por qué se cree que son las almas que regresan el Día de Muertos. Expansión. https://expansion.mx/tendencias/2025/10/20/por-que-mariposas-monarca-son-almas-regresan-dia-de-muertos", nullptr },
        { "México Desconocido. (2010, 10 de agosto). El Centro Mexicano de la tortuga en Mazunte, Oaxaca. https://www.mexicodesconocido.com.mx/el-centro-mexicano-de-la-tortuga-en-mazunte-oaxaca.html", nullptr },
    };
    inline const Entrada SEC2[] = {
        { "México Desconocido. (2017, 28 de julio). El milagro de Mazunte. https://www.mexicodesconocido.com.mx/el-milagro-de-mazunte-oaxaca.html", nullptr },
        { "Secretaría de Turismo de Oaxaca. (s. f.). Plan de marketing del Pueblo Mágico Mazunte [Documento]. Gobierno del Estado de Oaxaca. https://www.oaxaca.gob.mx/sectur/wp-content/uploads/sites/65/2020/12/Plan-de-Marketing-Playa-Mazunte.pdf", nullptr },
        { "Gobierno del Estado de Oaxaca. (2019). Mazunte [Documento]. https://www.oaxaca.gob.mx/wp-content/uploads/sites/65/2019/10/MAZUNTE.pdf", nullptr },
        { "The continued devaluation of the Magic Town program: 28 more Magic Towns named. (2015, 28 de septiembre). Geo-Mexico. https://geo-mexico.com/?p=13269", nullptr },
        { "Cervantes-Hernández, P., Pérez-Vives, E., y Gómez-Ponce, M. A. (2017). Arribada y explotación de la tortuga golfina en la Playa Escobilla, Oaxaca, México. Revista Ciencias Marinas y Costeras, 9(1), 91-107. https://doi.org/10.15359/revmar.9-1.6", nullptr },
    };
    inline const Entrada SEC3[] = {
        { "Colegio de Ciencias y Humanidades. (s. f.). El papel picado, una expresión del Día de Muertos. Gaceta CCH, UNAM. https://gaceta.cch.unam.mx/es/el-papel-picado-una-expresion-del-dia-de-muertos", nullptr },
        { "Gobierno del Estado de Oaxaca. (s. f.). Las calles de Oaxaca se llenan de vida con la Gran Comparsa del Día de Muertos. https://www.oaxaca.gob.mx/comunicacion/las-calles-de-oaxaca-se-llenan-de-vida-con-la-gran-comparsa-del-dia-de-muertos/", nullptr },
        { "Instituto Nacional de los Pueblos Indígenas. (2019, 23 de octubre). ¿Conoces el significado de los elementos de una ofrenda de Día de Muertos? Gobierno de México. https://www.gob.mx/inpi/articulos/conoces-el-significado-de-los-elementos-de-una-ofrenda-de-dia-de-muertos", nullptr },
        { "Mazunte. (s. f.). Punta Cometa Mazunte: Mirador sagrado de México. https://mazunte.gob.mx/punta-cometa-mazunte-mirador-sagrado-de-mexico/", nullptr },
        { "México Desconocido. (s. f.). ¡Recorre los panteones de Oaxaca! Una tradición de Día de Muertos. https://www.mexicodesconocido.com.mx/panteones-de-oaxaca-dia-de-muertos.html", nullptr },
        { "Secretaría de Turismo. (2019, 5 de septiembre). Mazunte, Oaxaca. Gobierno de México. https://www.gob.mx/sectur/articulos/mazunte-oaxaca", nullptr },
        { "Google Arts & Culture. (s. f.). Día de Muertos. https://artsandculture.google.com/project/dia-de-muertos", nullptr },
        { "Instituto Nacional de Antropología e Historia. (s. f.). Recorridos virtuales. https://www.inah.gob.mx/interactivos/recorridos-virtuales", nullptr },
        { "Museo Memoria y Tolerancia. (s. f.). Museo digital. https://museodigital.myt.org.mx/", nullptr },
        { "Secretaría de Cultura. (s. f.). Recorrido virtual. Contigo en la Distancia. https://contigoenladistancia.cultura.gob.mx/lista/seccion/recorrido-virtual", nullptr },
    };
    inline const Entrada SEC4[] = {
        { "Tizo, R. (2012, 24 de octubre). Dios Nuca Muere con la Banda Mixe de Oaxaca [Video]. YouTube. https://www.youtube.com/watch?v=EyObXpnZsJg", "Música de fondo (Audio/musica.mp3). Externa, uso educativo no comercial. El título conserva la errata del video original." },
        { "Quiet. (2020, 31 de marzo). Relajarse Relax - Sonido Olas del Mar - Playa Atardecer HD [Video]. YouTube. https://www.youtube.com/watch?v=6TGHwQHdDlo", "Oleaje (Audio/oleaje.mp3): tramo de 123 s recortado en bucle por el equipo. Externo, uso educativo no comercial." },
        { "ElevenLabs. (2026). Eleven v4 [Modelo de síntesis de voz]. https://elevenlabs.io/docs/overview/models", "Voz de la narración del recorrido guiado (T). Sintética: lee un guion, no es un personaje." },
        { "ElevenLabs. (2026, 9 de octubre). Terms of use [Condiciones del servicio]. https://elevenlabs.io/terms-of-use", nullptr },
    };
    inline const Entrada SEC5[] = {
        { "Baglioni, C. (2023). Sandy gravel [Textura, CC0]. Poly Haven. https://polyhaven.com/a/sandy_gravel", "Arena de la playa." },
        { "Savva, D., y Cilliers, R. (2021). Clay floor 001 [Textura, CC0]. Poly Haven. https://polyhaven.com/a/clay_floor_001", "Piso del cuarto." },
        { "Tuytel, R. (2020). Large sandstone blocks 01 [Textura, CC0]. Poly Haven. https://polyhaven.com/a/large_sandstone_blocks_01", "Barda del panteón." },
        { "Poly Haven. (s. f.). License. https://polyhaven.com/license", "Licencia CC0. Las demás texturas son del equipo (generadas con scripts o creadas en Blender)." },
    };
    inline const Entrada SEC6[] = {
        { "GLFW. (s. f.). GLFW [Software]. https://www.glfw.org/", "Licencia zlib/libpng. Ventana, contexto, teclado y ratón." },
        { "The GLEW contributors. (s. f.). The OpenGL Extension Wrangler Library (GLEW) [Software]. https://glew.sourceforge.net/", "Carga de funciones de OpenGL." },
        { "g-truc. (s. f.). OpenGL Mathematics (GLM) [Software]. GitHub. https://github.com/g-truc/glm", "Licencia Happy Bunny (MIT modificada) o MIT. Vectores, matrices y cuaterniones." },
        { "Assimp. (s. f.). Open Asset Import Library (assimp) [Software]. GitHub. https://github.com/assimp/assimp", "Licencia BSD de 3 cláusulas modificada. Lectura de .obj." },
        { "Barrett, S. T. (s. f.). stb: Single-file public domain libraries for C/C++ [Software]. GitHub. https://github.com/nothings/stb", "Dominio público o MIT. stb_image v2.15 (en vendor/SOIL2)." },
        { "SpartanJ. (s. f.). SOIL2: Simple OpenGL Image Library 2 [Software]. GitHub. https://github.com/SpartanJ/SOIL2", "Licencia MIT-0. Bifurcación de SOIL de Jonathan Dummer." },
        { "Cornut, O. (s. f.). Dear ImGui [Software]. GitHub. https://github.com/ocornut/imgui", "Licencia MIT. Versión 1.91.9 (fichas, menú y subtítulos)." },
        { "Reid, D. (s. f.). miniaudio: Audio playback and capture library [Software]. https://miniaud.io/", "Dominio público o MIT No Attribution. Versión 0.11.26." },
        { "Microsoft. (s. f.). vcpkg: C++ library manager for Windows, Linux, and macOS [Software]. GitHub. https://github.com/microsoft/vcpkg", "Licencia MIT. Instala Assimp, GLFW, GLEW y GLM." },
        { "Blender Foundation. (s. f.). Blender [Software]. https://www.blender.org/", "Licencia GPL. Modelado, UV y exportación de .obj." },
        { "Autodesk. (s. f.). Maya [Software]. https://www.autodesk.com/products/maya/overview", "Modelos hechos por el equipo (calavera, pan, flor y capilla provisional)." },
        { "The GIMP Development Team. (s. f.). GIMP: GNU Image Manipulation Program [Software]. https://www.gimp.org/", nullptr },
        { "Microsoft. (s. f.). Visual Studio 2022 release notes [Software]. https://learn.microsoft.com/en-us/visualstudio/releases/2022/release-notes", nullptr },
        { "Python Software Foundation. (s. f.). Python [Software]. https://www.python.org/", "Scripts de tools/ y bpy." },
        { "FFmpeg. (s. f.). FFmpeg [Software]. https://ffmpeg.org/", "Medidas y recorte del audio." },
        { "Clark, J. A., Lundh, F., y Pillow Contributors. (s. f.). Pillow [Software]. https://python-pillow.org/", "Texturas procedurales." },
        { "NumPy. (s. f.). NumPy [Software]. https://numpy.org/", "Texturas procedurales." },
        { "OBS Project. (s. f.). OBS Studio [Software]. https://obsproject.com/", "Grabación de los videos." },
        { "Segal, M., y Akeley, K. (2010). The OpenGL graphics system: A specification (Version 3.3, Core Profile). The Khronos Group. https://registry.khronos.org/OpenGL/specs/gl/glspec33.core.pdf", nullptr },
        { "Kessenich, J., Baldwin, D., y Rost, R. (2010). The OpenGL shading language (Language version 3.30, Document revision 6). The Khronos Group. https://registry.khronos.org/OpenGL/specs/gl/GLSLangSpec.3.30.pdf", nullptr },
        { "International Organization for Standardization / International Electrotechnical Commission. (2017). Programming languages - C++ (ISO/IEC 14882:2017, Edición 5). https://www.iso.org/standard/68564.html", nullptr },
        { "de Vries, J. (2020). Learn OpenGL: Learn modern OpenGL graphics programming in a step-by-step fashion. Kendall & Welling. https://learnopengl.com/", nullptr },
        { "Anderson, D. J. (2010). Kanban: Successful evolutionary change for your technology business. Blue Hole Press.", nullptr },
    };
    inline const Entrada SEC7[] = {
        { "Phong, B. T. (1975). Illumination for computer generated pictures. Communications of the ACM, 18(6), 311-317. https://doi.org/10.1145/360825.360839", "Modelo de iluminación (base de Blinn-Phong)." },
        { "Blinn, J. F. (1977). Models of light reflection for computer synthesized pictures. ACM SIGGRAPH Computer Graphics, 11(2), 192-198. https://doi.org/10.1145/965141.563893", "Blinn-Phong y atenuación de luces." },
        { "Catmull, E., y Rom, R. (1974). A class of local interpolating splines. En Computer aided geometric design (pp. 317-326). Elsevier. https://doi.org/10.1016/B978-0-12-079050-0.50020-5", "Ruta de cámara del recorrido guiado." },
        { "Khronos Group. (s. f.). smoothstep. En OpenGL 4 reference pages. https://registry.khronos.org/OpenGL-Refpages/gl4/html/smoothstep.xhtml", "Interpolación suave (arranque y frenado)." },
        { "Shoemake, K. (1985). Animating rotation with quaternion curves. En Proceedings of the 12th annual conference on computer graphics and interactive techniques (SIGGRAPH '85) (pp. 245-254). ACM. https://doi.org/10.1145/325334.325242", "Cuaterniones y slerp (KeyFrames)." },
        { "Perlin, K. (1985). An image synthesizer. ACM SIGGRAPH Computer Graphics, 19(3), 287-296. https://doi.org/10.1145/325165.325247", "Ruido (llamas)." },
        { "Perlin, K. (2002). Improving noise. ACM Transactions on Graphics, 21(3), 681-682. https://doi.org/10.1145/566654.566636", "Ruido mejorado." },
        { "Farin, G. (2002). Curves and surfaces for CAGD: A practical guide (5th ed.). Morgan Kaufmann.", "Curvas de Bézier." },
        { "Mikkelsen, M. S. (2010). Bump mapping unparametrized surfaces on the GPU. Journal of Graphics, GPU, and Game Tools, 15(1), 49-61.", "Normales de relieve." },
        { "Akenine-Möller, T., Haines, E., y Hoffman, N. (2018). Real-time rendering (4th ed.). A K Peters/CRC Press.", "Referencia general de render en tiempo real." },
    };

    inline const Seccion SECCIONES[] = {
        { "Equipo Nierika Studio (Equipo 15)", SEC0, 3 },
        { "Fuentes de las fichas culturales y de la narración", SEC1, 9 },
        { "Otras fuentes consultadas sobre Mazunte", SEC2, 5 },
        { "Citadas en la propuesta v2.0", SEC3, 10 },
        { "Música, oleaje y voz", SEC4, 4 },
        { "Texturas de terceros (CC0, Poly Haven)", SEC5, 4 },
        { "Bibliotecas, software y estándares", SEC6, 23 },
        { "Bibliografía técnica y matemática", SEC7, 10 },
    };
    inline const int NUM_SECCIONES = 8;
}
