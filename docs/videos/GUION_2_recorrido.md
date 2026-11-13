# Video 2 — El recorrido (≈ 3:00–4:00)

> **Qué muestra:** una visita completa, parada por parada, tal como la haría un visitante (CONTEXTO §5, manual de usuario §3).
> **Verificado en el código:** posición de inicio (0, 1.6, 6) mirando hacia el cuarto (−z); orilla en z = 24; la silueta arranca sola en la orilla
> y camina 18.33 s hasta z ≈ 0.2, y luego deja la flor; **R** la reinicia; velocidad 2.0 m/s; barda con puertas norte y sur de 2 m.
> **Inferido:** los tiempos de cada tramo, y que la ficha de las velas y la del cempasúchil aparezcan a la primera (posiciones del manual de usuario §4; hay que verlo).

## Truco importante
La silueta empieza a caminar **al abrir el programa**, así que cuando llegues a la orilla ya habrá avanzado. Para filmarla desde el principio, **pulsa R** cuando estés en la orilla. La cámara va a 2.0 m/s y la silueta a 1.4 m/s: para seguirla sin adelantarla, **camina a ratos y detente**, o déjala pasar y mírala de frente.

## Ruta (una sola toma larga, o 5 tomas unidas)
| # | Parada | Qué hacer | Tiempo aprox. |
|---|---|---|---|
| 1 | Llegada a la playa | Abrir el programa; girar 180° con el ratón (en el inicio miras al cuarto) y caminar con W hacia el mar | 0:00–0:35 |
| 2 | Orilla | Detenerte cerca de z ≈ 23. Dejar ver olas, velas y flores; esperar la ficha de velas (≈ x −3) y la de cempasúchil (≈ x +3) | 0:35–1:15 |
| 3 | Sendero | **Pulsar R.** La silueta sale de la orilla; mirarla alejarse y empezar a seguirla hacia el panteón | 1:15–1:45 |
| 4 | Panteón | Entrar por la puerta de la barda (x entre −1 y +1); mostrar tumbas, cruz, roca, papel picado y su ficha | 1:45–2:40 |
| 5 | Cuarto | Entrar al cuarto, ver el altar, esperar a que la silueta llegue y suelte la flor; mostrar las fichas del altar y del pan de muerto | 2:40–3:40 |
| 6 | Cierre | Plano final del altar y cierre | 3:40–4:00 |

## Guion
| Tiempo | Imagen | Narración |
|---|---|---|
| 0:00 | Parada 1 | «Empezamos en la playa de Mazunte. Nos movemos con las teclas W, A, S, D y miramos con el ratón.» |
| 0:35 | Parada 2 | «En la orilla, las velas y las flores de cempasúchil flotan sobre el mar; así se despide a los difuntos en esta escena. Al acercarnos aparece la ficha cultural con su fuente.» |
| 1:15 | Parada 3 | «Una silueta sale de la orilla y camina hacia el panteón por el eje central. Es una animación por KeyFrames.» |
| 1:45 | Parada 4 | «El panteón tiene cuatro tumbas, una cruz y una barda baja de piedra con dos puertas. El papel picado cuelga entre dos postes.» |
| 2:15 | Ficha de papel picado | «Esta ficha explica el papel picado y cita su fuente en formato APA 7.» |
| 2:40 | Parada 5 | «Entramos al cuarto del cementerio, una adaptación del estudio. La ofrenda tiene tres niveles.» |
| 3:05 | Silueta con flor | «La silueta llega, extiende el brazo y deja una flor de cempasúchil en la repisa del altar.» |
| 3:30 | Ficha del altar / pan | «Las fichas del altar y del pan de muerto explican cada elemento de la ofrenda.» |
| 3:45 | Cierre | «Este fue el recorrido de Nierika Studio.» |

## Si algo falla en la toma
- Te atoras en un muro: retrocede con S y usa la puerta; las colisiones son sólidas (A-9).
- No sale una ficha: acércate a **menos de 2 m en horizontal** del punto (manual de usuario §4); si aun así no sale, anótalo como error para E-1 y **no lo narres como si funcionara**.
- La silueta ya terminó: **R**.

## Confirmar antes de publicar
- [ ] Que el texto de las narraciones culturales coincida con `docs/fichas_culturales.md` (ese archivo aún dice «por verificar contra la fuente»; no afirmes más de lo que dice la ficha).
- [ ] Que al grabar se vean puertas y colisiones sin atascos (E-2 pidió que D2 recorra el panteón con W/A/S/D).
