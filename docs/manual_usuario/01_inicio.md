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
| Rendimiento esperado | ≥ 30 FPS en una laptop con gráficos integrados | **Objetivo**, aún sin medir en ese equipo |
| Teclado y ratón | Sí | Real |

## 1.3 Cómo abrirlo
**PENDIENTE (E-3).** Aquí irá: descomprimir el `.zip` de `dist/` y ejecutar el `.exe`. Todavía no existe el
paquete autónomo. Hasta entonces el programa solo se compila desde Visual Studio 2022 (ver `README.md` de la raíz).

> 📷 Captura pendiente: ventana del programa recién abierta.

## 1.4 Qué ves al abrir
La ventana se titula *Nierika Studio - Recorrido virtual: Dia de Muertos en Mazunte*. El cursor desaparece
(ver sección 2) y la cámara empieza en la posición (0, 2, 6) m, es decir, 6 m al sur del centro del cuarto, a 2 m de altura.
