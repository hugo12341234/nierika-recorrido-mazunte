# Guía de grabación con OBS Studio (E-4)

> **Qué es verificado y qué no:** los valores del programa (ventana 1280×720, controles, posiciones) salen del código y del manual de usuario.
> Los **ajustes de OBS** son los habituales de OBS 30.x/31.x **de memoria, no comprobados en tu equipo**: si un menú tiene otro nombre, usa el equivalente.

## 1. Antes de grabar (una sola vez)
1. Compila **Release | x64** y ejecuta el programa **desde el paquete** (`dist\NierikaStudio\NierikaStudio.exe`, ver `docs/entrega/LISTA_VERIFICACION.md`) o con F5. Se graba Release, no Debug (los FPS cambian mucho).
2. Cierra todo lo que no haga falta (navegador, chat, notificaciones). Activa **No molestar** de Windows.
3. Pon el escritorio en 1920×1080 o 1280×720 y la **escala de Windows al 100 %** (si no, la ventana de 1280×720 sale borrosa o recortada).
4. Ten a la mano `docs/videos/GUION_*.md`.

## 2. Configurar OBS
1. **Archivo → Configuración → Video:** Resolución base y de salida **1280×720**; FPS **30**. (Es la resolución del programa y del criterio del cliente.)
2. **Salida → Modo avanzado → Grabación:** Formato **mp4**, codificador **x264** (o NVENC/QSV si lo hay), control de tasa **CRF 20–23**, preajuste *veryfast*. Si el MP4 se corrompe al cerrar, usa **mkv** y luego *Archivo → Remux de grabaciones* a MP4.
3. **Audio:** *Audio del escritorio* activo (sale el oleaje y la música del programa); *Mic/Aux* solo si narras en vivo.
4. **Fuentes → + → Captura de juego** (modo *Capturar ventana específica* → `NierikaStudio.exe`). Si sale **pantalla negra**, borra la fuente y prueba **Captura de ventana** (con *Capturar cursor* apagado); es el problema típico de OpenGL.
5. Prueba 10 s: ¿se ve la escena completa? ¿se oye el oleaje? ¿la ventana ocupa todo el cuadro? Reproduce el archivo (*Archivo → Mostrar grabaciones*).

## 3. Mientras grabas
- Que se vea el **título de la ventana con los FPS** (`NNN FPS`): sirve de evidencia para E-1. Si se corta, grabar el cuadro completo de la ventana con borde.
- **Ratón:** el cursor está oculto y atrapado; mueve el ratón despacio (la sensibilidad es 0.25) y camina recto con W. Los giros bruscos marean.
- **No pulses** `T G Y H U J` ni `1`–`6` (son de depuración y mueven el reflector). **L** muestra cubos de las luces: úsala solo si el guion lo pide.
- Si algo sale mal, **no cortes**: haz una pausa de 3 s y repite la toma; luego se recorta. Mejor 2–3 tomas por escena.
- Para detener: la tecla de OBS (asígnala en *Configuración → Atajos*, p. ej. F9/F10) o el botón de OBS; **Esc** cierra el programa, no la grabación.

## 4. Edición mínima
- Recortar inicio y final y unir tomas con **Clipchamp** (viene con Windows 11), **DaVinci Resolve** (gratis) o el editor que ya uses. Exportar **MP4 H.264 1280×720 30 fps**.
- Rótulos opcionales: título al inicio («Nierika Studio — Recorrido virtual: Día de Muertos en Mazunte»), y créditos al final (modelos propios, texturas CC0 de Poly Haven y ambientCG, librerías).
- Nombrar: `Nierika_1_entorno.mp4`, `Nierika_2_recorrido.mp4`, `Nierika_3_animaciones.mp4`.
- Revisar cada MP4 completo, con audio, en **otro reproductor** antes de subirlo.

## 5. Publicar
1. Subir a Drive o YouTube (**sin listar**) y confirmar que el enlace abre **sin iniciar sesión**.
2. Pegar los enlaces en la tabla de `docs/videos/README.md` (cambiar ⏳ por el enlace) y actualizar E-4 en el ESTADO.

## 6. Lista rápida antes de dar por buena una toma
- [ ] Release x64, 1280×720, FPS visibles
- [ ] Sin cursor de Windows ni notificaciones
- [ ] Se oye el oleaje (y la música, muy baja)
- [ ] La toma muestra lo que dice el guion, sin atascos contra muros
- [ ] Nada fuera de alcance mencionado como existente
