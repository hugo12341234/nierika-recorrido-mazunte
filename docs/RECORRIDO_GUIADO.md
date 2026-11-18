# Recorrido virtual guiado (X-62) · Diseño, guion y lo que se implementó

Fecha: 17/11/2026 · Estado: **implementado (17/11), visto solo en Linux; sin compilar en VS 2022, sin FPS medidos y sin oír el audio**. D2 aprobó el orden, la tecla **T**, los subtítulos y la silueta sincronizada («adelante», 17/11).
Todo lo de "dónde está cada cosa" sale de `src/Extras.h`, `Fichas.cpp`, `PlazaTrasera.h`, `OfrendasFamiliares.h`, `Silueta.cpp` y del ESTADO (sec. 5) de `develop` (f6ea1de). Las coordenadas de cámara son **propuestas inferidas de esos datos**: se afinan con capturas en Linux al implementar.

## 1. La idea en una frase
Una visita nocturna que **sigue el camino de las ánimas**: llegan por el mar (velas y flores), cruzan la orilla y el sendero, entran a casa (el altar, donde la silueta deja su flor), y después la fiesta sigue: las ofrendas de las familias, el camino de faroles colina arriba, las mariposas y un mirador final desde donde se ve todo.
La frase que sostiene toda la narración es la de la SEP que ya está en las fichas: *la flama guía a los difuntos de regreso a casa y el aroma de las flores los guía hacia el altar* (SEP, 2026).

## 2. Tecla y controles (DECISIÓN PENDIENTE de D2)
- **R ya reinicia la caminata de la silueta** (D-2). No la piso. Propuesta: **T** («Tour», libre desde X-61). Alternativa: G («Guía»).
- **T** inicia; **T otra vez, Esc, o cualquier W A S D / flecha** cancelan y te devuelven la cámara libre **donde quedó** (con colisiones). Mientras el tour va, el ratón no mueve la cámara. Esc cancela el tour **antes** de cerrar el programa (hoy Esc cierra directo; es un cambio en `KeyCallback`).
- V (velas) y M (sonido) siguen funcionando durante el tour. F1 y el manual ES/EN y LEEME dirán la tecla nueva.

## 3. Estaciones, en orden (12), con tiempos
Velocidad de diseño: tramo = máx(4 s, distancia / 1.8 m/s promedio) con arranque y frenado suaves (pico ~2.7 m/s). Ojos a 1.6 m sobre el suelo (`Colisiones::alturaTerreno` + 1.6). «Retén» = lo que dura la estación (≈ palabras ÷ 2.4 + 2 s); si la voz real dura más, **el tour espera a que termine** (así no hay que reajustar tiempos al meter la voz).

| # | Estación (ficha que se activa sola a < 2 m) | Cámara (x, z) → mira a | Tramo | Retén | Archivo de voz |
|---|---|---|---|---|---|
| 0 | **Apertura: Mazunte y el mar** (Tortugas de Mazunte, punto (21, 22.4)) | (17.5, 22.2) → crías y mar (22, 23.5) | ≤ 6 s desde donde estés | 34 s | `Audio/narracion/narracion_00.mp3` |
| 1 | **Velas y flores al mar** (Flor de cempasúchil, (3, 23)) | (1.8, 22.8) → petates y veladoras (9, 21) | 8.7 s | 32 s | `…_01` |
| 2 | **Comparsa en la orilla** (Comparsa, (−12, 22)) | (−9.5, 22.2) → la comparsa (ver riesgo 4) | 6.3 s | 23 s | `…_02` |
| 3 | **Camino de flores al arco** (Papel picado, (0, 1.95, 6)) | de (−0.8, 20.5) a (−1.6, 7.5) mirando al arco (0, 1.6, 1.3) | 6.5 s + 12 s de paseo | 23 s | `…_03` |
| 4 | **Banda de viento** (Banda, (12.6 y 15.4, 10.4)) | (11.0, 9.6) → la banda (14, 8) | 7 s | 17 s | `…_04` |
| 5 | **Panteón** (Panteón y tumbas, 4 puntos) | travelling lateral de (2.6, 7.0) a (−2.6, 7.0), mirando a las tumbas y a los diseños de velas (±3.9, 8.6) | 5 s | 26 s | `…_05` |
| 6 | **Copal** (Copal y copalero, (±1.05, 1.75)) | (0, 3.3) → los copaleros y el arco | 4 s | 22 s | `…_06` |
| 7 | **El altar** (Altar de ofrenda; la del pan **no** sale, ver riesgo 6) | entra por el arco (ancho 1.18 m, sin tocarlo) hasta (0, 0.95) → altar (0, 0.9, −0.7) | 5 s | 24 s | `…_07` |
| 8 | **Ofrendas de las familias** (Ofrendas familiares, frente de cada una) | por el pasillo oeste, de (0, 3) a (−3.2, 3.6), (−3.2, −8.5) → «pescador» (−6.1, −13.2) | 12 s | 29 s | `…_08` |
| 9 | **Camino de las ánimas** (Camino de las ánimas, (0.5, −28)) | sube desde (0.5, −26.6) siguiendo el eje del sendero | 11 s | 17 s | `…_09` |
| 10 | **Mariposas** (Mariposas monarca, (0, −37.5)) | (0.3, −35.8) → mariposas (±4.5, −37) | 6 s | 20 s | `…_10` |
| 11 | **Mirador y cierre** (misma ficha) | en el mismo punto, giro de 180° (8 s) hacia el sur: capilla, playa y mar | 0 s | 31 s | `…_11` |

**Duración total de diseño: ≈ 6 min 30 s tras el cambio de guion del 17/11 (antes ≈ 7 min)** (≈ 5 min 36 s de retenes + ≈ 1 min 30 s de tramos; la narración suma 757 palabras ≈ 5 min 15 s a ~145 palabras/min). Si es mucho para la defensa, lo que **menos** daña recortar: 2 (comparsa), 4 (banda) y 10 (mariposas): con eso baja a ≈ 5 min 30 s.

**Silueta sincronizada (idea):** durante el tour la caminata de la silueta se programa para **llegar al altar justo cuando la cámara está ahí** (estación 7): `tSilueta = tTour − t0`, con t0 = (llegada al altar) − `Duracion()` − 1 s. Antes de t0 la silueta espera en la orilla (`Actualizar` ya recorta t < 0 a 0). Así se ve pasar por el patio durante el panteón y deja su flor mientras suena la narración del altar. Son 2 líneas en `main.cpp`. Al cancelar, la silueta sigue desde donde iba.

## 4. Cómo se mueve la cámara (matemática, para el manual)
Por tramo: posición **P(u) = Catmull-Rom** (curva cúbica que pasa por los puntos de paso, continuidad C¹ en las esquinas) con parámetro **u = smoothstep(τ) = 3τ² − 2τ³**, τ = t_tramo / T_tramo; así la velocidad es 0 al salir y al llegar de cada estación. Orientación: **yaw** por el arco más corto y **pitch** lineal, ambos con el mismo smoothstep, apuntando al punto de mira. Dentro de una estación, un balanceo mínimo (±0.03 m, T = 7 s, seno) para que no se vea congelada.
**No son KeyFrames**: los KeyFrames del proyecto (silueta y «dejar flores») animan **modelos**; esto es una ruta de cámara por fórmula, **no cuenta** entre las 10 animaciones. Ojo: un profesor estricto podría llamarlo «cuadros clave de cámara». Se registrará en el ESTADO la aclaración, pero el riesgo de interpretación es de D2.

## 5. Audio (enganche para la voz)
- Archivos esperados: `Audio/narracion/narracion_00.mp3` … `_11.mp3` (12; carpeta aparte para no chocar con la regla D-6h de «solo oleaje.mp3 y musica.mp3»). **Si falta uno, no truena**: se imprime una línea en consola, la estación dura su «retén» y listo.
- Mientras habla la voz, la música baja a ×0.35 y el mar a ×0.6 (rampa de 0.6 s); al terminar, vuelven. Es un multiplicador nuevo en `Audio` que no toca las curvas de D-6j ni X-40.
- **Subtítulos** (propuesta, por defecto encendidos): el texto del guion en una franja abajo (ImGui, el mismo contexto de las fichas) mientras no haya voz y también con ella. Sirven para probar el tour hoy y para accesibilidad. D2 decide si se quitan.
- Crédito APA 7 de la herramienta de voz: **pendiente hasta que D2 la elija** (va en `Audio/README.md`, ESTADO y manual).

## 6. Riesgos y puntos honestos (léelos)
1. **No compilado en VS 2022** (solo Linux/Mesa), sin FPS medidos y sin oír el audio (la nube no tiene tarjeta de sonido). El tour solo corre al pulsar T, pero toca `main.cpp` (Esc, silueta) y `Audio.cpp`: D2 debe compilar y probar en su PC antes de fusionar.
2. **Posiciones de cámara inferidas** del código, no vistas aún: algunas pueden atravesar un farol o un árbol (la cámara ignora colisiones). Se corrige con capturas en Linux.
3. **Esc hoy cierra el programa**; con el tour activo pasa a cancelarlo. Cambia un comportamiento ya documentado en el manual.
4. **La comparsa se mueve sola** (1 m/s, vuelta de 96 m) y no se puede garantizar que pase junto a la cámara. Propuesta: en la estación 2 la cámara se coloca 7 m delante del centro del grupo y lo mira venir; la ficha de la comparsa (punto fijo (−12, 22)) puede no salir.
5. **«Casas del fondo» / pueblo**: el cliente dejó fuera «calles y edificios del pueblo» (CLAUDE.md §6) y las casas están tras `MOSTRAR_FUERA_DE_ALCANCE`. **No las hice estación ni las nombro en la narración**; en el mirador se verán de fondo. Dime si prefieres ocultarlas ahí. **Tampoco hice estación de «fogata»**: no tiene ficha; si la quieres, se agrega una de ~20 s con texto 100 % de adaptación del estudio (sin datos).
6. **La ficha del pan de muerto no sale** en el tour: el punto de la ficha (0, 0.8, −0.75) está dentro de la huella del altar y la cámara no puede ir ahí. El pan se menciona en la voz del altar (SEP, 2026). Cubre 12 de las 13 fichas.
7. **«Mesón de Oaxaca»** (tu ejemplo): no aparece en ningún documento del repo ni en las fichas, así que **no lo usé**. Si es un lugar real que quieres nombrar, necesito la fuente para agregarlo.
8. **Voz sintética ≠ «personaje con IA»** (restricción del profesor): es una narración que lee texto, no un personaje que decide ni responde. Se anotará así en el ESTADO; la última palabra de si el profesor lo acepta es de D2.
9. Precisión del guion: todo dato lleva su fuente dicha en voz alta («según la SEP…»), tomada de las fichas; marco **[F]** = dato de ficha con fuente, **[A]** = adaptación/narrativa del estudio (sin dato). La descripción de lo que se ve («mira cómo…») es [A].

## 7. Guion de narración (para la herramienta de voz)
Tono: voz masculina cálida, cercana, sin solemnidad ni locución de documental; frases cortas, pausas donde hay «…». Duración estimada con ~145 palabras/min. [F] y [A] **no se leen**: son marcas para el equipo.

### 00 · Apertura: Mazunte y el mar (~32 s)
Oaxaca… una costa, una noche, y una fiesta que se vive con más alma. [A; la frase «se vive con más alma» es del título de Baños, 2025]
Estás en la playa de Mazunte. Hubo un tiempo en que la explotación de la tortuga fue la actividad principal de este pueblo, hasta que en mayo de 1990 se decretó la veda total; desde entonces, Mazunte cuida a sus tortugas. [F: México Desconocido, 2010]
Pero esta noche venimos por otra razón: es noche de muertos, y los que se fueron… vuelven a casa. Ven, camina conmigo. [A]

### 01 · Velas y flores al mar (~30 s)
En la ceremonia local que se vive al atardecer, se lanzan velas y flores al mar para recordar a quienes partieron, cuenta Baños, en dos mil veinticinco. [F]
La flama de cada vela guía a los difuntos de regreso a casa, y el aroma de las flores, sobre todo del cempasúchil, los guía hacia el altar. Así lo explica la Secretaría de Educación Pública. [F: SEP, 2026]
Mira cómo la luz se va con el agua. [A]

### 02 · Comparsa en la orilla (~28 s)
Y mientras el mar se lleva las velas, por la orilla llega la música. En la Gran Comparsa del Día de Muertos de la ciudad de Oaxaca desfilan más de cincuenta contingentes, entre catrinas, esqueletos y disfraces de papel, con música y color, según La Razón. La muerte, vestida de fiesta.

### 03 · Camino de flores al arco (~24 s)
Ahora sigue el camino de flores hasta la puerta. Ese papel picado que se mece sobre el sendero le da color a la ofrenda y representa el aire, dice la Secretaría de Educación Pública. Fíjate en la figura que va caminando rumbo al altar: lleva una flor para dejarla ahí.

### 04 · Banda de viento (~22 s)
¿Los oyes? Son metales. Las bandas de viento son una tradición oaxaqueña: dan identidad y hermanan a los pueblos indígenas del estado, y acompañan sus fiestas comunales, según México Desconocido. Su música llena la noche.

### 05 · Panteón (~27 s)
En las alumbradas de Oaxaca, las familias pasan la noche del treinta y uno de octubre junto a las tumbas de sus seres queridos, y las velas sobre las lápidas iluminan, de forma simbólica, el camino de regreso, según México Desconocido. Los platillos del difunto se colocan alrededor del altar familiar y de la tumba, dice la UNESCO.

### 06 · Copal (~26 s)
Antes de entrar, un aroma: el copal. Forma parte de la ofrenda, junto con las velas, las flores y el pan, según la Secretaría de Educación Pública. En el panteón tepehua de Pisaflores, en Veracruz, las tumbas se adornan en medio del humo del copal, informó el INAH.

### 07 · El altar (~25 s)
Y llegamos al altar. Cada objeto cumple una función para recibir a las ánimas: velas, flores, pan, agua y sal, y copal, entre otros, según la Secretaría de Educación Pública. El pan de muerto representa el afecto y la fraternidad hacia los difuntos. Mira... llega la figura del camino, y deja su flor.

### 08 · Ofrendas de las familias (~28 s)
Afuera, detrás de la capilla, cada familia prepara su ofrenda a su manera. Los platillos del difunto se colocan alrededor del altar y de la tumba, y la flama de las velas y el aroma de las flores guían a las almas de regreso, según la UNESCO y la Secretaría de Educación Pública. Fíjate en los objetos que cada familia deja para su difunto.

### 09 · Camino de las ánimas (~20 s)
Sube conmigo por el camino de las ánimas. En las alumbradas de Oaxaca, una comparsa recorre el pueblo entre música y baile, según México Desconocido. Los faroles, el cempasúchil y las velas marcan el camino.

### 10 · Mariposas (~21 s)
Y entre las luces, mariposas. Según una creencia asociada al pueblo purépecha de Michoacán, las mariposas monarca que llegan a finales de octubre y principios de noviembre son las almas de los difuntos que regresan, según Expansión Digital. Es una interpretación simbólica.

### 11 · Mirador y cierre (~29 s)
Desde aquí arriba se ve todo: la capilla, la playa, el mar. [A]
Así es esta fiesta: una noche en que la flama y el aroma guían a quienes se fueron de regreso a casa, y quienes se quedan los reciben con luz, con flores, con pan… y con música. [F: SEP, 2026 como idea de fondo; redacción nuestra]
Gracias por caminar con nosotros. Ahora el camino es tuyo: recórrelo a tu ritmo, acércate a cada ofrenda y lee su ficha. [A]

## 8. Qué se toca si se aprueba (plan de implementación)
Nuevo `src/Recorrido.h/.cpp` (estaciones, Catmull-Rom + smoothstep, audio por estación) · `main.cpp`: ~15 líneas (T, Esc, W A S D, actualizar cámara, silueta, dibujar subtítulos) · `Audio.h/.cpp`: carga perezosa de `Audio/narracion/*.mp3` + `SetAtenuacion` · `Fichas.cpp/Hud.h`: subtítulos y línea en F1 · `NierikaStudio.vcxproj`: el `.cpp` nuevo · `Audio/README.md`, manual ES/EN, LEEME (tecla y archivos) · ESTADO: tarea **X-62** y decisiones (tecla, no-KeyFrames, voz sintética ≠ IA, subtítulos, crédito pendiente). Rama `feature/x-62-recorrido`, PR a `develop`, **sin fusionar** hasta tu «fusiona» escrito.

## 9. Cambios respecto al diseño aprobado (17/11, tras ver capturas en Linux)
Las posiciones de la tabla de la sección 3 eran inferidas; con capturas de Linux se ajustaron así (valores vigentes: `src/Recorrido.cpp`, tabla `E[]`):
- **0** cámara en (19.5, 18.6) mirando al mar (22, 23.5): desde (17.5, 22.2) la comparsa pasaba por encima de la cámara. La ficha de las tortugas ya **no** sale en esta estación.
- **1** (−1.0, 23.3). **2** la cámara se coloca **9 m delante del grupo** de la comparsa y 3.8 m tierra adentro, a (cx + 9, 18.4) con tope en x ±40, y lo ve venir; el centro del grupo se filtra 1.5 s (si el grupo da la vuelta al final de la playa, la cámara no salta). La ficha de la comparsa puede no salir.
- **4** (13.0, 12.2): desde (11, 9.6) la cámara quedaba pegada a los músicos. **6** (0, 3.3) mirando al arco: se ven el altar y los copaleros. **7** (0.85, 0.55): es la posición donde gana la ficha del altar (a (0.5, 0.95) ganaba la del copal).
- **10 y 11** en (−0.8, −35.0): el terreno ahí está a 2.5 m de altura; desde (0.3, −35.8) un árbol tapaba la vista.
- **Silueta:** arranca su caminata para **llegar al altar 14 s después de que la cámara se detiene en la estación 7** (cuando la voz dice «llega la figura y deja su flor»), no en el copal como decía la sección 3. Con voz real esos 14 s son aproximados.
- Modo de desarrollo: `NIERIKA_RECORRIDO_CAPTURAS=carpeta` corre el recorrido a 30 pasos de 0.1 s por cuadro y guarda un PNG por cuadro (cada 3 s de recorrido); es lo que se usó en Linux.
- Duración medida en Linux, sin voz: **≈ 7 min** (139 cuadros × 3 s).

## 10. Cambio de guion (17/11, D2)
D2: «el chiste es mostrar la tradición, no que inventamos o cómo lo hicimos». Se quitaron de la voz las aclaraciones de adaptación (panteón, copaleros, altar, familias, procesión, mariposas, comparsa, banda y la nota sobre la música). **Siguen en las fichas culturales** (en pantalla), que es donde el ESTADO exige decir lo que es adaptación del estudio. La voz ya no afirma nada de lo inventado como si fuera real: solo dice lo que dicen las fuentes y señala lo que se ve. La canción de fondo cambiará a «Dios nunca muere» con la Banda Mixe de Oaxaca (otro hilo): la voz ya no menciona la música.

## 11. Ajustes tras la prueba de D2 en Visual Studio (18/11, X-64)
D2 probó T (≈ 199 FPS) y pidió: (1) **sin ficha cultural durante el recorrido** (con subtítulos arriba, ficha abajo y voz no se veía el mapa): `FichasDibuja` ignora la ficha mientras `hud.tour`; las fichas siguen en modo libre. (2) **Subtítulos compactos**: media pantalla de ancho (antes 70 %), abajo al centro, fondo más transparente; ya no tapan la parte alta de la imagen. (3) **Reencuadre** (la cámara se alejó para que se entienda de qué habla cada estación): **5 Panteón** (0.5 → −0.5, 11.2) mirando al centro de las 4 tumbas (0, 0.8, 5.3), con la capilla al fondo (antes (2.6, 7.0): un personaje tapaba todo); **6 Copal** (0, 5.2 → 4.4) mirando a los dos copaleros de la entrada (0, 0.9, 1.75), con el arco y el altar detrás; **7 Altar** (0.5, 2.6 → 0.3, 2.1), desde la puerta, con el altar completo y una tumba-capilla a un lado (antes (0.85, 0.55): solo se veía un tramo del altar); **8 Ofrendas de las familias** (−1.0, −9.5 → −1.6, −9.9) mirando a la lancha con cruz del pescador y las ofrendas de alrededor (antes (−3.2, −8.5), más cerca y confuso); **11 Mirador** (−3.4, −34.6), con tramo de 3.5 s (antes (−0.8, −35.0) con un farol y un personaje encima de la cámara). (4) En **Controles** (menú) la línea de T se acortó («Recorrido guiado (T, Esc o W A S D lo cancelan)»): se salía del marco. Modo de desarrollo nuevo: `NIERIKA_RECORRIDO_DESDE=N` (0–11) empieza en la estación N, para revisar un encuadre sin recorrer las anteriores. Visto solo en Linux; D2 debe confirmarlo en VS 2022.
