<#
.SYNOPSIS
    Empaqueta el Release x64 de Nierika Studio en dist/ y lo comprime en un .zip (tarea E-3).

.DESCRIPTION
    1. Compila NierikaStudio.sln en Release | x64 (se puede omitir con -SinCompilar).
    2. Crea dist/NierikaStudio/ con: NierikaStudio.exe, las DLL que haya junto al .exe,
       Models/, Shaders/ y Audio/ (si existe).
    3. Comprime esa carpeta en dist/NierikaStudio_<version>.zip.

    dist/ esta en .gitignore: el .zip NO se sube a Git, se adjunta al Release de GitHub (E-7).
    Los .blend (fuente de Blender) se dejan fuera del paquete: el programa no los carga.

.PARAMETER SinCompilar
    No ejecuta msbuild; usa lo que ya haya en build\x64\Release\.

.PARAMETER Version
    Texto para el nombre del .zip. Por defecto, la fecha (aaaa-MM-dd).

.PARAMETER IncluirRuntimeVC
    Copia tambien las DLL del runtime de Visual C++ (vcruntime140*.dll, msvcp140*.dll)
    desde el Visual Studio instalado, para PC sin el "Visual C++ Redistributable".

.EXAMPLE
    # Desde la raiz del repo (Developer PowerShell for VS 2022):
    powershell -ExecutionPolicy Bypass -File tools\empaquetar_release.ps1
#>
param(
    [switch]$SinCompilar,
    [string]$Version = (Get-Date -Format 'yyyy-MM-dd'),
    [switch]$IncluirRuntimeVC
)

$ErrorActionPreference = 'Stop'

# Raiz del repo = carpeta padre de tools\ (rutas siempre relativas a ella)
$raiz     = Split-Path -Parent $PSScriptRoot
$sln      = Join-Path $raiz 'NierikaStudio.sln'
$salida   = Join-Path $raiz 'build\x64\Release'      # OutDir del .vcxproj
$exeNombre = 'NierikaStudio.exe'                      # TargetName del .vcxproj
$dist     = Join-Path $raiz 'dist'
$paquete  = Join-Path $dist 'NierikaStudio'
$zip      = Join-Path $dist "NierikaStudio_$Version.zip"

function Paso($texto) { Write-Host "`n== $texto" -ForegroundColor Cyan }

# --- 1. Compilar -----------------------------------------------------------
if (-not $SinCompilar) {
    Paso 'Compilando Release | x64'
    $msbuild = (Get-Command msbuild -ErrorAction SilentlyContinue).Source
    if (-not $msbuild) {
        $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
        if (Test-Path $vswhere) {
            $msbuild = & $vswhere -latest -requires Microsoft.Component.MSBuild `
                -find 'MSBuild\**\Bin\MSBuild.exe' | Select-Object -First 1
        }
    }
    if (-not $msbuild) {
        throw 'No se encontro msbuild. Abre "Developer PowerShell for VS 2022" o usa -SinCompilar.'
    }
    & $msbuild $sln /p:Configuration=Release /p:Platform=x64 /m /v:minimal
    if ($LASTEXITCODE -ne 0) { throw "msbuild fallo (codigo $LASTEXITCODE). No se genera el paquete." }
}

# --- 2. Comprobar la salida -----------------------------------------------
Paso 'Comprobando build\x64\Release'
$exe = Join-Path $salida $exeNombre
if (-not (Test-Path $exe)) {
    throw "No existe $exe. Compila primero (Release | x64) o quita -SinCompilar."
}

# --- 3. Armar dist\NierikaStudio\ -----------------------------------------
Paso 'Armando dist\NierikaStudio'
if (Test-Path $paquete) { Remove-Item $paquete -Recurse -Force }
New-Item -ItemType Directory -Path $paquete | Out-Null

Copy-Item $exe $paquete

$dlls = Get-ChildItem $salida -Filter '*.dll' -File
if ($dlls.Count -eq 0) {
    Write-Warning ('No hay .dll junto al .exe en build\x64\Release. Si vcpkg usa el triplete dinamico ' +
                   '(x64-windows) deberian estar ahi; si no, revisa el triplete. Se sigue, pero el ' +
                   'paquete podria no correr en otra PC.')
} else {
    $dlls | Copy-Item -Destination $paquete
}

if ($IncluirRuntimeVC) {
    $redist = $env:VCToolsRedistDir
    if (-not $redist) {
        Write-Warning 'VCToolsRedistDir no esta definida (usa Developer PowerShell). No se copia el runtime de VC++.'
    } else {
        $rt = Join-Path $redist 'x64\Microsoft.VC*.CRT'
        Get-ChildItem (Join-Path $rt '*.dll') -ErrorAction SilentlyContinue | Copy-Item -Destination $paquete
    }
}

# Carpetas de recursos; los .blend (fuente de Blender) no se empaquetan
foreach ($carpeta in 'Models', 'Shaders', 'Audio') {
    $origen = Join-Path $raiz $carpeta
    if (-not (Test-Path $origen)) {
        Write-Warning "No existe la carpeta $carpeta\ (se omite)."
        continue
    }
    Copy-Item $origen (Join-Path $paquete $carpeta) -Recurse
}
Get-ChildItem (Join-Path $paquete 'Models') -Recurse -Include '*.blend', '*.blend1' -ErrorAction SilentlyContinue |
    Remove-Item -Force

# --- 4. Comprimir ---------------------------------------------------------
Paso "Creando $(Split-Path -Leaf $zip)"
if (Test-Path $zip) { Remove-Item $zip -Force }
Compress-Archive -Path (Join-Path $paquete '*') -DestinationPath $zip

# --- 5. Resumen -----------------------------------------------------------
$tam = '{0:N1}' -f ((Get-Item $zip).Length / 1MB)
Write-Host "`nListo." -ForegroundColor Green
Write-Host "  Carpeta : $paquete"
Write-Host "  Zip     : $zip ($tam MB)"
Write-Host "  DLL     : $($dlls.Count) copiadas"
Write-Host "`nPrueba pendiente (criterio de E-3): descomprimir el .zip en OTRA PC sin Visual Studio y ejecutar $exeNombre."
