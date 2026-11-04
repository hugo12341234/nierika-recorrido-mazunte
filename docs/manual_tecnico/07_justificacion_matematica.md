# 7. Justificación matemática de las 5 animaciones

> **Estado:** ⏳ ningún código de animación existe todavía (C-1, C-3, C-4, D-2, D-3, D-4). Esta sección fija **la estructura**; las fórmulas generales y los valores reales se escriben en C-8 y D-7, y **los valores deben ser los del código** (regla del proyecto).
> Ninguna animación puede provenir de las prácticas del curso y cada una necesita su justificación. ✅ **[repo, CONTEXTO §3]**

## 7.1 Plantilla de cada animación

Para cada una se escribirá, en este orden: (1) qué se ve; (2) fórmula; (3) significado de cada término; (4) valores usados en el código y por qué; (5) cómo se comprueba; (6) referencia.

## 7.2 Animaciones y técnica prevista ✅ **[repo, CONTEXTO §4]**

| # | Animación | Tipo | Técnica prevista | Tarea | Valores |
|---|---|---|---|---|---|
| 1 | Caminata de la silueta (orilla → altar) | KeyFrames | Rotaciones por articulación con **slerp**; avance con **smoothstep**; piernas desfasadas medio ciclo; brazos en oposición | D-2, D-3 | ❓ duración, número de cuadros y longitud de paso |
| 2 | Silueta dejando flores | KeyFrames | Brazo entre cuadros clave; flor en **Bézier cuadrática** mano → altar, con caída final | D-4 | ❓ |
| 3 | Olas | Simple | *Vertex shader*: altura = suma de senoidales | C-1 | ❓ frecuencias y amplitudes (malla ya lista: 100×100 cuadros de 1 m, y = −0.2) |
| 4 | Velas flotando | Simple | **Bézier** mar adentro + oscilación senoidal; desfase por vela; llama con ruido (Perlin o similar) en el *fragment shader* | C-3, C-2 | ❓ |
| 5 | Flores de cempasúchil | Simple | Movimiento circular de radio pequeño + rotación continua; velocidad variable con pausas | C-4 | ❓ |

**Advertencia ya registrada:** la línea de tiempo de la propuesta (llega al altar a los 6 s) debe contrastarse con la distancia real del recorrido (ver ESTADO y propuesta §5) antes de fijar la duración. ❓ **POR DEFINIR** en D-2.

## 7.3 Esquema de la sección final

- 7.3.1 Olas (suma de senos)
- 7.3.2 Velas (Bézier + seno)
- 7.3.3 Llama (ruido)
- 7.3.4 Flores (círculo + rotación)
- 7.3.5 Caminata (slerp, smoothstep, jerarquía de la silueta)
- 7.3.6 Dejar flores (Bézier cuadrática y caída libre)

Cada subsección se llena con su tarea; **no se escribe antes** de que existan los parámetros.

## 7.4 Referencia técnica principal

de Vries (2020), *Learn OpenGL* (ver [§11](11_referencias.md)). Las referencias de slerp, Bézier y ruido se agregan al escribir cada subsección, con su cita APA 7.
