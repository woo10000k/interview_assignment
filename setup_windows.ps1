# Assignment Environment Setup (Windows)
# Run: powershell -ExecutionPolicy Bypass -File setup_windows.ps1

$ErrorActionPreference = "Stop"
$ProjectRoot = $PSScriptRoot

Write-Host "======================================"
Write-Host "Assignment Environment Setup (Windows)"
Write-Host "======================================"
Write-Host ""

# Helper function to check command exists
function Test-CommandExists {
    param($Command, $TestPath)

    # First try the command directly
    try {
        $null = & $Command --version 2>$null
        if ($LASTEXITCODE -eq 0 -or $?) { return $true }
    } catch {}

    # Then try the specific path
    if ($TestPath -and (Test-Path $TestPath)) {
        return $true
    }

    return $false
}

# ============================================
# [1/5] CMake
# ============================================
Write-Host "[1/5] Checking CMake..."

$CmakePath = "C:\Program Files\CMake\bin\cmake.exe"
$CmakeFound = $false

if (Test-Path $CmakePath) {
    $CmakeFound = $true
    Write-Host "[OK] CMake found at $CmakePath"
} elseif (Get-Command cmake -ErrorAction SilentlyContinue) {
    $CmakePath = (Get-Command cmake).Source
    $CmakeFound = $true
    Write-Host "[OK] CMake found at $CmakePath"
}

if (-not $CmakeFound) {
    Write-Host "Installing CMake via winget..."
    winget install --id Kitware.CMake -e --silent --accept-package-agreements --accept-source-agreements
    $CmakePath = "C:\Program Files\CMake\bin\cmake.exe"
    if (-not (Test-Path $CmakePath)) {
        Write-Host "[ERROR] CMake installation failed"
        exit 1
    }
    Write-Host "[OK] CMake installed"
}

# ============================================
# [2/5] MSYS2 + MinGW
# ============================================
Write-Host ""
Write-Host "[2/5] Checking MinGW compiler..."

$Msys2Root = "C:\msys64"
$GppPath = "$Msys2Root\ucrt64\bin\g++.exe"
$MingwMakePath = "$Msys2Root\ucrt64\bin\mingw32-make.exe"
$GppFound = $false

if (Test-Path $GppPath) {
    $GppFound = $true
    Write-Host "[OK] MinGW g++ found at $GppPath"
}

if (-not $GppFound) {
    # Check if MSYS2 is installed
    if (-not (Test-Path $Msys2Root)) {
        Write-Host "Installing MSYS2 via winget..."
        winget install --id MSYS2.MSYS2 -e --silent --accept-package-agreements --accept-source-agreements
        if (-not (Test-Path $Msys2Root)) {
            Write-Host "[ERROR] MSYS2 installation failed"
            exit 1
        }
        Write-Host "[OK] MSYS2 installed"
    }

    # Install MinGW via pacman
    Write-Host "Installing MinGW compiler via MSYS2 pacman..."
    & "$Msys2Root\usr\bin\pacman.exe" -S --noconfirm mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make

    if (-not (Test-Path $GppPath)) {
        Write-Host "[ERROR] MinGW installation failed"
        exit 1
    }
    Write-Host "[OK] MinGW installed"
}

# Set PATH for this session
$env:PATH = "$Msys2Root\ucrt64\bin;C:\Program Files\CMake\bin;$env:PATH"

# ============================================
# [3/5] Build C++ Projects
# ============================================
Write-Host ""
Write-Host "[3/5] Building C++ projects..."

$Problems = @(
    @{Name="Problem 1: LogFileManager"; Path="problem1-log-file-manager"},
    @{Name="Problem 2: CircularBuffer"; Path="problem2-circular-buffer"},
    @{Name="Problem 3: ParallelProcessor"; Path="problem3-parallel-processor"}
)

foreach ($Problem in $Problems) {
    Write-Host ""
    Write-Host "Building $($Problem.Name)..."

    $ProblemPath = Join-Path $ProjectRoot $Problem.Path
    $BuildPath = Join-Path $ProblemPath "build"

    # Clean and create build directory
    if (Test-Path $BuildPath) {
        Remove-Item -Recurse -Force $BuildPath
    }
    New-Item -ItemType Directory -Path $BuildPath | Out-Null

    # Run CMake
    Push-Location $BuildPath
    try {
        & $CmakePath .. -G "MinGW Makefiles"
        if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed" }

        & $CmakePath --build .
        if ($LASTEXITCODE -ne 0) { throw "Build failed" }

        Write-Host "[OK] $($Problem.Name) build complete"
    } catch {
        Write-Host "[ERROR] $($Problem.Name) build failed: $_"
        Pop-Location
        exit 1
    }
    Pop-Location
}

# ============================================
# [4/5] Python
# ============================================
Write-Host ""
Write-Host "[4/5] Checking Python..."

$PythonFound = $false
$PythonCmd = "python"

# Check various Python locations
$PythonPaths = @(
    "$env:LOCALAPPDATA\Programs\Python\Python311\python.exe",
    "$env:LOCALAPPDATA\Programs\Python\Python312\python.exe",
    "$env:LOCALAPPDATA\Programs\Python\Python310\python.exe",
    "C:\Python311\python.exe",
    "C:\Python312\python.exe"
)

foreach ($pp in $PythonPaths) {
    if (Test-Path $pp) {
        $PythonCmd = $pp
        $PythonFound = $true
        Write-Host "[OK] Python found at $pp"
        break
    }
}

if (-not $PythonFound) {
    try {
        $null = python --version 2>$null
        if ($LASTEXITCODE -eq 0) {
            $PythonFound = $true
            Write-Host "[OK] Python found in PATH"
        }
    } catch {}
}

if (-not $PythonFound) {
    Write-Host "Installing Python 3.11 via winget..."
    winget install --id Python.Python.3.11 -e --silent --accept-package-agreements --accept-source-agreements

    $PythonCmd = "$env:LOCALAPPDATA\Programs\Python\Python311\python.exe"
    if (-not (Test-Path $PythonCmd)) {
        Write-Host "[ERROR] Python installation failed"
        exit 1
    }
    Write-Host "[OK] Python installed"
}

# ============================================
# [5/5] FastAPI Environment
# ============================================
Write-Host ""
Write-Host "[5/5] Setting up FastAPI environment..."

$Problem4Path = Join-Path $ProjectRoot "problem4-restful-api-server"
Push-Location $Problem4Path

try {
    # Check uv
    $UvPath = "$env:USERPROFILE\.local\bin\uv.exe"
    $UvFound = $false

    if (Test-Path $UvPath) {
        $UvFound = $true
        Write-Host "[OK] uv found at $UvPath"
    } elseif (Get-Command uv -ErrorAction SilentlyContinue) {
        $UvPath = "uv"
        $UvFound = $true
        Write-Host "[OK] uv found in PATH"
    }

    if (-not $UvFound) {
        Write-Host "Installing uv..."
        Invoke-RestMethod https://astral.sh/uv/install.ps1 | Invoke-Expression
        $UvPath = "$env:USERPROFILE\.local\bin\uv.exe"
        if (-not (Test-Path $UvPath)) {
            Write-Host "[ERROR] uv installation failed"
            exit 1
        }
        Write-Host "[OK] uv installed"
    }

    # Create venv
    Write-Host "Creating virtual environment..."
    if (Test-Path ".venv") {
        Remove-Item -Recurse -Force ".venv"
    }
    & $UvPath venv --python 3.11

    # Install dependencies
    Write-Host "Installing dependencies..."
    & $UvPath pip install -r requirements.txt

    Write-Host "[OK] FastAPI environment setup complete"
} catch {
    Write-Host "[ERROR] FastAPI setup failed: $_"
    Pop-Location
    exit 1
}
Pop-Location

# ============================================
# Done
# ============================================
Write-Host ""
Write-Host "======================================"
Write-Host "[OK] All environment setup and build complete!"
Write-Host "======================================"
Write-Host ""
Write-Host "How to run:"
Write-Host "1. cd problem1-log-file-manager\build"
Write-Host "   .\log_file_manager_test.exe"
Write-Host "2. cd problem2-circular-buffer\build"
Write-Host "   .\circular_buffer_test.exe"
Write-Host "3. cd problem3-parallel-processor\build"
Write-Host "   .\parallel_processor_test.exe"
Write-Host "4. cd problem4-restful-api-server"
Write-Host "   .\.venv\Scripts\activate"
Write-Host "   python -m uvicorn app.main:app --reload"
Write-Host ""
Write-Host "Press any key to exit..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
