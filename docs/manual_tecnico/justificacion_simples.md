# Justificación matemática de las 3 animaciones simples (C-8)

**Nierika Studio · Equipo 15 · Manual técnico — BORRADOR (04/11/2026)**
Tarea: C-8 (act. 45) · Cuentas: 319266894, 321332086

> **Estado del documento: borrador.** Las fórmulas y su deducción son generales y ya están completas.
> **Ninguna de las tres animaciones tiene código todavía** (C-1, C-3, C-4 y C-2 están ⏳ en el ESTADO).
> Por eso las tablas de parámetros tienen la columna *Valor en el código* vacía, marcada `[PENDIENTE]`.
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

Las tres animaciones simples son del tipo 1 **[propuesta]**; se confirma con el código.

---

## 1. Olas del mar (C-1) — suma de senoidales en el *vertex shader*

**Estado:** `[PENDIENTE]` — `Shaders/olas.vs` y `src/Olas.h` no existen.
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
| $N$ | número de olas sumadas | `[PENDIENTE]` |
| $A_i$ | amplitud: altura máxima de la ola $i$ sobre $y_0$ (m) | `[PENDIENTE]` |
| $\lambda_i$ | longitud de onda (m) | `[PENDIENTE]` |
| $k_i=2\pi/\lambda_i$ | número de onda (rad/m): cuántos radianes avanza la fase por metro | derivado |
| $\mathbf{d}_i=(\cos\theta_i,\sin\theta_i)$ | dirección de propagación, vector **unitario** en el plano XZ | $\theta_i$ `[PENDIENTE]` |
| $\omega_i$ | frecuencia angular (rad/s) | ver 1.3 |
| $\varphi_i$ | fase inicial (rad): desfasa unas olas de otras | `[PENDIENTE]` |

**Por qué el signo $-\omega t$:** con $\sin(k\,\mathbf{d}\cdot\mathbf{p}-\omega t)$ una cresta cumple $k\,\mathbf{d}\cdot\mathbf{p}-\omega t=\text{cte}$, es decir $\mathbf{d}\cdot\mathbf{p}=\text{cte}+(\omega/k)\,t$: la cresta avanza **en el sentido de $+\mathbf{d}$** con velocidad de fase $c_i=\omega_i/k_i$. Con $+\omega t$ viajaría al revés.

### 1.3 Elección de la frecuencia $\omega_i$
Dos opciones; el código debe declarar cuál usa:

- **Libre** $[propuesta]$: se fija un periodo $T_i$ y $\omega_i=2\pi/T_i$. Simple, controlable artísticamente.
- **Relación de dispersión de aguas profundas** $[propuesta]$: $\omega_i=\sqrt{g\,k_i}$, con $g=9.81\ \text{m/s}^2$. Las olas largas viajan más rápido que las cortas, como en el mar real; es la mejor justificación física y diferencia el resultado de un oleaje "arbitrario".

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

Con $y$ constante da $\mathbf{n}=(0,1,0)$, que es la normal actual del `.obj` [repo]. Sin esta normal, las olas se moverían pero la iluminación seguiría plana (el mar tiene especular 0.6 [repo, ESTADO §5]). `[PENDIENTE]`: confirmar si el código la calcula así.

### 1.5 Restricciones numéricas (verificables ya, antes del código)
1. **Muestreo (Nyquist).** La malla tiene un vértice cada $h=1$ m. Para que una senoide se vea como tal hacen falta ≥ 4 vértices por longitud de onda: $\lambda_i\ge 4h=4$ m. Esta es la razón registrada el 20/10 para los cuadros de 1 m [repo, decisión "Playa y mar"]. Con $\lambda<2$ m ocurre *aliasing* (la ola aparece con otra longitud).
2. **Pendiente máxima.** La pendiente de cada senoide es $A_i k_i=2\pi A_i/\lambda_i$. Para que la superficie no se pliegue sobre sí misma basta $\sum_i A_i k_i<1$ **[propuesta]** (criterio estándar: pendiente total < 1 ⇒ no hay autointersección en el modelo de altura pura).
3. **No romper la orilla.** La arena baja de $y=-0.2$ en $z=+24$ [repo]. Si $\sum A_i>0.1$ m, el mar puede subir a $y>-0.1$ y cruzar la arena donde ya no debe (en $z=+22$ la arena está en $y=-0.1$). Verificar $\sum_i A_i$ contra esos 0.1 m **[propuesta]** y revisar la prueba de z-fighting de A-7 con las olas activas.

### 1.6 Tabla de parámetros (a llenar con el código)

| Símbolo | Valor en el código | Unidad | Archivo:línea |
|---|---|---|---|
| $N$ | `[PENDIENTE]` | — | |
| $A_i$ | `[PENDIENTE]` | m | |
| $\lambda_i$ | `[PENDIENTE]` (≥ 4 m) | m | |
| $\theta_i$ | `[PENDIENTE]` | rad | |
| $\omega_i$ (o $T_i$) | `[PENDIENTE]` | rad/s | |
| $\varphi_i$ | `[PENDIENTE]` | rad | |

### 1.7 Integración
$t$ llega al shader como *uniform* (`glUniform1f`, `glfwGetTime()`); no depende de $\Delta t$ `[PENDIENTE: confirmar]`. Costo: $N$ senos + $N$ cosenos por vértice × 10 201 vértices por cuadro (si se calcula la normal en el mismo shader).

### 1.8 Por qué no es una animación del curso
`[PENDIENTE]` — se redacta con el código. Argumento base: lo que se pide justificar es el **modelo** (suma de $N$ senoidales direccionales con $\omega$ ligada a $k$ y normal analítica), no un desplazamiento de una sola onda. Si el código usa una sola senoide, este argumento no aplica y hay que decirlo.

---

## 2. Velas flotando (C-3) y llama (C-2)

**Estado:** `[PENDIENTE]` — no hay vela (B-2 ⏳), ni trayectoria, ni shader de llama.
**Aclaración del profesor [repo]:** las velas flotando **no** cuentan como KeyFrames (CONTEXTO §4).

### 2.1 Trayectoria: Bézier mar adentro
Curva de Bézier cuadrática con puntos de control $P_0$ (orilla), $P_1$ (control), $P_2$ (mar adentro):

$$
B(u)=(1-u)^2P_0+2(1-u)u\,P_1+u^2P_2,\qquad u\in[0,1]
$$

Deducción: es la interpolación lineal de dos interpolaciones lineales (algoritmo de De Casteljau): $Q_0=(1-u)P_0+uP_1$, $Q_1=(1-u)P_1+uP_2$, $B=(1-u)Q_0+uQ_1$. Propiedades útiles: pasa por $P_0$ y $P_2$; la tangente en $P_0$ apunta hacia $P_1$ ($B'(0)=2(P_1-P_0)$) y en $P_2$ desde $P_1$ ($B'(1)=2(P_2-P_1)$).

Si la propuesta final usa Bézier **cúbica** (4 puntos): $B(u)=(1-u)^3P_0+3(1-u)^2u\,P_1+3(1-u)u^2P_2+u^3P_3$. `[PENDIENTE: cuál usa el código]`.

**Parámetro $u$ en función del tiempo.** `[PENDIENTE]`. Opciones: $u=\min(t/T,1)$ (lineal; la vela se detiene al final) o $u=\text{frac}(t/T)$ (se reinicia: salto visible, hay que ocultarlo). Nota: con $u$ lineal la **velocidad no es constante** en el espacio, porque $\|B'(u)\|$ varía; si importa, reparametrizar por longitud de arco.

**Puntos de control.** `[PENDIENTE]`. Restricciones que sí están en el repo: la orilla es $z=+24$ y el mar llega hasta $z=+122$, $x\in[-50,50]$; la altura del agua es $y_0=-0.2$ m **[repo]** (con olas, ver 2.3).

### 2.2 Oscilación senoidal vertical y desfase por vela
Para la vela $j$ de un total de $M$:

$$
y_j(t)=y_{\text{base}}+a\,\sin(\omega_v\,t+\varphi_j),\qquad \varphi_j=\frac{2\pi\,j}{M}\ \ [\text{propuesta}]
$$

| Término | Significado | Valor |
|---|---|---|
| $y_{\text{base}}$ | altura de flotación | `[PENDIENTE]` |
| $a$ | amplitud del vaivén (m) | `[PENDIENTE]` |
| $\omega_v=2\pi/T_v$ | frecuencia angular (rad/s) | `[PENDIENTE]` |
| $\varphi_j$ | desfase de la vela $j$ | `[PENDIENTE]` (la propuesta pide "desfase por vela", **no fija la regla**; $2\pi j/M$ reparte las fases uniformemente) |
| $M$ | número de velas | `[PENDIENTE]` |

**Coherencia con las olas [propuesta]:** si la vela debe parecer flotar sobre el oleaje, lo más fiel es evaluar la **misma** $y(\mathbf{p},t)$ de la sección 1 en la posición $(x,z)$ de la vela, en vez de una senoide independiente. Hoy no está decidido; registrar la decisión en el ESTADO (§4) al implementar C-3.

### 2.3 Llama: *fragment shader* con ruido (C-2)
`[PENDIENTE]` — la propuesta pide "ruido Perlin o similar". Se documentará: función de ruido elegida (valor, Perlin, simplex…), su definición matemática (rejilla de gradientes, interpolación con $s(u)=6u^5-15u^4+10u^3$ en Perlin mejorado, o $3u^2-2u^3$ en el clásico), cuántas octavas se suman y cómo se mapea a color/alfa. **No hay código ni valores aún.** Recordatorio: la llama **no** cuenta como KeyFrames y el humo con partículas está fuera de alcance [repo].

### 2.4 Tabla de parámetros (a llenar con el código)

| Símbolo | Valor en el código | Unidad | Archivo:línea |
|---|---|---|---|
| $P_0,P_1,P_2\,(,P_3)$ | `[PENDIENTE]` | m | |
| $T$ (duración del recorrido) | `[PENDIENTE]` | s | |
| $M$ | `[PENDIENTE]` | — | |
| $a$, $T_v$, $\varphi_j$ | `[PENDIENTE]` | m, s, rad | |
| octavas y escala del ruido | `[PENDIENTE]` | — | |

### 2.5 Por qué no es una animación del curso
`[PENDIENTE]` — se redacta con el código. Argumento base: la trayectoria es una curva paramétrica (Bézier) con oscilación y desfase propios, no un desplazamiento lineal ni una órbita de las prácticas.

---

## 3. Flores de cempasúchil flotando (C-4)

**Estado:** `[PENDIENTE]` — `src/AnimFlores.h` no existe. Hoy `Models/flor/flor.obj` (extra de D1, X-3) solo se dibuja **estática** sobre el altar (`src/main.cpp:293-296`) [repo]; sirve de modelo provisional.

### 3.1 Movimiento circular de radio pequeño
Para la flor $j$ con centro $\mathbf{c}_j=(c_x,c_z)$ y radio $r$:

$$
x_j=c_x+r\cos\theta_j(t),\qquad z_j=c_z+r\sin\theta_j(t),\qquad y_j=y_{\text{agua}}\ [+\ \text{vaivén opcional}]
$$

Deducción: la parametrización de una circunferencia es $(\cos\theta,\sin\theta)$; escalar por $r$ y trasladar por $\mathbf{c}$ da el círculo de radio $r$. $\theta$ es el ángulo recorrido.

### 3.2 Rotación continua de la flor sobre su eje
Matriz de modelo: $M=T(x_j,y_j,z_j)\ R_y(\psi_j(t))\ S$, con $R_y(\psi)$ giro alrededor del eje vertical. En GLM: `glm::rotate(model, psi, glm::vec3(0,1,0))`. `[PENDIENTE]`: $\psi(t)=\omega_r t$ (rotación uniforme) o ligada a $\theta$.

### 3.3 Velocidad variable con pausas
La propuesta pide velocidad angular variable con pausas: $\theta(t)=\int_0^t \omega(s)\,ds$. **No está definida la forma de $\omega(t)$** (CONTEXTO §4, ESTADO C-4: "velocidad variable con pausas"). Opciones, todas `[propuesta]`:

- **Modulación senoidal rectificada:** $\omega(t)=\omega_{\max}\,\max\big(0,\sin(\Omega t+\phi_j)\big)$. Hay pausa mientras el seno es negativo. Su integral tiene forma cerrada: durante cada semiperiodo activo, $\theta$ crece $2\omega_{\max}/\Omega$ rad; es continua en $t$, sin saltos de posición.
- **Con suavizado ($s(u)=3u^2-2u^3$) en la entrada y salida de cada pausa**, para que la aceleración no sea un escalón.

**Cuidado numérico:** $\theta$ debe obtenerse **integrando** $\omega$. Si se escribe $\theta=\omega(t)\cdot t$ la posición salta cada vez que cambia $\omega$ (error típico). Al integrar con $\Delta t$ (`theta += omega * deltaTime`), el resultado depende de los FPS; con la forma cerrada es exacto. `[PENDIENTE]`: declarar cuál usa el código.
**Prioridad:** las pausas son lo **primero que se recorta** si falta tiempo (ESTADO, "qué recortar primero") [repo]; si se recortan, esta sección se reduce a $\omega$ constante.

### 3.4 Tabla de parámetros (a llenar con el código)

| Símbolo | Valor en el código | Unidad | Archivo:línea |
|---|---|---|---|
| $\mathbf{c}_j$ | `[PENDIENTE]` | m | |
| $r$ | `[PENDIENTE]` | m | |
| $\omega_{\max}$ / $\omega(t)$ | `[PENDIENTE]` | rad/s | |
| duración y posición de las pausas | `[PENDIENTE]` | s | |
| $\omega_r$ (giro propio) | `[PENDIENTE]` | rad/s | |
| número de flores | `[PENDIENTE]` | — | |

### 3.5 Por qué no es una animación del curso
`[PENDIENTE]` — se redacta con el código. Argumento base: la velocidad angular es variable con pausas (integrada), no una rotación uniforme `glm::rotate(..., glfwGetTime(), ...)` como en las prácticas; solo se sostiene si el código realmente implementa las pausas.

---

## 4. Resumen de huecos que impiden cerrar C-8

| Hueco | Quién lo cierra | Tarea |
|---|---|---|
| Parámetros de las olas ($N$, $A_i$, $\lambda_i$, $\theta_i$, $\omega_i$, $\varphi_i$) | hilo de C-1 | C-1 |
| Forma de la regla de desfase y puntos de la Bézier | hilo de C-3 | C-3 |
| Función de ruido de la llama | hilo de C-2 | C-2 |
| Forma de $\omega(t)$ con pausas | hilo de C-4 | C-4 |
| Argumento "no es del curso" de las tres | al tener código | C-8 |

**Criterio de terminado de C-8 [repo]:** fórmulas y parámetros **reales** en `docs/manual_tecnico/`. Este borrador **no lo cumple** todavía: lo adelanta.
