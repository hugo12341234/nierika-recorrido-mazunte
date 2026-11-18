# Anexo B. Pruebas, rendimiento y problemas conocidos

## B.1 Cómo se probó

- **Compilación y ejecución:** cada cambio se compila en Visual Studio 2022 (Release x64) en la PC de D2 y se ejecuta con F5. D2 revisó funcionamiento, audio y aspecto visual tras el PR #148.
- **Revisión de cambios:** desde el 20/10 la regla es un veredicto `/verificar` APROBAR antes de fusionar; hay veredictos registrados para A-1, A-2, A-5 a A-8, X-3, B-9 y B-10. Desde el 12/11 varios grupos de *pull requests* se fusionaron sin ese veredicto por decisión de D2 (excepciones registradas en el ESTADO, sección 4).
- **Pruebas numéricas de la matemática** con programas aparte en C++ (`tools/d7_pruebas_matematicas.cpp`, `x19_pruebas_extras.cpp`, `x49_pruebas.cpp`, `x53_pruebas.cpp`, `x55_pruebas.cpp`, `x65_pruebas_peregrinos.cpp`): comprueban fórmulas, caídas libres, continuidad de los ciclos y colisiones sin necesitar OpenGL.
- **Verificadores geométricos de la escena** en Python y C++ (`tools/x48_verificar_solapes.py`, `x53_verificar_alturas.py`, `x55_verificar_ofrendas.cpp`) que revisan solapes entre objetos y alturas contra el relieve.
- **Modo de captura del propio programa** (`NIERIKA_CAPTURAS`) para tomar imágenes reproducibles desde una pose exacta de cámara; de ahí salen las capturas de los manuales.
- No hay integración continua ni pruebas automáticas dentro del repositorio.

## B.2 Rendimiento

El criterio del cliente es **al menos 30 FPS a 1280 × 720 en una laptop con gráficos integrados**.

| Equipo | Estado del código | Resultado | Quién midió |
|---|---|---|---|
| Laptop con Intel Core i5 de 11.ª generación, **gráficos integrados** | Antes del PR #148 | Unos **15 FPS** | D2 |
| La misma laptop | Después del PR #148 (descarte por frustum y cachés) | **Mínimo 45 FPS, promedio de unos 60** | D2, en Visual Studio 2022 |
| PC con GPU dedicada | Después del PR #148 | Entre **200 y 340 FPS** (el recorrido guiado, unos 199) | D2 |

Se cumple el criterio de 30 FPS en el equipo con gráficos integrados que se midió. No se midió en otros equipos con gráficos integrados, ni se anotó el modelo exacto de procesador o de la GPU dedicada.

**Qué lo explicó.** La lectura del código y el conteo de llamadas (Mesa, pose inicial) mostraron 3 694 llamadas de dibujo y 1.27 millones de triángulos por cuadro, y 17 luces evaluadas por fragmento. Las optimizaciones del PR #148 no cambian la imagen (comprobado píxel a píxel contra la versión anterior en un renderizador por software): redujeron las llamadas de dibujo a 1 139 y los triángulos a 0.57 millones, y el tiempo total del cuadro bajó un 32 % en el renderizador por software usado como proxy. Con la tecla **F9** el programa imprime en consola una tabla de tiempos por etapa (CPU y GPU), y la variable `NIERIKA_SIN_CULLING=1` desactiva el descarte para comparar.

## B.3 Memoria de GPU

`cleanup()` explícito libera programas, VAO, VBO, EBO y texturas en `Shader`, `Mesh` y `Model` y se llama al final de `main` antes de `glfwTerminate()` (decisión del 19/10: no se usan destructores porque las clases se copian). **No se comprobó con una herramienta externa** (RenderDoc u otra) que no queden fugas de recursos.

## B.4 Problemas conocidos al 18/11/2026

| Tema | Estado |
|---|---|
| Fugas de GPU | No verificadas con herramienta (ver B.3) |
| Texturas de 512 px | `laminado1.jpg`, `concreto.jpg`, `humo.png`, `papel_fucsia.png` y `papel_morado.png` miden 512 px, y la convención pide 1K; se dejaron sin agrandar |
| Música y audio | D2 los escuchó y reportó que suenan bien; el balance de volúmenes lo decidió D2 |
| Narraciones 07 y 08 | Tienen la misma duración, pero **no son copia**: sus sumas MD5 son distintas (`cb44c8f8…` y `7356e37c…`, 18/11). D2 confirmó que son distintas |
| Datos de la escena | `tools/x33_vida_colinas.py` ya no reproduce el archivo `VidaColinasDatos.h`, porque este se ajustó después de generarse; no debe volver a correrse sin revisar el resultado |
| Fichas | 13 fichas; las fuentes de las fichas 6 a 13 se compararon por lectura automática de las páginas, no por lectura humana |
| Repositorio público | El repositorio es público y contiene los números de cuenta; el historial de Git conserva documentos de otras materias que se quitaron del árbol de archivos. Borrarlos del historial exigiría reescribirlo y no se hizo |
| Paquete `.zip` para otra PC | El script `tools/empaquetar_release.ps1` existe y se ejecutó en la PC de D2 el 04/11; el `.zip` final y su prueba en una PC sin Visual Studio quedaron pendientes |
