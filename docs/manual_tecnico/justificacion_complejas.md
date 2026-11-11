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

---|---|---|---|
| $T_p$ | `[PENDIENTE]` | s | |
| $\beta_{\max}$ (cadera), rodilla, $\gamma_{\max}$ (hombro) | `[PENDIENTE]` | grados | |
| desfase entre piernas | $\pi$ (medio ciclo) [repo] | rad | |
| $\mathbf{p}_A,\ \mathbf{p}_B,\ t_A,\ t_B$ | `[PENDIENTE]` | m, s | |

---

## 3. Silueta dejando flores: Bézier cuadrática y caída (D-4)

### 3.1 Brazo
El brazo se extiende entre cuadros clave (hombro y codo con slerp, sección 1), con los hitos 6 s → 7 s (extiende) y 8 s (suelta) [repo]. Poses: `[PENDIENTE]`.

### 3.2 Trayectoria de la flor
La propuesta dice: la flor sigue una **Bézier cuadrática mano → nivel del altar, con caída final** [repo, CONTEXTO §4]:

$$
B(u)=(1-u)^2P_0+2(1-u)u\,P_1+u^2P_2,\qquad u\in[0,1]
$$

| Punto | Significado | Valor |
|---|---|---|
| $P_0$ | posición de la mano en el instante de soltar ($t=8$ s) | `[PENDIENTE]` (sale de la pose del brazo) |
| $P_1$ | punto de control: define la altura del arco | `[PENDIENTE]` |
| $P_2$ | posición sobre el nivel del altar donde cae | `[PENDIENTE]` (altura de los escalones: D1 puso flores en y = 1.5 y 2.32, `src/main.cpp:293-296` [repo]; no es la altura final del altar de B-1) |

Deducción y propiedades de $B(u)$: ver `justificacion_simples.md` §2.1 (De Casteljau; $B'(0)=2(P_1-P_0)$, $B'(1)=2(P_2-P_1)$).

### 3.3 "Caída libre" final [repo, criterio de D-4]
Una Bézier cuadrática con $P_1$ **sobre** el punto medio da una parábola, pero su parametrización por $u$ no es la de un tiro real. El criterio de D-4 pide "caída libre de la flor". Dos formas de conseguirla `[propuesta]`:

- **Tramo 1 (Bézier):** mano → punto alto; **Tramo 2 (balístico):** $y(\tau)=y_s-\tfrac12 g\,\tau^2$, con $g=9.81\ \text{m/s}^2$ y $\tau$ = tiempo desde que acaba el tramo 1, hasta tocar el escalón en $y=y_{\text{altar}}$. Tiempo de caída desde una altura $h$: $\tau_c=\sqrt{2h/g}$ (para $h=0.3$ m: $\approx0.25$ s; ejemplo ilustrativo, **no** es un valor del proyecto).
- Tomar $u(t)=(t-t_8)/T_b$ con $u$ **no lineal** (por ejemplo $u=\sigma^2$ en la bajada) para que la flor acelere al caer.

`[PENDIENTE]`: cuál usa el código y la duración de la caída. El hito "retrocede 10 s" [repo] fija que quedan 2 s tras soltar (8 → 10 s).

### 3.4 Tabla de parámetros (a llenar con el código)

| Símbolo | Valor en el código | Unidad | Archivo:línea |
|---|---|---|---|
| $P_0,P_1,P_2$ | `[PENDIENTE]` | m | |
| duración del vuelo $T_b$ | `[PENDIENTE]` | s | |
| $g$ (si hay tramo balístico) | `[PENDIENTE]` (9.81 m/s² si es real) | m/s² | |
| poses del brazo (hombro, codo) | `[PENDIENTE]` | cuaternión | |

---

## 4. Integración con `deltaTime` (D-5)

Las dos animaciones se evalúan con un **reloj propio** $\tau_{\text{anim}}$ que avanza con $\Delta t$ (`src/main.cpp:171` [repo]):

$$
\tau_{\text{anim}}\leftarrow\tau_{\text{anim}}+\Delta t,\qquad \text{pose}=\text{Evaluar}(\tau_{\text{anim}})
$$

Se evalúa $\text{pose}(\tau)$ como función del tiempo (no se acumulan rotaciones), así que el resultado no depende de los FPS: 30 FPS o 144 FPS dan la misma pose al mismo $\tau$. `[PENDIENTE]`: si la animación se dispara (tecla/zona) o corre sola, y qué pasa a los 10 s (¿se reinicia, se queda?), que el repo **no define**.

---

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
