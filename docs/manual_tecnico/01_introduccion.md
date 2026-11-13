# 1. Introducción

> Estado de los datos: **12/11/2026, hora de México**, rama `develop` (PR #59 a #68 fusionados). Marcas: ✅ **[repo]** comprobado en el repositorio · 🔄 hecho en código pero **sin confirmar compilado en MSVC/visto en pantalla por D2** · ⏳ pendiente · ❓ por definir.

## 1.1 Objetivo

Construir un recorrido virtual 3D interactivo, en primera persona y en tiempo real, que represente del atardecer a la noche del 1 de noviembre en Mazunte, Oaxaca: la playa, el panteón y el cuarto del cementerio con su altar. ✅ **[repo]** `CONTEXTO_PROYECTO.md` §1–2.

## 1.2 Datos del proyecto

| Campo | Valor | Fuente |
|---|---|---|
| Empresa / equipo | Nierika Studio · Equipo 15 («Ver más allá de lo visible») | CONTEXTO §1 |
| Integrantes | D1 = 319266894 · D2 = 321332086 | CONTEXTO §1 |
| Periodo | 28/09/2026 → 18/11/2026 (entrega final) | CONTEXTO §1 |
| Propuesta | v2.0, 4 de octubre de 2026 (`docs/propuesta/`) | CONTEXTO §1 |
| Cliente | Profesor de la materia (Computación Gráfica e Interacción Humano-Computadora, FI UNAM) | CONTEXTO §1 |

## 1.3 Alcance

**Incluye** ✅ **[repo, CONTEXTO §2]:** escenario continuo con 3 zonas (playa y orilla, panteón con barda, cuarto del cementerio); navegación en primera persona con colisiones; 5 animaciones (2 por KeyFrames + 3 simples); 5 fichas culturales; iluminación Blinn-Phong; audio ambiental de un solo ciclo; proyecto de Visual Studio, repositorio, 3 videos y 2 manuales (ES + EN).

**No incluye** ✅ **[repo]:** calles y edificios del pueblo, Punta Cometa, tapete de arena, versiones VR/web/móvil, multijugador, personajes con IA o diálogo, humo de copal con partículas (el copalero se modela, sin humo). La reproducción exacta del panteón real tampoco: es una **adaptación del estudio**.

## 1.4 Criterios de aceptación y su estado

| Criterio | Meta | Estado al 12/11 | Qué falta para darlo por cumplido |
|---|---|---|---|
| Modelos propios | ≥ 18 (definición: ESTADO §4, 19/10) | 🔄 los **23** `.obj` del inventario existen, cada uno con su `.blend` (calculado de los archivos, [§6](06_modelos_y_recursos.md)); 11 tienen ✅ en el ESTADO y 12 siguen 🔄 | `/verificar` o verlos compilados; **que cada `.blend` sea del equipo** no lo prueba el repo (el de la fachada se reconstruyó del `.obj`) |
| Texturas | 1K (1024 px) | 🔄 casi todas 1024×1024; **excepción: `laminado1.jpg` (fachada y techo) mide 512×512**; 8 modelos siguen con textura provisional (tumbas 1–4, cruz, soportes, silueta y mar) de un solo color | Reemplazarlas o registrar la excepción en el ESTADO |
| Iluminación | Interior distinguible sin perder el ambiente nocturno | ✅ A-8 (04/11), con las luces de D1 | — |
| Animación | 5 (2 KeyFrames + 3 simples), ninguna copiada de las prácticas, con justificación matemática | 🔄 las 5 están en el código (olas, velas, flores, caminata y dejar flor) y las 5 tienen justificación ([§7](07_justificacion_matematica.md)) | Que D2 las compile y las vea; la comparación con las prácticas del curso (que no están en el repo) la revisa D2 |
| Interacción | 5 fichas a < 2 m con su fuente en APA 7 | 🔄 código listo (`Fichas.cpp`) y textos de C-7a «por verificar contra la fuente» | Compilar con ImGui en VS y verificar textos y referencias |
| Recorrido | Cámara 1.ª persona con colisiones | ✅ A-9 (04/11); E-2 corrigió la barda (12/11) | Prueba de atascos en la PC de D2 |
| Audio | Un ciclo: mar y música lejana | 🔄 dos pistas propias en `Audio/` (D-6) | **Nadie ha oído cómo suenan** |
| Rendimiento | ≥ 30 FPS a 1280×720 en gráficos integrados | ❓ **no medido** (el contador de FPS existe en el título de la ventana desde E-1a) | D2 mide con el protocolo de [§8.2.1](08_pruebas_y_rendimiento.md) |

**KeyFrames solo** en la caminata de la silueta y en «dejar flores»: velas, fuego, papel picado y humo no cuentan. ✅ **[repo]**

## 1.5 Organización del trabajo

- **Forma de trabajo (decisión del 18/10):** las columnas Resp./Verif. de la propuesta ya no se siguen; quien está libre toma la siguiente tarea del plan vigente y el otro integrante revisa antes de integrar. ✅ **[repo, ESTADO §4]**
- **Control de versiones:** ramas `main` (entregas), `develop` (integración) y `feature/<código>-<nombre>`; todo entra por pull request. ✅ **[repo, FLUJO_DE_TRABAJO]**
- **Verificación:** desde el 20/10 ningún PR entra a `develop` sin `/verificar` con veredicto APROBAR. **Excepción registrada:** los PR #59 a #68 (B-13, E-2, D-5, D-6, C-6, B-12, C-5, D-7, E-5 y E-6) se fusionaron el 12/11 **sin veredicto APROBAR** por decisión de D2 (ESTADO §4, 12/11); la autoverificación de Claude no sustituye la revisión de otra persona ni la compilación con MSVC. ✅ **[repo]** El ESTADO tiene además una excepción previa del 11/11 que cubre esas mismas tareas: ver «Contradicciones detectadas» en [README](README.md).
- **Gestión:** tablero Trello Kanban (Pendiente / En proceso, máx. 2 por persona / En revisión / Terminado). ✅ **[repo]**
- **Asistencia de IA:** el equipo usa Claude (Claude Code y un Proyecto de Claude) para planear, programar y documentar, con reglas escritas en `CLAUDE.md`. ❓ **POR DEFINIR** con qué redacción se declara; propuesta en [§9.3](09_conclusiones.md).

📷 **FIGURA pendiente:** escena completa a la hora de la entrega (se toma en la PC de D2 con el aspecto final).
