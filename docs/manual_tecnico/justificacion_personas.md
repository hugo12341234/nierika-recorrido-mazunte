# Justificación matemática de las 5 animaciones nuevas de personas (X-55, 17/11/2026)

> **Qué es:** las animaciones 6 a 10 del proyecto. Se sumaron a las 5 comprometidas (silueta, flor, olas, velas, flores) por decisión de D2 del 17/11 (ver ESTADO §4): **3 por KeyFrames y 2 simples**. Las 5 anteriores siguen en [`justificacion_complejas.md`](justificacion_complejas.md) y [`justificacion_simples.md`](justificacion_simples.md).
> **Código:** `src/PersonasAnim.h` y `src/PersonasAnim.cpp` (solo matemática, sin OpenGL); `main.cpp` las dibuja. **Pruebas numéricas:** `tools/x55_pruebas.cpp` (23 comprobaciones, 0 fallos el 17/11; cómo correrlas está en su primera línea).
> **Alcance y límites:** las animaciones se ven funcionando en el programa; con todo el proyecto activo el equipo midió un mínimo de 45 FPS en gráficos integrados Intel de 11.ª generación (tras el PR #148). La coreografía se revisó en pantalla solo por D2; la chilena, la hospitalidad y el juego con el perro son una **adaptación del estudio** sin fuente etnográfica específica.
> **Origen de cada dato:** «comprobado en el repositorio» = sale del código; «adaptación del estudio» = decisión del estudio sin fuente externa.

## 0. Base común ✅ **[repo]**

- **Sistema de KeyFrames reutilizado, no reescrito:** `anim::Pista`, `KeyFrame`, `Suavizar` y `Slerp` de `src/Animacion.cpp`, los mismos de la caminata de la silueta. Cada tramo entre dos KeyFrames $i$ e $i+1$ usa $u=(t-t_i)/(t_{i+1}-t_i)$, $s=\text{curva}(u)$, la posición con $\text{mix}(p_i,p_{i+1},s)$ y la rotación con
  $$\text{slerp}(a,b,s)=\frac{\sin((1-s)\theta)\,a+\sin(s\theta)\,b}{\sin\theta},\quad \cos\theta=a\cdot b$$
  (si $a\cdot b<0$ se invierte $b$: camino corto). Las pistas cíclicas repiten $t \bmod T$ y su último KeyFrame repite el primero.
- **Persona articulada** (modelo propio `Models/persona_art/`, 5 piezas con el pivote en la articulación, hechas con `tools/x55_personas_articuladas.py`): 10 nodos `RAIZ → TORSO → brazos → antebrazos` y `RAIZ → muslos → piernas`. Las piernas cuelgan de la `RAIZ` y no del torso: así el balanceo de la cintura no arrastra los pies. Medidas de la silueta: cadera 0.90 m, hombro 0.54 m sobre la cadera, brazo 0.30, antebrazo 0.26 + mano, muslo 0.45, pierna 0.45. Matriz de cada nodo: $M_{hijo}=M_{padre}\,T(\text{offset})\,R(q)$.
- **Rotaciones:** $+$ sobre $X$ mueve un miembro colgante hacia delante ($-z$); $+$ sobre $Z$ lo abre hacia $+x$ (brazo derecho); $+$ sobre $Y$ gira a la izquierda. Composición $q=R_z R_x$: primero se levanta (X) y después se abre (Z).
- **Vestuario:** `Atuendos::dibujarPartes` (método nuevo; los anteriores no cambian): cada pieza se pinta con la ropa, la piel y el pelo de una semilla, y falda, rebozo y sombrero siguen al torso.
- **Todo se evalúa en forma cerrada del tiempo** ($t$ del reloj), nada se integra por cuadro: no depende de los FPS.

---

## 6. Baile familiar (la chilena) — KeyFrames

**Función en la experiencia ✅ [repo]:** da vida a la playa cerca de la banda: la música tiene a quien la baile. Sitio `SITIOS[0]` = (22.5, 14.0), círculo libre de 1.5 m (verificador `x48` R16 = 0).
**Función cultural 🟡 [adaptación]:** la chilena es un baile de la costa de Oaxaca y Guerrero (pañuelo en alto, zapateado), pero **el repo no registra una fuente APA para ella**: la coreografía es una adaptación nuestra y la ficha debe decirlo; la fuente queda pendiente.

**Ciclo de paso (2 s), 5 pistas cíclicas por KeyFrames con smoothstep $s=3u^2-2u^3$:**

| Pista | KeyFrames $(t\to q)$ | Qué hace |
|---|---|---|
| `muslo_` | 0→0°, 0.25→+42°, 0.5→0°, 0.62→+16°, 0.74→0°, 2.0→0° (sobre X) | zapateado: levanta, pisa y da un repique corto |
| `pierna_` | 0→0°, 0.25→−58°, 0.5→0°, 0.62→−20°, 0.74→0°, 2.0→0° | la rodilla se flexiona solo hacia atrás |
| `cintura_` | 0→$R_z(+7)R_y(+8)$, 1→$R_z(-7)R_y(-8)$, 2→igual al 0 | balanceo de la cadera de ±7° y giro del torso de ±8° |
| `rebote_` | 0→0, 0.5→0.04 m, 1→0, 1.5→0.04, 2→0 | sube 4 cm con cada paso |
| brazos | por integrante (tabla del código, `BaileFamiliar::BaileFamiliar`) | papá: pañuelo con la derecha, brazo en $R_z(+20..38)R_x(+140..155)$ y antebrazo ondeando de +25° a +60° a 2 Hz; mamá: sostiene la falda, brazos en $R_z(\mp30..48)R_x(+22..32)$ en contrafase; niña: los dos brazos en alto ($R_x(+160..172)$) |

La pierna derecha usa la **misma pista** que la izquierda con $t+1$ s (medio ciclo): solo un pie se levanta a la vez. Mamá lleva medio segundo de desfase en los pasos.

**Órbita de papá y mamá (8 s):** 12 KeyFrames cada 30° con tramos lineales (velocidad angular constante):
$$p_k=(R\sin a_k,\;0,\;R\cos a_k),\quad a_k=\frac{2\pi k}{12},\quad q_k=R_y(30^\circ k),\quad R=0.95\text{ m}$$
Con $T=8$ s: $\omega=2\pi/8=0.785$ rad/s y $v=\omega R=0.75$ m/s. La rotación $q_k$ deja a cada uno mirando al centro. Mamá va en $t+4$ s: al otro lado del círculo. **Error de aproximar el círculo con un polígono de 12 lados:** $R(1-\cos 15^\circ)=0.032$ m (la prueba midió 0.0324 m; el círculo no se ve poligonal a esa escala).
**Niña (4 s por vuelta):** 4 KeyFrames con $R_y(90^\circ k)$ cada 1 s, tramos lineales: el slerp entre cuaterniones a 90° recorre el ángulo de forma uniforme ($\omega=\pi/2$ rad/s = 90°/s). Brinca el doble de rápido y alto (`rebote_(2t)·2`: 8 cm a 1 Hz).
**Pruebas:** pose idéntica en $t=0$ y $t=8-\varepsilon$; error de la órbita < 4 cm; papá y mamá a $2R\pm12$ cm.

---

## 7. Juego con el perro — KeyFrames

**Función en la experiencia ✅ [repo]:** una escena de convivencia en la playa oeste con inicio, nudo y final (lanza, corre, trae, devuelve). Sitio `SITIOS[1]` = (−14.5, 8.0), giro −90° (la pelota vuela hacia +x del mundo); franja de 6 m × 1.6 m libre (R16 = 0).
**Función cultural 🟡 [adaptación]:** no hay fuente registrada. El perro es un perro criollo cualquiera; **no se afirma** que sea un xoloitzcuintle ni que guíe a las ánimas. Es convivencia cotidiana de la playa.

**Ciclo de 10 s (pistas no cíclicas con primer = último valor).**
*Niño* (escala 0.62): `giroTorso_` 0→0°, 0.7→$R_y(-35)$ (carga), 0.95→$R_y(+25)$, 1.8→$R_y(+8)$, 2.8→0°; `brazoD_` 0.7→$R_x(-80)$, 0.88→$R_x(+155)$ (lanzamiento en 0.18 s), 1.5→$R_x(+95)$, 2.7→reposo; `inclina_`, `cuclillas_`, `muslo_` (+75°) y `pierna_` (−120°) lo agachan entre 6.4 y 7.0 s para recoger la pelota (la cadera baja 0.43 m).
*Perro:* `ruta_` (posición + giro), 12 KeyFrames con smoothstep: espera en $H=(0.60,0,-0.70)$ hasta 0.9 s, corre a $(0,0,-5.42)$ (llega a 2.6 s), recoge, da la vuelta ($0\to90\to172.5^\circ$ en dos tramos, para fijar el sentido del giro) y regresa a $H$ a 5.95 s. Con smoothstep la velocidad máxima es $1.5\,d/T$: $d=4.76$ m en 1.7 s da 4.2 m/s (medido 4.08 m/s; un perro mediano alcanza más de 5).

**Patas sin patinar:** la fase del trote es la distancia recorrida entre la zancada: $\varphi=s/1.2$ m. Las patas siguen un péndulo de ±36° por KeyFrames (`paso_`, 1 s por ciclo); delantera izquierda y trasera derecha juntas, y las otras dos medio ciclo después (trote en diagonal). El peso de la marcha $w$ es otra pista (0 parado, 1 corriendo) y se mezcla con slerp.
**Reverencia de juego para recoger (2.9–3.3 s):** el pecho baja 22°, el tronco queda a $h=0.349$ m, las traseras quedan verticales ($+22^\circ$ respecto del tronco) y las delanteras se estiran 84°. Comprobación geométrica: cadera delantera a $h-0.186$ m, pata de 0.35 m con ángulo de 62° respecto de la vertical: $0.186+0.35\cos62^\circ=0.349$ m, o sea, las cuatro patas tocan el suelo (la prueba mide < 6 cm de error). La cabeza baja 56° más y el hocico queda a < 12 cm de la pelota.
**Cola:** $\theta(t)=30^\circ\sin(2\pi\cdot3.2\,t)$ de lado, levantada 35° (simple, secundaria).

**Pelota — tiro parabólico en forma cerrada** (no es KeyFrame, no se integra):
$$x(\tau),\,z(\tau)\ \text{lineales},\qquad y(\tau)=y_0+v_y\tau-\tfrac12 g\tau^2,\qquad v_y=\frac{\tfrac12 g T_v^2-(y_0-r)}{T_v}$$
con $T_v=1.5$ s, $g=9.81$ m/s², $r=0.055$ m: la pelota aterriza **exactamente** en $y=r$, $z=-5.9$ (prueba: error < 1 mm). Altura máxima medida 3.43 m (un lanzamiento alto de niño). En el hocico del perro desde 3.1 s hasta 6.2 s; el perro la suelta con una caída parabólica de 0.45 s ($T_{toss}$, misma fórmula) hasta donde queda la mano del niño al recogerla (7.3 s), así no hay saltos. **Pruebas:** velocidad máxima de la pelota < 12 m/s (sin teletransportes); pelota, niño y perro iguales en $t=0$ y $t=10-\varepsilon$.

---

## 8. Sahumerio con copal — KeyFrames

**Función en la experiencia ✅ [repo]:** muestra el gesto de sahumar la ofrenda en la plaza trasera. Sitio `SITIOS[2]` = (1.9, −6.8), junto al arco de cempasúchil (0, −5.6) y frente al altar de pirámide. **Sin humo** (restricción del profesor; el copalero se mueve, no echa humo).
**Función cultural ✅ [repo]:** el copal entre los objetos de la ofrenda lo respalda la ficha 7 (SEP, 2026; INAH, 2025, Boletín 562; ver `docs/fichas_culturales.md`). **Lo que esas fuentes no cubren:** el gesto concreto de balancear el copalero (ver su fila de verificación): es 🟡 adaptación del estudio.

**Ciclo de 9 s:** `torso_` 0→0°, 1.5→$R_x(8)$, 3.0→$R_x(17)$ (reverencia), 4.5→8°, 5.5→0°. `brazoD_`: sube el copalero al frente ($R_x(55)$ a 1.5 s), lo balancea tres veces a los lados con $R_y(\pm28^\circ)R_x(55)$ (2.2, 3.0, 3.8 s), lo eleva a $R_x(105)$ (5.4–6.6 s) y vuelve a reposo a 9 s. `anteD_` 10°→55°→25°→55°→10°; la mano izquierda va al pecho (`brazoI_` $R_x(30)$, `anteI_` $R_x(100)$).
**Copalero colgante (secundaria, simple):** $\theta(t)=14^\circ\sin(2\pi t/1.1)$ sobre X, anclado en la mano derecha. Escala 1.8 (el modelo mide 0.09 m).

---

## 9. Banda al compás — simple (senos)

**Función en la experiencia ✅ [repo]:** la banda tiene más vida que el balanceo de ±2° anterior (X-40) y se mueve **al pulso** de la música. **Función cultural ✅ [repo]:** bandas de viento de Oaxaca en fiestas comunales (ficha 6; México Desconocido, 2023). La música elegida es «Dios Nunca Muere», de la Banda Mixe de Oaxaca (según D2).
Con $T_b=60/\text{BPM}=0.667$ s (90 BPM), para cada músico con fase $\phi$ y sección $i$:
$$\Delta y=A_i\left|\sin\!\left(\frac{\pi t}{T_b}+\phi\right)\right|,\qquad \theta_z=R_i\sin\!\left(\frac{\pi t}{T_b}+\phi\right),\qquad \theta_x=P_i\sin\!\left(\frac{2\pi t}{T_b}+\phi+\frac{\pi}{2}\right)$$
$|\sin|$ vale 0 una vez por pulso (golpe del pie) y su periodo es $T_b$; el giro lateral dura 2 pulsos; la inclinación del instrumento, 1 pulso. Valores por sección (trompeta, clarinete, saxofón, trombón, tuba, tambora, tarola, platillos): $A$ = 2.8, 2.2, 2.2, 2.8, 1.2, 3.6, 3.6, 3.2 cm; $R$ = 3, 4, 4, 3.5, 2, 2.5, 2.5, 3°; $P$ = 5, 3, 3.5, 5, 2, 1.5, 1.5, 6°. Pruebas: el rebote es ≥ 0, < 4 cm y se repite cada $T_b$.
> 🟡 **Límite:** el BPM de la música real («Dios Nunca Muere», Banda Mixe de Oaxaca, D-6k) **no se midió**; 90 BPM es el valor declarado de `Extras::BPM_BANDA`. Si no coincide, la banda se mueve a otro compás que la música.

---

## 10. Anfitriona — simple (senos + giro hacia el visitante)

**Función en la experiencia ✅ [repo]:** da la bienvenida al visitante cuando llega a la orilla y lo hace sentir recibido; reacciona a su posición (interacción). Sitio `SITIOS[3]` = (2.8, 22.0), giro de base 180° (mira al mar).
**Función cultural 🟡 [adaptación]:** la hospitalidad hacia quien llega es un valor de la fiesta, sin fuente registrada.
- **Peso del saludo** (smoothstep): con $d$ la distancia a la cámara, $w=S\!\left(\dfrac{12-d}{12-9}\right)$, $S(u)=3u^2-2u^3$: 1 a menos de 9 m, 0 a más de 12 m.
- **Giro hacia el visitante:** el frente mira a $(-\sin\psi,-\cos\psi)$, así que $\psi^*=\operatorname{atan2}(-dx,-dz)$ con $(dx,dz)$ del sitio a la cámara. Se acerca de forma exponencial: $\psi\leftarrow\psi+\Delta(1-e^{-3\,\Delta t})$ con $\Delta$ la diferencia más corta en $[-180°,180°)$; $\tau=1/3$ s, el 95 % del giro en 1 s, **sin depender de los FPS** (el factor lleva $\Delta t$). Prueba: error de giro < 1°.
- **Saludo:** $\theta_{ante}=85^\circ+25^\circ\sin(2\pi\cdot1.7\,t)$ (antebrazo vertical que oscila a 1.7 Hz) con el brazo a la altura del hombro ($R_z(95^\circ)$); se mezcla con el reposo con $\text{slerp}(\text{reposo},\text{saludo},w)$. Prueba: a 20 m la mano queda a 0.88 m; a 5 m, a 1.74 m.
- **Respiración y peso:** torso $2.5^\circ\sin(2\pi t/3.1)$ y $1.5^\circ\sin(2\pi t/4)$; la cadera se desplaza $1.5\,\text{cm}\cdot\sin(2\pi t/3.1)$.

---

## Límites de esta justificación

1. Los FPS se midieron con todo el proyecto activo, no animación por animación (cada persona suma una decena de piezas pequeñas, de unos 140 a 440 triángulos cada una).
2. No hay fuente APA de la chilena ni de la hospitalidad; por eso se declaran como adaptación del estudio.
