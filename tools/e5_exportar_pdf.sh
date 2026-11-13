#!/usr/bin/env bash
# Genera docs/dist_manuales/Manual_tecnico_<idioma>.pdf y Manual_usuario_<idioma>.pdf
# Requiere: pandoc y LibreOffice (soffice). Uso: bash tools/e5_exportar_pdf.sh
# Los PDF NO se versionan (salida en docs/dist_manuales/, ignorada por git).
set -euo pipefail
raiz="$(cd "$(dirname "$0")/.." && pwd)"
sal="$raiz/docs/dist_manuales"; mkdir -p "$sal"

unir() { # $1 = nombre de salida, $2 = carpeta, resto = archivos en orden
  local nombre="$1" dir="$2"; shift 2
  local args=()
  for f in "$@"; do args+=("$dir/$f"); done
  # --resource-path para que las imágenes relativas se encuentren
  pandoc "${args[@]}" -f gfm -t docx --resource-path="$dir" -o "$sal/$nombre.docx"
  soffice --headless --convert-to pdf --outdir "$sal" "$sal/$nombre.docx" >/dev/null
  rm -f "$sal/$nombre.docx"
  echo "Listo: $sal/$nombre.pdf"
}

mt="$raiz/docs/manual_tecnico"; mu="$raiz/docs/manual_usuario"
unir Manual_tecnico_ES "$mt" 01_introduccion.md 02_arquitectura.md 03_herramientas.md 04_cronograma.md 05_raci.md 06_modelos_y_recursos.md 07_justificacion_matematica.md justificacion_simples.md justificacion_complejas.md 08_pruebas_y_rendimiento.md 09_conclusiones.md 10_evidencias.md 11_referencias.md
[ -d "$mt/en" ] && unir Technical_Manual_EN "$mt/en" 01_introduction.md 02_architecture.md 03_tools.md 04_schedule.md 05_raci.md 06_models_and_resources.md 07_math_justification.md 08_testing_and_performance.md 09_conclusions.md 10_evidence.md 11_references.md
unir Manual_usuario_ES "$mu" 01_inicio.md 02_controles.md 03_recorrido.md 04_fichas_y_audio.md 05_problemas_frecuentes.md
[ -d "$mu/en" ] && unir User_Manual_EN "$mu/en" 01_start.md 02_controls.md 03_tour.md 04_cards_and_audio.md 05_troubleshooting.md
