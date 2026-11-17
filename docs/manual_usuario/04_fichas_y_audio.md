# 4. Fichas culturales y audio

## 4.1 Fichas culturales (C-7, X-52)
Son **13**, hechas con Dear ImGui (las 5 de la propuesta y 8 más que se agregaron el 17/11, X-52, para lo que se sumó al recorrido).
Aparecen **solas** en la parte baja de la ventana cuando estás a **menos de 2 m** (en el suelo, en horizontal) de un punto de la ficha; si estás cerca de dos, sale **la más cercana**.
Se van cuando te alejas. No se pueden cerrar ni mover (el ratón no interactúa con ellas).
Cada ficha muestra: el título, un texto breve, el **nivel de evidencia** (tradición general, local documentado o adaptación del estudio) y las **referencias en APA 7**.

| Ficha | Dónde (m, mundo; x, z) |
|---|---|
| Velas | orilla, (−3, 23) |
| Flor de cempasúchil | orilla, (3, 23) |
| Papel picado | panteón, bajo los postes, (0, 6) |
| Altar de ofrenda | cuarto, frente al altar, (0, −0.1) |
| Pan de muerto | cuarto, sobre el altar, (0, −0.75) |
| Banda de viento | frente a la banda, (12.6, 10.4) y (15.4, 10.4) |
| Copal y copalero | cuarto, junto a cada copalero, (−1.05, 1.75) y (1.05, 1.75) |
| Panteón y tumbas | al pie de las tumbas del panteón (cuatro puntos, z entre 5.75 y 10.6) |
| Ofrendas de las familias | frente a cada una de las 14 ofrendas de la playa |
| Comparsa en la orilla | orilla oeste, (−12, 22) |
| Camino de las ánimas | sendero norte, (0.5, −28) |
| Mariposas monarca | (0, −37.5) |
| Tortugas de Mazunte | orilla este, (21, 22.4) |

*(Según `src/Fichas.cpp` de `develop` del 17/11. Los textos son los de `docs/fichas_culturales.md`; verificados en pantalla en la PC de D2 solo los de papel picado, altar y copal, vistos en las capturas del 17/11; los demás **no se vieron al preparar este manual**. Las referencias de las 8 fichas nuevas y la ficha «Copal» no se contrastaron aquí con su fuente.)*

![Ficha del altar (cerca del altar sale la del copal)](capturas/03_parada5_cuarto_altar.jpg)

## 4.2 Audio (D-6, X-40, X-60)
Suenan dos pistas **en bucle**, cuyo volumen cambia con tu posición (`src/Audio.cpp`):
- **Oleaje:** crece al acercarte al mar, de 0.02 (z ≤ 0) a 0.30 (en la orilla, z = 24).
- **Música:** la fuente es la **banda de viento**. Se oye a tope (0.24) a 3 m o menos de ella y baja con la distancia (−6 dB cada vez que la distancia se duplica); a 60 m ya no se oye. Se mueve entre los parlantes según hacia dónde mires.

La tecla **M** cambia entre «mar y música», «solo el mar», «solo la música» y «silencio» (hay aviso arriba). Si el equipo no tiene salida de audio o faltan los `.mp3` de `Audio/`, el programa avisa en la consola y sigue sin sonido.
**No se pudo comprobar cómo suena** al preparar las capturas (solo se vio la pantalla).
