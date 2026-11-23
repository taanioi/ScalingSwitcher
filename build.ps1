$ErrorActionPreference = "Stop"

$root = $PSScriptRoot
if (-not $root) {
    $root = Split-Path -Parent $MyInvocation.MyCommand.Path
}

if (-not [Environment]::Is64BitOperatingSystem) {
    throw "Scaling Switcher requires 64-bit Windows."
}

$gpp     = Get-Command "g++.exe"     -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Source -First 1
$windres = Get-Command "windres.exe" -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Source -First 1

if (-not $gpp -or -not $windres) {
    $devkitBin     = Join-Path $env:LOCALAPPDATA "w64devkit\bin"
    $devkitGpp     = Join-Path $devkitBin "g++.exe"
    $devkitWindres = Join-Path $devkitBin "windres.exe"

    if ((Test-Path $devkitGpp) -and (Test-Path $devkitWindres)) {
        $env:PATH = "$devkitBin;$env:PATH"
        $gpp      = $devkitGpp
        $windres  = $devkitWindres
    } else {
        throw "MinGW-w64 toolchain (g++.exe and windres.exe) was not found in PATH or $devkitBin.`nInstall w64devkit (https://github.com/skeeto/w64devkit/releases) or MSYS2 MinGW-w64 and ensure its bin directory is in PATH."
    }
}

$srcDir  = Join-Path $root "src"
$mainCpp = Join-Path $srcDir "main.cpp"
$rcFile  = Join-Path $srcDir "app.rc"
$resObj  = Join-Path $env:TEMP ("scalingswitcher_res_{0}.o" -f $PID)
$output  = Join-Path $root "ScalingSwitcher.exe"

try {
    Write-Host "Compiling resources..."
    & $windres -I "$srcDir" "$rcFile" -o "$resObj"
    if ($LASTEXITCODE -ne 0) {
        throw "Resource compilation failed (windres exit code $LASTEXITCODE)."
    }

    Write-Host "Compiling Native C++ Scaling Switcher..."
    & $gpp -std=c++20 -Oz -s -Wall -Wextra -fno-exceptions -fno-rtti `
        -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-ident `
        -fno-tree-loop-distribute-patterns `
        -ffunction-sections -fmerge-all-constants `
        "-Wl,--gc-sections" "-Wl,--no-insert-timestamp" `
        -nostdlib "-Wl,-e,EntryPoint" -mwindows `
        "$mainCpp" "$resObj" `
        -o "$output" `
        -lgcc -luser32 -lkernel32

    if ($LASTEXITCODE -ne 0) {
        throw "Build failed (g++ exit code $LASTEXITCODE)."
    }
}
finally {
    Remove-Item $resObj -Force -ErrorAction SilentlyContinue
}

$size = (Get-Item $output).Length
Write-Host "Built: $output ($size bytes)"
