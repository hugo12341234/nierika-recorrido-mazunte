# Revisión de escena (X-41) — 15/11/2026

Pedido de D2: revisar a fondo que no haya texturas raras, que todos los modelos carguen bien y que no haya cosas flotando o sin sentido; y proponer qué agregar o mover para dar más variedad.

## Cómo se revisó (y qué NO se pudo)

- **Dónde:** se compiló `develop` (PR #106) con g++ 13 y se ejecutó con Mesa llvmpipe (render por software) en Linux. **No es Visual Studio ni una GPU integrada**, así que lo de *MSVC* y los *FPS* queda sin verificar. Los archivos de la PC de D2 no se tocaron.
- **Capturas:** 40 poses con el modo captura (`NIERIKA_CAPTURAS`). Ojo: ese modo pega la cámara al suelo (y = terreno + 1.6 m), por eso no hay vistas aéreas.
- **Medición de posiciones:** una copia temporal de `Model.h` (fuera del repo) escribió, por cada `Draw`, la caja del modelo en coordenadas del mundo y el origen de su matriz. Se compararon 2 853 dibujos de un cuadro contra `Terreno::altura` (misma fórmula que `src/Terreno.h`). Los números salen de ahí, no de la lectura del código.
- **Archivos:** un script revisó los 70 `Models/*/*.obj`: `.mtl` presente, cada `map_Kd`/`map_Bump` existe, todo `.obj` tiene UV, y toda ruta `Models/...` de `src/` existe.

## Lo que está bien (verificado)

| Qué | Resultado |
|---|---|
| Carga de modelos | Sin errores en consola. Ningún `.mtl` ni textura faltante; todos los `.obj` con UV; ninguna ruta rota. |
| Texturas | 127 imágenes revisadas en hoja de contacto: todas 1024 px y cuadradas, salvo las 3 de `Models/altar/` (2048 px, **modelo sin usar**). Sin magenta ni color plano inesperado en pantalla. |
| Contacto con el suelo | Árboles, arbustos, palmas, faroles, tumbas, cruces, petates, canastas, hamacas, barda, fachada: origen a menos de 0.25 m del terreno (la diferencia es la pendiente). Personas de pie en playa/colinas: ≤ 0.1 m. |
| Sobre el agua | Lanchas, tablas de remo y tortugas siguen la altura de la ola; las personas de las tablas van sobre la tabla. |
| Objetos a propósito en el aire | Palapa del mirador (y = 25.1 sobre la roca grande), guitarras, tambores, trompetas y velas en manos, redes colgando de su bastidor, ofrenda del altar: todo es intencional. |

## Defectos corregidos en este PR

1. **Ofrendas dentro de casas (visto en pantalla y medido).** X-30 generó las casas del pueblo sin saber de X-33/X-36. Las ofrendas con cruz de (−60, −36) y (65, −35) quedaron *dentro* de una casa (en la captura de "antes" la cámara estaba frente a una pared donde debía haber una cruz), y había faroles, palmas y personas atravesando paredes. `tools/x30_pueblo_fondo.py` ahora descarta toda casa que contenga una de esas cosas (44 → 33 casas). Evidencia: `docs/manual_tecnico/evidencias/X-41_antes_despues_casas_ofrendas_linux_15-11.jpg`.
2. **Palma dentro de un grupo de personas.** La palma de (−5.5, 21.5) coincidía con el niño del grupo E1; pasa a (−6.0, 19.0).
3. **Textura "rara" del tronco de la palma (visto en pantalla).** Las bandas onduladas de X-9 se leían como un zigzag repetido en todas las palmas de la playa. Nuevo `tools/x41_tronco_palma.py`: anillos casi rectos con fibras verticales (mismo mapeo UV, no se tocó el `.obj`). Evidencia: `X-41_tronco_palma_antes_despues.jpg`.

## Hallazgos que NO se corrigieron (decide D2)

| # | Hallazgo | Evidencia | Cómo se vio | Propuesta |
|---|---|---|---|---|
| H1 | **La comparsa atraviesa faroles y palmas.** Marcha por z = 21.7 y 23.5 y los faroles de la orilla están en z = 21.4–22.3 (y el grupo E1 está en z 21.2–21.8). | — | Medido (posiciones); no se vio el cruce en movimiento | Mover la comparsa a z = 23.1 y 24.0 *o* retirar 3 faroles de la orilla. Conviene decidirlo junto con el hilo de la banda. |
| H2 | **Una palma coincide con un músico** de la banda actual en (−11.0, 19.0) contra (−11.4, 19.0). | — | Medido | No se tocó porque el otro hilo (banda de folclor) va a rehacer ese grupo; moverla 1.5 m al oeste cuando se sepa dónde queda. |
| H3 | **Roca y banco en el mismo punto bajo la palapa de (26, −17)** (la roca de X-33 cae justo en el centro de la palapa). | 1.er cuadro de `X-41_hallazgos_sin_corregir_linux_15-11.jpg` | Visto en pantalla | Quitar esa roca (es de `VidaColinasDatos.h`, la genera `tools/x33_vida_colinas.py`). |
| H4 | **Borde arena/pasto recto como regla en z = −20**, sin transición, a lo largo de 100 m. Es lo que más delata que la playa es un rectángulo. | cuadros 2 y 3 del mismo archivo (el 4.º es el mar de noche, sin hallazgo) | Visto en pantalla | Opción de variedad A (abajo). |
| H5 | **Personas = maniquíes** (sin cara; tono plano de piel). | todas las capturas | Visto en pantalla | Opción de variedad G (abajo). |
| H6 | **~2 830 llamadas a `Model::Draw` por cuadro** (498 luciérnagas, 206 arbustos, 188 flores, 144 llamas, 135 velas, 133 palmas…). Solo las personas y algunas plantas se descartan por distancia. | — | Medido | Antes de sumar variedad, conviene medir FPS en la laptop integrada de D2 (E-1); si hace falta, descartar por distancia luciérnagas, llamas y flores. |
| H7 | **Carpetas que cargan pero no se usan:** `Models/pan/` se carga y nunca se dibuja (un `Draw` con 0 llamadas); `Models/altar/` (3 PNG de 2048 px, 8 MB) no se usa. `Models/atuendos/` solo tiene texturas (correcto). | — | Medido / por código | Limpiar en un PR aparte; no afecta lo que se ve. |
| H8 | **Promontorio rocoso de la bahía oeste** (rocas de 9–30 m + palapa de mirador). Es una formación genérica ya registrada como excepción (X-30), pero Punta Cometa figura como fuera de alcance. | — | Visto en pantalla (bahía oeste) | Solo recordatorio: si el profesor pregunta, es la excepción X-30. |

## Opciones para más variedad y actividades (SIN implementar; espera la elección de D2)

Costo: se da en **llamadas de dibujo extra** porque los FPS en integrada no están medidos (ver H6). Regla práctica: ~3 000 llamadas hoy; +100 es +3 %.

| Opción | Qué se vería | Modelos nuevos (Blender) | Costo | Riesgo / alcance |
|---|---|---|---|---|
| **A. Borde de playa con vida** | Pasto que se mezcla con la arena (parches), troncos de mar varados, algas secas en la línea del agua (z ≈ 23) | 1 tronco, 1 alga (pueden ser > 18 propios) | ≈ +40 | Bajo. Resuelve H4 con poco costo. |
| **B. Puesto de pan y chocolate** bajo la palapa de (−25, −16) | Mesa con pan de muerto, jarros y canastas; dos personas atendiendo | 1 mesa, 1 jarro | ≈ +25 | Bajo. Usa `pan_muerto`, `cesta`, `farol` ya existentes. Aporta "actividad" en la franja norte. |
| **C. Rueda de velas** en la franja norte | 10–12 personas con vela en círculo que gira lento (senos, sin KeyFrames) alrededor de una cruz | ninguno | ≈ +40 | Bajo. Extra ambiental: se registra como decisión. |
| **D. Niños con papalotes** en la playa sur | 3 niños corren y 3 papalotes se mueven con senos | 1 papalote | ≈ +20 | Bajo-medio: necesita pose de correr (hoy solo pie/agachada). |
| **E. Pescadores remendando red + fogata de velada** | Grupo sentado junto a lancha varada y red; fogata pequeña | 1 persona sentada | ≈ +30 | Medio: la persona sentada es un modelo nuevo; reutiliza `red_pesca`, `lancha`, `fogata`. |
| **F. Mirador de las ánimas** en la cima este (65, −35) | Más ofrendas, bancas, tendedero y luces, con una ficha cultural | 1 banca | ≈ +50 | Medio: sumar una 6.ª ficha exige fuente APA 7 verificada. |
| **G. Caras en las personas** | Textura de rostro (ojos, boca, cejas) en cabezas; 4 tonos de piel | 0 (solo texturas) | 0 | Bajo; es la mejora de mayor efecto visual por costo. |

**Recomendación:** G + A + B primero (≈ +65 llamadas, sin riesgo de alcance), después C si sobra tiempo. Dejar D, E y F para después de medir FPS (E-1). Cualquiera de estas opciones es un **extra ambiental** y se anota como decisión en el ESTADO antes de implementarla.
