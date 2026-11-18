# Dimensiones verificadas (1 unidad = 1 metro) — despues

> Generado por `tools/x32_dimensiones.py` el 13/11/2026: mide la caja envolvente de cada `.obj` y la multiplica por la escala con que `src/main.cpp` lo dibuja.
> **Las medidas esperadas son referencias típicas o indicadas por el equipo, no mediciones hechas en Mazunte** (la columna «Fuente» lo dice). Los rangos son de aceptación, no verdades exactas.

| Modelo | Eje | Esperado (m) | Medida real en escena (m) | Escala en escena | Resultado | Fuente de la medida esperada |
|---|---|---|---|---|---|---|
| `persona_pie_a` | y | 1.55 – 1.80 | 1.76 | 1.00 | OK | estatura adulta tipica ~1.6-1.75 m (indicacion de D2: ~1.6 m) |
| `persona_pie_b` | y | 1.55 – 1.80 | 1.76 | 1.00 | OK | estatura adulta tipica ~1.6-1.75 m |
| `persona_agachada_a` | y | 0.90 – 1.20 | 1.14 | 1.00 | OK | persona de rodillas: ~60 % de la estatura |
| `silueta/torso` | y | 0.50 – 0.70 | 0.58 | 1.00 | OK | torso de adulto ~0.55-0.65 m |
| `casa_a` | y | 3.00 – 4.20 | 3.80 | 1.00 | OK | casa de un piso: muros ~2.6 m y techo a dos aguas hasta ~3.8 m (indicacion de D2: ~3 m de alto) |
| `casa_b` | y | 3.00 – 4.20 | 3.80 | 1.00 | OK | igual que casa_a |
| `palapa` | y | 3.50 – 5.00 | 4.60 | 1.00 | OK | palapa de playa: postes de ~2.8 m y techo hasta ~4.4 m |
| `lancha` | z | 5.50 – 7.00 | 6.27 | 1.00 | OK | panga de pescador ~6 m (indicacion de D2: ~6 m) |
| `palma` | y | 8.00 – 12.00 | 8.72 | 1.00 | OK | palma de coco adulta 8-12 m (indicacion de D2) |
| `arbusto` | y | 0.40 – 1.00 | 0.45 | 1.00 | OK | agave/arbusto bajo de ~0.5-0.8 m |
| `tortuga` | z | 0.55 – 0.80 | 0.79 | 0.85 | OK | tortuga golfina adulta: caparazon de ~0.6-0.7 m (referencia general, por verificar) |
| `ave_ala` | x | 0.90 – 1.30 | 1.14 | 1.20 | OK | una ala de pelicano/fragata ~1.0-1.2 m (envergadura 2-2.5 m, referencia general) |
| `murcielago_ala` | x | 0.35 – 0.60 | 0.44 | 1.00 | OK | un ala de murcielago ~0.2-0.3 m (se exagera para que se vea de lejos) |
| `tabla_remo` | z | 3.00 – 3.60 | 3.20 | 1.00 | OK | tabla de remo (SUP) 3.0-3.6 m |
| `farol` | y | 1.50 – 1.90 | 1.72 | 1.00 | OK | farol de calle o de jardin ~1.6-1.8 m |
| `guitarra` | y | 0.80 – 1.05 | 1.05 | 1.00 | OK | guitarra clasica ~1.0 m |
| `tambor` | x | 0.35 – 0.50 | 0.44 | 1.00 | OK | tambor de marcha ~0.4 m de diametro |
| `trompeta` | z | 0.45 – 0.65 | 0.58 | 1.00 | OK | trompeta ~0.5 m |
| `hamaca` | x | 3.00 – 3.60 | 3.29 | 1.00 | OK | hamaca con postes: ~3.2 m |
| `fogata` | x | 0.80 – 1.20 | 1.04 | 1.00 | OK | anillo de fogata ~1 m |
| `vela` | y | 0.12 – 0.20 | 0.15 | 1.00 | OK | vela de ofrenda ~0.15 m |
| `altar3` | y | 1.10 – 1.30 | 1.20 | 1.00 | OK | altar de tres niveles de ~1.2 m |
| `cruz` | y | 1.30 – 1.70 | 1.50 | 1.00 | OK | cruz de entrada ~1.5 m |
| `barda` | y | 0.60 – 1.00 | 0.70 | 1.00 | OK | barda baja de ~0.7 m |
| `fachada` | y | 2.50 – 3.50 | 3.28 | 1.00 | OK | cuarto: muros de ~2.6 m |
| `arco` | ymax | 2.00 – 2.40 | 2.29 | 1.00 | OK | cima del arco de entrada = alto de puerta ~2.1 m (+ grosor de las dovelas); indicacion de D2: puerta ~2.1 m |
| `botella` | y | 0.25 – 0.35 | 0.30 | 1.00 | OK | botella de ~0.3 m |
| `marco` | y | 0.25 – 0.45 | 0.25 | 1.00 | OK | marco de foto ~0.3 m |

**Resumen:** 28 modelos revisados, 0 fuera de rango.
