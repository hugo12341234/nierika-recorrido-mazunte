# 4. Cultural cards and audio

## 4.1 Cultural cards (C-7, X-52)
There are **13**, made with Dear ImGui (the 5 from the proposal and 8 more added on Nov 17, X-52, for what was added to the tour). They appear **by themselves** at the bottom of the window when you are within **2 m** (on the ground, horizontally) of a point of the card; if you are near two, **the nearest** shows. They disappear when you walk away. They cannot be closed or moved (the mouse does not interact with them). Each card shows: the title, a short text, the **evidence level** (general tradition, documented local, or studio adaptation) and the **APA 7 references**.

| Card | Where (m, world; x, z) |
|---|---|
| Candles | shore, (−3, 23) |
| Marigold flower | shore, (3, 23) |
| Papel picado | cemetery, under the posts, (0, 6) |
| Offering altar | room, in front of the altar, (0, −0.1) |
| Pan de muerto | room, on the altar, (0, −0.75) |
| Wind band | in front of the band, (12.6, 10.4) and (15.4, 10.4) |
| Copal and copal burner | room, next to each burner, (−1.05, 1.75) and (1.05, 1.75) |
| Cemetery and tombs | at the foot of the cemetery tombs (four points, z between 5.75 and 10.6) |
| Family offerings | in front of each of the 14 beach offerings |
| Comparsa on the shore | west shore, (−12, 22) |
| Path of the souls | north path, (0.5, −28) |
| Monarch butterflies | (0, −37.5) |
| Mazunte turtles | east shore, (21, 22.4) |

*(According to `src/Fichas.cpp` in `develop` of Nov 17. Texts are those of `docs/fichas_culturales.md`; only the papel picado, altar and copal cards were seen on D2's PC (in the Nov 17 screenshots); the others **were not seen when preparing this manual**. The references of the 8 new cards and the copal card were not checked here against their source.)*

![Altar card (near the altar the copal card shows)](../capturas/03_parada5_cuarto_altar.jpg)

## 4.2 Audio (D-6, X-40, X-60)
Two **looping** tracks play, and their volume changes with your position (`src/Audio.cpp`):
- **Waves:** grow as you approach the sea, from 0.02 (z ≤ 0) to 0.30 (at the shore, z = 24).
- **Music:** the source is the **wind band**. It is loudest (0.24) at 3 m or less from it and drops with distance (−6 dB each time the distance doubles); at 60 m it is silent. It pans between the speakers depending on where you look.

Key **M** cycles through "sea and music", "sea only", "music only" and "silence" (a notice appears at the top). If the machine has no audio output or the `.mp3` files in `Audio/` are missing, the program prints a message to the console and runs without sound.
**How it sounds could not be checked** when preparing the screenshots (only the screen was seen).
