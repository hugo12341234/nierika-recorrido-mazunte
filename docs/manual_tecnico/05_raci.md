# 5. Matriz RACI

R = Responsable de ejecutar · A = Aprueba y rinde cuentas · C = Consultado · I = Informado. D1 = 319266894, D2 = 321332086.

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

Desde el 18/10 las columnas Resp./Verif. **ya no se siguen**: quien está libre toma la siguiente tarea y **el otro revisa** antes de integrar. Por eso la Tabla 8 no describe la ejecución real.

## 5.3 Registro real (lo que hoy se puede afirmar)

Solo se anota lo que el ESTADO dice; no se infiere nada de los commits.

| Tarea | Ejecución según el ESTADO | Verificación según el ESTADO |
|---|---|---|
| A-5, A-7 | Playa y mar «modeladas por D2» | A-5 y A-6: APROBAR, comentario de D2 en el PR; A-7: el ESTADO no registra veredicto |
| A-1…A-4, A-6 | El ESTADO no nombra al ejecutor (por la decisión del 04/11 los PR están a nombre de D2) | A-1, A-2 y A-6: APROBAR registrado (A-2 y A-6 con comentario de D2 en el PR); A-3 y A-4: el ESTADO no registra veredicto |
| A-8 | Luces de D1 integradas por D2 | APROBAR «según D2» (04/11) |
| X-3 | Pan, flor y calavera de D1 (Maya), integrados por D2 | APROBAR «según D2» (04/11) |

**Importante, sin maquillar:** en este momento **no consta** en el repositorio que D1 haya aceptado la invitación como colaborador (ESTADO, act. 3 y §7) y **el ESTADO no registra ninguna verificación hecha por D1**: la regla «verifica el otro» se sustituyó por la propuesta «D2 verifica en una sesión nueva», que sigue **pendiente de confirmar** (ESTADO §4). El manual no debe presentar el RACI planeado como cumplido.

## 5.4 Qué falta

1. ❓ D1 y D2 deciden qué RACI real se publica (con las horas y tareas de cada uno). Nadie ha entregado ese dato.
2. ❓ Autoría de commits y PR: D2 puso todo a su nombre (decisión del 04/11, X-3); el crédito de D1 va en la descripción del PR y D2 corregirá después. El RACI real debe coincidir con eso.
3. ⏳ Cuando D1 revise PR, anotar quién aprobó cada uno.
4. ⏳ Matriz RACI final por área con los datos de 1–3.
