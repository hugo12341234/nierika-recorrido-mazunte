# Dear ImGui en `vendor/imgui/` (C-7b)

Aqui va el codigo fuente de Dear ImGui (MIT, https://github.com/ocornut/imgui). **Falta copiarlo**: el hilo de Claude
no pudo descargarlo (la red de la sesion solo alcanza este repositorio).

## Que copiar (version recomendada: v1.91.x, la ultima estable; anota la que uses en `docs/ESTADO_PROYECTO.md`)
Del release de ImGui, copia **tal cual** a esta carpeta:

```
vendor/imgui/imgui.h  imgui.cpp  imgui_draw.cpp  imgui_tables.cpp  imgui_widgets.cpp
vendor/imgui/imgui_internal.h  imconfig.h  imstb_rectpack.h  imstb_textedit.h  imstb_truetype.h
vendor/imgui/backends/imgui_impl_glfw.h  imgui_impl_glfw.cpp
vendor/imgui/backends/imgui_impl_opengl3.h  imgui_impl_opengl3.cpp  imgui_impl_opengl3_loader.h
```

No hace falta nada mas (ni `examples/`, ni `misc/`, ni `imgui_demo.cpp`). `NierikaStudio.vcxproj` ya compila esos
6 `.cpp` y `Fichas.cpp` usa `imgui/imgui.h` y `imgui/backends/...`.
