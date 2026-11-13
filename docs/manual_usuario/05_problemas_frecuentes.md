# 5. Problemas frecuentes

| Síntoma | Causa probable | Qué hacer | Estado |
|---|---|---|---|
| El programa no abre y pide DLL | Falta alguna biblioteca junto al `.exe` | Usar el paquete completo de `dist/` | **Pendiente** de comprobar en otra PC (E-3) |
| Ventana negra o cierra al abrir | Gráficos sin soporte de OpenGL 3.3 | Actualizar los controladores de video | Plausible, **sin probar** |
| Los controles no responden | La ventana no tiene el foco | Hacer clic en la ventana | Plausible, **sin probar** |
| No puedo pasar por un muro de la barda | Hay colisiones (A-9): la barda y los muros del cuarto son sólidos | Usar las puertas: la barda tiene dos de 2 m (norte y sur); el cuarto, una al frente | **Real** |
| No suena nada | Equipo sin salida de audio o sin los archivos de `Audio/` | Revisar la consola: avisa qué pista falta | **Real** (comportamiento de `Audio.cpp`) |
| El cursor no aparece | Es intencional (cámara en primera persona) | Salir con Esc | **Real** |
| La silueta ya terminó y quiero verla de nuevo | Su animación corre una sola vez | Tecla **R** | **Real** |
| Va lento | Equipo sin gráficos suficientes; lo que más pesa es la iluminación por píxel | Se mide en E-1; no hay cifras en gráficos integrados todavía | **Pendiente** |

> Las filas "sin probar" se confirman o se eliminan al hacer las pruebas de E-1 y E-3. No se publican como hechos antes.
