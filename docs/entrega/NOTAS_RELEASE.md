# Notas del Release (borrador para pegar en GitHub)

> **Estado:** borrador actualizado el 18/11/2026 (X-71; el del 12/11 decía cosas que ya no son ciertas). Se completa con los datos reales
> (fecha, tamaño, SHA-256) **después** de correr `tools/empaquetar_release.ps1` y probar el .zip en otra PC. Lo marcado ⚠️ **no está verificado**.

**Etiqueta (tag) propuesta:** `v1.0.0` sobre `main` (⚠️ hoy `main` sigue en la estructura inicial: hace falta el PR `develop` → `main`) · **Título:** `Nierika Studio v1.0.0 — Entrega final` ·
**Archivo adjunto:** `NierikaStudio_<fecha>.zip` (+ `NierikaStudio_<fecha>.zip.sha256`)

---

## Nierika Studio — Recorrido virtual: Día de Muertos en Mazunte

Recorrido 3D interactivo en primera persona, hecho en C++17 con OpenGL 3.3 Core. Proyecto final de Computación Gráfica e
Interacción Humano-Computadora (FI UNAM, 2027-1), Equipo 15 (cuentas 319266894 y 321332086).

### Qué incluye
- Escena continua: playa y orilla con ofrendas familiares, colinas con casas, faroles y fogatas al fondo, panteón con barda y cuarto con ofrenda de tres niveles.
- Menú de inicio, controles, créditos y pausa; cámara en primera persona con colisiones (W A S D / flechas + ratón).
- Recorrido guiado de 12 estaciones con voz y subtítulos (tecla **T**).
- Iluminación Blinn-Phong; las velas del altar se encienden o apagan con **V**.
- 10 animaciones: 5 por KeyFrames (caminata de la silueta, dejar la flor, baile familiar, juego con el perro y sahumerio con copal) y 5 simples (olas, velas flotando con su llama, flores flotando, banda al compás y anfitriona). Detalle y matemática en `docs/manual_tecnico/09_i_animaciones.md`; solo las 5 primeras estaban en la propuesta aprobada.
- 13 fichas culturales (Dear ImGui) a menos de 2 m, con fuente en APA 7.
- Audio: oleaje y música de banda que cambian con la posición (grabaciones de terceros, ver Créditos) y la voz del recorrido (síntesis con ElevenLabs).

### Cómo usarlo
1. Descarga `NierikaStudio_<fecha>.zip` y descomprímelo completo.
2. Abre `NierikaStudio.exe` (Windows 64 bits, OpenGL 3.3).
3. Controles en `LEEME.txt` dentro del paquete y en `docs/manual_usuario/`.

### Verifica la descarga
SHA-256 del .zip: `<pegar el valor del archivo .sha256>`

### Límites conocidos (dichos con claridad)
- Rendimiento: D2 midió **mínimo 45 FPS y unos 60 de promedio** a 1280×720 en un Intel Core i5 de 11.ª generación con gráficos integrados (después del PR #148). Es **una sola máquina**.
- ⚠️ La prueba del .zip en una PC **sin Visual Studio** (criterio de E-3) debe quedar anotada en el ESTADO antes de publicar.
- El oleaje y la música son de terceros, con uso educativo no comercial y sin licencia libre confirmada: no se redistribuyen comercialmente. Solo el **código** tiene licencia MIT.
- Fuera de alcance por acuerdo con el profesor: pueblo, Punta Cometa, tapete de arena, VR/web/móvil, multijugador, IA y humo de copal con partículas.

### Documentación
Manual técnico y manual de usuario (PDF en `docs/entrega/`, fuente en `docs/manual_tecnico/` y `docs/manual_usuario/`), videos (`docs/videos/README.md`; los MP4 se entregan aparte),
estado del proyecto (`docs/ESTADO_PROYECTO.md`).

### Créditos
Modelos propios del estudio (Blender y Maya; parte generada con scripts de `bpy` hechos con ayuda de IA, ver el manual técnico); texturas CC0 de Poly Haven;
librerías: GLFW, GLEW, GLM, Assimp, SOIL2/stb_image, Dear ImGui y miniaudio. Lista completa: `docs/REFERENCIAS.md`.
