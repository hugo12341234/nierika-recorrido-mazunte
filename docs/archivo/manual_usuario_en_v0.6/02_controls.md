# 2. Controls

> **Status: real.** Taken from `src/main.cpp` (`DoMovement`, `KeyCallback`, `MouseCallback`) in `develop`.

## 2.1 Controls for the visitor

| Action | Key / device | What it does |
|---|---|---|
| Move forward / back | **W** / **S**, or arrows **↑** / **↓** | Moves the camera where you look / the opposite way |
| Move sideways | **A** / **D**, or arrows **←** / **→** | Moves the camera left / right |
| Look around | **Mouse** | Rotates the camera. The cursor is hidden and trapped in the window |
| Altar candles | **V** (within 3 m of the altar) | Turns the altar candles on or off: the warm light and the flames fade in or out in a quarter of a second. Away from the altar it only shows a notice |
| Sound | **M** | Cycles through "sea and music", "sea only", "music only" and "silence" (a notice appears at the top of the screen) |
| Controls help | **F1** | Shows or hides the list of controls |
| Guided tour | **T** | A narrated visit through 12 stations (beach, offerings, altar, path of the souls, viewpoint), with subtitles and, if the audio files exist, a voice. **T**, **Esc** or **W A S D** cancel it and leave you where you are (see `docs/RECORRIDO_GUIADO.md`, in Spanish) |
| Restart the silhouette | **R** | The silhouette returns to the shore and repeats its walk and the flower |
| Pause / menu | **Esc** | With the guided tour active, the first press only cancels it. Otherwise it opens the pause menu (Continue, Controls, Credits, Quit); **Esc** again continues. On the start menu, **Esc** quits |

Notes verified in the code (`Camera.h`, `Colisiones.cpp`, `main.cpp`):
- Walking speed is **2.0 m/s** (`SPEED` in `Camera.h`); mouse sensitivity is **0.25**.
- You walk **only on the horizontal plane**: looking up or down does not make you fly; your eyes are always **1.6 m** above the ground (which slopes down at the shore).
- There are **collisions** (A-9): you cannot pass through walls, the cemetery wall or the altar; if you hit one head-on you slide along it. The walkable area reaches the shore (z = 24).
- At start the altar candles are **on**. Near the altar a reminder for **V** appears at the top.
- Since X-60 (Nov 17) the **space bar does nothing**: the colored lights were removed (ESTADO sec. 4, Nov 17).
- Since X-63 (Nov 17) **Esc no longer closes the program directly**: it opens the pause menu. To quit: Esc, then "Salir" (click or Enter). On the start menu, Esc does quit.
- The cursor reappears in the menus and you can click; the arrow keys (or W S) and Enter also work. In Credits, the up/down arrows or the mouse wheel scroll the text.
- **T** (X-62, Nov 17): during the guided tour the mouse does not move the camera. Not compiled in VS 2022 when this note was written.

## 2.2 Keys without a function

Since X-61 (Nov 17) the code no longer has key **L** (light cubes) or the debug keys for the cross spotlight (`T G Y H U J` and `1`–`6`). The space bar does nothing either. Only the keys in the table above work.

## 2.3 Other behavior

| Task | Control | Status |
|---|---|---|
| C-7 (cards) | None: the card appears by itself within **2 m** of the object | Implemented (see section 4); not yet compiled with ImGui in VS |
| D-2 / D-4 (silhouette) | Starts by itself when the program opens; **R** restarts it | Implemented (D-5, merged Nov 12); D2 still has to accept it when seeing it |

> 📷 Screenshot pending: keyboard diagram with the keys highlighted (made in GIMP; no outside image is used without credit).

## 2.4 What V and F1 look like

Altar candles turned off with **V** (at the top, the notice to turn them back on):

![Altar candles off](../capturas/02_velas_apagadas_V.jpg)

The controls list shown by **F1**:

![Controls help with F1](../capturas/02_ayuda_F1.jpg)

*Screenshots of Nov 17, 2026 on D2's PC (using the capture mode variables `NIERIKA_UI=velas_apagadas` and `NIERIKA_UI=ayuda`; the game shows the same when you press the keys).*

The same list on the start menu (**Controles** button):

![Controls in the menu](../capturas/01_menu_controles.jpg)

*Screenshot of Nov 17, 2026 on Linux (X-63); not yet seen on D2's PC.*
