# Justificación matemática de las 2 animaciones por KeyFrames (D-7)

**Nierika Studio · Equipo 15 · Manual técnico — BORRADOR (04/11/2026)**
Tarea: D-7 (act. 53), con D-2 (slerp), D-3 (smoothstep y desfase) y D-4 (Bézier de la flor) · Cuentas: 319266894, 321332086

> **Estado del documento: borrador.** Las fórmulas y su deducción son generales y están completas.
> **No existe código** de la silueta, de los KeyFrames ni de la flor (`src/Animacion.*` no existe; D-1…D-5 están ⏳ en el ESTADO).
> Los valores del código quedan como `[PENDIENTE]`; no se inventa ninguno.
>
> **Marcas:** `[repo]` = escrito en el repo · `[PENDIENTE]` = falta el código · `[propuesta]` = idea redactora, no es regla hasta registrarla en el ESTADO (§4/§5).

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

### Contradicción a resolver antes de implementar (no se elige en silencio)
La caminata dura 6 s "orilla → altar" **[repo]**, pero los documentos **no fijan el punto de partida ni la altura de la silueta**. Con los únicos números del repo: si parte de la orilla ($z=+24$, ESTADO §5, playa y mar) y el altar queda en $z\approx-0.6$ (capilla provisional, ESTADO §5), la distancia es $\approx 24.6$ m, es decir **$\approx 4.1$ m/s promedio**, más del doble de la velocidad de la cámara (2.0 m/s [repo, valor inicial]) y propia de una carrera, no de una caminata (una caminata humana típica ronda 1.4 m/s: dato general, no del repo). Opciones: (a) acortar el recorrido (partir cerca de la fachada), (b) alargar la fase de caminata, (c) aceptar un ritmo rápido. **Propuesta:** registrar la decisión en el ESTADO §4 antes de D-2. El mismo problema ya estaba en la auditoría del 19/10 (`/mnt/project-files/auditoria/`).

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
2. **$\Omega\to0$:** $\sin\Omega\to0$ y la fórmula da $0/0$. Si $\mathbf{q}_0\cdot\mathbf{q}_1>1-\varepsilon$ se usa interpolación lineal normalizada (nlerp). `glm::slerp` ya incluye ambos casos `[PENDIENTE: confirmar si el código usa glm::slerp o una implementación propia]`.

### 1.3 Jerarquía: cómo "los hijos siguen al padre" (D-1)
Matriz global de cada pieza = producto de las locales a lo largo de la cadena:

$$
M_{\text{antebrazo}}=M_{\text{torso}}\cdot M_{\text{brazo}}\cdot M_{\text{antebrazo}}^{\text{local}},\qquad
M^{\text{local}}=T(\text{pivote})\,R(\mathbf{q})\,T(-\text{pivote})
$$

Con el pivote de cada `.obj` ya **en la articulación** (D-1), basta $M^{\text{local}}=T(\mathbf{o})\,R(\mathbf{q})$, con $\mathbf{o}$ la posición de la articulación respecto al padre. Orden de aplicación: la rotación de la articulación se aplica **antes** de heredar el movimiento del padre; por eso las matrices se multiplican de la raíz hacia la hoja. Criterio de D-1 [repo]: "al mover el padre, los hijos lo siguen". `[PENDIENTE]`: posiciones $\mathbf{o}$ por articulación (dependen del modelo de D-1 y de la altura, **no definida**).

### 1.4 Cuadros clave y tiempo
Para un tramo entre cuadros clave $k$ y $k+1$ con tiempos $t_k<t_{k+1}$:

$$
s_{\text{lineal}}=\frac{t-t_k}{t_{k+1}-t_k}\in[0,1],\qquad \mathbf{q}(t)=\operatorname{slerp}\big(\mathbf{q}_k,\mathbf{q}_{k+1},\,f(s_{\text{lineal}})\big)
$$

con $f$ = identidad (velocidad angular constante) o $f$ = smoothstep (sección 2). El criterio de D-2 [repo] es "curvas de aceleración no lineales", así que $f$ **no** debe ser la identidad `[PENDIENTE: confirmar]`.

### 1.5 Tabla de parámetros (a llenar con el código)

| Símbolo | Valor en el código | Unidad | Archivo:línea |
|---|---|---|---|
| tiempos de los cuadros clave $t_k$ | `[PENDIENTE]` (hitos del repo: 0, 6, 7, 8, 10 s) | s | |
| $\mathbf{q}_k$ por articulación y cuadro | `[PENDIENTE]` | cuaternión | |
| ciclo de paso (duración de un paso) | `[PENDIENTE]` | s | |
| ángulo máximo de cadera / rodilla / hombro / codo | `[PENDIENTE]` | grados | |
| posición de cada articulación $\mathbf{o}$ | `[PENDIENTE]` | m | |

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

Las dos animaciones (caminata y dejar la flor) comparten **un solo reloj**, `tSilueta` (`src/main.cpp`), que avanza con $\Delta t$ en cada cuadro:

$$
t_{\text{sil}}\leftarrow t_{\text{sil}}+\Delta t,\qquad \text{pose}=\text{Actualizar}(t_{\text{sil}})
$$

- **Una función del tiempo, no una acumulación.** `Silueta::Actualizar(t)` calcula toda la pose desde $t$ (no suma rotaciones cuadro a cuadro), así que 30 FPS o 144 FPS dan la misma pose al mismo $t$.
- **Dónde empieza y termina cada una** (valores del código, `Silueta.cpp` y `Silueta.h`): la caminata ocupa $0\le t\le 18.33$ s (`Duracion()`); la de dejar la flor va con $\tau=t_{\text{sil}}-18.33$: el brazo se extiende de 0 a 1 s, la flor sale de la mano a los 2 s, el brazo regresa de 3 a 4 s. Después de $\tau=4$ s todo queda quieto: la silueta en su punto de llegada y la flor sobre la repisa.
- **Se dispara sola.** No hay tecla ni zona que la active: corre desde que empieza el bucle de dibujo. La tecla **R** pone $t_{\text{sil}}=0$ y todo se repite, porque todo es función de $t_{\text{sil}}$ (incluida la posición de la flor).
- **Arranque (D-5, 12/11).** `lastFrame` valía 0 al entrar al bucle, así que el primer $\Delta t$ incluía todo el tiempo de carga de modelos y texturas (segundos). Se fija `lastFrame = glfwGetTime()` justo antes del bucle y `tSilueta = 0`: la caminata empieza en la orilla y no ya avanzada.
- **Fin de la caminata contra el altar (D-5, 12/11).** La caminata termina en $(x,z)=(0.45,\ 0.20)$ (antes $x=1.0$, dentro de la caja del `altar3`, que ocupa $x\in[0.90,1.80]$). Calculado con el código (programa aparte, `Silueta.cpp` + `Animacion.cpp`): en el último segundo las articulaciones y la mano llegan a $x\le 0.67$ m (la mano al soltar queda en $(0.670,\ 1.336,\ -0.389)$); sumando el grosor del brazo ($\approx 0.05$ m) hay $\approx 0.18$ m de holgura. La flor recorre 0.38 m horizontales en $T_b=0.429$ s hasta la repisa en $(1.05,\ 0.435,\ -0.389)$.

## 5. Por qué no es una animación del curso (D-7) `[PENDIENTE]`
Se redacta con el código. Argumento base: jerarquía de 9 piezas con cuaterniones por articulación y slerp, tiempo suavizado con smoothstep, y trayectoria Bézier + caída para la flor; las prácticas del curso usan transformaciones simples (traslación/rotación con teclas o `glfwGetTime()`) sin jerarquía ni cuadros clave. Solo se sostiene cuando exista el módulo y se haya comparado con las prácticas. **Cuidado:** el problema #13 del ESTADO ya mostró que una animación puede resultar copiada sin que se note; revisar contra las prácticas antes de escribir esta sección.

---

## 6. Resumen de huecos que impiden cerrar D-7

| Hueco | Quién lo cierra | Tarea |
|---|---|---|
| Contradicción 6 s vs. distancia (sección 0) | decisión de D2 en el ESTADO §4 | antes de D-2 |
| Altura de la silueta, posiciones de articulaciones | modelo de D-1 | D-1 |
| Poses clave, ángulos y tiempos de cada cuadro | módulo de KeyFrames | D-2, D-3 |
| Puntos Bézier, tramo de caída, altura del altar | hilo de D-4 y B-1 | D-4 |
| Argumento "no es del curso" | al tener código | D-7 |

**Criterio de terminado de D-7 [repo]:** slerp, Bézier y smoothstep con fórmulas y parámetros **reales** en `docs/manual_tecnico/`. Este borrador **no lo cumple** todavía: lo adelanta.
