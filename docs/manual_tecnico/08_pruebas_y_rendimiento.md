# 8. Pruebas y rendimiento

## 8.1 Cómo se prueba hoy ✅ **[repo]**

- Cada tarea se **compila en Release x64** en la PC de D2 y se corre con F5; se documentan capturas y la consola sin errores.
- Cada PR pasa `/verificar <código>` (subagente `verificador`) con veredicto APROBAR antes de fusionarse (desde el 20/10); **excepción:** los PR #59 a #68 se fusionaron el 12/11 sin ese veredicto (ESTADO §4).
- No hay CI ni pruebas automáticas en el repositorio. Claude no puede compilar en Windows ni ver la pantalla, así que la compilación y la inspección visual las hace una persona.

## 8.2 Rendimiento

| Criterio | Valor | Estado |
|---|---|---|
| ≥ 30 FPS a 1280×720 en gráficos integrados | meta del cliente | ❓ **sin medir**: A-8 lo registra como «los FPS están bien» (palabra de D2, sin valor). El contador existe desde E-1a (sin compilar) |
| PC de medición | ❓ **POR DEFINIR** (¿integrada o dedicada?) | — |
| Valor medido | ⏳ | El contador ya existe (título de la ventana, promedio cada 0.5 s; E-1a). **Falta que D2 anote los valores** |

**Tabla que D2 llena (hoy vacía):**

| Lugar | FPS (título) | GPU | Conectada a la corriente | Fecha |
|---|---|---|---|---|
| (1) Orilla, z ≈ 23, mirando al mar | ⏳ | | | |
| (2) Panteón, mirando al cuarto | ⏳ | | | |
| (3) Cuarto, frente al altar | ⏳ | | | |

📷 **CAPTURA pendiente** de cada lugar con el contador visible.

### 8.2.1 Integración de las 3 animaciones simples (C-5) — lo que se midió el 12/11

> Las mediciones son **anteriores a la fusión** de B-13 y C-6 (PR #59 y #63, ya en `develop`): la fila «con B-13 + C-6» describe hoy el código vigente; la primera, el estado previo.

Olas (C-1, vertex shader), velas flotando (C-3), llamas (C-2, fragment shader) y flores flotando (C-4) ya corren **a la vez** en el ciclo de dibujo de `src/main.cpp`; no hay animación que dependa de otra ni se pisen los `uniform` (`oleaje` y `llama` se encienden y apagan alrededor de cada dibujo).

**Triángulos por cuadro** (contados de los `.obj` × las veces que `main.cpp` los dibuja; es un conteo por lectura del código, no una medición de la GPU):

| Estado del código | Triángulos por cuadro | De ellos: flor de Maya (18 874 tris cada una) |
|---|---|---|
| `develop` del 12/11 | ≈ 226 000 | 9 flores = 169 866 (75 %) |
| con B-13 + C-6 (retira capilla y pan de Maya; flores flotantes con la flor propia de 6 tris) | ≈ 78 000 | 2 flores = 37 748 |

**Tiempo por cuadro medido en un renderizador por software** (Mesa llvmpipe, 4 hilos de CPU, ventana 1280×720, 300 cuadros): **no es una GPU integrada**, solo sirve para comparar vistas y cambios entre sí.

| Vista (cámara a 1.6 m) | Con B-13 + C-6 |
|---|---|
| Mirando al cielo (casi solo geometría y vértices) | 8.2 ms |
| Mirando al mar desde z = 6 | 25.0 ms |
| Mirando la fachada desde z = 6 | 43.3 ms |
| Dentro del cuarto, frente al altar | 67.7 ms (antes de B-13 + C-6, 77.1 ms) |

**Lectura (inferida):** el costo domina por el *fragment shader* (cada píxel evalúa 1 luz direccional, 4 puntuales, 1 reflector y 6 reflectores de escalón), no por los triángulos: quitar ~148 000 triángulos solo bajó un 12 % el tiempo en la vista del altar. En una GPU integrada lo que más pesa será entonces la superficie de pantalla cubierta por paredes cerca de la cámara.

**Lo que NO se sabe:** los FPS en la GPU del cliente. El criterio de C-5 (≥ 30 FPS con las tres animaciones) **sigue sin comprobarse** hasta que D2 mida con el contador del título de la ventana.

**Protocolo para D2 (3 minutos):** compilar Release x64, ejecutar sin depurador (Ctrl+F5) y anotar el número del título en 3 lugares, 10 s en cada uno: (1) en la orilla (z ≈ 23) mirando al mar, con velas y flores a la vista; (2) dentro del panteón mirando hacia el cuarto; (3) dentro del cuarto de frente al altar. Anotar también modelo de GPU y si la laptop iba conectada. Si alguno baja de 30, el primer candidato a recortar son las luces de escalón del *fragment shader*.

## 8.3 Memoria de GPU

`cleanup()` explícito existe en `Shader`, `Mesh` y `Model` y se llama al final de `main` antes de `glfwTerminate()` (E-1a, 04/11; decisión del 19/10: no destructores, porque las clases se copian). **Nadie ha comprobado con una herramienta (RenderDoc o similar) que no queden fugas**: el problema #7 sigue «por verificar». ⏳ E-1.

## 8.4 Problemas del ESTADO §6 al 12/11 ✅ **[repo]**

| # | Problema | Estado |
|---|---|---|
| 3 | Texturas de 2048 px de la capilla provisional | Cerrado en B-9 (04/11); la capilla ya no se carga (B-13) |
| 5 | Falta de iluminación | Cerrado en A-8 (04/11) |
| 7 | Memoria de GPU | Implementado (E-1a); **falta compilar y verificar con herramienta** |
| 8 | `Model.h` sin mapas de relieve | Resuelto en B-12 (12/11); falta verlo compilado |
| 9 | `fachada.obj` incluía el techo | Separado en A-11 (03/11); falta confirmar |
| 12 | Arco no semicircular | Corregido en A-10 (03/11); falta confirmar |
| 13 | Animación de las 4 luces posiblemente de la Práctica 8 | Cerrado **por palabra de D2** («es propio»); sin verificar |
| 14 | 8 PDFs ajenos en `Models/playa/` | Borrados el 11/11; **siguen en el historial de un repositorio público** (incluye un cuestionario); borrarlos del historial exigiría reescribirlo: decisión de D2 |

Cerrados antes: #1, #2 (A-1), #4, #6, #11 (A-2). **Hallazgos nuevos de E-5 (12/11):** `laminado1.jpg` mide 512×512 (la convención pide 1K) y E-2 corrigió que la barda estaba espejada en z. ✅ **[repo]**

## 8.5 Registro de errores de la prueba final

⏳ E-1: lista de errores priorizada (criterio del ESTADO). Se llena después de que D2 compile y pruebe; no se escribe antes.
