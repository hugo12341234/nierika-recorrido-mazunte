# Fichas culturales — Nierika Studio (C-7a)

> **Qué es este archivo:** los textos de las 5 fichas culturales que mostrará Dear ImGui (tarea C-7b) cuando la cámara
> esté a **< 2 m** de cada objeto. Cada ficha lleva su cita en el texto, su referencia en APA 7 y su nivel de evidencia.
> **Solo documento:** no toca código.

## 1. Cómo se obtuvo el contenido (léelo primero)

- **Todo el contenido sale de la Propuesta v2.0** (`docs/propuesta/Propuesta_Nierika_v2.0.pdf`): tablas 2.1 y 19, y lista de
  referencias. No se agregó ningún dato nuevo de afuera.
- **Las fuentes originales NO se leyeron.** Este entorno no tiene acceso a esos sitios (SEP, UNESCO, Oaxaca Informa y
  CCH devolvieron bloqueo del proxy el 03/11). Por eso cada ficha lleva el estado **«por verificar contra la fuente»**: lo que
  afirma está respaldado por lo que la propuesta dice que la fuente documenta, pero nadie del equipo lo ha comparado todavía
  con la página original. Verificarlo es una tarea pendiente (sección 5).
- Niveles de evidencia (definidos en la propuesta): **Tradición general**, **Local documentado**, **Adaptación del estudio**.

## 2. Formato y límites para C-7b

- 2 a 3 frases por ficha, unos 250 caracteres de texto más la línea de nivel y la referencia. **El largo no está definido en
  los documentos:** es una propuesta de este archivo, no una decisión registrada.
- Usar solo letras y signos de **ASCII y Latin-1** (acentos, ñ, «¿» y «¡» sí). **Evitar** raya larga (—) y comillas tipográficas
  (“ ”), porque la fuente por defecto de ImGui puede no tenerlas (**inferido, no probado**).
- La ficha muestra: título, texto, línea **«Nivel de evidencia: …»** y la **referencia completa**. La cita en el texto
  debe verse; la URL puede acortarse en pantalla solo si la referencia completa queda en el manual (decidir en C-7b).

## 3. Las 5 fichas

### Ficha 1 · Velas

- **Dónde:** parada 2 (orilla), velas flotando; también velas del altar. **Objeto:** `vela` (B-2, aún no existe).
- **Título en pantalla:** Velas
- **Texto en pantalla:**
  > La flama de la vela guía a los difuntos de regreso a casa (Secretaría de Educación Pública [SEP], 2026). En la ceremonia
  > local que se vive al atardecer, se lanzan velas y flores al mar para recordar a quienes partieron (Baños, 2025).
- **Nivel de evidencia:** *Tradición general* (SEP, 2026) y *Local documentado* (Baños, 2025).
- **Adaptación del estudio:** que las velas floten en el mar del recorrido es una **representación del estudio**; lo dice la ficha:
  *«En el recorrido, las velas flotan sobre el agua como representación del estudio.»* (frase opcional si cabe).
- **Referencias:**
  - Baños, L. (2025, 27 de octubre). *Los pueblos mágicos de Oaxaca donde el Día de Muertos se vive con más alma*. Oaxaca Informa. https://www.oaxacainforma.com/es/cultura/2025/pueblos-magicos-oaxaca-dia-de-muertos-2025
  - Secretaría de Educación Pública. (2026, 22 de mayo). *Elementos básicos para la ofrenda de Día de Muertos*. Nueva Escuela Mexicana Digital. https://nuevaescuelamexicana.sep.gob.mx/contenido/recurso/elementos-basicos-para-la-ofrenda-de-dia-de-muertos/
- **Verificación:** ✅ verificada el 12/11/2026 (sección 6) (SEP y Baños). Además, confirmar que el artículo de Baños habla de **Mazunte**
  y no solo de «Oaxaca»: el texto de la ficha dice «ceremonia local» sin nombrar el lugar justo por eso.

### Ficha 2 · Cempasúchil

- **Dónde:** parada 2 (flores flotando) y altar. **Objeto:** `flor` (provisional, de D1 en X-3) y luego B-3.
- **Título en pantalla:** Flor de cempasúchil
- **Texto en pantalla:**
  > El aroma de las flores, sobre todo del cempasúchil, guía a las almas hacia el altar (Secretaría de Educación Pública [SEP], 2026). Al
  > atardecer, las flores también se lanzan al mar para recordar a quienes partieron (Baños, 2025).
- **Nivel de evidencia:** *Tradición general* (SEP, 2026) y *Local documentado* (Baños, 2025).
- **Referencias:** las mismas dos de la ficha 1.
- **Verificación:** ✅ verificada el 12/11/2026 (sección 6). La propuesta resume la fuente SEP como «aromatiza y guía a las almas al
  altar»; la ficha lo reformula: confirmar que no cambia el sentido.

### Ficha 3 · Papel picado

- **Dónde:** el arco con papel picado. **Objeto:** soportes de papel picado (B-8, aún no existe).
- **Título en pantalla:** Papel picado
- **Texto en pantalla:**
  > El papel picado da color a la ofrenda y representa el aire (Secretaría de Educación Pública [SEP], 2026). En el recorrido,
  > las tiras cuelgan de soportes de madera en el arco del cuarto.
- **Nivel de evidencia:** *Tradición general* (SEP, 2026). La frase sobre los soportes y el arco describe **la escena del
  estudio**, no una fuente: va **sin cita** a propósito.
- **Referencia:**
  - Secretaría de Educación Pública. (2026, 22 de mayo). *Elementos básicos para la ofrenda de Día de Muertos*. Nueva Escuela Mexicana Digital. https://nuevaescuelamexicana.sep.gob.mx/contenido/recurso/elementos-basicos-para-la-ofrenda-de-dia-de-muertos/
- **Verificación:** ✅ verificada el 12/11/2026 (sección 6).
- **⚠️ Contradicción de ubicación entre documentos (señalada, no resuelta):** `CONTEXTO_PROYECTO.md` §5 pone la ficha
  de papel picado en la **parada 4 (panteón)**, «arco con papel picado»; la Propuesta (tabla 2.1) dice «tiras colgadas de
  soportes de madera en **el arco del cuarto**» (parada 5). El texto de arriba dice «arco del cuarto» (propuesta) pero la
  posición de la ficha en C-7b depende de qué arco sea. **Propuesta:** registrar en el ESTADO §4 cuál arco lleva el papel picado.
- Hay una referencia sobre papel picado en la propuesta (Colegio de Ciencias y Humanidades, s. f., *Gaceta CCH*), pero el
  CONTEXTO asigna esta ficha a SEP (2026); **no se usa** hasta que alguien decida añadirla y la lea.

### Ficha 4 · Altar de tres niveles

- **Dónde:** parada 5 (cuarto del cementerio). **Objeto:** `altar3` (B-1, aún no existe; hoy hay una capilla provisional).
- **Título en pantalla:** Altar de ofrenda
- **Texto en pantalla:**
  > En el altar, cada objeto cumple una función para recibir a las ánimas: velas, flores, pan, agua y sal, y copal, entre otros
  > (Secretaría de Educación Pública [SEP], 2026). Los platillos del difunto se colocan alrededor del altar familiar y de la tumba (UNESCO, s. f.).
  > Aquí la ofrenda se presenta sobre un altar de tres niveles, con una forma que es adaptación del estudio.
- **Nivel de evidencia:** *Tradición general* (SEP, 2026) en las funciones de los objetos; **Adaptación del estudio** en el
  altar de tres niveles y en el cuarto (**la propia ficha lo dice**, última frase). La referencia UNESCO respalda la
  altar familiar, no la forma del altar.
- **Referencias:**
  - Secretaría de Educación Pública. (2026, 22 de mayo). *Elementos básicos para la ofrenda de Día de Muertos*. Nueva Escuela Mexicana Digital. https://nuevaescuelamexicana.sep.gob.mx/contenido/recurso/elementos-basicos-para-la-ofrenda-de-dia-de-muertos/
  - UNESCO. (s. f.). *Las fiestas indígenas dedicadas a los muertos*. https://ich.unesco.org/es/RL/las-fiestas-indigenas-dedicadas-a-los-muertos-00054
- **Verificación:** ✅ verificada el 12/11/2026 (sección 6) (SEP y UNESCO). Si el límite de caracteres lo exige, recortar la lista de
  objetos y dejar solo «cada objeto cumple una función».

### Ficha 5 · Pan de muerto

- **Dónde:** altar, parada 5. **Objeto:** `pan` (de D1 en X-3).
- **Título en pantalla:** Pan de muerto
- **Texto en pantalla:**
  > El pan de muerto representa el afecto y la fraternidad hacia los difuntos (Secretaría de Educación Pública [SEP], 2026).
  > Se coloca en el altar junto con las demás ofrendas para recibir a las ánimas.
- **Nivel de evidencia:** *Tradición general* (SEP, 2026). La segunda frase se apoya en la tabla 2.1 de la propuesta
  (altar con pan de muerto, SEP, 2026) y debe confirmarse contra la fuente.
- **Referencia:**
  - Secretaría de Educación Pública. (2026, 22 de mayo). *Elementos básicos para la ofrenda de Día de Muertos*. Nueva Escuela Mexicana Digital. https://nuevaescuelamexicana.sep.gob.mx/contenido/recurso/elementos-basicos-para-la-ofrenda-de-dia-de-muertos/
- **Verificación:** ✅ verificada el 12/11/2026 (sección 6).

## 4. Resumen

| # | Ficha | Fuentes | Nivel | ¿Texto de adaptación? | Verificado en la fuente |
|---|---|---|---|---|---|
| 1 | Velas | SEP (2026); Baños (2025) | Tradición general + Local documentado | Opcional (velas flotando) | ✅ 12/11 (X-17) |
| 2 | Cempasúchil | SEP (2026); Baños (2025) | Tradición general + Local documentado | No | ✅ 12/11 (X-17) |
| 3 | Papel picado | SEP (2026) | Tradición general | Descripción de la escena, sin cita | ✅ 12/11 (X-17) |
| 4 | Altar | SEP (2026); UNESCO (s. f.) | Tradición general + **Adaptación del estudio** | **Sí, dentro del texto** | ✅ 12/11 (X-17) |
| 5 | Pan de muerto | SEP (2026) | Tradición general | No | ✅ 12/11 (X-17) |

## 5. Pendientes antes de dar las fichas por cerradas

1. **Verificar cada ficha contra su fuente original** (4 páginas: SEP, Baños, UNESCO y, si se usa, CCH). Alguien con internet
   debe abrir cada URL, comprobar que existe, que dice lo que afirma la ficha y que fecha, título y autor coinciden con la
   referencia. Hasta entonces, las referencias son **las de la propuesta, copiadas sin comprobar**.
2. **URL de Baños:** en el PDF la URL se corta al final de línea («…dia-de-muertos» / «-2025»); aquí se unió como
   `…dia-de-muertos-2025`. Confirmar que ese es el enlace real.
3. **Decidir el arco del papel picado** (ver ficha 3) y registrarlo en el ESTADO §4.
4. **Decidir el largo definitivo** de las fichas (los documentos no lo fijan) cuando C-7b muestre cómo se ven a 1280×720.
5. **Adaptación del estudio:** si se agregan más afirmaciones sobre Mazunte (p. ej. el cuarto del cementerio), deben
   decirlo en la propia ficha, como exige la propuesta.
6. **Criterio de C-7 (estado):** «la ficha aparece a < 2 m y cita su fuente» sigue en ⏳; este documento solo cubre el texto
   (C-7a). La parte de código es C-7b.

## 6. Verificación contra las fuentes reales (X-17, 12/11/2026, PC de D2)

> **Cómo se hizo y su límite:** las 3 páginas se abrieron con la herramienta de web del asistente, que **resume** cada página (no es una lectura humana).
> Las frases entre comillas son las que esa herramienta devolvió. D2 o D1 pueden abrir las URL para una última comprobación visual.

| Fuente (acceso 12/11/2026) | Qué se comprobó | Resultado |
|---|---|---|
| SEP (2026, 22 de mayo), *Elementos básicos para la ofrenda de Día de Muertos*, https://nuevaescuelamexicana.sep.gob.mx/contenido/recurso/elementos-basicos-para-la-ofrenda-de-dia-de-muertos/ | Título, fecha y autoría (sin autor; institución SEP) | ✅ coinciden. Es una **infografía**; la cita APA no cambia |
| (SEP) Velas | «Su flama guía a los muertos de regreso a casa» | ✅ ficha 1 correcta |
| (SEP) Cempasúchil | «Aromatizan y guían a las almas al altar, especialmente el cempasúchil» | ❌→✅ **corregida**: la ficha decía «el aroma **y el color**»; la fuente no menciona el color. Ahora: «El aroma de las flores, sobre todo del cempasúchil, guía a las almas hacia el altar» |
| (SEP) Papel picado | «Da color y representa el aire en la ofrenda» | ✅ ficha 3 correcta (la frase de los soportes es de la escena y va sin cita) |
| (SEP) Pan de muerto | «Representa afecto y fraternidad hacia los difuntos» | ✅ ficha 5 correcta |
| (SEP) Agua, sal, copal e incienso | Presentes en la infografía | ✅ la lista de la ficha 4 (velas, flores, pan, agua y sal, y copal) está respaldada |
| Baños (2025, 27 de octubre), *Los pueblos mágicos de Oaxaca donde el Día de Muertos se vive con más alma*, Oaxaca Informa, https://www.oaxacainforma.com/es/cultura/2025/pueblos-magicos-oaxaca-dia-de-muertos-2025 | Título, fecha, autoría (Lili Baños), URL, mención de Mazunte | ✅ la URL existe y los datos coinciden; **sí habla de Mazunte** y dice que se «lanzan flores y velas al mar para recordar a quienes partieron» (fichas 1 y 2 correctas) |
| UNESCO (s. f.), *Las fiestas indígenas dedicadas a los muertos*, https://ich.unesco.org/es/RL/las-fiestas-indigenas-dedicadas-a-los-muertos-00054 | Título y lo que dice del altar | ❌→✅ **corregida**: la ficha 4 decía «La ofrenda familiar es el acto central de la **velación**»; la página **no menciona la velación**. Dice que los platillos preferidos del difunto se ponen «alrededor del altar familiar y de la tumba». Ahora la ficha dice: «Los platillos del difunto se colocan alrededor del altar familiar y de la tumba» |

**Correcciones hechas: 2** (ficha 2 y ficha 4), en `src/Fichas.cpp` y en este archivo. **La URL de Baños** quedó confirmada (era una de las dudas de la sección 5).
**Sigue pendiente:** decidir el arco del papel picado (sección 5, punto 3) y el largo definitivo de las fichas (las 5 se ven completas a 1280×720 en las capturas de X-4).

## 7. Fichas nuevas (X-52, 17/11/2026): de 5 a 12

> **Cambio de alcance registrado en el ESTADO (sección 4, 17/11).** Las 5 fichas de arriba no cambian. Estas 7 cubren lo que se agregó después de la propuesta.
> **Cómo se hizo y su límite:** cada página se leyó con la herramienta de web del asistente (resumen, no lectura humana) el 17/11/2026. Las frases del texto
> están respaldadas por lo que esa lectura devolvió; D1 o D2 pueden abrir las URL para una comprobación visual. **Todo lo que es del estudio lo dice la propia ficha.**

| # | Ficha | Dónde (x, z) | Fuente | Qué es adaptación | Debilidad de la fuente |
|---|---|---|---|---|---|
| 6 | Banda de viento | frente a la banda (12.6 y 15.4, 10.4) | México Desconocido (2023) | Los 9 músicos, sus instrumentos y la música elegida (Xochipitzahuatl, náhuatl, no oaxaqueña) | El artículo **no habla de Día de Muertos** ni de Mazunte: la ficha solo dice «tradición oaxaqueña» y «fiestas comunales» |
| 7 | Copal y copalero | (±1.05, 1.75) | SEP (2026); INAH (2025, Boletín 562) | Los dos copaleros y el humo (modelo fijo) | El boletín del INAH describe a los tepehuas de Veracruz, no a Oaxaca; no define el copal ni su significado |
| 8 | Panteón y tumbas | al pie de las 4 tumbas | México Desconocido (2026); UNESCO (s. f.) | El panteón de 4 tumbas | La nota de las alumbradas es contenido de marca («advertorial MD») y no cita fuentes |
| 9 | Comparsa en la orilla | (-12, 22) | La Razón Online (2025) | La comparsa de 12 figuras | Crónica festiva de Oaxaca ciudad; no es de Mazunte |
| 10 | Camino de las ánimas | (0.5, -28) | SEP (2026); México Desconocido (2026) | **Toda la procesión** con velas, faroles y cempasúchil | La frase de la comparsa viene de la misma nota de marca; no hay fuente de peregrinaciones en Mazunte |
| 11 | Mariposas monarca | (0, -37.5) | Expansión Digital (2025) | Las 18 mariposas de las colinas | Periodismo sin fuente primaria; la creencia es purépecha (Michoacán), no de Oaxaca |
| 12 | Tortugas de Mazunte | (21, 22.4) | México Desconocido (2010) | Tortugas y crías del modelo | Cifras del artículo de los años 1988 a 1997; la ficha solo usa la historia (veda de mayo de 1990) y la golfina. Es contexto local, no de Día de Muertos |

**Referencias (APA 7).** Están completas en `src/Fichas.cpp` y se pintan en cada ficha. Nuevas:

- México Desconocido. (2023, 19 de diciembre). *Bandas de viento de Oaxaca, música popular que engalana las fiestas comunales*. https://www.mexicodesconocido.com.mx/bandas-de-viento-de-oaxaca.html
- Instituto Nacional de Antropología e Historia. (2025, 27 de octubre). *Tepehuas celebran a sus fieles difuntos en el Museo Nacional de Antropología* (Boletín No. 562). https://inah.gob.mx/images/boletines/2025/562/Boletin%20562.pdf
- México Desconocido. (2026, 28 de octubre). *Alumbradas de Oaxaca: una noche para reencontrarse con los difuntos*. https://www.mexicodesconocido.com.mx/alumbradas-de-oaxaca.html
- La Razón Online. (2025, 1 de noviembre). *Calles de Oaxaca se llenan de vida con la Gran Comparsa del Día de Muertos*. La Razón. https://www.razon.com.mx/estados/2025/10/31/calles-de-oaxaca-se-llenan-de-vida-con-la-gran-comparsa-del-dia-de-muertos/
- Expansión Digital. (2025, 20 de octubre). *Mariposa monarca: por qué se cree que son las almas que regresan el Día de Muertos*. Expansión. https://expansion.mx/tendencias/2025/10/20/por-que-mariposas-monarca-son-almas-regresan-dia-de-muertos
- México Desconocido. (2010, 10 de agosto). *El Centro Mexicano de la tortuga en Mazunte, Oaxaca*. https://www.mexicodesconocido.com.mx/el-centro-mexicano-de-la-tortuga-en-mazunte-oaxaca.html

**Fechas:** salen de los metadatos de cada página (la de La Razón dice 31/10 en la URL y 01/11 en la firma; se usó 01/11). **No se citó** un artículo de la UNAM Aragón sobre la monarca porque el sitio bloquea la lectura automática.

**Pendientes:** (1) que D2 compile y compruebe a 1280×720 que las 7 fichas caben completas (son más largas que las originales); (2) créditos de audio de las dos pistas (canal, año, fecha); (3) si D2 prefiere una fuente mejor para las fichas 8 y 11, cambiarla aquí y en `Fichas.cpp`.
