# Justificación matemática de las 2 animaciones por KeyFrames (D-7)

**Nierika Studio · Equipo 15 · Manual técnico — versión del 12/11/2026**
Tarea: D-7 (act. 53), con D-2 (slerp), D-3 (smoothstep y desfase) y D-4 (Bézier de la flor) · Cuentas: 319266894, 321332086

> **Estado del documento (12/11): completo con los valores del código.** Las fórmulas, su deducción y los parámetros reales (con archivo)
> están escritos; **las cifras se comprobaron con un programa** (`tools/d7_pruebas_matematicas.cpp`, 23 comprobaciones sobre `Animacion.cpp`,
> `Silueta.cpp` y `DejarFlor.h`). Lo que sigue **sin comprobar**: que la animación se vea bien en pantalla con el modelo real de D1 (nadie la ha
> compilado con MSVC desde el cambio de cubos a modelo) y la comparación con las prácticas del curso (§5, que no están en el repo).
>
> **Marcas:** `[repo]` = escrito en el repo · `[propuesta]` = idea redactora, no es regla hasta registrarla en el ESTADO (§4/§5) · `[inferido]` = deducción de quien escribió, sin fuente en el repo.

---

## 0. Lo que el repo ya fija

**Animaciones [repo, CONTEXTO §4]:** (1) caminata de la silueta (orilla → altar); (2) la silueta deja flores en el altar. Son las **únicas** dos por KeyFrames; velas, fuego, papel picado y humo no cuentan.

**Jerarquía [repo, CONTEXTO §4]:** Torso (raíz) → Cabeza; Brazo izq/der → Antebrazo izq/der; Muslo izq/der → Pierna izq/der. Cada cuadro clave guarda la **posición del torso** y la **rotación de cada articulación**. Pivote de cada pieza en su articulación (hombro, codo, cadera, rodilla; D-1).

**Línea de tiempo [repo, CONTEXTO §4]:**

| Evento | $t$ (s) |
|---|---|
| Inicio de la caminata | 0 |
| Llega al altar | 6 |
| Extiende el brazo | 7 |
| Suelta la flor | 8 |
| Retrocede (termina) | 10 |

### Contradicción 6 s contra distancia: **resuelta el 04/11** (decisión del ESTADO; manda sobre CONTEXTO §4)
Los 6 s "orilla → altar" de CONTEXTO §4 implicaban ≈ 4.1 m/s (24.6 m en 6 s), más del doble de la velocidad de la cámara (2.0 m/s) y propia de una carrera. El ESTADO (decisión del 04/11, "Caminata de la silueta") fijó 17 zancadas de 1.4 m a 1.4 m/s: **23.8 m en 18.33 s** (sección 2.1). Los hitos de la tabla de arriba **ya no valen en segundos absolutos**; los de "extiende, suelta y retrocede" pasan a ser tiempos relativos al final de la caminata (decisión del 11/11, sección 3.1).

---

## 1. Interpolación de rotaciones: cuaterniones y slerp (D-2)

### 1.1 Por qué cuaterniones y no ángulos de Euler
Interpolar tres ángulos de Euler por separado (a) no sigue el camino más corto entre dos orientaciones, y (b) sufre *gimbal lock* (se pierde un grado de libertad cuando dos ejes se alinean). Un cuaternión unitario $\mathbf{q}=(w,\mathbf{v})=(\cos\tfrac{\alpha}{2},\ \sin\tfrac{\alpha}{2}\,\hat{\mathbf{u}})$ representa un giro de ángulo $\alpha$ alrededor del eje unitario $\hat{\mathbf{u}}$ sin esas singularidades.

### 1.2 Fórmula (slerp)
Entre dos cuaterniones unitarios $\mathbf{q}_0,\mathbf{q}_1$ y $s\in[0,1]$:

$$
\operatorname{slerp}(\mathbf{q}_0,\mathbf{q}_1,s)=\frac{\sin\big((1-s)\Omega\big)}{\sin\Omega}\,\mathbf{q}_0+\frac{\sin(s\,\Omega)}{\sin\Omega}\,\mathbf{q}_1,\qquad \cos\Omega=\mathbf{q}_0\cdot\mathbf{q}_1
$$

| Término | Significado |
|---|---|
| $\mathbf{q}_0,\mathbf{q}_1$ | cuaterniones de las poses clave inicial y final de **una articulación** |
| $s$ | parámetro normalizado del tramo (ver 1.4) |
| $\Omega$ | ángulo entre $\mathbf{q}_0$ y $\mathbf{q}_1$ sobre la hiperesfera $S^3$ |
| $\mathbf{q}_0\cdot\mathbf{q}_1$ | producto punto de los 4 componentes |

**Deducción (idea):** los cuaterniones unitarios viven en la esfera unitaria de $\mathbb{R}^4$. El camino más corto entre dos puntos de una esfera es el arco de círculo máximo. Recorrerlo a velocidad angular constante da la fórmula anterior (misma que para interpolar sobre una circunferencia: ponderar cada extremo con $\sin$ del ángulo restante / $\sin\Omega$). Resultado: la articulación gira con **velocidad angular constante** en $s$ y por el camino mínimo.

**Dos detalles obligatorios en la implementación:**
1. **Camino corto:** $\mathbf{q}$ y $-\mathbf{q}$ son la misma rotación. Si $\mathbf{q}_0\cdot\mathbf{q}_1<0$, se usa $-\mathbf{q}_1$; si no, la articulación gira "por el camino largo".
2. **$\Omega\to0$:** $\sin\Omega\to0$ y la fórmula da $0/0$. Si $\mathbf{q}_0\cdot\mathbf{q}_1>1-\varepsilon$ se usa interpolación lineal normalizada (nlerp). El código usa una **implementación propia** (`Slerp` en `src/Animacion.cpp:26-47`, no `glm::slerp`) que hace las dos cosas: invierte $\mathbf{q}_1$ si $\mathbf{q}_0\cdot\mathbf{q}_1<0$ y cae a nlerp si $\mathbf{q}_0\cdot\mathbf{q}_1>0.9995$ (ángulo < 3.6°). Comprobado con `tools/d7_pruebas_matematicas.cpp`: de 0° a 90° en X, $s=0.5$ da 45°; el ángulo es lineal en $s$ con desvío máximo $6.8\times10^{-6}$ rad y $|\mathbf{q}|$ se mantiene en $1\pm1.2\times10^{-7}$; con $-\mathbf{q}_1$ también da 45° (camino corto).

### 1.3 Jerarquía: cómo "los hijos siguen al padre" (D-1)
Matriz global de cada pieza = producto de las locales a lo largo de la cadena:

$$
M_{\text{antebrazo}}=M_{\text{torso}}\cdot M_{\text{brazo}}\cdot M_{\text{antebrazo}}^{\text{local}},\qquad
M^{\text{local}}=T(\text{pivote})\,R(\mathbf{q})\,T(-\text{pivote})
$$

Con el pivote de cada `.obj` ya **en la articulación** (D-1), basta $M^{\text{local}}=T(\mathbf{o})\,R(\mathbf{q})$, con $\mathbf{o}$ la posición de la articulación respecto al padre. Orden de aplicación: la rotación de la articulación se aplica **antes** de heredar el movimiento del padre; por eso las matrices se multiplican de la raíz hacia la hoja. Criterio de D-1 [repo]: "al mover el padre, los hijos lo siguen". Las posiciones $\mathbf{o}$ del código (`src/Silueta.cpp:42-55`; adulto de ≈ 1.72 m, medida de referencia **no medida** [repo]):

| Articulación | $\mathbf{o}$ respecto del padre (m) |
|---|---|
| torso (raíz; cadera) | $(x_{\text{ruta}},\ 0.83\text{–}0.89,\ z_{\text{ruta}})$: la altura oscila con el paso; en reposo 0.90 |
| cabeza | $(0,\ 0.60,\ 0)$ |
| hombro izq / der | $(\mp0.22,\ 0.54,\ 0)$ |
| codo (antebrazo) | $(0,\ -0.30,\ 0)$ |
| cadera izq / der (muslo) | $(\mp0.09,\ 0,\ 0)$ |
| rodilla (pierna) | $(0,\ -0.45,\ 0)$ |

### 1.4 Cuadros clave y tiempo
Para un tramo entre cuadros clave $k$ y $k+1$ con tiempos $t_k<t_{k+1}$:

$$
s_{\text{lineal}}=\frac{t-t_k}{t_{k+1}-t_k}\in[0,1],\qquad \mathbf{q}(t)=\operatorname{slerp}\big(\mathbf{q}_k,\mathbf{q}_{k+1},\,f(s_{\text{lineal}})\big)
$$

con $f$ = identidad (velocidad angular constante) o $f$ = smoothstep (sección 2). El criterio de D-2 [repo] es "curvas de aceleración no lineales", así que $f$ **no** es la identidad en los extremos de balanceo: el tramo de cada KeyFrame lleva su propia curva (`Curva` en `src/Animacion.h`): `Smoothstep` en todo el ciclo de paso y en el brazo que deja la flor, `SmoothIn`/`SmoothOut` en la salida y la llegada del torso y `Lineal` solo en el crucero (donde la velocidad ya es constante, sección 2.1) y en el tramo en que el brazo se sostiene extendido.

### 1.5 Tabla de parámetros (valores del código)

| Símbolo | Valor en el código | Unidad | Archivo |
|---|---|---|---|
| tiempos de los cuadros clave del torso $t_k$ | 0, 2, 16.33, 18.33 | s | `src/Silueta.cpp:59-68` |
| tiempos del ciclo de paso (muslo, brazo, giro) | 0, 0.5, 1 (pierna: 0, 0.15, 0.45, 0.75, 1) | s de ciclo | `src/Silueta.cpp:78-100` |
| $\mathbf{q}_k$ del muslo | giro en X de +23° y −23° | grados | `src/Silueta.cpp:78-80` |
| $\mathbf{q}_k$ de la pierna (rodilla) | −3°, −15°, −8°, −60°, −3° en X | grados | `src/Silueta.cpp:82-86` |
| $\mathbf{q}_k$ del brazo / antebrazo | ±20° / −10° y −35° en X | grados | `src/Silueta.cpp:88-94` |
| giro del torso | ±4° alrededor de Y (una vez por ciclo) | grados | `src/Silueta.cpp` (`giro_`) |
| duración de un paso (ciclo) $T_p$ | 1.0 (zancada 1.4 m / crucero 1.4 m/s) | s | `src/Silueta.cpp:17` |
| mezcla con la pose de pie | 0.6 | s | `src/Silueta.cpp:16` |
| posición de cada articulación $\mathbf{o}$ | tabla de 1.3 | m | `src/Silueta.cpp:42-55` |

---

## 2. Smoothstep y desfase de piernas (D-3)

### 2.1 Smoothstep
$$
S(s)=3s^2-2s^3,\qquad s\in[0,1]
$$

**Deducción:** se busca el polinomio cúbico más simple con $S(0)=0$, $S(1)=1$ y **pendiente cero en los extremos** ($S'(0)=S'(1)=0$). Con $S(s)=as^3+bs^2+cs+d$: $d=0$ y $c=0$ (por $S(0)=0$, $S'(0)=0$); $a+b=1$ (por $S(1)=1$) y $3a+2b=0$ (por $S'(1)=0$) ⇒ $a=-2,\ b=3$. Derivada: $S'(s)=6s(1-s)$, máxima en $s=\tfrac12$ con valor $\tfrac32$.

**Significado físico:** el movimiento arranca y frena con **velocidad cero**, sin tirón; en la mitad avanza 1.5 veces más rápido que el promedio. Por eso no es interpolación lineal (criterio "curvas de aceleración no lineales" [repo]).

Variante más suave (no en el repo) $[propuesta]$: $6s^5-15s^4+10s^3$ (también con aceleración cero en los extremos).

**Avance del torso en la caminata (código real, `src/Silueta.cpp:59-68`).** No se usa un solo smoothstep para todo el trayecto (con él la velocidad máxima sería $1.5\,\bar v$). El recorrido de $23.8$ m (17 zancadas de 1.4 m, `Silueta.cpp:12-18`) se parte en **tres tramos con velocidad continua**: arranque `SmoothIn`, crucero lineal y frenado `SmoothOut`. `SmoothIn` y `SmoothOut` son las dos mitades de $S$ reescaladas a $[0,1]$ (`src/Animacion.cpp:19-20`):

$$
\text{SmoothIn}(u)=2\,S(u/2),\qquad \text{SmoothOut}(u)=2\,S\!\big((u+1)/2\big)-1
$$

La velocidad al final de `SmoothIn` es $d\cdot\text{SmoothIn}'(1)/T=d\cdot S'(\tfrac12)/T=1.5\,d/T$; igualarla a la de crucero $V$ da $d=V\,T/1.5$. Con $V=1.4$ m/s y $T=2$ s: $d=1.867$ m por rampa, crucero de $23.8-2(1.867)=20.07$ m en $14.33$ s y **duración total $18.33$ s** (cálculo repetido el 11/11 con Python; coincide con el ESTADO, decisión 04/11). Esto **resuelve** la advertencia del borrador: los 6 s de CONTEXTO §4 implicaban 4 m/s y quedan sustituidos por la decisión del ESTADO del 04/11 (el ESTADO manda sobre CONTEXTO). Valores de $S$: $S(0.25)=0.156$ y $S(0.75)=0.844$ (no lineal).

### 2.2 Desfase de piernas y brazos en oposición
El código **no evalúa un seno por cuadro**: cada miembro tiene una `Pista` de KeyFrames que son los **extremos** del movimiento (el miembro se detiene y regresa), todos los tramos con `Smoothstep`, así la velocidad es 0 en el extremo y máxima a mitad, como un péndulo. Con ciclo de periodo $T_p$ (1 s a velocidad de crucero, `Silueta.cpp:17`) y KeyFrames en $\phi=0,\ \pi,\ 2\pi$ (tiempos 0, 0.5, 1 s), la pierna izquierda va de $+\beta_{\max}$ a $-\beta_{\max}$ y vuelve (`Silueta.cpp:78-80`). El lado derecho usa **la misma pista con medio ciclo de desfase**:

$$
t_{\text{izq}}=\text{fase}\cdot T_p,\qquad t_{\text{der}}=t_{\text{izq}}+\tfrac12T_p,\qquad \text{fase}=\frac{\text{distancia recorrida}}{\text{zancada}}\ \ (\text{`Silueta.cpp:120-122`})
$$

Es el equivalente de $\beta_{\text{der}}(t)=\beta_{\text{izq}}(t+T_p/2)=-\beta_{\text{izq}}(t)$ del modelo senoidal: cuando una pierna va adelante la otra va atrás. Los **brazos en oposición** salen del signo contrario de la pista (brazo izquierdo en $-20^\circ$ cuando el muslo izquierdo está en $+23^\circ$, `Silueta.cpp:88`). Dos decisiones propias: (1) la **fase depende de la distancia**, no del tiempo, así en el arranque y el frenado los pasos se acortan en tiempo y el pie no patina de más; (2) al salir y al llegar se mezcla con la pose de pie con `Slerp(identidad, pose, w)` y $w=\min\big(S(t/0.6),\,S((T_{tot}-t)/0.6)\big)$ (`Silueta.cpp:125-126`). Relación entre ángulo y zancada: $\text{zancada}=4\,L\sin\theta=4(0.9)\sin23^\circ=1.41$ m, que cuadra con los 1.4 m del código (cálculo con Python el 11/11; $L=0.9$ m = muslo 0.45 + pierna 0.45).

**Límite conocido (ESTADO, 04/11, medido en Linux):** el pie de apoyo se desliza ≈ 0.41 m/s de media, porque smoothstep frena el muslo en los extremos; es una caminata estilizada, no exacta.

| Símbolo | Valor en el código | Unidad | Archivo:línea |
|---|---|---|---|
| $T_p$ | 1.0 (= zancada 1.4 m / crucero 1.4 m/s) | s | `src/Silueta.cpp:17` |
| $\beta_{\max}$ (muslo) | ±23 | grados | `src/Silueta.cpp:78-80` |
| rodilla | −3 a −60 (solo hacia atrás; 5 KeyFrames) | grados | `src/Silueta.cpp:82-86` |
| $\gamma_{\max}$ (brazo) | ±20, antebrazo −10 a −35 | grados | `src/Silueta.cpp:88-94` |
| desfase entre lados | $\pi$ (medio ciclo, 0.5 s) | rad | `src/Silueta.cpp:122` |
| $\mathbf{p}_A,\ \mathbf{p}_B$ del torso | $z=24 \to 0.2$; al final paso al costado a $x=+1.0$ | m | `src/Silueta.cpp:11,19,59-68` |
| $t_A,\ t_B$ | 0 y 18.33 (rampas de 2 s) | s | `src/Silueta.cpp:15,61` |

**Estado:** los valores son los escritos en el código. La caminata se compiló y vio en la PC de D2 el 04/11 con cubos (ESTADO, D-2); con el modelo real de D1 sigue **sin compilar**.

---

## 3. Silueta dejando flores: Bézier cuadrática y caída (D-4)

### 3.1 Brazo
El brazo derecho se mueve por KeyFrames con slerp (`src/Silueta.cpp`, pistas `brazoFlor_` y `antebrazoFlor_`), con $\tau$ = segundos desde que termina la caminata ($t_{\text{caminata}}=18.33$ s, sección 2). El primer KeyFrame es la identidad porque la mezcla $w$ de la sección 2.2 deja el brazo en reposo al llegar:

| $\tau$ (s) | Hombro | Antebrazo | Tramo que sale de ahí |
|---|---|---|---|
| 0 | reposo (identidad) | reposo | smoothstep |
| 1 (`T_EXTIENDE`) | +75° en X (hacia delante, −z) | +10° | lineal (se sostiene) |
| 3 (`T_REGRESA`) | +75° | +10° | smoothstep |
| 4 (`T_FIN`) | reposo | reposo | — |

La flor sale de la mano en $\tau=2$ s (`T_SUELTA`), con el brazo ya extendido. Con el torso en $(1.0,\ 0.90,\ 0.2)$ y el hombro en $(+0.22,\ +0.54,\ 0)$ respecto de él, la mano (punta del antebrazo, 0.30 m bajo el codo) queda en $P_0\approx(1.22,\ 1.336,\ -0.389)$ m: se calculó con una réplica en Python de la cadena de matrices el 11/11; el código la calcula en ejecución (`Silueta::ManoAlSoltar`). **"Retrocede" del CONTEXTO se interpreta como el regreso del brazo a reposo** (la silueta no da pasos hacia atrás; simplificación registrada en el ESTADO, 11/11).

### 3.2 Trayectoria de la flor
La propuesta dice: la flor sigue una **Bézier cuadrática mano → nivel del altar, con caída final** [repo, CONTEXTO §4]:

$$
B(u)=(1-u)^2P_0+2(1-u)u\,P_1+u^2P_2,\qquad u\in[0,1]
$$

| Punto | Significado | Valor |
|---|---|---|
| $P_0$ | mano al soltar ($\tau=2$ s) | $\approx(1.22,\ 1.336,\ -0.389)$ m, calculada de la pose del brazo |
| $P_1$ | punto de control | $\big(\tfrac{x_0+x_2}{2},\ y_0,\ \tfrac{z_0+z_2}{2}\big)$: a la altura de la mano y a medio camino horizontal |
| $P_2$ | destino sobre la repisa del nivel inferior de `altar3` | $(1.05,\ 0.435,\ z_0)$ m, **provisional** (`src/main.cpp`; B-13 lo ajusta): `altar3` en $(1.35,0,0)$ girado −90° tiene la repisa en $x\in[0.90,1.15]$ con tope a $y=0.40$ [repo, ESTADO §5]; 0.035 m es la mitad de la flor |

Deducción y propiedades de $B(u)$: ver `justificacion_simples.md` §2.1 (De Casteljau; $B'(0)=2(P_1-P_0)$, $B'(1)=2(P_2-P_1)$).

### 3.3 "Caída libre" final [criterio de D-4]
Con ese $P_1$ y $u=\tau/T_b$, las componentes de $B$ son:

$$
x(u)=x_0+(x_2-x_0)\,u,\qquad z(u)=z_0+(z_2-z_0)\,u,\qquad y(u)=(1-u)^2y_0+2(1-u)u\,y_0+u^2y_2=y_0-u^2\,(y_0-y_2)
$$

(en $x$: $(1-u)^2x_0+2(1-u)u\,\tfrac{x_0+x_2}{2}+u^2x_2=(1-u)x_0+u\,x_2$, porque $x_1$ es el punto medio; igual en $z$). Es decir, **velocidad horizontal constante y $y$ parabólica**. Un tiro horizontal en caída libre cumple $y(\tau)=y_0-\tfrac12g\tau^2$; ambas coinciden si $u^2(y_0-y_2)=\tfrac12g\tau^2$ con $u=\tau/T_b$, es decir

$$
T_b=\sqrt{\frac{2\,h}{g}},\qquad h=y_0-y_2,\qquad g=9.81\ \text{m/s}^2
$$

Así la Bézier **es** la caída libre: no hace falta un segundo tramo balístico. Con $h=1.336-0.435=0.901$ m, $T_b=0.429$ s. Comprobación numérica (programa aparte con `g++` y réplica en Python, 11/11): el error máximo entre la Bézier y $y_0-\tfrac12g\tau^2$ es $2.4\times10^{-7}$ m en `float` ($4\times10^{-16}$ en Python). `dejarflor::Posicion` (`src/DejarFlor.h`) es función cerrada de $\tau$: antes de soltar la flor está en $P_0$ y después de $T_b$ se queda en $P_2$.

### 3.4 Tabla de parámetros

| Símbolo | Valor en el código | Unidad | Archivo:línea |
|---|---|---|---|
| $P_0$ | mano al soltar (≈ (1.22, 1.336, −0.389)) | m | `src/Silueta.cpp` (`ManoAlSoltar`) |
| $P_1$ | punto medio horizontal a la altura de $P_0$ | m | `src/DejarFlor.h` (`Posicion`) |
| $P_2$ | (1.05, 0.435, $z_0$) | m | `src/main.cpp` (`destinoFlor`) |
| duración del vuelo $T_b$ | $\sqrt{2h/g}\approx0.429$ | s | `src/DejarFlor.h` (`TiempoCaida`) |
| $g$ | 9.81 | m/s² | `src/DejarFlor.h` (`G`) |
| hombro / antebrazo al extender | +75° / +10° (en X) | grados | `src/Silueta.cpp` (`brazoFlor_`, `antebrazoFlor_`) |
| `T_EXTIENDE`, `T_SUELTA`, `T_REGRESA`, `T_FIN` | 1, 2, 3, 4 | s tras la caminata | `src/Silueta.h` |

**Estado:** escrito, **sin compilar** y sin verse en pantalla; la parte matemática (`DejarFlor.h`) sí se compiló y probó aparte con `g++`.

---

## 4. Integración con `deltaTime` (D-5)

Las dos animaciones se evalúan con un **reloj propio** $\tau_{\text{anim}}$ que avanza con $\Delta t$ (`src/main.cpp:171` [repo]):

$$
\tau_{\text{anim}}\leftarrow\tau_{\text{anim}}+\Delta t,\qquad \text{pose}=\text{Evaluar}(\tau_{\text{anim}})
$$

Se evalúa $\text{pose}(\tau)$ como función del tiempo (no se acumulan rotaciones), así que el resultado no depende de los FPS: 30 FPS o 144 FPS dan la misma pose al mismo $\tau$. `[PENDIENTE]`: si la animación se dispara (tecla/zona) o corre sola, y qué pasa a los 10 s (¿se reinicia, se queda?), que el repo **no define**.

---

## 5. Por qué no es una animación del curso (D-7)

**Lo que el repo permite afirmar** (código propio de `src/Animacion.h/.cpp`, `src/Silueta.cpp`, `src/DejarFlor.h`):

| Rasgo | En esta animación | Qué buscar en la práctica del curso `[inferido]` |
|---|---|---|
| Rotación | cuaternión por articulación con **`Slerp` propio** (camino corto, caso nlerp) | si la práctica interpola ángulos de Euler o con incrementos lineales |
| Estructura | **10 piezas jerárquicas** (torso → cabeza; brazo → antebrazo; muslo → pierna) cuyas matrices se heredan raíz → hoja | si la práctica mueve un solo modelo o piezas sueltas |
| Aceleración | `Smoothstep` ($3s^2-2s^3$), `SmoothIn` y `SmoothOut` deducidas aquí; velocidad continua entre rampa y crucero (comprobada: 1.40 m/s a los dos lados) | si la práctica avanza con incremento constante por cuadro |
| Tiempo | pose = **función del tiempo** (no acumula); misma pose a 30 o 144 FPS | si la práctica suma incrementos por cuadro (`FrameIndex`, pasos) |
| Fase del paso | ligada a la **distancia recorrida**, no al tiempo (los pies no patinan en arranque y frenado) | — |
| Flor | Bézier cuadrática cuyo punto de control hace que la curva **sea** un tiro horizontal en caída libre ($T_b=\sqrt{2h/g}$); error máximo $2.4\times10^{-7}$ m | si la práctica usa una trayectoria poligonal o lineal |

**Lo que NO se pudo hacer:** comparar contra las prácticas del curso, porque **no están en el repositorio** (solo el código base de la Práctica 6/8). La columna de la derecha es una lista de preguntas basada en cómo suelen plantearse estas prácticas, **no un hecho comprobado**. Antes de entregar, D2 (o D1) debe abrir sus prácticas de animación por KeyFrames y marcar cada fila (el problema #13 del ESTADO ya mostró que una animación puede resultar copiada sin que se note). Si alguna fila coincide, se cambia el rasgo o se anota como similitud.

---

## 6. Qué falta para dar D-7 por terminado

| Hueco | Quién lo cierra |
|---|---|
| Comparar la tabla de §5 con las prácticas del curso | D2 / D1 (no están en el repo) |
| Ver la caminata y la flor en pantalla con el modelo real de D1 | D2, compilando en VS (F5) |
| Aceptar los valores propuestos por Claude (tiempos de dejar la flor, destino de la flor) | D2, en el ESTADO §4 |
| Pasar el manual a su versión final (ES + EN, E-5) | E-5 |

**Criterio de terminado de D-7 [repo]:** slerp, Bézier y smoothstep con fórmulas y parámetros **reales** en `docs/manual_tecnico/`. Con este documento, **fórmulas y parámetros están completos y probados con programa**; queda el hueco de la comparación con el curso, que depende de las prácticas.
