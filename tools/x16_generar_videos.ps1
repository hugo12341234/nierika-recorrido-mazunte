<#
.SYNOPSIS
    X-16: genera los 3 videos BORRADOR (sin narracion) con el modo video de NierikaStudio.exe + ffmpeg.
.DESCRIPTION
    Para cada ruta de tools/video_rutas/*.txt: corre el programa con reloj fijo (NIERIKA_VIDEO), guarda los cuadros PNG en
    dist/videos/<nombre>/ y los une con ffmpeg a dist/videos/<nombre>.mp4 con el oleaje y la musica del propio programa.
    Son borradores generados automaticamente (sin voz); el equipo puede regrabarlos con OBS siguiendo docs/videos/.
.PARAMETER Ffmpeg
    Ruta de ffmpeg.exe (por defecto el que este en el PATH).
.PARAMETER Fps
    Cuadros por segundo (30 por defecto).
.EXAMPLE
    powershell -ExecutionPolicy Bypass -File tools\x16_generar_videos.ps1 -Ffmpeg C:\ruta\ffmpeg.exe
#>
param([string]$Ffmpeg = "ffmpeg", [int]$Fps = 30)
$ErrorActionPreference = 'Stop'
$raiz = Split-Path -Parent $PSScriptRoot
$exe = Join-Path $raiz 'build\x64\Release\NierikaStudio.exe'
$salida = Join-Path $raiz 'dist\videos'
New-Item -ItemType Directory -Force $salida | Out-Null
foreach ($ruta in Get-ChildItem (Join-Path $raiz 'tools\video_rutas') -Filter *.txt) {
    $nombre = $ruta.BaseName
    $cuadros = Join-Path $salida $nombre
    Remove-Item $cuadros -Recurse -Force -ErrorAction SilentlyContinue
    New-Item -ItemType Directory -Force $cuadros | Out-Null
    Write-Host "== $nombre : cuadros"
    $env:NIERIKA_VIDEO = "$cuadros;$Fps;$($ruta.FullName)"
    Start-Process $exe -WorkingDirectory $raiz -Wait
    Remove-Item Env:NIERIKA_VIDEO
    Write-Host "== $nombre : ffmpeg"
    & $Ffmpeg -y -framerate $Fps -i (Join-Path $cuadros 'frame_%05d.png') -stream_loop -1 -i (Join-Path $raiz 'Audio\oleaje.wav') `
        -stream_loop -1 -i (Join-Path $raiz 'Audio\musica.wav') `
        -filter_complex "[1:a]volume=0.7[o];[2:a]volume=0.25[m];[o][m]amix=inputs=2:duration=longest[a]" `
        -map 0:v -map "[a]" -c:v libx264 -pix_fmt yuv420p -crf 18 -c:a aac -shortest (Join-Path $salida "$nombre.mp4")
}
Write-Host "Listo: $salida"
