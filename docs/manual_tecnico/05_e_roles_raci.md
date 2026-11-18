# e) Roles y responsabilidades: ejecución real frente a la matriz RACI inicial

R = Responsable de ejecutar · A = Aprueba y rinde cuentas · C = Consultado · I = Informado. D1 = 319266894, D2 = 321332086.

## e.1 RACI planeado (propuesta v2.0, Tabla 8)

Entre paréntesis, el número de actividades de cada área. Esta tabla es **lo que se propuso**, no lo que ocurrió.

| Área de trabajo | Actividades | D1 | D2 | Cliente |
|---|---|---|---|---|
| Investigación y propuesta | 1–2 | R (1) / A (1) | R (1) / A (1) | A |
| Infraestructura y herramientas | 3–6 | R (1) / A (3) | R (3) / A (1) | I |
| Diseño del cuarto del cementerio | 7 | R (1) | A (1) | C |
| Playa, mar y panteón | 8, 10, 13, 14 | R (1) / A (3) | R (3) / A (1) | I |
| Cámara, ciclo de render y colisiones | 9, 11, 12, 15 | R (3) / A (1) | R (1) / A (3) | I |
| Cuarto del cementerio | 16–22 | R (3) / A (4) | R (4) / A (3) | I |
| Objetos de la ofrenda | 23–30 | R (4) / A (4) | R (4) / A (4) | I |
| Texturizado | 31–34 | R (2) / A (2) | R (2) / A (2) | I |
| Integración e iluminación | 35–38 | R (2) / A (2) | R (2) / A (2) | I |
| Animaciones simples | 39–45 | R (3) / A (4) | R (4) / A (3) | I |
| Animaciones complejas | 46–50, 53 | R (3) / A (3) | R (3) / A (3) | I |
| Puntos de información y audio | 51–52 | R (1) / A (1) | R (1) / A (1) | C |
| Pruebas, videos, manuales y entrega | 54–60 | R (4) / A (3) | R (3) / A (4) | A |
| **Total** | **60** | **R 29 · A 31** | **R 31 · A 29** | |

## e.2 Cambio de forma de trabajo

Desde el 18/10 las columnas Resp./Verif. de la propuesta dejaron de seguirse: quien está libre toma la siguiente tarea del plan vigente y el otro integrante la revisa antes de integrarla (decisión del 18/10 en el ESTADO). Por eso la Tabla 8 no describe la ejecución real.

## e.3 Lo que el repositorio permite afirmar

Solo se anota lo que consta en el ESTADO y en el historial de Git; no se infiere nada de los nombres de commits.

| Qué | Quién, según el repositorio | Fuente |
|---|---|---|
| Código C++, shaders, scripts de modelos y texturas, audio, documentación, integración de todas las piezas | **D2**: 489 de los 491 commits de `develop` tienen a D2 como autor; el restante es de la plataforma Claude (API de GitHub, 18/11) | Historial de Git |
| Iluminación Blinn-Phong con 4 luces puntuales, reflectores y su animación | **D1** (adaptación de su Práctica 8), integrada por D2 en A-8 (04/11) | ESTADO, decisión 04/11 |
| Calavera, pan, flor y otros modelos de Maya (humo, fuego, mariposas, mazetas, papel, cuadro); modelo de la silueta de 10 piezas | **D1**, integrados por D2 (X-3, D-1, X-43, X-47, X-49) | ESTADO |
| Playa y mar en Blender | D2 | ESTADO, A-5 y A-7 |
| Modelos B-3, B-4, B-7, B-9, B-10, B-12 y otros generados con `bpy` y scripts | Generados con scripts a pedido de D2 con ayuda de un asistente de IA (Claude) y revisados por D2 | ESTADO, decisiones del 11 y 12/11 |
| Decisiones de alcance, valores y aceptación | **D2** (cada decisión del ESTADO lleva «D2 decidió» o «pedido de D2») | ESTADO, sección 4 |
| Verificaciones `/verificar` con veredicto APROBAR | Todas hechas por D2 (según D2 en A-8 y X-3); ninguna por D1 | ESTADO, sección 2 |

**Sin maquillar.** En el repositorio no consta que D1 haya aceptado la invitación como colaborador de GitHub ni que haya hecho commits, abierto *pull requests* o revisado alguno. Los commits están a nombre de D2 por la decisión del 04/11 de integrar el trabajo de D1 a nombre de D2 y corregir el crédito después. La matriz de la propuesta **no debe leerse como cumplida** tal como se planeó.
