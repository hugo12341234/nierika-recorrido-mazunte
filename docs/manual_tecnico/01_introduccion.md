# 1. Introducción

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

## 1.4 Criterios de aceptación y restricciones del cliente

| Criterio | Meta | Estado al 04/11 |
|---|---|---|
| Modelos propios | ≥ 18 (definición: ESTADO §4, decisión del 19/10) | ✅ 1 cuenta hoy (fachada); ver [§6](06_modelos_y_recursos.md) |
| Texturas | 1K (1024 px), sin estiramientos | 🔄 playa y mar en 1K; fachada 512 px; la capilla provisional 2048 px (problema #3) |
| Iluminación | Interior distinguible sin perder el ambiente nocturno | ✅ A-8 (04/11), con las luces de D1 |
| Animación | 5 (2 KeyFrames + 3 simples), ninguna copiada de las prácticas, cada una con justificación matemática | ⏳ ninguna implementada aún |
| Interacción | Ficha a < 2 m con su fuente en APA 7 | ⏳ C-7 |
| Recorrido | Cámara 1.ª persona con colisiones | 🔄 cámara existe; colisiones ⏳ A-9 |
| Rendimiento | ≥ 30 FPS a 1280×720 en gráficos integrados | ❓ **no medido** (el ESTADO, A-8, lo registra "de palabra") |

**KeyFrames solo** en la caminata de la silueta y en «dejar flores»: velas, fuego, papel picado y humo no cuentan. ✅ **[repo]**

## 1.5 Organización del trabajo

- **Forma de trabajo (decisión del 18/10):** las columnas Resp./Verif. de la propuesta ya no se siguen; quien está libre toma la siguiente tarea del plan vigente y el otro integrante revisa antes de integrar. ✅ **[repo, ESTADO §4]**
- **Control de versiones:** ramas `main` (entregas), `develop` (integración) y `feature/<código>-<nombre>`; todo entra por pull request. ✅ **[repo, FLUJO_DE_TRABAJO]**
- **Verificación:** desde el 20/10 ningún PR entra a `develop` sin `/verificar` con veredicto APROBAR (decisión del 19/10, vigente desde el 20/10). Quién verifica mientras D1 no esté en el repositorio sigue **pendiente de confirmar** en el ESTADO §4. ✅ **[repo]**
- **Gestión:** tablero Trello Kanban (Pendiente / En proceso, máx. 2 por persona / En revisión / Terminado). ✅ **[repo]**
- **Asistencia de IA:** el equipo usa Claude (Claude Code y un Proyecto de Claude) para planear, programar y documentar, con reglas escritas en `CLAUDE.md`. ❓ **POR DEFINIR** si el manual debe declararlo y con qué redacción; propuesta: una nota de transparencia en §9 (conclusiones), a decidir por D1 y D2.

📷 **CAPTURA pendiente:** figura de la escena completa (cuando existan el panteón y el cuarto amueblado).
