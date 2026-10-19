# Flujo de trabajo — Nierika Studio

> Cómo avanzar una tarea de principio a fin sin perder cambios ni contexto.
> **Regla de oro:** `docs/ESTADO_PROYECTO.md` en la rama `develop` es la memoria del proyecto.
> Lo que no esté ahí, Claude no lo sabrá la próxima vez.

---

## 1. Qué hace cada herramienta

| Herramienta | Para qué se usa | Dónde |
|---|---|---|
| **Trello** | Quién hace qué y en qué estado va cada tarea | https://trello.com/b/8FNkEY97/nierika-studio-recorrido-mazunte |
| **GitHub** | Código y documentos; revisión por *pull request* (PR) | https://github.com/hugo12341234/nierika-recorrido-mazunte |
| **Claude Code** | Todo lo que **modifica archivos del repo** (código, shaders, `.mtl`, docs) | PowerShell, dentro de la carpeta del repo (`claude`) |
| **Proyecto de Claude (web)** | Planear, dudas de teoría, matemáticas, guías de Blender/GIMP, manuales | claude.ai → proyecto "Nierika – Recorrido Mazunte" |
| **Visual Studio 2022** | Compilar (`NierikaStudio.sln`, Release \| x64) y ver la escena | PC de cada integrante |

---

## 2. Ciclo de cada tarea

### Antes de empezar
1. **Trello:** tomar una tarjeta de *Pendiente* (primero las rojas = Alta) → moverla a *En proceso* → asignarse en **Members**. Máximo 2 tarjetas en proceso por persona.
2. **Actualizar la copia local** (PowerShell, en la carpeta del repo):
   ```
   git checkout develop
   git pull
   ```
3. **Crear la rama de la tarea:**
   ```
   git checkout -b feature/act-<N>-<nombre-corto>
   ```
   Ejemplo: `feature/act-17-dovelas`.

### Mientras se trabaja
4. Abrir Claude Code (`claude`) y escribir **`/actividad <N>`**: lee el estado, el criterio de aceptación y las dependencias, y propone los pasos.
5. Claude edita los archivos; **quien trabaja compila en Visual Studio (F5)** y le dice qué se ve o qué error sale (Claude no ve la pantalla ni compila en Windows).
6. Tareas de **Blender o GIMP**: abrir un hilo nuevo en el Proyecto de Claude (p. ej. "Act 17 — Dovelas"), seguir la guía, exportar el `.obj` a `Models/<objeto>/` y volver a Claude Code para integrarlo.

### Al terminar
7. Comprobar el **criterio de aceptación** y que compile en **Release | x64** sin errores.
8. En Claude Code: **`/cierre`** → actualiza el ESTADO (estado de la actividad, parámetros con valores exactos, decisiones, problemas abiertos). **Leer el resumen antes de aprobarlo.**
9. Pedir a Claude Code **"haz commit y push de la rama"** (no lo hace sin que se le pida).
10. **Abrir el PR** en GitHub: base `develop` ← la rama; título "Act <N> — <nombre>". (O pedir a Claude Code "abre el PR".)
11. **Trello:** tarjeta a *En revisión* y avisar al compañero.

### Revisión (la hace el otro integrante)
12. ```
    git fetch
    git checkout feature/act-<N>-<nombre-corto>
    ```
    Compilar, probar y en Claude Code escribir **`/verificar <N>`**. Aprobar o comentar en el PR.
13. Si se aprueba: **Merge** en GitHub y borrar la rama. Tarjeta a *Terminado*. Si sirve para el manual técnico, guardar una captura en ese momento.
14. **Los dos** actualizan:
    ```
    git checkout develop
    git pull
    ```

---

## 3. Cuándo usar cada Claude

- **Claude Code** → si el resultado es un archivo cambiado del repo.
- **Proyecto de Claude (web)** → si es para entender o planear ("¿qué sigue?", fórmulas, Blender, textos de fichas, manuales). **Un hilo por actividad.**
- **No duplicar el cierre:** si la tarea se hizo en Claude Code → `/cierre`. Escribir `CIERRE` en el proyecto web solo si todo se hizo ahí.
- **Modelo:** Sonnet para el día a día; Opus para lo difícil (KeyFrames, shaders, errores raros); Haiku para cosas simples. En Claude Code se cambia con `/model`.

---

## 4. Reglas para no rehacer trabajo

1. **Nunca trabajar directo en `develop` ni en `main`**; siempre en una rama `feature/...`.
2. **Siempre `git pull` en `develop` antes de crear una rama.**
3. **Un PR = una tarea**, pequeño, para revisarlo rápido y evitar conflictos.
4. **Antes de cada commit, revisar `git status`**: no deben aparecer `build/`, `.vs/`, `.exe` ni `.dll` (el `.gitignore` los bloquea). Si aparece algo raro, preguntar.
5. **Todo valor que cambie** (escala, posición, color de luz) queda en la **sección 5 del ESTADO**.
6. **Toda decisión nueva** (techo plano, quitar un objeto, cambiar una librería) va al **registro de decisiones** del ESTADO.
7. **Conflictos** (casi siempre en `main.cpp`): pedir a Claude Code "actualiza mi rama con develop y resuelve el conflicto".
8. **Viernes, 30 min juntos:** revisar Trello y ESTADO, ver atraso y qué recortar (orden de recorte en la sección 2 del ESTADO).
9. **Al cerrar cada bloque:** capturas y parámetros en `docs/manual_tecnico/`, para no juntarlo todo al final.

---

## 5. Ejemplo: primera tarea del Bloque A

```
Trello: "Corregir mtllib y material del altar" → En proceso
git checkout develop && git pull
git checkout -b feature/act-a1-mtllib-altar
claude  →  /actividad A1
   (Claude corrige altar.obj y altar.mtl; F5 en Visual Studio: el altar ya tiene textura)
/cierre  →  revisar el resumen
"haz commit, push y abre el PR"
Trello → En revisión; avisar al compañero
Compañero: git fetch + checkout + F5 + /verificar → aprueba → Merge
Trello → Terminado; los dos: git checkout develop && git pull
```

---

## 6. Lista rápida

- [ ] Tarjeta en *En proceso* y asignada
- [ ] `git pull` en `develop` y rama nueva
- [ ] `/actividad <N>`
- [ ] Compila en Release x64 y cumple el criterio
- [ ] `/cierre` revisado
- [ ] Commit + push + PR
- [ ] Tarjeta en *En revisión* y aviso al compañero
- [ ] `/verificar <N>` y merge
- [ ] Tarjeta en *Terminado* y `git pull` de los dos
