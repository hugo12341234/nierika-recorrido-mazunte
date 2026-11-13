# 5. Matriz RACI

R = Responsable de ejecutar · A = Aprueba y rinde cuentas · C = Consultado · I = Informado. D1 = 319266894, D2 = 321332086. Estado al 12/11/2026.

## 5.1 RACI planeado (propuesta v2.0, Tabla 8) ✅ **[repo, PDF de la propuesta]**

Entre paréntesis, el número de actividades. Esta tabla es **lo que se propuso**, no lo que ocurrió.

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

## 5.2 Cambio de forma de trabajo ✅ **[repo, ESTADO §4, 18/10]**

Desde el 18/10 las columnas Resp./Verif. ya no se siguen: quien está libre toma la siguiente tarea y **el otro revisa** antes de integrar. La Tabla 8 no describe la ejecución real.

## 5.3 Registro real (lo que hoy se puede afirmar)

Solo se anota lo que el ESTADO y el repositorio dicen; no se infiere nada de los commits.

| Qué | Quién, según el repositorio | Fuente |
|---|---|---|
| Código, modelos de Blender, texturas y audio generados por script, documentación | **D2** (`Hugo Chacón`) figura como autor de **189 + 8 commits**; hay 1 commit de la plataforma (`Claude`) | `git shortlog -sne origin/develop`, 12/11 |
| Iluminación Blinn-Phong con 4 puntuales, reflectores y su animación | **D1** (Práctica 8 adaptada), integrada por D2 en A-8 | ESTADO §4, 04/11 |
| Pan, flor y calavera (Maya); modelo de la silueta de 10 piezas | **D1**, integrados por D2 (X-3, D-1) | ESTADO X-3 y D-1 |
| Playa y mar | D2 (Blender 5.2.2) | ESTADO A-5, A-7 |
| Modelos B-3, B-4, B-7, B-9, B-10, B-12, E-2, audio D-6 | Generados con `bpy` y scripts por Claude a pedido de D2 (revisados por D2 en cada PR; sin `/verificar` de otra persona en los PR #59–#68) | ESTADO §4, 11/11 y 12/11 |
| Verificaciones | `/verificar` APROBAR registrado para A-1, A-2, A-5, A-6, A-7 (con observaciones), A-8, X-3, B-9 y B-10; **todas por D2** («según D2» en A-8 y X-3). **Ninguna por D1** | ESTADO §2 |

**Sin maquillar:** en el repositorio **no consta** que D1 haya aceptado la invitación como colaborador ni que haya verificado un PR; los commits están todos a nombre de D2 por la decisión del 04/11 (D2 corregirá el crédito de D1 después). El RACI planeado de §5.1 **no** debe presentarse como cumplido.

## 5.4 Qué falta (decisión de D1 y D2, no de Claude)

1. ❓ **Qué RACI real se publica**, con las tareas y horas que cada uno declare. Este manual no puede inventarlo.
2. ❓ **Crédito de D1** en la autoría: D2 dijo que lo corregirá; hasta entonces el historial de Git no refleja el aporte de D1.
3. ⏳ Si D1 revisa PR antes de entregar, anotar quién aprobó cada uno.
4. ⏳ Matriz RACI final por área con los datos de 1–3.
