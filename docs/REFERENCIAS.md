# Referencias del proyecto (APA 7) · Nierika Studio

Equipo 15 · Números de cuenta: 321332086 · 319266894 · Proyecto final de Computación Gráfica e Interacción Humano-Computadora (FI UNAM, 2027-1)
Fecha de consulta de todas las páginas web de este archivo: **18 de noviembre de 2026** (hora de México), salvo donde se indique otra.

> **Qué es este archivo.** El inventario único de **todas** las fuentes que usa el proyecto: fichas culturales, narración, música y voz, texturas, bibliotecas, software y bibliografía técnica. Reúne y corrige lo que estaba repartido en `docs/manual_tecnico/11_referencias.md`, `docs/fichas_culturales.md`, `docs/investigacion_mazunte.md`, `Audio/README.md`, `src/Fichas.cpp` y `src/Menu.cpp`. **No cambia código, fichas, narración ni audio** (decisión de D2 del 18/11: ya no se modifica el proyecto): lo que no tiene respaldo directo en la fuente citada queda anotado aquí con honestidad (sección 7), sin tocar el proyecto.

## Cómo se verificó y su límite (léelo primero)

- Cada página se abrió el 18/11/2026 con la herramienta de lectura web del asistente (agentes de lectura), que **devuelve el texto o un resumen de la página; no es lectura humana**. Los DOI de artículos se comprobaron en la API de Crossref y los libros en Open Library.
- Leyenda de estado: 🤝 **confirmada por el equipo** (dato dado por D2 el 18/11/2026, no comprobado por el asistente) · ✅ **verificada** (título, autor/organización, fecha y, si aplica, el dato citado coinciden con la página) · 🔄 **parcial** (abrió, pero falta un dato o parte del texto solo llegó resumido) · ❌ **NO VERIFICADO** (no abrió, o no se pudo comprobar) · ⏳ **PENDIENTE** (falta un dato que solo D2 tiene).
- Cuando la página no muestra la fecha, la que se usa sale de sus **metadatos** (se dice en la nota). Un «s. f.» significa que ni la página ni sus metadatos dan fecha.
- **Segunda ronda (18/11, tras la respuesta de D2):** se volvieron a leer las fuentes de los hallazgos; el resultado está en cada entrada y en la sección 7. **Nada se rellenó de memoria.** Si un dato no apareció, dice NO APARECE.
- Sin logos ni datos personales (solo los dos números de cuenta).

---

## 1. Fuentes culturales y de contenido (fichas y narración)

### 1.1 Las que citan las 13 fichas de `src/Fichas.cpp` y la narración (`Audio/narracion/`)

**[C-01]** Secretaría de Educación Pública. (2026, 22 de mayo). *Elementos básicos para la ofrenda de Día de Muertos*. Nueva Escuela Mexicana Digital. https://nuevaescuelamexicana.sep.gob.mx/contenido/recurso/elementos-basicos-para-la-ofrenda-de-dia-de-muertos/
- Estado: ✅ Título, autoría (SEP) y fecha («22 de Mayo de 2026») coinciden. Comprobado en el texto: velas («su flama guía a los muertos de regreso a casa»), flores («aromatizan y guían a las almas al altar»), papel picado («da color y representa el aire»), pan de muerto («representa afecto y fraternidad»), copal («limpian el lugar y alejan malos espíritus»), agua y sal.
- Nota: la página atribuye «aromatizan y guían» a las flores en conjunto. La ficha 2 dice «el aroma de las flores, sobre todo del cempasúchil, guía…»: el matiz «sobre todo del cempasúchil» ya estaba en la fuente en la revisión del 12/11 (X-17); hoy no se repitió esa frase. La infografía (imagen) no se pudo leer; solo el texto de la página.
- Usada en: fichas 1, 2, 3, 4, 5, 7, 9 y 11; narración 01, 03, 06, 07, 08 y 11.

**[C-02]** Baños, L. (2025, 27 de octubre). *Los pueblos mágicos de Oaxaca donde el Día de Muertos se vive con más alma*. Oaxaca Informa. https://www.oaxacainforma.com/es/cultura/2025/pueblos-magicos-oaxaca-dia-de-muertos-2025
- Estado: 🔄 La URL abre y el título y la autora (Lili Baños) coinciden; la página menciona Mazunte. **Solo se leyeron literalmente ~39,900 de ~68,000 caracteres; la sección de Mazunte llegó en un resumen automático, no en texto literal.** Ese resumen **no es consistente**: en la primera lectura (18/11) no mostraba «al atardecer»; en la segunda (18/11, pedida solo para esa sección) sí decía que las ceremonias se hacen «al atardecer» y que «se lanzan flores y velas al mar para recordar a quienes partieron». **Conclusión honesta: «al atardecer» está respaldado solo por un resumen automático y no se pudo confirmar en texto literal** (ver H-1). Fecha 27/10/2025 y autoría: de la misma vía.
- Usada en: fichas 1, 2 y 9; narración 01 (y el título en la 00).

**[C-03]** UNESCO. (s. f.). *Las fiestas indígenas dedicadas a los muertos*. https://ich.unesco.org/es/RL/las-fiestas-indigenas-dedicadas-a-los-muertos-00054
- Estado: ✅ Título y organización coinciden; expediente n.º 00054, México, **inscrita en 2008** en la Lista Representativa del Patrimonio Cultural Inmaterial (la página no da fecha de publicación, por eso «s. f.»). La página dice que los «manjares favoritos del difunto» se colocan «alrededor del altar familiar y de la tumba» (la ficha lo parafrasea como «platillos»).
- Usada en: fichas 4, 8 y 9; narración 05 y 08.

**[C-04]** México Desconocido. (2023, 19 de diciembre). *Bandas de viento de Oaxaca, música popular que engalana las fiestas comunales*. https://www.mexicodesconocido.com.mx/bandas-de-viento-de-oaxaca.html
- Estado: ✅ Título, autoría («México Desconocido») y contenido coinciden: «además de dar identidad, hermanan a los pueblos indígenas del estado». **La fecha no se ve en la página; sale de los metadatos (2023-12-19).** El título dice «fiestas comunales»; el cuerpo del texto dice «fiestas municipales».
- Usada en: ficha 6; narración 04.

**[C-05]** Instituto Nacional de Antropología e Historia. (2025, 27 de octubre). *Tepehuas celebran a sus fieles difuntos en el Museo Nacional de Antropología* (Boletín No. 562). https://inah.gob.mx/images/boletines/2025/562/Boletin%20562.pdf
- Estado: ✅ Abre (PDF). Organización (INAH, Dirección de Medios de Comunicación), fecha («27 de octubre de 2025») y número (562) coinciden; el texto dice «en medio del humo del copal, adornan los sepúlcros». La frase exacta «panteón tepehua de Pisaflores» **no aparece**: el boletín nombra a la comunidad «lhima'alh'ama' (tepehua) de Pisaflores, Ixhuatlán de Madero, en Veracruz» y dice que «acuden al panteón» (ver H-4).
- Usada en: ficha 7; narración 06.

**[C-06]** México Desconocido. (2026, 28 de octubre). *Alumbradas de Oaxaca: una noche para reencontrarse con los difuntos*. https://www.mexicodesconocido.com.mx/alumbradas-de-oaxaca.html
- Estado: ✅ Título coincide. **La autoría que da la página es «advertorial MD»** (contenido de marca, no periodismo). **La fecha sale de los metadatos (2026-10-28).** Comprobado: «la noche del 31 de octubre», «las velas encendidas sobre las lápidas iluminan», y en Atzompa «una comparsa recorre el pueblo entre música y baile».
- Usada en: fichas 8 y 11; narración 05 y 09.

**[C-07]** La Razón Online. (2025, 1 de noviembre). *Calles de Oaxaca se llenan de vida con la Gran Comparsa del Día de Muertos*. La Razón. https://www.razon.com.mx/estados/2025/10/31/calles-de-oaxaca-se-llenan-de-vida-con-la-gran-comparsa-del-dia-de-muertos/
- Estado: ✅ Firma «Por: La Razón Online»; fecha de la firma «01/11/2025» (la URL dice 31/10; se usa la de la firma). Comprobado: «más de 50 contingentes», «catrinas, catrines, esqueletos».
- Usada en: ficha 10; narración 02.

**[C-08]** Expansión Digital. (2025, 20 de octubre). *Mariposa monarca: por qué se cree que son las almas que regresan el Día de Muertos*. Expansión. https://expansion.mx/tendencias/2025/10/20/por-que-mariposas-monarca-son-almas-regresan-dia-de-muertos
- Estado: ✅ Byline «Expansión Digital»; fecha «lun 20 octubre 2025». Comprobado: los purépechas las consideraron «portadoras del espíritu de quienes murieron» y «entre finales de octubre y principios de noviembre». La imagen del artículo se atribuye a «Gemini» (pie de foto), no a una fuente documental.
- Usada en: ficha 12; narración 10.

**[C-09]** México Desconocido. (2010, 10 de agosto). *El Centro Mexicano de la tortuga en Mazunte, Oaxaca*. https://www.mexicodesconocido.com.mx/el-centro-mexicano-de-la-tortuga-en-mazunte-oaxaca.html
- Estado: ✅ Título y autoría («México Desconocido») coinciden. **La fecha no se ve en la página; sale de los metadatos (publicado 2010-08-10, modificado 2020-05-29).** Segunda lectura (18/11) con texto literal: «Para la década de los años sesenta la explotación de estos reptiles se convirtió en la actividad preponderante de la población»; «en mayo de 1990, cuando se decreta la veda total de la tortuga»; «gracias al apoyo del CMT, hoy Mazunte ya no es ejemplo de devastación y sacrificio de tortugas»; «es la golfina la especie que más se reproduce». **Las dos afirmaciones de la ficha 13 que la primera lectura no encontró («años sesenta» y el apoyo del Centro) SÍ están en esta página** (la primera lectura estaba incompleta). Límites: es una sola fuente secundaria y **no da el año de creación del Centro** (menciona 1977 PIOSA, 1985 Instituto Nacional de la Pesca y «en 1992 nace el nuevo concepto»; ver H-3).
- Usada en: ficha 13; narración 00.

### 1.2 Investigación de los lados de la escena (`docs/investigacion_mazunte.md`, X-25)

**[C-10]** México Desconocido. (2017, 28 de julio). *El milagro de Mazunte*. https://www.mexicodesconocido.com.mx/el-milagro-de-mazunte-oaxaca.html
- Estado: ✅ Abre; título y autoría («México Desconocido») coinciden. **Fecha de los metadatos (2017-07-28)**; el documento de la investigación y el manual la citaban «s. f.». Contenido: tras la prohibición de la caza de 1990 la comunidad impulsó reforestación, ecoturismo y una cooperativa de cosméticos naturales (Cosméticos Naturales de Mazunte, 15 familias). **Dice que el Centro Mexicano de la Tortuga fue «abierto en 1994 por la Secretaría de Recursos Ambientales»**, y no le atribuye el paso a la conservación (lo atribuye a la comunidad); C-09 da otras fechas para el Centro (ver H-3).
- Usada en: `investigacion_mazunte.md` (pesca, lanchas y cambio del pueblo). Ninguna ficha ni la narración la citan.

**[C-11]** Secretaría de Turismo de Oaxaca. (s. f.). *Plan de marketing del Pueblo Mágico Mazunte* [Documento]. Gobierno del Estado de Oaxaca. https://www.oaxaca.gob.mx/sectur/wp-content/uploads/sites/65/2020/12/Plan-de-Marketing-Playa-Mazunte.pdf
- Estado: 🔄 Abre (PDF; se leyeron ~39,900 de ~56,400 caracteres). Título del encabezado: «PLAN DE MARKETING DEL PUEBLO MÁGICO MAZUNTE»; autoría: «Dirección de Comercialización Turística, Secretaría de Turismo del Gobierno del Estado de Oaxaca». **El documento no declara año**: el «2020» que usaban los demás archivos sale solo de la ruta de subida (`/2020/12/`), por eso va «s. f.». El texto dice que Mazunte fue declarado Pueblo Mágico **«el 28 de septiembre del 2015»**: **es la fecha que se usa en el proyecto** (ver H-12).
- Usada en: `investigacion_mazunte.md`; manual técnico §11.

**[C-12]** ❌ Gobierno del Estado de Oaxaca. (2019). *Mazunte* [Documento]. https://www.oaxaca.gob.mx/wp-content/uploads/sites/65/2019/10/MAZUNTE.pdf
- Estado: ❌ **NO VERIFICADO**: la URL no abrió el 18/11 (error de cliente). Título, organismo y año vienen solo de `investigacion_mazunte.md` (que ya la marcaba «por verificar»). No se usa en ninguna ficha ni narración; si no se abre, quitarla del manual (§11) o citarla sin datos de más.

**[C-23]** *The continued devaluation of the Magic Town program: 28 more Magic Towns named*. (2015, 28 de septiembre). Geo-Mexico. https://geo-mexico.com/?p=13269
- Estado: ✅ (solo para lo que se cita) Entrada de blog sin autor visible, fechada 28/09/2015; dice que la Secretaría de Turismo anunció 28 nuevos Pueblos Mágicos en la Segunda Feria Anual de Pueblos Mágicos en Puebla («recently») y **Mazunte está en la lista**. **No da el día de la declaratoria**: el 28/09 es la fecha de la entrada. El título es opinión del blog; se cita solo para confirmar que Mazunte fue nombrado en septiembre de 2015.

**[C-24]** Cervantes-Hernández, P., Pérez-Vives, E., y Gómez-Ponce, M. A. (2017). Arribada y explotación de la tortuga golfina en la Playa Escobilla, Oaxaca, México. *Revista Ciencias Marinas y Costeras, 9*(1), 91–107. https://doi.org/10.15359/revmar.9-1.6
- Estado: ✅ (datos bibliográficos del encabezado del PDF, leído en https://www.redalyc.org/pdf/6337/633766726006.pdf). **No se usa en ninguna ficha ni narración**: se anota solo porque su fecha de la veda permanente («a partir del 16 de marzo de 1994», Pacífico mexicano; vedas desde 1972; datos de captura de Escobilla de 1978 a mayo de 1990) **no coincide con la «veda total de mayo de 1990»** de C-09 (ver H-3). No habla de Mazunte como pueblo, solo como sede del Centro que recopiló los datos.

### 1.3 Citadas en la propuesta v2.0 (PDF entregado), no usadas en fichas ni narración

Textos copiados tal cual de `docs/propuesta/Propuesta_Nierika_v2.0.pdf` (§ Referencias). **Hoy no se abrieron** salvo lo indicado; ❌ **NO VERIFICADAS**. La propuesta es un documento entregado: no se edita; si alguna se usa en el manual, hay que abrirla primero.

**[C-13]** Colegio de Ciencias y Humanidades. (s. f.). *El papel picado, una expresión del Día de Muertos*. Gaceta CCH, UNAM. https://gaceta.cch.unam.mx/es/el-papel-picado-una-expresion-del-dia-de-muertos

**[C-14]** Gobierno del Estado de Oaxaca. (s. f.). *Las calles de Oaxaca se llenan de vida con la Gran Comparsa del Día de Muertos*. https://www.oaxaca.gob.mx/comunicacion/las-calles-de-oaxaca-se-llenan-de-vida-con-la-gran-comparsa-del-dia-de-muertos/

**[C-15]** Instituto Nacional de los Pueblos Indígenas. (2019, 23 de octubre). *¿Conoces el significado de los elementos de una ofrenda de Día de Muertos?* Gobierno de México. https://www.gob.mx/inpi/articulos/conoces-el-significado-de-los-elementos-de-una-ofrenda-de-dia-de-muertos

**[C-16]** Mazunte. (s. f.). *Punta Cometa Mazunte: Mirador sagrado de México*. https://mazunte.gob.mx/punta-cometa-mazunte-mirador-sagrado-de-mexico/ (Punta Cometa está fuera de alcance.)

**[C-17]** México Desconocido. (s. f.). *¡Recorre los panteones de Oaxaca! Una tradición de Día de Muertos*. https://www.mexicodesconocido.com.mx/panteones-de-oaxaca-dia-de-muertos.html

**[C-18]** Secretaría de Turismo. (2019, 5 de septiembre). *Mazunte, Oaxaca*. Gobierno de México. https://www.gob.mx/sectur/articulos/mazunte-oaxaca — ❌ se intentó abrir el 18/11: **no abrió**; la fecha del 5/09/2019 no se pudo confirmar.

**[C-19]** Google Arts & Culture. (s. f.). *Día de Muertos*. https://artsandculture.google.com/project/dia-de-muertos

**[C-20]** Instituto Nacional de Antropología e Historia. (s. f.). *Recorridos virtuales*. https://www.inah.gob.mx/interactivos/recorridos-virtuales

**[C-21]** Museo Memoria y Tolerancia. (s. f.). *Museo digital*. https://museodigital.myt.org.mx/

**[C-22]** Secretaría de Cultura. (s. f.). *Recorrido virtual. Contigo en la Distancia*. https://contigoenladistancia.cultura.gob.mx/lista/seccion/recorrido-virtual

(C-19 a C-22 son el «estado del arte» de la propuesta; la propuesta también lista fuentes de costos y gestión que no se reproducen aquí porque ningún manual ni código las usa.)

---

## 2. Música, audio y voz

**[A-01] Oleaje** (`Audio/oleaje.mp3`; externo, uso educativo no comercial; tramo de 123 s recortado en bucle por el equipo, ver `Audio/README.md`).
Quiet. (2020, 31 de marzo). *Relajarse Relax - Sonido Olas del Mar - Playa Atardecer HD* [Video]. YouTube. https://www.youtube.com/watch?v=6TGHwQHdDlo
- Estado: 🔄 Cierra el pendiente «canal / año» de `Audio/README.md`: la lectura devolvió **canal «Quiet»** y **«Premiered Mar 31, 2020»** (estreno). YouTube mostró una página anti-bot, así que esos dos datos salieron de los **metadatos incrustados**, no de la página visible: **D2 debe abrir el enlace una vez y confirmar canal y fecha** (⏳). Duración no verificada. Consulta: 18/11/2026.

**[A-02] Música de fondo** (`Audio/musica.mp3`; D-6k; externo, uso educativo no comercial).
Tizo, R. (2012, 24 de octubre). *Dios Nuca Muere con la Banda Mixe de Oaxaca* [Video]. YouTube. https://www.youtube.com/watch?v=EyObXpnZsJg
- Estado: 🤝 **D2 confirmó (18/11/2026) que `Audio/musica.mp3` es ese video del canal «Ricardo Tizo»**. La página con el id `EyObXpnZsJg` (sufijo del nombre del archivo que D2 subió el 17/11) da: **título «Dios Nuca Muere con la Banda Mixe de Oaxaca»** (con la errata «Nuca» tal como está publicado; en APA se respeta el título original), **canal «Ricardo Tizo»**, **fecha 24/10/2012**, descripción solo «Villa Hidalgo Yalàlag» (título, canal y fecha leídos de metadatos, no de la página visible).
- **Lo que sigue sin respaldo:** **quién compuso la obra «Dios Nunca Muere»** (no se encontró en ninguna fuente abierta; no se afirma) y qué banda concreta toca (solo el título del video dice «Banda Mixe de Oaxaca»; ver H-2).
- La pista anterior («Xochipitzahuatl», náhuatl) ya no está en el repo; no se cita.

**[A-03] Voz de la narración del recorrido guiado** (`Audio/narracion/narracion_00..11.mp3`, 12 archivos, X-62; la voz es sintética y **lee un guion, no es un personaje**).
ElevenLabs. (2026). *Eleven v4* [Modelo de síntesis de voz]. https://elevenlabs.io/docs/overview/models
- Estado: 🤝 **D2 confirmó (18/11/2026): la voz salió de ElevenLabs** (herramienta; año de uso 2026; **sin autor de voz pública**, por eso la referencia no lleva creador de voz). **Modelo:** la página de ElevenLabs lista «Eleven v4» (id `eleven_v4`, «our most emotionally rich, expressive speech synthesis model»; publicador ElevenLabs); la página **no da fecha de lanzamiento** y el 2026 es el año de uso que indica D2. El nombre «El Faraon» lo dio el equipo; **no figura en la biblioteca pública de voces** (no se abrió la búsqueda interactiva) y no se cita como autor.
- No se oyeron los audios. El plan de ElevenLabs con que se generaron no está registrado; los términos dicen que el plan gratuito es solo para uso no comercial y el de pago permite uso comercial (el proyecto es educativo, no comercial).

**[A-04]** ElevenLabs. (2026, 9 de octubre). *Terms of use* [Condiciones del servicio]. https://elevenlabs.io/terms-of-use
- Estado: 🔄 Abre (versión no EEE, «Last Updated: 09 October 2026»; leída hasta la sección 12, el resto resumido). **No se encontró exigencia de atribución** del audio generado; el nombre y logotipos de ElevenLabs no se pueden usar sin permiso (por eso los créditos nombran la herramienta en texto, sin logo). No se leyó el *Voice Library Addendum* (https://elevenlabs.io/vla), que puede tener condiciones propias para las voces de la biblioteca.

---

## 3. Texturas y modelos externos

### 3.1 Poly Haven (licencia CC0; el crédito no es obligatorio, pero se da)

**[T-01]** Baglioni, C. (2023). *Sandy gravel* [Textura, CC0]. Poly Haven. https://polyhaven.com/a/sandy_gravel — ✅ Autora Charlotte Baglioni, licencia CC0; la página dice «Released: 3 years ago» (consistente con 2023; el año exacto sale de la API de Poly Haven, X-17). Uso: `Models/playa/arena.jpg` (arena de la playa).

**[T-02]** Savva, D., y Cilliers, R. (2021). *Clay floor 001* [Textura, CC0]. Poly Haven. https://polyhaven.com/a/clay_floor_001 — ✅ Dimitrios Savva (fotografía) y Rico Cilliers (procesamiento), CC0; «Released: 5 years ago» (consistente con 2021). Uso: `Models/piso/piso_barro.jpg`.

**[T-03]** Tuytel, R. (2020). *Large sandstone blocks 01* [Textura, CC0]. Poly Haven. https://polyhaven.com/a/large_sandstone_blocks_01 — ✅ Rob Tuytel, CC0; «Released: 6 years ago» (consistente con 2020). Uso: `Models/barda/barda_arenisca.jpg`.

**[T-04]** Poly Haven. (s. f.). *License*. https://polyhaven.com/license — ✅ Todos los assets son CC0 («effectively Public Domain»), uso libre incluso comercial; «You do not need to give credit… although it is appreciated».

### 3.2 Texturas de la fachada (propias, confirmado por D2) y modelos de Maya de D1 (pendiente)

**[T-05] `laminado1.jpg`** (512×512; fachada y techo): 🤝 **textura propia: D2 confirmó el 18/11/2026 que la crearon ellos en Blender.** No es una fuente externa y no lleva referencia (el historial de git y el archivo no lo contradicen, pero tampoco lo prueban: sin metadatos, llegó en el commit `5ef12a2` del 19/10 junto con la fachada de Blender 4.4.3; SHA-1 `6a3bc76bca9f…`). El menú de créditos aún dice «Pendiente: origen de laminado1.jpg» (`src/Menu.cpp`): ese texto quedó desactualizado, y no se modifica por decisión de D2.

**[T-06] `concreto.jpg`** (arco, fachada y piso): 🤝 **textura propia, creada en Blender (confirmado por D2 el 18/11/2026)**; mismas condiciones que T-05 (mismo commit `5ef12a2`). No lleva referencia y no aparecía en créditos (solo cae bajo «las demás texturas son del equipo», H-6).

**[T-07] Modelos de Maya de D1** (`calavera`, `pan`/`pan_muerto` de D1, `flor`, `altar` (capilla provisional), y el zip del altar subido en X-43: `cuadro`, `fuego`, `humo`, `mariposa_*`, `mazeta`, `refresco`, `papel_fucsia`, `papel_morado`, `papel_naranja`): ⏳ **PENDIENTE — autoría y herramienta sin confirmar.** El ESTADO los marca como extras que **no cuentan como propios**; D1 se los pasó a D2, la herramienta con la que se hicieron **no está confirmada** (las texturas tienen formato de generador automático) y el retrato del `cuadro` parece una persona real (**permiso sin confirmar**). Sin esos datos no se puede escribir su referencia APA. D1 debe indicar herramienta, fuente y si hay terceros.

### 3.3 Texturas y modelos propios (no llevan referencia)

`piedra.jpg` (procedural, Voronoi, A-12), `mar_agua.png`, `altar3_tela.png`, `vela_cera.png`, `botella_vidrio.png`, `marco_madera.png`, `copalero_barro.png`, `pan_muerto_relieve.png`, `cempasuchil.png`, `papel_picado.png`, `pan_muerto.png`, tumbas, cruz, soportes, silueta, palma y arbusto: generadas por los scripts de `tools/` o modeladas en Blender por el equipo (manual técnico §6.4). Son **adaptación o creación del estudio**, no fuentes externas.

---

## 4. Bibliotecas y software

Las versiones **vendorizadas** (en `vendor/`) salen de los encabezados de los archivos del repo (✅ comprobado); las versiones de las que instala **vcpkg** no se conocen: `vcpkg.json` fija solo el `builtin-baseline` `664933b8d385dfc01f4e0cdff4c7c74e71f6a740` (⏳ para anotarlas: en la PC de D2, `vcpkg list` desde la raíz). Las «últimas versiones» de la columna son lo que mostró cada página el 18/11, no necesariamente lo que usa el proyecto.

| ID | Referencia APA 7 | Licencia (según la página) | Versión en el proyecto | Estado |
|---|---|---|---|---|
| **S-01** | GLFW. (s. f.). *GLFW* [Software]. https://www.glfw.org/ | zlib/libpng | vcpkg (baseline). Página: 3.5.1 (31/07/2026) | ✅ (sin autor persona en la página) |
| **S-02** | The GLEW contributors. (s. f.). *The OpenGL Extension Wrangler Library (GLEW)* [Software]. https://glew.sourceforge.net/ | **NO APARECE** en la página | vcpkg (baseline). Página: 2.3.1 (24/01/2026) | 🔄 el sitio no nombra autores ni licencia |
| **S-03** | g-truc. (s. f.). *OpenGL Mathematics (GLM)* [Software]. GitHub. https://github.com/g-truc/glm | «Happy Bunny License (Modified MIT) or the MIT License» | vcpkg (baseline). README: 1.0.3 (31/12/2025) | 🔄 página truncada; el repo figura como fork de `icaven/glm` |
| **S-04** | Assimp. (s. f.). *Open Asset Import Library (assimp)* [Software]. GitHub. https://github.com/assimp/assimp | «modified, 3-clause BSD-License» | vcpkg (baseline) | ✅ (el sitio https://www.assimp.org/ pone «Kim Kulling» en el pie y «BSD license»; versión no verificada) |
| **S-05** | Barrett, S. T. (s. f.). *stb: Single-file public domain libraries for C/C++* [Software]. GitHub. https://github.com/nothings/stb | Dominio público o MIT, a elección | `stb_image` **v2.15** (encabezado de `vendor/SOIL2/stb_image.h`) | ✅ |
| **S-06** | SpartanJ. (s. f.). *SOIL2: Simple OpenGL Image Library 2* [Software]. GitHub. https://github.com/SpartanJ/SOIL2 | «MIT-0 (see LICENSE file)» | `vendor/SOIL2` (versión no registrada) | ✅ (fork de SOIL de Jonathan Dummer, según la página) |
| **S-07** | Cornut, O. (s. f.). *Dear ImGui* [Software]. GitHub. https://github.com/ocornut/imgui | MIT | **1.91.9** (`IMGUI_VERSION` en `vendor/imgui/imgui.h`). Página: última v1.93.0 | ✅ (fecha de la release 1.91.9 NO APARECE) |
| **S-08** | Reid, D. (s. f.). *miniaudio: Audio playback and capture library* [Software]. https://miniaud.io/ | Dominio público o MIT No Attribution | **0.11.26** (encabezado: «v0.11.26 - 2026-10-31»). GitHub mostraba 0.11.25 (04/03/2026) como «Latest» | ✅ (la versión de `vendor/` es posterior a la que mostró GitHub; ver H-9) |
| **S-09** | Microsoft. (s. f.). *vcpkg: C++ library manager for Windows, Linux, and macOS* [Software]. GitHub. https://github.com/microsoft/vcpkg | MIT (los puertos conservan su licencia) | baseline `664933b8…` | ✅ |
| **S-10** | Blender Foundation. (s. f.). *Blender* [Software]. https://www.blender.org/ | GPL (código v2 o posterior; binarios v3 o posterior; Cycles Apache 2.0) — https://www.blender.org/about/license/ | 4.4.3 (fachada), 5.2.x y `bpy` 5.2/5.3 según el documento (ver H-7); página de releases ❌ no abrió | 🔄 |
| **S-11** | The GIMP Development Team. (s. f.). *GIMP: GNU Image Manipulation Program* [Software]. https://www.gimp.org/ | La página no nombra la licencia del programa (el sitio web es CC BY-SA 4.0) | no registrada; página: 3.2.8 (02/11/2026) | 🔄 (ver H-8: sin uso registrado) |
| **S-12** | Microsoft. (s. f.). *Visual Studio 2022 release notes* [Software]. https://learn.microsoft.com/en-us/visualstudio/releases/2022/release-notes | Comercial (Professional, según la propuesta) | Visual Studio 2022; subversión exacta no registrada | 🔄 la página (rama 17.14) solo llegó parcial |
| **S-13** | Autodesk. (s. f.). *Maya* [Software]. https://www.autodesk.com/products/maya/overview | Comercial (suscripción) | **versión de D1 no registrada** (⏳) | 🔄 (la página muestra «Maya 2027») |
| **S-14** | Python Software Foundation. (s. f.). *Python* [Software]. https://www.python.org/ | PSF (la página enlaza la licencia, no la nombra) | `bpy` y scripts de `tools/` (versión no registrada) | 🔄 |
| **S-15** | FFmpeg. (s. f.). *FFmpeg* [Software]. https://ffmpeg.org/ | NO APARECE en la página | usado en las medidas de `Audio/README.md` y el recorte de audio (versión no registrada). Página: 8.1 (16/03/2026) | 🔄 |
| **S-16** | Clark, J. A., Lundh, F., y Pillow Contributors. (s. f.). *Pillow* [Software]. https://python-pillow.org/ | NO APARECE en la página | usado por `tools/` (texturas procedurales) | 🔄 (autores según el metadato de la página) |
| **S-17** | NumPy. (s. f.). *NumPy* [Software]. https://numpy.org/ | «BSD license» (variante no indicada) | usado por `tools/b12_texturas.py` y otros | 🔄 |
| **S-18** | OBS Project. (s. f.). *OBS Studio* [Software]. https://obsproject.com/ | GPL (según la propuesta) | grabación de videos (E-4, aún pendiente) | ❌ no abierta hoy (cita de la propuesta) |

**Estándares y documentación técnica**

**[S-19]** Segal, M., y Akeley, K. (2010). *The OpenGL graphics system: A specification* (Version 3.3, Core Profile). The Khronos Group. https://registry.khronos.org/OpenGL/specs/gl/glspec33.core.pdf — ❌ **no abrió el 18/11** (error de cliente); la cita viene de la propuesta y de X-15, que ya no la tenía verificada. No se afirman autores ni fecha de más.

**[S-20]** Kessenich, J., Baldwin, D., y Rost, R. (2010). *The OpenGL shading language* (Language version 3.30, Document revision 6). The Khronos Group. https://registry.khronos.org/OpenGL/specs/gl/GLSLangSpec.3.30.pdf — ✅ Abre (PDF; texto parcial). Portada: «Language Version 3.30», «Document Revision 6», 11 de marzo de 2010; editor John Kessenich; autores de la versión 1.1: Kessenich, Baldwin y Rost. **Es la referencia de GLSL `#version 330 core`**, que el proyecto usa y la propuesta no citaba.

**[S-21]** International Organization for Standardization / International Electrotechnical Commission. (2017). *Programming languages — C++* (ISO/IEC 14882:2017, Edición 5). https://www.iso.org/standard/68564.html — ✅ Abre. Estado en la página: «Withdrawn» (retirada; el sustituto no aparece en el texto leído). Es el estándar de C++17.

**[S-22]** de Vries, J. (2020). *Learn OpenGL: Learn modern OpenGL graphics programming in a step-by-step fashion*. Kendall & Welling. https://learnopengl.com/ — ❌ cita de la propuesta, **no abierta hoy**.

**[S-23]** Anderson, D. J. (2010). *Kanban: Successful evolutionary change for your technology business*. Blue Hole Press. — ❌ cita de la propuesta (gestión), sin comprobar.

**Herramientas de apoyo sin referencia propia:** Git y GitHub, Trello, Google Docs/Sheets (gestión del equipo); Claude (asistente de IA, **no es una fuente**: el manual técnico §3.1 ya declara su uso; la redacción final es del equipo).

---

## 5. Bibliografía técnica y matemática de animaciones e iluminación

Solo se citan técnicas que el proyecto **usa de verdad** (comprobado en el código o en `docs/manual_tecnico/justificacion_*.md`). Columna «Dónde» = lo que se verificó en el repo.

| ID | Técnica | Dónde en el proyecto | Referencia APA 7 | Estado |
|---|---|---|---|---|
| **M-01** | Modelo de reflexión de Phong (base de Blinn-Phong) | `Shaders/lighting.frag` (componentes ambiente, difusa y especular) | Phong, B. T. (1975). Illumination for computer generated pictures. *Communications of the ACM, 18*(6), 311–317. https://doi.org/10.1145/360825.360839 | ✅ Crossref |
| **M-02** | Blinn-Phong: vector medio H = normalize(L + V) | `BlinnSpec` en `lighting.frag`; atenuación de luces puntuales (`constant`, `linear`, `quadratic`) | Blinn, J. F. (1977). Models of light reflection for computer synthesized pictures. *ACM SIGGRAPH Computer Graphics, 11*(2), 192–198. https://doi.org/10.1145/965141.563893 | ✅ Crossref |
| **M-03** | Spline de Catmull-Rom (ruta de cámara del recorrido guiado) | `docs/RECORRIDO_GUIADO.md` §4; `src/Recorrido.cpp` | Catmull, E., y Rom, R. (1974). A class of local interpolating splines. En *Computer aided geometric design* (pp. 317–326). Elsevier. https://doi.org/10.1016/B978-0-12-079050-0.50020-5 | 🔄 Crossref confirma autores, año, páginas, DOI e ISBN 9780120790500 y lista a Elsevier como editorial; **no da los editores del libro** (la propuesta o el manual podrían decir «Academic Press»: sin confirmar, no se usa) |
| **M-04** | Interpolación suave de Hermite, `smoothstep(τ) = 3τ² − 2τ³` (arranque/frenado de la silueta y de la cámara) | `justificacion_complejas.md` §2; `RECORRIDO_GUIADO.md` §4 | Khronos Group. (s. f.). *smoothstep*. En *OpenGL 4 reference pages*. https://registry.khronos.org/OpenGL-Refpages/gl4/html/smoothstep.xhtml | ✅ Abre; define `t*t*(3.0 − 2.0*t)` con `t = clamp((x − edge0)/(edge1 − edge0), 0, 1)`. Sin fecha (copyright 2011–2014) y sin autor persona |
| **M-05** | Cuaterniones y **slerp** (KeyFrames de la silueta) | `justificacion_complejas.md` §1 | Shoemake, K. (1985). Animating rotation with quaternion curves. En *Proceedings of the 12th annual conference on computer graphics and interactive techniques (SIGGRAPH '85)* (pp. 245–254). ACM. https://doi.org/10.1145/325334.325242 | ✅ Crossref: Ken Shoemake, págs. 245–254, actas de SIGGRAPH '85; ver H-10 |
| **M-06** | Ruido de Perlin (llama de las velas; base del fbm) | `justificacion_simples.md` §2; `ruidoFbm` en `lighting.frag` | Perlin, K. (1985). An image synthesizer. *ACM SIGGRAPH Computer Graphics, 19*(3), 287–296. https://doi.org/10.1145/325165.325247 | ✅ Crossref |
| **M-07** | Ruido mejorado de Perlin (curva de fundido) | misma | Perlin, K. (2002). Improving noise. *ACM Transactions on Graphics, 21*(3), 681–682. https://doi.org/10.1145/566654.566636 | ✅ Crossref |
| **M-08** | Curvas de **Bézier** (flor que cae, velas flotando) | `justificacion_complejas.md` §3; `justificacion_simples.md` §2 | Farin, G. (2002). *Curves and surfaces for CAGD: A practical guide* (5th ed.). Morgan Kaufmann. | 🔄 Autor, año y editorial coinciden (X-17); **«5th ed.» y el subtítulo no se pudieron confirmar** y la búsqueda de hoy no los encontró: ⏳ confirmar con el libro físico o la editorial |
| **M-09** | Normal de relieve sin tangentes (`perturbaNormal`, mapa de altura del pan y de la barda) | `lighting.frag` | Mikkelsen, M. S. (2010). Bump mapping unparametrized surfaces on the GPU. *Journal of Graphics, GPU, and Game Tools, 15*(1), 49–61. | 🔄 ✅ en Crossref el 12/11 (X-17); **hoy no se repitió** (sin DOI a mano) |
| **M-10** | Panorama general de render en tiempo real (Blinn-Phong, atenuación, texturas, colisiones AABB) | marco general | Akenine-Möller, T., Haines, E., y Hoffman, N. (2018). *Real-time rendering* (4th ed.). A K Peters/CRC Press. | ✅ Open Library: 4.ª ed., 2018, ISBN-13 9781138627000, 1198 págs. **No se cita en ningún manual todavía**: es una referencia propuesta, solo citarla si se lee y se usa |

> **Fbm, colisiones AABB, olas de suma de senoidales, caída libre y suma de senos de la banda:** el código y la justificación los usan, pero **ninguna fuente de la lista de arriba demuestra esas fórmulas específicas**. Se justifican con las fórmulas del propio manual (matemática elemental) y no se les puso cita para no inventar apoyo. Si el profesor pide fuente para el fbm o las olas, hay candidatas en la sección 7.3 (**sin verificar**).
> **Verificadas pero NO usadas por el proyecto:** Fournier, A., y Reeves, W. T. (1986), *A simple model of ocean waves*, *ACM SIGGRAPH Computer Graphics, 20*(4), 75–84, https://doi.org/10.1145/15886.15894 (olas; el proyecto usa una suma de senoidales distinta, no el modelo de ese artículo); Reeves, W. T. (1983), *Particle systems—A technique for modeling a class of fuzzy objects*, *ACM Transactions on Graphics, 2*(2), 91–108, https://doi.org/10.1145/357318.357320 (partículas: **el cliente dejó fuera partículas de humo**). **No se citan.**

---

## 6. La ficha de cada elemento y su referencia exacta

Referencias completas de las fichas = las de la sección 1 (en la pantalla salen en APA 7 completo desde `src/Fichas.cpp`). Resumen de qué cita cada ficha y qué es adaptación (esto es lo que el propio texto de la ficha dice en pantalla):

| # | Ficha (título en pantalla) | Referencias | Qué dice la ficha que es adaptación del estudio | Estado de la cita |
|---|---|---|---|---|
| 1 | Velas | C-01, C-02 | «Representación del estudio» solo en la escena (velas flotando) | 🔄 H-1 («al atardecer» solo en un resumen automático) |
| 2 | Flor de cempasúchil | C-01, C-02 | — | 🔄 H-1 |
| 3 | Papel picado | C-01 | «las tiras cuelgan de soportes… en el arco del cuarto» es la escena (sin cita) | ✅ |
| 4 | Altar de ofrenda | C-01, C-03 | altar de tres niveles y cuarto | ✅ |
| 5 | Pan de muerto | C-01 | — | ✅ |
| 6 | Banda de viento | C-04 | músicos, instrumentos y música elegida | ✅ (H-2: la ficha dice «Banda Mixe»; la descripción del video dice Yalálag) |
| 7 | Copal y copalero | C-01, C-05 | copaleros y humo (modelo fijo) | 🔄 H-4 |
| 8 | Panteón y tumbas | C-06, C-03 | panteón de cuatro tumbas | ✅ (fuente de contenido de marca) |
| 9 | Ofrendas de las familias | C-03, C-01, C-02 | las 14 ofrendas, familias, objetos y colores | 🔄 H-1 |
| 10 | Comparsa en la orilla | C-07 | la comparsa de la escena | ✅ (es la de la ciudad de Oaxaca, no de Mazunte) |
| 11 | Camino de las ánimas | C-01, C-06 | la procesión completa | ✅ |
| 12 | Mariposas monarca | C-08 | las mariposas de las colinas | 🔄 H-11 (sin fuente primaria) |
| 13 | Tortugas de Mazunte | C-09 | tortugas y crías del modelo | ✅ («años sesenta» y el Centro sí están en C-09; fuente única, H-3) |

**Narración del recorrido (`docs/RECORRIDO_GUIADO.md` §7)** — qué fuente respalda cada estación (las dichas en voz alta son las mismas que las de las fichas): 00 → C-02 (el título «se vive con más alma») y C-09; 01 → C-02 y C-01; 02 → C-07; 03 → C-01; 04 → C-04; 05 → C-06 y C-03; 06 → C-01 y C-05; 07 → C-01; 08 → C-03 y C-01; 09 → C-06; 10 → C-08; 11 → idea de fondo de C-01 (redacción del estudio). **Las frases de «mira cómo…», «ven, camina conmigo», «gracias por caminar con nosotros» y la apertura/cierre no tienen fuente: son narrativa del estudio.**

**Créditos del menú (`src/Menu.cpp`, X-63)** — qué incluye y qué le falta: lista las 3 texturas Poly Haven (T-01 a T-03), la música (A-02, sin año/canal/URL), la voz (A-03, sin herramienta), el oleaje (A-01, sin canal/año), las bibliotecas y las herramientas. **Datos que ya existen para completarlo (si D2 decide tocarlo algún día; hoy no se modifica):** A-02 (canal «Ricardo Tizo», 2012) y A-03 (ElevenLabs, 2026), ya confirmados por D2; A-01 (canal «Quiet», 2020) falta que D2 lo confirme. `laminado1.jpg` y `concreto.jpg` (T-05, T-06) siguen sin origen.

---

## 7. Hallazgos (lo que no coincide o no tiene fuente)

### 7.1 Afirmaciones del proyecto contra lo leído en la fuente

- **H-1 · «al atardecer» (fichas 1, 2 y 9; narración 01).** **Resuelto a medias.** Dos lecturas automáticas de la página de Baños (C-02) dieron resultados opuestos: la primera no mostró «al atardecer»; la segunda, pedida solo para la sección de Mazunte, sí lo dice (junto con «se lanzan flores y velas al mar para recordar a quienes partieron»). Ninguna llegó como texto literal (la sección de Mazunte cae en la parte que la herramienta resume). **Estado: respaldada solo por un resumen automático.** Como D2 pidió no modificar las fichas, se deja así y se anota; **comprobación humana de 30 segundos:** abrir el artículo y buscar «atardecer» en la sección de Mazunte.
- **H-2 · Banda Mixe vs. el video.** **Resuelto en lo esencial:** D2 confirmó (18/11) que el mp3 es ese video (A-02). Sigue sin respaldo en las fuentes qué banda concreta toca: solo el título del video dice «Banda Mixe de Oaxaca» (la descripción dice «Villa Hidalgo Yalàlag»); el título trae la errata «Nuca» y no se identificó al compositor.
- **H-3 · Ficha 13 (tortugas) y narración 00.** **Resuelto:** «años sesenta» y «con el apoyo del Centro Mexicano de la Tortuga» **sí están** en C-09 (segunda lectura, texto literal; la primera lectura estaba incompleta: ver C-09). **Quedan dos discrepancias entre fuentes, sin tocar el proyecto:** (a) **año del Centro:** C-10 dice «abierto en 1994»; C-09 menciona 1977 (PIOSA), 1985 (Instituto Nacional de la Pesca) y 1992 («nace el nuevo concepto»); `investigacion_mazunte.md` ya anotaba 1991 vs. 1994. Ninguna ficha ni narración da el año del Centro, así que no hay error en pantalla; (b) **veda:** la ficha y la narración dicen «mayo de 1990» (C-09), pero C-24 da la veda permanente del Pacífico mexicano desde el 16/03/1994 (con vedas desde 1972). **La ficha sigue a su fuente citada (C-09); no hay otra fuente que confirme «mayo de 1990»** (queda como «respaldo en una sola fuente secundaria»). Además C-10 atribuye la recuperación a la comunidad y a la cooperativa, no al Centro, a diferencia de C-09.
- **H-4 · Ficha 7 (copal).** «En el panteón tepehua de Pisaflores, Veracruz, las tumbas se adornan en medio del humo del copal»: el boletín (C-05) dice que los tepehuas de Pisaflores, Ixhuatlán de Madero, «acuden al panteón» y que «en medio del humo del copal, adornan los sepúlcros»; une dos frases del mismo texto, sin contradecirlas. La fuente **no define el copal ni su significado** (eso lo respalda la SEP, C-01: «limpian el lugar y alejan malos espíritus»).
- **H-5 · Año del Plan de marketing.** Los documentos citan «(2020)»; el documento no declara año (solo la ruta de subida dice 2020/12). Corregido en la sección 1 a «s. f.» hasta que alguien lo confirme en la portada.
- **H-11 · Mariposas (ficha 12 / narración 10).** La ficha agrega «los bosques de oyamel»: **no se comprobó en la página de Expansión** (sí: creencia purépecha, finales de octubre–principios de noviembre). La fuente es periodismo sin fuente primaria y la imagen del artículo es de una IA («Gemini»).
- **H-12 · Fecha de Pueblo Mágico.** **Resuelto:** se usa **28 de septiembre de 2015**, que es la que da un documento oficial (C-11, «el 28 de septiembre del 2015»). El **25/09/2015** de `investigacion_mazunte.md` **no tiene respaldo** en ninguna fuente leída (venía de un resultado de búsqueda sin abrir). C-23 confirma que Mazunte fue nombrado Pueblo Mágico en septiembre de 2015 pero no da el día. Ninguna ficha ni narración la menciona.
- **H-13 · Fuente de contenido de marca.** La ficha 8 y 11 y las narraciones 05 y 09 se apoyan en un artículo firmado «advertorial MD» (C-06). No es incorrecto, pero es contenido patrocinado: el equipo ya lo declara en `fichas_culturales.md` §7.

### 7.2 Documentación del proyecto incompleta o contradictoria

- **H-6 · `concreto.jpg`.** Se usa en arco, fachada y piso y no aparecía en ningún crédito; **resuelto:** es propia (Blender), D2 lo confirmó el 18/11. El menú de créditos sigue diciendo «Pendiente: origen de laminado1.jpg»: texto desactualizado, sin cambiar por decisión de D2.
- **H-7 · Versión de Blender.** Los documentos citan Blender 4.4.3, 5.2, 5.2.2, 5.2.4 y 5.3 (`bpy`). Puede ser correcto (distintas PC/etapas), pero no hay una versión única para citar. Propuesta: anotar `bpy.app.version_string` o `Help > About` en cada PC (era X-1 🔄).
- **H-8 · GIMP.** El manual técnico §3 dice «sin uso registrado» (las texturas propias salieron de scripts de Python), pero el menú de créditos (`Menu.cpp`) y el README lo listan. Decidir: quitar GIMP de los créditos o registrar para qué se usó.
- **H-9 · miniaudio.** El repo trae la v0.11.26 (31/10/2026); la página de GitHub leída mostraba 0.11.25 (04/03/2026) como «Latest». No es un error del proyecto (se cita la del repo); solo conviene no escribir la versión de la web.
- **H-10 · Shoemake (1985).** El manual lo cita como artículo de *ACM SIGGRAPH Computer Graphics* 19(3); la comprobación de hoy en Crossref (DOI 10.1145/325334.325242) lo registra en las actas *Proceedings of SIGGRAPH '85* (págs. 245–254). Autor, año, páginas y DOI coinciden; aquí se usa la forma de las actas. Si D2 prefiere la forma del manual, que lo registre.
- **H-14 · README del repo** dice «CC0 textures (Poly Haven, ambientCG)»: **no hay ninguna textura de ambientCG registrada** en el repo. Quitar «ambientCG» o decir cuál es.
- **H-15 · Cita del título de Reeves y otros.** El título que se recordaba para Reeves (1983) era abreviado («Particle systems»); el exacto es el de la sección 5. Aplica a cualquier cita futura.

### 7.3 Bibliografía candidata, NO VERIFICADA y NO usada en el proyecto

No se pudo comprobar hoy (sin ISBN/DOI a mano o error de lectura): Marschner y Shirley, *Fundamentals of Computer Graphics*; Hughes y colaboradores, *Computer Graphics: Principles and Practice*; Lengyel, *Mathematics for 3D Game Programming and Computer Graphics*; Ebert y colaboradores, *Texturing and Modeling: A Procedural Approach* (la lista de Perlin 2002 cita la **2.ª edición, 1998**, no la 3.ª); Ericson, *Real-Time Collision Detection* (AABB); Gregory, *Game Engine Architecture*; Möller y Trumbore (1997), *Fast, minimum storage ray-triangle intersection* (Crossref falló; el proyecto no lo usa); Tessendorf (2001), *Simulating ocean water*. ❌ **No citar ninguna sin abrirla antes**; solo se dejan como pista si el profesor pide fuentes de fbm, colisiones u olas.

---

## 8. Qué falta (lo que queda abierto)

Cerrado el 18/11/2026 por D2: voz = ElevenLabs (A-03), `musica.mp3` = el video de «Ricardo Tizo» (A-02) y `laminado1.jpg`/`concreto.jpg` = propias, hechas en Blender (T-05, T-06). D2 pidió **no modificar el proyecto**, así que lo siguiente solo se anota:

1. ~~`laminado1.jpg` y `concreto.jpg`~~: **cerrado (18/11)**, son propias (Blender).
2. **Oleaje (A-01):** abrir el video una vez y confirmar canal («Quiet») y fecha (31/03/2020).
3. **Modelos de Maya de D1 (T-07):** herramienta, fuente y permiso del retrato del cuadro (D1).
4. **«Al atardecer» (H-1):** comprobación visual en el artículo de Baños (opcional).
5. **Versiones:** `vcpkg list` (GLFW, GLEW, GLM, Assimp), versión de Blender, de Visual Studio y de Maya.
6. **Farin (M-08):** edición y subtítulo del libro.

---

## 9. Conteo (consulta del 18/11/2026)

| Grupo | Total | ✅ verificadas | 🤝 confirmadas por el equipo | 🔄 parciales | ❌ NO VERIFICADO | ⏳ pendiente (T-07) |
|---|---|---|---|---|---|---|
| C · fuentes culturales y de contenido | 24 | 11 | 0 | 2 | 11 | 0 |
| A · música, audio y voz | 4 | 0 | 2 | 2 | 0 | 0 |
| T · texturas y modelos externos | 7 | 4 | 2 | 0 | 0 | 1 |
| S · bibliotecas, software y estándares | 23 | 9 | 0 | 10 | 4 | 0 |
| M · bibliografía técnica y matemática | 10 | 7 | 0 | 3 | 0 | 0 |
| **Total** | **68** | **31** | **4** | **17** | **15** | **1** |

Los 11 ❌ del grupo C son 10 citas de la propuesta que no se abrieron (C-13 a C-22, no se usan en fichas ni narración) más `MAZUNTE.pdf` (C-12), que no abrió. «Parcial» incluye A-01 (falta que D2 confirme canal y fecha) y A-04.
