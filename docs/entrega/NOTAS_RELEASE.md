# Notas del Release (borrador para pegar en GitHub)

> **Estado:** borrador del 12/11/2026. Se completa con los datos reales (fecha, tamaño, SHA-256, FPS) **después** de correr
> `tools/empaquetar_release.ps1` y probar el .zip en otra PC. Lo marcado ⚠️ **no está verificado**.

**Etiqueta (tag) propuesta:** `v1.0.0` sobre `main` · **Título:** `Nierika Studio v1.0.0 — Entrega final` ·
**Archivo adjunto:** `NierikaStudio_<fecha>.zip` (+ `NierikaStudio_<fecha>.zip.sha256`)

---

## Nierika Studio — Recorrido virtual: Día de Muertos en Mazunte

Recorrido 3D interactivo en primera persona, hecho en C++17 con OpenGL 3.3 Core. Proyecto final de Computación Gráfica e
Interacción Humano-Computadora (FI UNAM, 2027-1), Equipo 15 (cuentas 319266894 y 321332086).

### Qué incluye
- Escena continua: playa y orilla, panteón con barda y cuarto con ofrenda de tres niveles.
- Cámara en primera persona con colisiones (W A S D / flechas + ratón).
- Iluminación Blinn-Phong: luz ambiental cálida y 3 luces clave, más 4 luces de colores (barra espaciadora).
- 5 animaciones: olas, velas flotando y flores flotando (simples); caminata de la silueta y dejar la flor (KeyFrames).
- 5 fichas culturales (Dear ImGui) a menos de 2 m, con fuente en APA 7.
- Audio ambiental en bucle (oleaje y música lejana, generados por el equipo).

### Cómo usarlo
1. Descarga `NierikaStudio_<fecha>.zip` y descomprímelo completo.
2. Abre `NierikaStudio.exe` (Windows 64 bits, OpenGL 3.3).
3. Controles en `LEEME.txt` dentro del paquete y en `docs/manual_usuario/`.

### Verifica la descarga
SHA-256 del .zip: `<pegar el valor del archivo .sha256>`

### Límites conocidos (dichos con claridad)
- ⚠️ El rendimiento **≥ 30 FPS a 1280×720 en gráficos integrados** es la meta del cliente; **no se ha medido** en ese equipo.
- ⚠️ La prueba del .zip en una PC **sin Visual Studio** (criterio de E-3) debe quedar anotada en el ESTADO antes de publicar.
- ⚠️ Cómo suena el audio no se comprobó en un entorno con tarjeta de sonido hasta la prueba de D2.
- Fuera de alcance por acuerdo con el profesor: pueblo, Punta Cometa, tapete de arena, VR/web/móvil, multijugador, IA y humo de copal con partículas.

### Documentación
Manual técnico (`docs/manual_tecnico/`), manual de usuario (`docs/manual_usuario/`), videos (enlaces en `docs/videos/README.md`),
estado del proyecto (`docs/ESTADO_PROYECTO.md`).

### Créditos
Modelos propios del estudio (Blender); texturas CC0 de Poly Haven y ambientCG según `docs/manual_tecnico/11_referencias.md`;
librerías: GLFW, GLEW, GLM, Assimp, SOIL2/stb_image, Dear ImGui y miniaudio.
