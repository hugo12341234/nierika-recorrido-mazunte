# Lista de verificación de la entrega (E-3 y E-7)

> Marca con ✅ solo lo que viste tú. Cada casilla dice **dónde** correrla. Fecha límite: **mié 18/11/2026**.
> Verificado hasta hoy (12/11): nada de esto se ha ejecutado en Windows desde este repositorio; es una lista de pasos.

## A. Antes de empaquetar (PC de D2)
- [ ] `git checkout develop` y `git pull`; `git status` limpio.
- [ ] Todo lo que quieras entregar ya está fusionado en `develop` (PR abiertos revisados; desde el PR #69 rige `/verificar` APROBAR).
- [ ] Visual Studio 2022 → **Release | x64** → Build Solution: **0 errores** (anota los warnings si hay).
- [ ] F5 en Release: recorrer playa → panteón → cuarto sin atascos; ver la silueta caminar y dejar la flor; fichas a < 2 m; barra espaciadora y **R** funcionan.
- [ ] Mirar el título de la ventana: anotar los **FPS** (E-1a) en la escena de la orilla y en el cuarto.

## B. Empaquetar (E-3)
- [ ] Developer PowerShell en la raíz del repo: `powershell -ExecutionPolicy Bypass -File tools\empaquetar_release.ps1`
      (opcionales: `-SinModelosSinUsar` ahorra ≈ 12 MB; `-IncluirRuntimeVC` copia las DLL de Visual C++).
- [ ] El script termina con "Contenido minimo: OK" y sin `Warning` de DLL o de archivos faltantes.
- [ ] Existen `dist\NierikaStudio_<fecha>.zip` y `.zip.sha256`. Anota tamaño y SHA-256.
- [ ] El `.zip` contiene en la raíz: `NierikaStudio.exe`, las `.dll`, `LEEME.txt`, `Models\`, `Shaders\`, `Audio\` y **ningún `.blend`**.

## C. Prueba en otra PC (criterio de E-3)
- [ ] Copiar el `.zip` a una PC **sin Visual Studio** (otra laptop, de un compañero o del laboratorio), descomprimir y abrir el `.exe`.
- [ ] Se abre, se ve la escena, suena el audio, no pide DLL. Si pide `VCRUNTIME140.dll`/`MSVCP140.dll`: repetir el paso B con `-IncluirRuntimeVC`.
- [ ] Anotar en el ESTADO: modelo de PC, gráficos, FPS observados y captura. **Esto cierra E-3 (🟡 → ✅).**

## D. Publicar (E-7)
- [ ] PR `develop` → `main` con los manuales y videos incluidos (decisión del equipo: `main` solo recibe entregas).
- [ ] Crear el Release en GitHub: etiqueta `v1.0.0` sobre `main`, título y texto de `docs/entrega/NOTAS_RELEASE.md` (con los ⚠️ resueltos o dejados como límites).
- [ ] Adjuntar el `.zip` y el `.sha256`. Descargar el `.zip` **desde el Release** y comprobar el hash: `Get-FileHash <archivo> -Algorithm SHA256`.
- [ ] Decidir y registrar si el repositorio queda **público o privado** (pendiente en el ESTADO §4; hoy es público y expone los números de cuenta).
- [ ] Los 3 videos MP4 subidos (Drive/YouTube sin listar) y sus enlaces en `docs/videos/README.md`.

## E. Entregables finales (CONTEXTO §9)
- [ ] 1. Proyecto de Visual Studio + repositorio con Release publicado
- [ ] 2. `dist/` con .exe autónomo y .dll
- [ ] 3. 3 videos MP4 (entorno, recorrido, animaciones e interactividad)
- [ ] 4. Manual técnico ES + EN (PDF) — **la versión en inglés aún no existe**
- [ ] 5. Manual de usuario ES + EN (PDF) — **la versión en inglés aún no existe**
- [ ] Enlace de entrega enviado al profesor
