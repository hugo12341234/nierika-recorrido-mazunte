# Justificación matemática de las 3 animaciones simples (C-8)

**Nierika Studio · Equipo 15 · Manual técnico — BORRADOR con valores del código (11/11/2026)**
Tarea: C-8 (act. 45) · Cuentas: 319266894, 321332086

> **Estado del documento (11/11/2026):** las cuatro piezas ya tienen código en `develop` (olas C-1, flores C-4, velas C-3
> y llama C-2) y las tablas llevan **los valores del código** con su `archivo:línea`. **Ninguna se ha compilado ni visto
> en pantalla en el entorno donde se redactó esto** (ESTADO: "falta compilar"): los valores son los escritos en el código,
> no los observados. Lo que sigue sin cerrar queda como `[PENDIENTE]`.
> Regla del proyecto: los valores deben ser **los del código** (archivo:línea). No se rellena nada de memoria.
>
> **Marcas:** `[repo]` = está escrito en el repo · `[PENDIENTE]` = falta el código · `[propuesta]` = idea redactora, no es regla hasta registrarla en el ESTADO (§4/§5).

**Cómo cerrar este documento:** cuando una tarea se fusione, sustituir cada `[PENDIENTE]` por el valor y el `archivo:línea` reales, y quitar el aviso de la sección correspondiente.

---

## 0. Convenciones comunes

| Símbolo | Significado | Fuente |
|---|---|---|
| $t$ | tiempo en segundos desde el inicio, `glfwGetTime()` | `src/main.cpp:170` [repo] |
| $\Delta t$ | tiempo entre cuadros, `deltaTime` | `src/main.cpp:171` [repo] |
| unidades | 1 unidad = 1 m; $y$ hacia arriba; el arco del cuarto mira a $+z$ | CLAUDE.md §4, ESTADO §4 (20/10) [repo] |

**Relación con `deltaTime`.** Hay dos formas de animar, y cada animación debe declarar cuál usa:

1. **Posición como función del tiempo absoluto:** $\mathbf{p}(t)$ se evalúa con $t$ = `glfwGetTime()`. No acumula error y no depende de los FPS: a 30 o a 144 FPS el objeto está en el mismo sitio en el mismo segundo. Es la opción usada en olas, velas y flores (todas son periódicas o paramétricas).
2. **Estado integrado:** $s_{n+1}=s_n+v\,\Delta t$. Se usa solo si la velocidad cambia con la entrada del usuario (movimiento de cámara, `DoMovement()`).

Las animaciones simples son del tipo 1 y el código lo confirma: olas (`tiempo` = `glfwGetTime()`, `src/main.cpp:377`), flores (`src/main.cpp:486`) y velas (`src/main.cpp:495`) evalúan funciones cerradas de $t$; ninguna acumula `deltaTime`.

---

## 1. Olas del mar (C-1) — suma de senoidales en el *vertex shader*

**Estado:** implementado (C-1, 04/11) en `src/Olas.h` (parámetros y misma altura en CPU) y `Shaders/lighting.vs` (con `uniform int oleaje`; no existe `olas.vs`: decisión 04/11 "Olas"). Sin compilar.
**Malla ya fija [repo]:** `Models/mar/mar.obj`, 100 × 100 cuadros de 1 m, 10 201 vértices, x −50…+50, z +22…+122, plano en y = −0.2 (ESTADO §1 y decisión 20/10).

### 1.1 Objetivo visual y por qué esta técnica
Un oleaje que se vea continuo, sin repetirse de forma obvia, a un costo mínimo: la malla es estática y la GPU desplaza cada vértice. Una sola senoide se ve como "persiana"; la **suma de varias** con distinta dirección, longitud de onda y fase rompe la periodicidad visible. Es la técnica que pide la propuesta (CONTEXTO §4, animación 3).

### 1.2 Fórmula
Altura del vértice en la posición horizontal $\mathbf{p}=(x,z)$ (coordenadas del mundo, pues el `.obj` ya las trae [repo]):

$$
y(\mathbf{p},t)=y_0+\sum_{i=1}^{N} A_i\,\sin\!\big(k_i\,(\mathbf{d}_i\cdot\mathbf{p})-\omega_i\,t+\varphi_i\big)
$$

| Término | Significado | Relación |
|---|---|---|
| $y_0$ | altura de reposo del mar | $-0.2$ m [repo] |
| $N$ | número de olas sumadas | 3 (`Olas.h:19`) |
| $A_i$ | amplitud: altura máxima de la ola $i$ sobre $y_0$ (m) | 0.040 / 0.025 / 0.015 (`Olas.h:35-37`) |
| $\lambda_i$ | longitud de onda (m) | 12 / 7 / 4.5 (`Olas.h:35-37`) |
| $k_i=2\pi/\lambda_i$ | número de onda (rad/m): cuántos radianes avanza la fase por metro | derivado |
| $\mathbf{d}_i=(\sin\theta_i,-\cos\theta_i)$ | dirección de propagación, vector **unitario** en el plano XZ (**el código usa esta forma**, no $(\cos\theta,\sin\theta)$: con $\theta=0$ la ola avanza hacia la orilla, $-z$; `Olas.h:42-43`) | $\theta_i$ = 0° / 25° / −30° |
| $\omega_i$ | frecuencia angular (rad/s) | ver 1.3 |
| $\varphi_i$ | fase inicial (rad): desfasa unas olas de otras | 0 / 1.7 / 4.1 (`Olas.h:35-37`) |

**Por qué el signo $-\omega t$:** con $\sin(k\,\mathbf{d}\cdot\mathbf{p}-\omega t)$ una cresta cumple $k\,\mathbf{d}\cdot\mathbf{p}-\omega t=\text{cte}$, es decir $\mathbf{d}\cdot\mathbf{p}=\text{cte}+(\omega/k)\,t$: la cresta avanza **en el sentido de $+\mathbf{d}$** con velocidad de fase $c_i=\omega_i/k_i$. Con $+\omega t$ viajaría al revés.

### 1.3 Elección de la frecuencia $\omega_i$
El código usa la **relación de dispersión de aguas profundas** (`Olas.h:41`): $\omega_i=\sqrt{g\,k_i}$, con $g=9.81\ \text{m/s}^2$ (`Olas.h:21`). Las olas largas viajan más rápido que las cortas, como en el mar real. Con los valores de 1.6: periodos $T_i=2\pi/\omega_i$ = 2.77 / 2.12 / 1.70 s y velocidades de fase $c_i=\omega_i/k_i$ = 4.33 / 3.31 / 2.65 m/s (cálculo de la decisión 04/11 del ESTADO; no medidos en pantalla). La alternativa descartada era fijar $T_i$ a mano ($\omega_i=2\pi/T_i$).

### 1.4 Normal analítica (para que la luz Blinn-Phong reaccione a las olas)
Derivadas parciales de $y$:

$$
\frac{\partial y}{\partial x}=\sum_i A_i\,k_i\,d_{i,x}\cos(\cdot),\qquad
\frac{\partial y}{\partial z}=\sum_i A_i\,k_i\,d_{i,z}\cos(\cdot)
$$

La normal de la superficie $y=f(x,z)$ es el gradiente de $F=y-f(x,z)$, normalizado:

$$
\mathbf{n}=\frac{\left(-\dfrac{\partial y}{\partial x},\ 1,\ -\dfrac{\partial y}{\partial z}\right)}{\left\|\left(-\dfrac{\partial y}{\partial x},\ 1,\ -\dfrac{\partial y}{\partial z}\right)\right\|}
$$

Con $y$ constante da $\mathbf{n}=(0,1,0)$, que es la normal actual del `.obj` [repo]. Sin esta normal, las olas se moverían pero la iluminación seguiría plana (el mar tiene especular 0.6 [repo, ESTADO §5]). El código la calcula así, tanto en GPU (`lighting.vs:42`, `normalize(vec3(-dhdx, 1.0, -dhdz))`) como en CPU (`Olas.h:58-67`). Según el ESTADO (04/11), una prueba en CPU con `g++` dio un error de 3×10⁻⁵ contra diferencias finitas; no se repitió en este borrador.

### 1.5 Restricciones numéricas (verificables ya, antes del código)
1. **Muestreo (Nyquist).** La malla tiene un vértice cada $h=1$ m. Para que una senoide se vea como tal hacen falta ≥ 4 vértices por longitud de onda: $\lambda_i\ge 4h=4$ m. Esta es la razón registrada el 20/10 para los cuadros de 1 m [repo, decisión "Playa y mar"]. Con $\lambda<2$ m ocurre *aliasing* (la ola aparece con otra longitud).
2. **Pendiente máxima.** La pendiente de cada senoide es $A_i k_i=2\pi A_i/\lambda_i$. Para que la superficie no se pliegue sobre sí misma basta $\sum_i A_i k_i<1$ **[propuesta]** (criterio estándar: pendiente total < 1 ⇒ no hay autointersección en el modelo de altura pura).
3. **No romper la orilla.** La arena baja de $y=-0.2$ en $z=+24$ [repo]. Si $\sum A_i>0.1$ m, el mar puede subir a $y>-0.1$ y cruzar la arena donde ya no debe (en $z=+22$ la arena está en $y=-0.1$). Verificar $\sum_i A_i$ contra esos 0.1 m **[propuesta]** y revisar la prueba de z-fighting de A-7 con las olas activas.

### 1.6 Tabla de parámetros (a llenar con el código)

| Símbolo | Valor en el código | Unidad | Archivo:línea |
|---|---|---|---|
| $N$ | 3 | — | `src/Olas.h:19` |
| $A_i$ | 0.040, 0.025, 0.015 (suma 0.080 < 0.1 m) | m | `src/Olas.h:35-37` |
| $\lambda_i$ | 12, 7, 4.5 (todas ≥ 4 m) | m | `src/Olas.h:35-37` |
| $\theta_i$ | 0°, 25°, −30° (en grados en el código) | ° | `src/Olas.h:35-37` |
| $\omega_i$ | $\sqrt{9.81\,k_i}$ (periodos 2.77 / 2.12 / 1.70 s, calculados) | rad/s | `src/Olas.h:41` |
| $\varphi_i$ | 0, 1.7, 4.1 | rad | `src/Olas.h:35-37` |
| pendiente total $\sum A_ik_i$ | ≈ 0.064 (< 1, calculada con esos valores) | — | ESTADO §4, 04/11 |

### 1.7 Integración
$t$ llega al shader como *uniform* (`glUniform1f(... "tiempo", glfwGetTime())`, `src/main.cpp:377`); no depende de $\Delta t$. Costo: $N$ senos + $N$ cosenos por vértice × 10 201 vértices por cuadro (si se calcula la normal en el mismo shader).

### 1.8 Por qué no es una animación del curso
Argumento (a revisar por D2: **no se comparó con el código de las prácticas del curso**, que no está en este repositorio): el código implementa una **suma de 3 senoidales direccionales** con $\omega$ ligada a $k$ por la relación de dispersión y **normal analítica** para la luz, y comparte los parámetros entre CPU (flores y velas flotan sobre la misma $y(x,z,t)$) y GPU. No es el desplazamiento de una sola onda.

---

## 2. Velas flotando (C-3) y llama (C-2)

**Estado:** implementado el 11/11: trayectoria y vaivén en `src/AnimVelas.h` (C-3) y llama en `Shaders/lighting.frag` (C-2). Sin compilar; el GLSL no se validó. Valores y motivos: ESTADO §4, decisiones del 11/11 "Velas flotando" y "Llama".
**Aclaración del profesor [repo]:** las velas flotando **no** cuentan como KeyFrames (CONTEXTO §4).

### 2.1 Trayectoria: Bézier mar adentro
Curva de Bézier cuadrática con puntos de control $P_0$ (orilla), $P_1$ (control), $P_2$ (mar adentro):

$$
B(u)=(1-u)^2P_0+2(1-u)u\,P_1+u^2P_2,\qquad u\in[0,1]
$$

Deducción: es la interpolación lineal de dos interpolaciones lineales (algoritmo de De Casteljau): $Q_0=(1-u)P_0+uP_1$, $Q_1=(1-u)P_1+uP_2$, $B=(1-u)Q_0+uQ_1$. Propiedades útiles: pasa por $P_0$ y $P_2$; la tangente en $P_0$ apunta hacia $P_1$ ($B'(0)=2(P_1-P_0)$) y en $P_2$ desde $P_1$ ($B'(1)=2(P_2-P_1)$).

El código usa la **cuadrática** (3 puntos; `AnimVelas.h`, `PosicionVelaXZ`). La cúbica ($B(u)=(1-u)^3P_0+3(1-u)^2u\,P_1+3(1-u)u^2P_2+u^3P_3$) no se usa.

**Parámetro $u$ en función del tiempo.** El código usa una **tercera opción**, ida y vuelta suave (`ParametroVela`):

$$
u(t)=\tfrac12-\tfrac12\cos\!\big(2\pi\,(t+t_0)/T\big),\qquad \dot u=\tfrac{\pi}{T}\sin\!\big(2\pi\,(t+t_0)/T\big)
$$

$u(0)=0$ (orilla), $u(T/2)=1$ (mar adentro), $u(T)=0$; $\dot u=0$ en los extremos, así la vela arranca y se detiene sin saltos y **no hay reinicio visible** (las opciones $\min(t/T,1)$ y $\text{frac}(t/T)$ se descartaron: la primera deja la vela parada para siempre y la segunda salta). La velocidad en el espacio **no es constante** ($\|B'(u)\|$ varía y $\dot u$ también); no se reparametrizó por longitud de arco. En una simulación aparte con `g++` (3 periodos por vela, paso de 0.05 s) la velocidad máxima fue de 1.11 m/s.

**Puntos de control** (`src/main.cpp:208-214`, 4 velas): $P_0=(x_0,25)$, $P_1=(x_0\pm8,\,40)$, $P_2=(x_0\pm3,\,60)$ con $x_0=-2.5,\,0.5,\,2.5,\,5.0$ y signo alterno ($+,-,+,-$). La orilla es $z=+24$ y el mar llega hasta $z=+122$ **[repo]**: la curva queda entre $z=25$ y $z=60$.

### 2.2 Oscilación senoidal vertical y desfase por vela
Para la vela $j$ de un total de $M$:

$$
y_j(t)=y_{\text{base}}+a\,\sin(\omega_v\,t+\varphi_j),\qquad \varphi_j=\frac{2\pi\,j}{M}\ \ [\text{propuesta}]
$$

| Término | Significado | Valor |
|---|---|---|
| $y_{\text{base}}$ | altura de flotación | `Olas::Altura(x, z, t)` − 0.03 m (calado, `CALADO_VELA`) |
| $a$ | amplitud del vaivén (m) | 0.012 |
| $\omega_v=2\pi/T_v$ | frecuencia angular (rad/s) | $2\pi/2.4$ ($T_v=2.4$ s) |
| $\varphi_j$ | desfase de la vela $j$ | $0,\ \pi/2,\ \pi,\ 3\pi/2$ (regla $2\pi j/M$, decisión 11/11) |
| $M$ | número de velas | 4 |

**Coherencia con las olas:** la altura base de cada vela es **la misma** $y(\mathbf{p},t)$ de la sección 1 evaluada en $(x,z)$ de la vela (`Olas::Altura`, `src/main.cpp:498`), a la que se suma el vaivén propio; así la vela sigue al oleaje y además tiene su oscilación senoidal con desfase.

### 2.3 Llama: *fragment shader* con ruido (C-2)
El código (`Shaders/lighting.frag`, rama `llama == 1`) usa **ruido de Perlin 2D**: en cada vértice entero $\mathbf{v}$ de la rejilla hay un gradiente unitario $\mathbf{g}(\mathbf{v})=(\cos\alpha,\sin\alpha)$ con $\alpha=2\pi\,\text{fract}\big(\sin(\mathbf{v}\cdot(127.1,311.7))\cdot43758.5453\big)$; para un punto $\mathbf{p}$ con $\mathbf{i}=\lfloor\mathbf{p}\rfloor$, $\mathbf{f}=\mathbf{p}-\mathbf{i}$:

$$
\text{perlin}(\mathbf{p})=\text{mix}\big(\text{mix}(a,b,u_x),\ \text{mix}(c,d,u_x),\ u_y\big),\quad a=\mathbf{g}(\mathbf{i})\cdot\mathbf{f},\ b=\mathbf{g}(\mathbf{i}+\hat x)\cdot(\mathbf{f}-\hat x),\ \dots,\quad u=s(\mathbf{f})
$$

con $s(u)=6u^5-15u^4+10u^3$ (Perlin mejorado: derivada primera y segunda nulas en los extremos de la celda, sin costuras). Se suman **3 octavas** (frecuencia ×2 y amplitud ×0.5 cada vez): $\text{fbm}=\sum_{k=0}^{2}0.5^{k+1}\,\text{perlin}(2^k\mathbf{p})$; rango medido en una copia numpy: −0.40…+0.38. Mapeo a la llama: con $h$ = altura en la llama (0 base, 1 punta) y $t'=t+\text{fase}$,

$$
n=\text{fbm}\big(2.5u,\ 3h-2t'\big),\quad x'=x+0.55\,n\,h,\quad \text{ancho}=0.85\,(1-h)^{0.7},\quad \text{forma}=1-\frac{|x'|}{\text{ancho}}-0.25\,h\,(0.5+0.5n)
$$

Se descarta el fragmento si `forma < 0.1`; el color pasa de rojo a naranja y a amarillo claro según $\text{calor}=\text{clamp}(1.2\,\text{forma}-0.5h+0.3)$. Restar $2t'$ a la coordenada vertical hace que el ruido **suba**; el desplazamiento lateral crece con $h$, por eso la punta se mueve más. La llama **no** cuenta como KeyFrames y el humo con partículas está fuera de alcance [repo].

### 2.4 Tabla de parámetros (a llenar con el código)

| Símbolo | Valor en el código | Unidad | Archivo:línea |
|---|---|---|---|
| $P_0,P_1,P_2$ | ver 2.1 | m | `src/main.cpp:208-214` |
| $T$ (ida y vuelta) y $t_0$ | 100 / 110 / 120 / 130 s y 0 / 17 / 41 / 63 s | s | `src/main.cpp:208-214` |
| $M$ | 4 | — | `src/main.cpp:208-214` |
| $a$, $T_v$, $\varphi_j$ | 0.012 m, 2.4 s, $2\pi j/4$ | m, s, rad | `src/main.cpp:208-214` |
| octavas y escala del ruido | 3 octavas; escala (2.5, 3) en (u, h); ruido sube a 2 unidades/s | — | `Shaders/lighting.frag` (rama `llama`) |

### 2.5 Por qué no es una animación del curso
Argumento (a revisar por D2: **no se comparó con las prácticas del curso**): la trayectoria es una curva de Bézier cuadrática recorrida con una ley $u(t)$ de ida y vuelta, más un vaivén con desfase por vela y la altura de las olas de C-1; la llama es ruido de Perlin con fbm de 3 octavas animado en el tiempo. No es un desplazamiento lineal ni una rotación uniforme.

---

## 3. Flores de cempasúchil flotando (C-4)

**Estado:** implementado (C-4, 04/11) en `src/AnimFlores.h` y `src/main.cpp` (5 flores en la orilla). Usa `Models/flor/flor.obj` (extra de D1, X-3) como modelo provisional. Sin compilar.

### 3.1 Movimiento circular de radio pequeño
Para la flor $j$ con centro $\mathbf{c}_j=(c_x,c_z)$ y radio $r$:

$$
x_j=c_x+r\cos\theta_j(t),\qquad z_j=c_z+r\sin\theta_j(t),\qquad y_j=y_{\text{agua}}\ [+\ \text{vaivén opcional}]
$$

Deducción: la parametrización de una circunferencia es $(\cos\theta,\sin\theta)$; escalar por $r$ y trasladar por $\mathbf{c}$ da el círculo de radio $r$. $\theta$ es el ángulo recorrido.

### 3.2 Rotación continua de la flor sobre su eje
Matriz de modelo: $M=T(x_j,y_j,z_j)\ R_y(\psi_j(t))\ S$, con $R_y(\psi)$ giro alrededor del eje vertical. En GLM: `glm::rotate(model, psi, glm::vec3(0,1,0))`. El código usa $\psi_j(t)=\psi_{0,j}+\omega_{g,j}\,t$, rotación uniforme **independiente** de $\theta$ y que no se detiene en las pausas (`AnimFlores.h:60-69`); se gira sobre el centro del `.obj`, $(0,0,-0.075)$.

### 3.3 Velocidad variable con pausas
La propuesta pide velocidad angular variable con pausas: $\theta(t)=\int_0^t \omega(s)\,ds$. El código (decisión 04/11 "Flores flotando") usa un ciclo de periodo $T=t_{mov}+t_{pausa}$; con $u=t \bmod T$:

$$
\omega(u)=\omega_{\max}\sin^2\!\big(\pi u/t_{mov}\big)\ \ (0\le u<t_{mov}),\qquad \omega=0\ \ (\text{pausa}),\qquad \omega_{\max}=2\pi/t_{mov}
$$

Como $\sin^2x=(1-\cos2x)/2$, la integral tiene forma cerrada: $\theta(u)=\omega_{\max}\big(u/2-t_{mov}\sin(2\pi u/t_{mov})/(4\pi)\big)$, y cada fase de movimiento recorre exactamente $\pi$ (media vuelta); tras $n$ ciclos $\theta=n\pi+\theta(u)$ (`AnimFlores.h:36-45`). $\omega$ vale 0 en los extremos de la fase, así que la velocidad es continua (sin escalón de aceleración). **Cuidado numérico resuelto:** $\theta$ es función cerrada de $t$ (no se acumula `theta += omega * deltaTime`), así que no depende de los FPS ni deriva.
**Prioridad:** las pausas son lo **primero que se recorta** si falta tiempo (ESTADO, "qué recortar primero") [repo]; si se recortan, esta sección se reduce a $\omega$ constante.

### 3.4 Tabla de parámetros (a llenar con el código)

| Símbolo | Valor en el código | Unidad | Archivo:línea |
|---|---|---|---|
| $\mathbf{c}_j$ | (−4, 28), (−1.5, 30), (1, 27), (3.5, 29.5), (6, 27.5) | m | `src/main.cpp:198-203` |
| $r$ | 0.60, 0.45, 0.75, 0.50, 0.65 | m | `src/main.cpp:198-203` |
| $\omega_{\max}$ / $\omega(t)$ | $2\pi/t_{mov}$ ($t_{mov}$ = 5, 4.5, 5.5, 5, 4 s) con perfil $\sin^2$ | rad/s | `src/AnimFlores.h:36-45` |
| duración de las pausas ($t_{pausa}$) y desfases | 2, 2.5, 1.5, 3, 2 s · desfases 0, 1.7, 3.1, 4.4, 5.8 s | s | `src/main.cpp:198-203` |
| $\omega_r$ (giro propio) | 0.8, 0.6, 1.0, 0.7, 0.9 | rad/s | `src/main.cpp:198-203` |
| número de flores | 5 | — | `src/main.cpp:198` |

### 3.5 Por qué no es una animación del curso
Argumento (a revisar por D2: **no se comparó con las prácticas del curso**): la velocidad angular es variable con perfil $\sin^2$ y pausas, integrada en forma cerrada, y la altura sigue a las olas de C-1; no es una rotación uniforme `glm::rotate(..., glfwGetTime(), ...)`. El código sí implementa las pausas (`AnimFlores.h:36-45`).

---

## 4. Qué falta para cerrar C-8

| Hueco | Quién lo cierra |
|---|---|
| Compilar y ver las cuatro animaciones en pantalla; los valores de este documento son los del código, **no los observados** | D2 (F5, Release x64) |
| Medir FPS con las tres animaciones a la vez (C-5): no hay ningún valor medido | D2 |
| Confirmar con D2 los valores propuestos el 11/11 (velas y llama) y los del 04/11 (olas y flores) | D2, en el ESTADO §4 |
| Revisar el argumento "no es una animación del curso" de cada una contra las prácticas (no están en el repo) | D2 / D1 |
| Número de página y referencias APA 7 del documento final (manual técnico, E-5) | E-5 |

**Criterio de terminado de C-8 [repo]:** fórmulas y parámetros **reales** en `docs/manual_tecnico/`. Los valores ya son los del código; la tarea pasa a ✅ cuando se vean funcionando y D2 confirme los argumentos.
