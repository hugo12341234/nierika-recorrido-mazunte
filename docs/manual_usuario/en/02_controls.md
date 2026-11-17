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
| See where the lights are | **L** | Shows or hides the white cubes that mark the lights |
| Restart the silhouette | **R** | The silhouette returns to the shore and repeats its walk and the flower |
| Quit | **Esc** | Closes the program |

Notes verified in the code (`Camera.h`, `Colisiones.cpp`, `main.cpp`):
- Walking speed is **2.0 m/s** (`SPEED` in `Camera.h`); mouse sensitivity is **0.25**.
- You walk **only on the horizontal plane**: looking up or down does not make you fly; your eyes are always **1.6 m** above the ground (which slopes down at the shore).
- There are **collisions** (A-9): you cannot pass through walls, the cemetery wall or the altar; if you hit one head-on you slide along it. The walkable area reaches the shore (z = 24).
- At start the altar candles are **on**. Near the altar a reminder for **V** appears at the top.
- Since X-60 (Nov 17) the **space bar does nothing**: the colored lights were removed (ESTADO sec. 4, Nov 17).
- Esc closes the program directly, with no confirmation.

> **Questions to review with D2 before publishing** (not confirmed in the code):
> 1. Key **L** is described in the code as a "debug aid". Does it stay for the visitor or go before delivery?

## 2.2 Keys that do NOT go in the manual

These keys exist in the code but are for **debugging** (they move and aim the cross spotlight) and should leave before delivery, so they are **not documented for the visitor**. **E-2 did not remove them**: they are still in `main.cpp` (lines 643–654 on Nov 12) and the decision to remove them is **pending with D2**:

`T G Y H U J` (spotlight position) · `1 2 3 4 5 6` (spotlight direction)

If they do not leave, they must be added here.

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

*Screenshots of Nov 17, 2026 on D2's PC (using the capture mode variables `NIERIKA_UI=velas_apagadas` and `NIERIKA_UI=ayuda`; the game shows the same when you press the keys). The F1 help also lists **L** and does not list the debug keys.*
