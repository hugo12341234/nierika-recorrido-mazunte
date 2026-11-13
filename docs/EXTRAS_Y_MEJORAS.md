# Extras y mejoras sobre la propuesta (fase «mejorar y producir»)

> **Para qué sirve este archivo:** es el registro, pensado para pasar después al documento de entrega, de todo lo que se agregó **además** de lo prometido en la propuesta v2.0:
> qué es, en qué tarea (`X-n`), con qué archivos, cómo se anima (con su fórmula), de dónde salen los recursos externos, cuánto cuesta en rendimiento y cómo se comprobó.
> **Fecha de inicio:** 12/11/2026 (decisión de D2). Las decisiones y parámetros viven en `docs/ESTADO_PROYECTO.md` (secciones 4 y 5); este archivo no las sustituye.

## 1. Qué prometía el documento inicial y qué hay hoy

Inventario hecho con la Propuesta v2.0 (§1 ficha, tablas 2.1 y 3, figuras 1 a 10, tabla de paradas) contra la escena del 12/11/2026.

| # | Lo que dice o muestra la propuesta | Estado antes de esta fase | Qué se hace |
|---|---|---|---|
| 1 | **Del atardecer a la noche** del 1 de noviembre (ficha del proyecto) | Solo noche (cielo azul oscuro con estrellas y luna) | Bloque (e): el cielo ya tiene un horizonte cálido (resplandor del ocaso) |
| 2 | **Ceremonia en la orilla:** velas y flores flotando (tabla 2.1) | Velas y flores flotan; **no hay gente** | Bloque (b): figuras estáticas con ofrendas y veladoras en la orilla |
| 3 | **Figura 6:** veladoras sobre la arena | No hay | Bloque (b) |
| 4 | **Figura 1:** ceremonia en la playa con figuras agachadas ante ofrendas | No hay | Bloque (b) |
| 5 | **Fig. 10 (ilustración conceptual):** colinas verdes con vegetación detrás del panteón | Horizonte vacío | **Bloque (a), hecho en X-18** |
| 6 | **Papel picado en tendederos** (tabla 2.1 habla de tiras colgadas de soportes) | Solo en el arco del panteón | Bloque (c): tendederos cruzando la playa |
| 7 | **Persona que lleva una ofrenda** | La silueta camina y deja una flor | Ya cumplido (D-1 a D-5) |
| 8 | **Figura 7:** comparsa de Día de Muertos | No hay | No se incluye (ver sección 3, riesgo de alcance) |
| 9 | **Gente en tablas de remo sobre las olas** (fotos de referencia de Hugo) | No hay | Bloque (d) |
| 10 | Audio de oleaje y música lejana | Hecho (D-6) | Sin cambio |
| 11 | Cinco fichas culturales | Hecho y verificado (X-17) | Sin cambio |

## 2. Choques con las reglas del cliente (revisados antes de implementar)

| Regla | Riesgo | Cómo se respeta |
|---|---|---|
| Fuera de alcance: **calles y edificios del pueblo** | Al ampliar el escenario podrían aparecer casas | No se modela ninguna construcción nueva |
| Fuera de alcance: **Punta Cometa (colina y mirador)** | Las colinas del fondo podrían leerse como Punta Cometa | Son **colinas genéricas** de relieve aleatorio, sin mirador ni forma de punta; así se dice en el ESTADO y aquí |
| Fuera de alcance: **tapete de arena** | Las ofrendas en la playa podrían parecer un tapete | Solo hay objetos pequeños (veladoras, flores, frutas) sobre la arena lisa; no hay figuras dibujadas con arena |
| Fuera de alcance: **personajes con IA** | Las figuras humanas | Son **modelos estáticos con poses fijas**; su única animación es matemática simple (balanceo senoidal) y no toman decisiones |
| Fuera de alcance: **humo de copal con partículas** | Ambiente | No se agrega humo; las luciérnagas son puntos de luz con movimiento matemático, no un sistema de partículas de humo |
| **KeyFrames solo** en la caminata de la silueta y en dejar flores | Las animaciones nuevas | Todas las nuevas usan fórmulas simples (senos, péndulo, Lissajous), **sin KeyFrames**, y cada una tiene su justificación en `docs/manual_tecnico/justificacion_extras.md` |
| **5 animaciones** (2 KeyFrames + 3 simples) | Las nuevas podrían contarse como animaciones adicionales | Se documentan como **mejoras de ambiente** (extras), no sustituyen ni cambian las 5 animaciones comprometidas |
| **≥ 18 modelos propios** | Los extras no deben rebajar el conteo | El conteo de 18 no cambia: los modelos nuevos son extras, cada uno con `.blend` en `Models/<objeto>/` |
| **≥ 30 FPS en gráficos integrados** | Más geometría y más llamadas de dibujo | Se miden los FPS tras cada bloque (RX 5700 XT, referencia) y se usan mallas de pocas caras; el requisito en integrada sigue **sin probarse** |
| Créditos de lo externo | Texturas o modelos nuevos | Todo lo nuevo hasta ahora es propio (generado por código y Blender); lo externo se cita en `manual_tecnico/11_referencias.md` |
| Escala 1 u = 1 m, texturas 1K, `.obj` + `.mtl` + textura en `Models/<objeto>/` | Modelos nuevos | Se cumple en cada extra |

## 3. Decisión de expansión de alcance

Registrada en `docs/ESTADO_PROYECTO.md` (sección 4, decisión «X-18 · expansión del escenario»). Resumen:

- El escenario jugable **no cambia** (playa x ±50, z −20…+30; la cámara no puede salir de ahí).
- Se agrega **geometría de fondo** (colinas) fuera de la zona jugable, y objetos y figuras decorativas dentro de la playa.
- La comparsa (figura 7 de la propuesta) **no se incluye**: movería muchas figuras y roza el límite de personajes animados; si D2 la quiere, debe decidirlo y registrarlo.

## 4. Registro de extras

| Extra | Tarea / rama | Modelos y archivos | Animación y fórmula | Fuentes y créditos | Rendimiento | Estado |
|---|---|---|---|---|---|---|
| Captura de pantalla de desarrollo | X-4 `x-captura-pantalla` | `src/main.cpp` (`NIERIKA_CAPTURAS`) | — | Propio | Sin costo | ✅ en develop |
| Cielo nocturno procedural | X-5 | `Shaders/sky.vs`, `sky.frag` | Estrellas por hash de celda; luna con halo | Propio | Despreciable | ✅ en develop |
| Texturas propias 1K | X-6, X-10 | `tools/x6_texturas.py` | — | Propias | — | ✅ en develop |
| Silueta orgánica (10 piezas) | X-7 | `tools/x7_silueta.py`, `Models/silueta/*.obj` | KeyFrames del D-2 (sin cambio) | Propio (Blender 5.2) | ~1.5 k caras | ✅ en develop |
| Luz cálida de las velas del altar | X-8 | `Shaders/lighting.frag` (`velasAltar`) | Parpadeo 0.85 + 0.10·sen(7.3t) + 0.05·sen(13.1t + 1.3) | Propio | +1 luz puntual | ✅ en develop |
| Palmas y arbustos | X-9 | `Models/palma/`, `Models/arbusto/` | — | Propios | 154 y 100 vértices | ✅ en develop |
| Videos con reloj fijo | X-16 | `tools/x16_*` | — | Propio | — | ✅ en develop (videos pendientes de ffmpeg) |
| **Colinas verdes de fondo + 70 palmas y 140 arbustos** | **X-18** `x-18-colinas-vegetacion` | `Models/colinas/` (3 720 vértices), `src/VegetacionColinas.h` (generado), `tools/x18_colinas.py` | — (estático); bruma de distancia f = 1 − exp(−(d·0.012)^1.5) | Propio (Blender 5.2, ruido de valor); sin texturas externas | 1 501 → 1 245 FPS (RX 5700 XT) | ✅ (captura abajo) |
| **Ceremonia en la playa:** 12 figuras estáticas, 3 petates con ofrenda (velas, cempasúchil, pan, copalero) y 26 veladoras sobre la arena | **X-19** `x-19-ceremonia` | `Models/persona_pie_a/`, `persona_pie_b/`, `persona_agachada_a/`, `persona_agachada_b/` (≈ 400 caras cada una), `Models/petate/`, `src/Extras.h`, `tools/x19_ceremonia.py` | Balanceo senoidal θ = A·sen(2πt/T + φ) (A 1.2° o 2°; T 4.5 o 5 s); llamas con ruido de Perlin (C-2) | Propio (Blender 5.2; texturas 1K generadas por código; retrato sin rostro, sin personas reales) | 12 cajas de colisión nuevas | ✅ |
| **Tendederos de papel picado cruzando la playa (17 tiras) y ofrendas al pie de 4 tumbas y la cruz** | **X-20** `x-20-tendederos` | reusa `Models/soportes/`, `papel_picado/`, `vela/`, `cempasuchil/`; datos en `src/Extras.h` | Péndulo de pequeña amplitud θ = 6°·sen(2πt/3.6 + φ) | Propios | 20 cajas de colisión (postes) | ✅ |
| **5 personas de pie sobre tablas de remo en la bahía** | **X-21** `x-21-tablas-remo` | `Models/tabla_remo/` (114 caras, tabla + remo), reusa `persona_pie_a/b`; `tools/x21_tablas.py` | Círculo lento (R 3–4 m, 70 s) + altura y normal de `Olas.h`; sin KeyFrames | Propio (Blender 5.2) | 10 llamadas de dibujo | ✅ |
| **48 luciérnagas, espuma de la orilla y resplandor del ocaso en el cielo** | **X-22** `x-22-ambiente` | `Models/luciernaga/` (icosaedro de 20 caras), `Shaders/lighting.frag` (`llama = 2`, `espuma`), `Shaders/sky.frag` (`ocaso`), `src/Extras.h` | Lissajous 3D + parpadeo sen³; espuma = ruido de Perlin que avanza hacia la playa | Propio | 48 llamadas de dibujo muy pequeñas | ✅ |
| **Relieve en arena, barda, tumbas y cruz** | **X-23** (misma rama `x-22-ambiente`) | `Shaders/lighting.frag` (`relieveLuma`, `arena`) | Mapa de altura = luminancia de la textura (Mikkelsen, 2010) | Mikkelsen (2010), ver `11_referencias.md` | Sin costo de geometría | ✅ |

| **Narrativa en 5 estaciones:** sendero de cempasúchil (≈ 70 flores) de la orilla a la puerta, 16 veladoras y 16 faroles con luz parpadeante, 24 figuras (familias, niños, abuelas con vela, músico con guitarra), 4 cestas con flores, 7 murciélagos al anochecer y **sonido por zona** (oleaje, música, voces) | **X-24** `x-24-narrativa` | `Models/farol/`, `guitarra/`, `cesta/`, `murcielago_cuerpo/`, `murcielago_ala/` (≤ 220 caras cada uno), `Audio/voces.wav` (sintetizada), `src/Extras.h`, `src/Audio.cpp`, `tools/x24_narrativa.py`, `tools/x24_voces.py` | Balanceo senoidal de las figuras y de la guitarra; parpadeo de los faroles (dos senos); murciélagos en círculos con aleteo senoidal; volumen = rampa lineal de la distancia | Propio (Blender 5.2, síntesis por código; sin voces reales ni grabaciones) | ≈ 880 FPS en la RX 5700 XT (antes ≈ 1 050); ≈ 330 llamadas de dibujo más | ✅ (el audio no se pudo escuchar desde aquí) |

### Hilo narrativo (coincide con las 5 fichas)

| Estación | Ficha | Dónde | Qué cuenta |
|---|---|---|---|
| E1 Llegada a la playa | Velas | orilla oeste (x −11…−3, z ≈ 21.6) | una familia llega con su cesta de flores, una abuela deja velas en la arena y un farol marca la entrada |
| E2 Ofrendas y velas al mar | Velas y cempasúchil | orilla este (x 3…19, z 17…21) | la ceremonia (X-19) con un músico y un niño; faroles a los lados |
| E3 Sendero de cempasúchil y papel picado | Papel picado | x ≈ 0, z 22 → 2 | las flores guían de la orilla al panteón entre veladoras y faroles; los tendederos (X-20) cruzan la playa; una familia camina hacia el panteón |
| E4 Panteón con familias | (altar, tumbas) | dentro de la barda | cada tumba tiene visitantes, ofrendas (X-20) y un farol en cada esquina; un músico toca junto a la tumba 1 |
| E5 El altar | Altar y pan de muerto | la puerta del cuarto y el altar | una abuela con su vela y un niño esperan afuera; dos faroles flanquean la puerta; el altar con su luz cálida (X-8) |

### Pendiente de decisión de D2

- **Comparsa (figura 7 de la propuesta):** no se incluye.
- **Mariposas y aves diurnas:** no aplican de noche; se usaron murciélagos.
- **Audio:** las voces son un murmullo sintetizado; si D2 prefiere grabaciones libres (CC0), hay que descargarlas y citarlas.

| **Lados de la escena (inspirados en Mazunte):** 18 rocas y acantilados, 16 palmas, 4 lanchas varadas y 6 ancladas, 3 redes de pesca, 40 linternas flotantes, 4 tortugas marinas y 8 crías en la arena, 7 aves, 2 fogatas sin humo y 2 hamacas | **X-25 a X-28** `x-25-lados` | `Models/roca_mar/`, `lancha/`, `red_pesca/`, `ave_cuerpo/`, `ave_ala/`, `tortuga/`, `hamaca/`, `fogata/` (≤ 130 caras cada uno), `tools/x25_lados.py`, `src/Extras.h` | Olas + péndulos + círculos + módulo (ver `justificacion_extras.md` §8) | Propios (Blender 5.2); datos de la investigación en `docs/investigacion_mazunte.md` (fuentes por verificar) | ~250 llamadas de dibujo más | ✅ |
| **Palapas (5) y casas costeras (6)** — **FUERA DEL ALCANCE ORIGINAL** (edificios del pueblo) | **X-26** `x-25-lados` | `Models/palapa/`, `casa_a/`, `casa_b/` (60 y 36 caras), `tools/x26_construcciones.py` | — (estáticas) | Propios; **según D2, el profesor autorizó agregar extras** (evidencia: ver campo de abajo) | 11 llamadas | ✅ pero **desactivable**: `Extras::MOSTRAR_FUERA_DE_ALCANCE` o `NIERIKA_SIN_FUERA_ALCANCE=1` |

### Autorización del profesor para agregar extras (por completar)

> Según D2 (13/11/2026), **el profesor dio luz verde para agregar extras si el equipo lo considera necesario o bueno**.
> **Evidencia (correo, mensaje o fecha):** _______________ (D2: pega aquí la evidencia cuando la tengas; **no se inventa la fuente ni la fecha**).

| **Comparsa (12 figuras que marchan por la orilla), música en vivo (5 músicos con guitarras, tambor y trompetas) y 3 alfombras de pétalos de cempasúchil** — **FUERA DEL ALCANCE ORIGINAL** (comparsa, música en vivo y alfombras de flores quedaron fuera en la primera versión de este plan por decisión de alcance) | **X-29** `x-29-comparsa-musica-alfombra` | `Models/tambor/`, `trompeta/`, `alfombra_flores/`, `alfombra_cruz/`, `Audio/banda.wav`, `tools/x29_comparsa.py`, `tools/x29_banda.py` | Marcha con senos y módulo; pulso de 90 bpm (ver `justificacion_extras.md` §9); **música sintetizada, no grabada** | Propio (Blender 5.2, síntesis por código); la alfombra es de pétalos, **no de arena** | ~40 llamadas de dibujo | ✅ detrás de `MOSTRAR_FUERA_DE_ALCANCE` |

## 5. Evidencias (antes / después)

Capturas reales con el modo de captura (`NIERIKA_CAPTURAS`), en `docs/extras_evidencias/`. Los archivos `antes_*.png` son del 12/11/2026 con develop `c65109f`.

| Bloque | Antes | Después |
|---|---|---|
| X-18 colinas y vegetación | `antes_fondo.png`, `antes_panteon.png`, `antes_orilla.png` | `x18_fondo.png`, `x18_panteon.png`, `x18_costa.png` |
| X-25 a X-28 lados | `antes_lado_oeste.png`, `antes_lado_este.png` | `x25_lado_oeste.png`, `x25_lado_este.png`, `x25_bahia.png`, `x25_casas_oeste.png` |
| X-24 narrativa | `antes24_ruta.jpg` | `x24_ruta.jpg` (misma ruta de 9 poses) |
| X-22/X-23 ambiente y relieve | — | `x22_ambiente_a.png`, `x22_ambiente_b.png`, `x23_relieve.png` |
| X-21 tablas de remo | — | `x21_tablas_a.png`, `x21_tablas_b.png` |
| X-20 tendederos y ofrendas | — | `x20_tendedero.png`, `x20_ofrenda_tumba.png` |
| X-19 ceremonia | `antes_playa_este.png` (solo mar y arena; imagen no conservada, ver `antes_orilla.png`) | `x19_ceremonia_a.png`, `x19_ceremonia_b.png`, `x19_ceremonia_c.png` |

## 6. Rendimiento medido (RX 5700 XT, Release x64, 1280 × 720, título de la ventana)

| Momento | FPS aproximados |
|---|---|
| develop antes de la fase (12/11) | 1 680 – 2 100 |
| tras X-18 (colinas y 210 plantas) | 1 250 – 1 500 |
| tras X-22/X-23 (todo lo anterior + ceremonia, tendederos, tablas, luciérnagas, espuma, relieve) | 1 020 – 1 120 |
| tras X-24 (narrativa: sendero, faroles, 24 figuras, murciélagos) | 830 – 920 |

El requisito (≥ 30 FPS en gráficos integrados) **sigue sin probarse en una laptop integrada**: la GPU de D2 es mucho más rápida. El costo más visible es el número de llamadas de dibujo (≈ 450); si hiciera falta recortar, el primer paso es dibujar las 210 plantas de las colinas con instancing o reducirlas.
