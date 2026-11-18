# Manual técnico — Nierika Studio (E-5)

> **Proyecto:** Recorrido virtual interactivo 3D — Día de Muertos en Mazunte, Oaxaca · **Equipo 15** · 319266894 (D1) · 321332086 (D2)
> **Versión:** 1.0 (18/11/2026, hora de México). **PDF de entrega:** `docs/entrega/ManualTecnico_Equipo15_319266894_321332086.pdf`

## Archivos (en el orden del PDF)

| Archivo | Contenido (sección de la rúbrica) |
|---|---|
| `00_resumen.md` | Resumen, cifras y mapa de la rúbrica |
| `01_a_arquitectura.md` | a) Arquitectura |
| `02_b_investigacion_cultural.md` | b) Investigación cultural |
| `03_c_tecnologias.md` | c) Tecnologías |
| `04_d_cronograma.md` | d) Cronograma final contra el inicial |
| `05_e_roles_raci.md` + `secciones/roles_raci_real.md` | e) Roles y RACI real |
| `06_f_metodologia.md` + `secciones/evidencias_colaborativas.md` | f) Metodología con evidencias |
| `07_g_aprendizaje_autonomo.md` + `individual/D1_…`, `D2_…` | g) Aprendizaje autónomo por integrante |
| `08_h_herramientas_colaborativas.md` | h) Herramientas colaborativas |
| `09_i_animaciones.md` + `justificacion_*.md` | i) Animaciones complejas y su justificación matemática (anexos C, D, E) |
| `10_j_redaccion_individual.md` + `individual/D1_english.md`, `D2_english.md` | j) Redacción individual en inglés |
| `11_k_bibliografia.md` | k) Bibliografía APA 7 (de `docs/REFERENCIAS.md`) |
| `12_l_conclusiones.md` | l) Conclusiones |
| `13_m_creditos_licencias.md` | m) Créditos y licencias |
| `14_anexo_modelos.md`, `15_anexo_pruebas.md` | Anexos A y B |

## Textos de cada integrante
- `individual/D1_*` y `individual/D2_*`: los textos individuales finales (g.4, g.5, j.4 y j.5), cada uno de su integrante.
- `secciones/`: secciones sueltas de otros hilos. Si un archivo no existe, el PDF muestra un aviso de pendiente. `evidencias_colaborativas.md` se integra como f.4 y `roles_raci_real.md` como e.4 (sin sus notas internas).

## Cómo generar los PDF
Requisitos: python3, pandoc, poppler (`pdfunite`), Node.js con `playwright` y Chromium.
```
python3 tools/e5_figuras.py          # figuras SVG (arquitectura, ciclo de render, cronograma)
python3 tools/e5_exportar_pdf.py     # usuario | tecnico | todo (por omisión)
```
Salida en `docs/entrega/`. El resumen en inglés antiguo (v0.5) se movió a `docs/archivo/manual_tecnico_en_v0.5/` y **no forma parte de la entrega**.
