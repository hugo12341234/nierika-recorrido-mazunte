# Guía corta para D1: contribuir HOY desde tu cuenta (Windows)

> **Para que Hugo la copie y se la pase a D1.** No es parte del manual técnico. Tiempo estimado: 30–45 min. Todo se hace en **PowerShell**. No uses Vim: los commits van con `-m`.
> **Regla de oro:** escribe solo lo que de verdad hiciste. Los commits quedarán con tu nombre y la fecha de hoy; no se cambia el pasado.

## Antes de empezar (Hugo)

1. Pregunta a D1 su **usuario de GitHub**. Hoy el repositorio tiene como colaborador con permiso *write* a `thecurlyguy`; si es D1, no hay que hacer nada. Si D1 usa otro usuario: Hugo → repositorio → *Settings* → *Collaborators* → *Add people*.
2. **Fusiona primero el PR de este hilo** para que los archivos plantilla ya existan en `develop`. Escribe «fusiona» en el hilo.

## Pasos de D1

**1. Acepta la invitación** (si aún no): entra a https://github.com/hugo12341234/nierika-recorrido-mazunte e inicia sesión con tu cuenta; si aparece el aviso de invitación, pulsa *Accept*.

**2. Instala Git para Windows** si no lo tienes (https://git-scm.com/download/win; deja las opciones por defecto). Cierra y abre PowerShell. Comprueba: `git --version`

**3. Configura tu nombre y correo** (tu nombre real o el de tu cuenta de GitHub, y el correo de tu cuenta). En GitHub → *Settings* → *Emails* copia tu correo `ID+usuario@users.noreply.github.com` y activa «Keep my email addresses private» si quieres privacidad:

```powershell
git config --global user.name "Tu Nombre"
git config --global user.email "ID+tuusuario@users.noreply.github.com"
git config --global core.editor notepad
git config --global --list
```

**4. Clona y crea tu rama:**

```powershell
cd $HOME\Documents
git clone https://github.com/hugo12341234/nierika-recorrido-mazunte.git
cd nierika-recorrido-mazunte
git switch develop
git pull
git switch -c feature/d1-textos-individuales
```

Si pide iniciar sesión, usa el navegador cuando lo ofrezca (Git Credential Manager).

**5. Escribe TU texto** (los archivos ya existen con las instrucciones):

```powershell
notepad docs\manual_tecnico\secciones\individual_D1_aprendizaje.md
notepad docs\manual_tecnico\secciones\individual_D1_english.md
```

- Aprendizaje autónomo: una herramienta nueva, para qué, cómo la aprendiste, evidencia.
- Inglés: experiencia, participación, dificultades y aprendizaje, en tus palabras.
- Borra los corchetes y las instrucciones. Guarda (Ctrl+S).
- Lee también `docs\manual_tecnico\secciones\roles_raci_real.md`, apartado 2.2: **corrige lo que sea inexacto** sobre lo que tú hiciste.

**6. (Opcional, muy valioso) Aporta evidencia real de tu trabajo**: copia a `docs\manual_tecnico\evidencias\d1\` capturas, tus archivos fuente (`.ma`, `.blend`, `REPORTE8_GPO8.cpp`) **que pesen poco (< 20 MB en total)**. No subas `estadoCuenta.jfif`, comprobantes, ni datos personales.

**7. Confirma lo que vas a subir y haz commit:**

```powershell
git status
git add docs/manual_tecnico/secciones/individual_D1_aprendizaje.md docs/manual_tecnico/secciones/individual_D1_english.md
git commit -m "D1 · Aprendizaje autónomo y texto individual en inglés"
git push -u origin feature/d1-textos-individuales
```

(Si hiciste el paso 6: `git add docs/manual_tecnico/evidencias/d1` antes del commit.)

**8. Abre el PR:** en GitHub aparecerá «Compare & pull request» → base **develop** → título `D1 · Textos individuales (aprendizaje e inglés)` → *Create pull request*. Avísale a Hugo.

## Otras contribuciones que cuentan hoy (si hay tiempo)

- **Compila `develop` (Release x64)** y deja un **comentario de revisión** en el último PR fusionado diciendo qué viste (FPS, errores). Es real, firmado por ti y verificable.
- **Revisa un PR** de Hugo desde *Files changed* → *Review changes* → *Approve* o *Comment*.
- Anota en `individual_D1_english.md` los enlaces a tus PR/comentarios.

## Si algo falla

- `Permission denied` / 403 al hacer push: tu cuenta no tiene permiso de escritura → Hugo revisa *Settings → Collaborators*.
- `fatal: not a git repository`: estás en otra carpeta; haz `cd` a la carpeta clonada.
- Se abrió una ventana de editor al hacer commit: cierra con Ctrl+S y cierra Notepad (por `core.editor`); siempre usa `-m`.
- No hagas `git push --force` ni `git rebase`.

## Qué hará Hugo con tu PR

Revisa que los textos sean tuyos, los fusiona con «fusiona» escrito y la coordinación los integra al manual técnico. Tu autoría quedará en el historial con tu cuenta y la fecha real.
