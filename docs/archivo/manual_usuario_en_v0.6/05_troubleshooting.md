# 5. Troubleshooting

| Symptom | Likely cause | What to do | Status |
|---|---|---|---|
| The program does not open and asks for DLLs | A library is missing next to the `.exe` | Use the complete `dist/` package | **Pending** check on another PC (E-3) |
| Black window or closes on start | Graphics without OpenGL 3.3 support | Update video drivers | Plausible, **untested** |
| Controls do not respond | The window lacks focus | Click on the window | Plausible, **untested** |
| I cannot get through a wall | There are collisions (A-9): the cemetery wall and room walls are solid | Use the gates: the wall has two 2 m gates (north and south); the room has one in front | **Real** |
| No sound | Machine without audio output or without the `Audio/` files | Check the console: it says which track is missing | **Real** (`Audio.cpp` behavior) |
| The cursor does not show | Intentional (first-person camera) | Quit with Esc | **Real** |
| The silhouette finished and I want to see it again | Its animation runs once | Key **R** | **Real** |
| It runs slowly | Machine without enough graphics; per-pixel lighting weighs most | Measured in E-1; no figures on integrated graphics yet | **Pending** |

> "Untested" rows are confirmed or removed during E-1 and E-3 tests. They are not published as facts before then.
