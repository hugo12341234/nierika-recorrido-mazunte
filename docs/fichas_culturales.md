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
- **Verificación:** ⚠️ por verificar contra la fuente (SEP y Baños). Además, confirmar que el artículo de Baños habla de **Mazunte**
  y no solo de «Oaxaca»: el texto de la ficha dice «ceremonia local» sin nombrar el lugar justo por eso.

### Ficha 2 · Cempasúchil

- **Dónde:** parada 2 (flores flotando) y altar. **Objeto:** `flor` (provisional, de D1 en X-3) y luego B-3.
- **Título en pantalla:** Flor de cempasúchil
- **Texto en pantalla:**
  > El aroma y el color del cempasúchil guían a las almas hacia el altar (Secretaría de Educación Pública [SEP], 2026). Al
  > atardecer, las flores también se lanzan al mar para recordar a quienes partieron (Baños, 2025).
- **Nivel de evidencia:** *Tradición general* (SEP, 2026) y *Local documentado* (Baños, 2025).
- **Referencias:** las mismas dos de la ficha 1.
- **Verificación:** ⚠️ por verificar contra la fuente. La propuesta resume la fuente SEP como «aromatiza y guía a las almas al
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
- **Verificación:** ⚠️ por verificar contra la fuente.
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
  > (Secretaría de Educación Pública [SEP], 2026). La ofrenda familiar es el acto central de la velación (UNESCO, s. f.).
  > Aquí la ofrenda se presenta sobre un altar de tres niveles, con una forma que es adaptación del estudio.
- **Nivel de evidencia:** *Tradición general* (SEP, 2026) en las funciones de los objetos; **Adaptación del estudio** en el
  altar de tres niveles y en el cuarto (**la propia ficha lo dice**, última frase). La referencia UNESCO respalda la
  velación familiar, no la forma del altar.
- **Referencias:**
  - Secretaría de Educación Pública. (2026, 22 de mayo). *Elementos básicos para la ofrenda de Día de Muertos*. Nueva Escuela Mexicana Digital. https://nuevaescuelamexicana.sep.gob.mx/contenido/recurso/elementos-basicos-para-la-ofrenda-de-dia-de-muertos/
  - UNESCO. (s. f.). *Las fiestas indígenas dedicadas a los muertos*. https://ich.unesco.org/es/RL/las-fiestas-indigenas-dedicadas-a-los-muertos-00054
- **Verificación:** ⚠️ por verificar contra la fuente (SEP y UNESCO). Si el límite de caracteres lo exige, recortar la lista de
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
- **Verificación:** ⚠️ por verificar contra la fuente.

## 4. Resumen

| # | Ficha | Fuentes | Nivel | ¿Texto de adaptación? | Verificado en la fuente |
|---|---|---|---|---|---|
| 1 | Velas | SEP (2026); Baños (2025) | Tradición general + Local documentado | Opcional (velas flotando) | ⚠️ No |
| 2 | Cempasúchil | SEP (2026); Baños (2025) | Tradición general + Local documentado | No | ⚠️ No |
| 3 | Papel picado | SEP (2026) | Tradición general | Descripción de la escena, sin cita | ⚠️ No |
| 4 | Altar | SEP (2026); UNESCO (s. f.) | Tradición general + **Adaptación del estudio** | **Sí, dentro del texto** | ⚠️ No |
| 5 | Pan de muerto | SEP (2026) | Tradición general | No | ⚠️ No |

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
