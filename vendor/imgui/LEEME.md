# Dear ImGui en `vendor/imgui/` (C-7b)

Código fuente de Dear ImGui **v1.91.9** (MIT, https://github.com/ocornut/imgui), copiado tal cual el 11/11 (ver `LICENSE.txt`).
Solo se incluyen los archivos que necesita el proyecto: núcleo (`imgui*.cpp/.h`, `imstb_*.h`, `imconfig.h`) y los backends
`backends/imgui_impl_glfw.*` e `backends/imgui_impl_opengl3.*` (con su `imgui_impl_opengl3_loader.h`).

`NierikaStudio.vcxproj` compila los 6 `.cpp` y `src/Fichas.cpp` usa `imgui/imgui.h` y `imgui/backends/...`.
Para actualizar la versión: reemplaza esos mismos archivos por los del nuevo release y anota la versión en `docs/ESTADO_PROYECTO.md`.
