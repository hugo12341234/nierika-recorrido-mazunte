# 1. Qué es el recorrido y qué necesitas

## 1.1 Qué es
Un recorrido virtual 3D en primera persona del Día de Muertos en Mazunte, Oaxaca: caminas de la playa
al panteón y entras a un cuarto con una ofrenda. *(Alcance: `docs/CONTEXTO_PROYECTO.md`.)*

## 1.2 Qué necesitas
| Requisito | Valor | Estado |
|---|---|---|
| Sistema | Windows, **solo 64 bits (x64)** | Real (CLAUDE.md §3) |
| Gráficos | Compatibles con **OpenGL 3.3** | Real |
| Resolución de la ventana | **1280 × 720** (fija en el código) | Real (`WIDTH, HEIGHT`) |
| Rendimiento esperado | ≥ 30 FPS en una laptop con gráficos integrados | **Objetivo, sin medir** en ese equipo (ver `manual_tecnico/08_pruebas_y_rendimiento.md` §8.2.1) |
| Teclado y ratón | Sí | Real |
| Sonido | Opcional: si no hay tarjeta de sonido o faltan los archivos de `Audio/`, el programa avisa en la consola y sigue sin audio | Real (`Audio.cpp`) |

## 1.3 Cómo abrirlo
**PENDIENTE (E-3, E-7).** Aquí irá: descomprimir el `.zip` publicado en el *Release* de GitHub y ejecutar `NierikaStudio.exe`.
El script que arma el paquete existe (`tools/empaquetar_release.ps1`, E-3) pero el `.zip` todavía no se publicó ni se probó en otra PC.
Hasta entonces el programa se compila desde Visual Studio 2022 (ver `README.md` de la raíz).

## 1.4 Menú de inicio (X-63)
Al abrir el programa aparece primero un **menú de inicio** sobre la escena, que da vueltas despacio de fondo. La silueta, el recorrido y la música **todavía no empiezan**. Botones: **Iniciar recorrido** (arranca todo desde el panteón, como antes), **Controles** (las teclas reales), **Créditos** (equipo, texturas y audio de terceros, bibliotecas; lo que falta de las referencias dice PENDIENTE) y **Salir**. Se elige con clic o con flechas y Enter; **Esc** sale. Dentro del recorrido, **Esc** abre la misma pantalla como pausa.

![Menú de inicio](capturas/01_menu_inicio.jpg)

![Créditos](capturas/01_menu_creditos.jpg)

*Capturas del 17/11/2026 hechas en Linux (Mesa, sin sonido) con las variables de desarrollo `NIERIKA_UI=menu_creditos_fin` y `NIERIKA_MENU_PNG`; falta verlo en la PC de D2 con VS 2022.*

## 1.5 Qué ves al abrir el recorrido
La ventana se titula *Nierika Studio - Recorrido virtual: Dia de Muertos en Mazunte* y, tras unos instantes, agrega los cuadros por segundo (`NNN FPS`).
El cursor desaparece (ver sección 2). La cámara empieza en el **panteón**, en la posición (0, 1.6, 6) m: 6 m al frente del cuarto, a la altura de los ojos de una persona (1.6 m).
Ese punto queda bajo el papel picado, así que **la primera ficha que aparece es la del papel picado** (ver sección 4).

![Ventana recién abierta](capturas/01_ventana_inicio.jpg)

*Captura tomada el 17/11/2026 en la PC de D2 con el código de `develop` hasta el PR #131 (modo de captura del programa, Release x64, 1280×720). Se ve la ficha del papel picado porque la cámara empieza bajo los postes.*
