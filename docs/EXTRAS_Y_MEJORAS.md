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
| Ceremonia en la playa con figuras y ofrendas | X-19 | pendiente | Balanceo senoidal de las figuras de pie | — | — | ⏳ |
| Tendederos de papel picado y ofrendas extra | X-20 | pendiente | Péndulo θ = A·sen(ωt + φ) | — | — | ⏳ |
| Gente en tablas sobre las olas | X-21 | pendiente | Altura y pendiente de `Olas.h` (sin KeyFrames) | — | — | ⏳ |
| Luciérnagas, espuma y ambiente | X-22 | pendiente | Lissajous + parpadeo senoidal | — | — | ⏳ |
| Relieve en arena y barda | X-23 | pendiente | Mapa de altura = luminancia de la textura | — | — | ⏳ |

## 5. Evidencias (antes / después)

Capturas reales con el modo de captura (`NIERIKA_CAPTURAS`), en `docs/extras_evidencias/`. Los archivos `antes_*.png` son del 12/11/2026 con develop `c65109f`.

| Bloque | Antes | Después |
|---|---|---|
| X-18 colinas y vegetación | `antes_fondo.png`, `antes_panteon.png`, `antes_orilla.png` | `x18_fondo.png`, `x18_panteon.png`, `x18_costa.png` |
