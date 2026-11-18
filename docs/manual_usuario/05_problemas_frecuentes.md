# 5. Problemas frecuentes

| Síntoma | Causa probable | Qué hacer |
|---|---|---|
| El programa no abre y pide una DLL (`VCRUNTIME140.dll`, `MSVCP140.dll`) | Falta el *Visual C++ Redistributable* de 64 bits | Instálalo (gratuito, de Microsoft) y vuelve a abrir |
| Ventana negra o el programa se cierra al abrir | Gráficos sin soporte de OpenGL 3.3, o controladores viejos | Actualiza los controladores de video |
| Los controles no responden | La ventana no tiene el foco | Haz clic dentro de la ventana |
| El programa no encuentra los modelos o shaders | El `.exe` no está junto a las carpetas `Models`, `Shaders` y `Audio` | Ejecuta el programa desde la carpeta completa; no lo muevas solo |
| No puedo pasar por un muro de la barda | Hay colisiones: la barda y los muros del cuarto son sólidos | Usa las puertas: la barda tiene dos de 2 m (norte y sur) y el cuarto, una al frente |
| No suena nada | Sin salida de audio o faltan los archivos de `Audio/` | Revisa la consola, que avisa qué pista falta; prueba la tecla **M** |
| El cursor no aparece | Es intencional (primera persona) | Pulsa **Esc** para abrir el menú de pausa, donde el cursor sí aparece |
| La silueta ya terminó y quiero verla de nuevo | Su animación corre una sola vez | Pulsa **R** |
| No me sale una ficha | Estás a más de 2 m, o el recorrido guiado está activo | Acércate al objeto; cancela el recorrido guiado con **T** |
| Va lento | Gráficos insuficientes: lo que más pesa es la iluminación por píxel | Cierra otros programas y conecta la laptop a la corriente. En una laptop con gráficos integrados Intel de 11.ª generación el equipo midió un mínimo de 45 cuadros por segundo |
