# 1. What the tour is and what you need

## 1.1 What it is
A first-person 3D virtual tour of the Day of the Dead in Mazunte, Oaxaca: you walk from the beach to the cemetery and enter a room with an offering (*ofrenda*). *(Scope: `docs/CONTEXTO_PROYECTO.md`.)*

## 1.2 What you need
| Requirement | Value | Status |
|---|---|---|
| System | Windows, **64-bit only (x64)** | Real (CLAUDE.md §3) |
| Graphics | **OpenGL 3.3** compatible | Real |
| Window size | **1280 × 720** (fixed in the code) | Real (`WIDTH, HEIGHT`) |
| Expected performance | ≥ 30 FPS on a laptop with integrated graphics | **Target, not measured** on such a machine (see `manual_tecnico/en/08_testing_and_performance.md` §8.2.1) |
| Keyboard and mouse | Yes | Real |
| Sound | Optional: with no sound card or missing `Audio/` files the program warns in the console and runs without audio | Real (`Audio.cpp`) |

## 1.3 How to open it
**PENDING (E-3, E-7).** This will explain: unzip the `.zip` published in the GitHub *Release* and run `NierikaStudio.exe`. The script that builds the package exists (`tools/empaquetar_release.ps1`, E-3), but the `.zip` has not yet been published or tested on another PC. Until then the program is built from Visual Studio 2022 (see the root `README.md`).

## 1.4 Start menu (X-63)
When the program opens, a **start menu** shows first over the scene, which slowly circles in the background. The silhouette, the tour and the music **have not started yet**. Buttons: **Iniciar recorrido** (starts everything from the cemetery, as before), **Controles** (the real keys), **Créditos** (team and **all the project's references in APA 7**, grouped by topic: cultural sources, music and voice, textures, libraries and technical bibliography; it is a long list: scroll with the mouse wheel or the arrow keys) and **Salir** (quit). Pick with a click or with the arrow keys and Enter; **Esc** quits. During the tour, **Esc** opens the same screen as a pause menu.

![Start menu](../capturas/01_menu_inicio.jpg)

![Credits](../capturas/01_menu_creditos.jpg)

![Credits, end of the list](../capturas/01_menu_creditos_fin.jpg)

*Screenshots of Nov 17, 2026 taken on Linux (Mesa, no sound) with the development variables `NIERIKA_UI=menu_creditos_fin` and `NIERIKA_MENU_PNG`; not yet seen on D2's PC with VS 2022.*

## 1.5 What you see when the tour opens
The window is titled *Nierika Studio - Recorrido virtual: Dia de Muertos en Mazunte* and, after a moment, adds the frames per second (`NNN FPS`). The cursor disappears (see section 2). The camera starts in the **cemetery** at position (0, 1.6, 6) m: 6 m in front of the room, at human eye height (1.6 m). That spot is under the papel picado, so **the first card to appear is the papel picado card** (see section 4).

![Window just opened](../capturas/01_ventana_inicio.jpg)

*Screenshot taken on Nov 17, 2026 on D2's PC with `develop` code up to PR #131 (the program's capture mode, Release x64, 1280×720). The papel picado card shows because the camera starts under the posts.*
