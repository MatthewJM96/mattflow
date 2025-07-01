# Save the original location at the start of the script
$originalLocation = Get-Location

# Function to restore the original location and exit
function Exit-Script {
    [CmdletBinding()]
    param (
        [int]$ExitCode = 0
    )
    Set-Location $originalLocation
    exit $ExitCode
}

# Trap for unexpected exits (e.g., Ctrl+C, exceptions)
trap {
    Exit-Script 1
}

# Show help information
function Show-Help {
    Write-Host "LLVM Build Script Usage:" -ForegroundColor Green
    Write-Host "  /Debug                Build debug configuration."
    Write-Host "  /Release              Build release configuration."
    Write-Host "  /Force                Overwrite existing build and install directories."
    Write-Host "  /Partial              Continue from last build, without overwriting."
    Write-Host "  /Jobs:N               Use N parallel jobs for building (default: total processors less 4)."
    Write-Host "  /Help or /?           Display this help information." -ForegroundColor Green
    Write-Host "`nDefault behavior: Build both Debug and Release if neither is specified."
    Write-Host "Examples:"
    Write-Host "  .\build.ps1 /Debug /Force /Jobs:8   # Build debug only with 8 jobs replacing any previous debug build"
    Write-Host "  .\build.ps1 /Release                # Build release only, if no release build currently exists"
    Write-Host "  .\build.ps1 /Help                   # Display this help"
}

# Parse command line arguments
$buildDebug = $false
$buildRelease = $false
$forceBuild = $false
$partialBuild = $false
$jobCount = -1

foreach ($arg in $args) {
    if ($arg -eq "/Debug") {
        $buildDebug = $true
    } elseif ($arg -eq "/Release") {
        $buildRelease = $true
    } elseif ($arg -eq "/Force") {
        $forceBuild = $true
    } elseif ($arg -eq "/Partial") {
        $partialBuild = $true
    } elseif ($arg -like "/Jobs:*") {
        # Parse the job count from the argument
        $jobValue = $arg -replace "^/Jobs:", ""
        [int]::TryParse($jobValue, [ref]$jobCount)
        if ($jobCount -le 0) {
            Write-Host "Invalid value for /Jobs:N. N must be a positive integer." -ForegroundColor Red
            Exit-Script 1
        }
    } elseif ($arg -eq "/Help" -or $arg -eq "/?") {
        Show-Help
        Exit-Script 0
    } else {
        Write-Host "Unknown argument: $arg" -ForegroundColor Red
        Write-Host "Use /Help or /? for usage information." -ForegroundColor Yellow
        Exit-Script 1
    }
}

# Default behavior: build both if none specified
if (-not $buildDebug -and -not $buildRelease) {
    $buildDebug = $true
    $buildRelease = $true
}

# Ensure deps directory exists and change to deps\llvm
New-Item deps\llvm -ItemType Directory -Force
Set-Location deps\llvm

# Check if source directories already exist
$srcExists = Test-Path "src"

# Download LLVM source files only if they don't exist
if ($srcExists) {
    Write-Host "LLVM source already exists, skipping download."
} else {
    # Ensure 7Zip is available for extracting files.
    Install-Module -Name 7Zip4Powershell -Force

    # Download source files.
    Invoke-WebRequest https://github.com/llvm/llvm-project/releases/download/llvmorg-18.1.8/llvm-18.1.8.src.tar.xz -OutFile llvm.tar.xz
    Invoke-WebRequest https://github.com/llvm/llvm-project/releases/download/llvmorg-18.1.8/third-party-18.1.8.src.tar.xz -OutFile third-party.tar.xz
    Invoke-WebRequest https://github.com/llvm/llvm-project/releases/download/llvmorg-18.1.8/cmake-18.1.8.src.tar.xz -OutFile cmake.tar.xz

    # Extract source files.
    Expand-7Zip llvm.tar.xz .
    Expand-7Zip llvm.tar src
    Expand-7Zip third-party.tar.xz .
    Expand-7Zip third-party.tar src
    Expand-7Zip cmake.tar.xz .
    Expand-7Zip cmake.tar src

    # Move source files into expected named directories.
    Move-Item src\third-party-18.1.8.src src\third-party -ErrorAction SilentlyContinue
    Move-Item src\cmake-18.1.8.src src\cmake -ErrorAction SilentlyContinue
}

# Function for building LLVM
function BuildLLVM {
    param (
        [string]$BuildType,
        [string]$BuildDir,
        [string]$InstallDir,
        [switch]$Force,
        [switch]$Partial,
        [int]$Jobs
    )

    # Check if build or install directories exist
    $buildDirExists = Test-Path $BuildDir
    $installDirExists = Test-Path $InstallDir

    # Fail to build if not set to Force the build and one already exists in some form.
    if (($buildDirExists -or $installDirExists) -and -not $Force -and -not $Partial) {
        Write-Host "Build or install directory already exists. Use /Force to overwrite or /Partial to continue from the last build." -ForegroundColor Yellow
        return
    }

    # Delete existing build or install directories on Force.
    if ($Force) {
        if ($buildDirExists) {
            Remove-Item $BuildDir -Recurse -Force
        }
        if ($installDirExists) {
            Remove-Item $InstallDir -Recurse -Force
        }
    }

    # Create build and install directories
    New-Item $BuildDir -ItemType Directory -Force
    New-Item $InstallDir -ItemType Directory -Force

    # Configure CMake
    cmake -B $BuildDir                                  `
        -DCMAKE_CXX_COMPILER=cl                         `
        -DCMAKE_C_COMPILER=cl                           `
        -DCMAKE_BUILD_TYPE="$BuildType"                 `
        -DCMAKE_INSTALL_PREFIX:PATH="../$InstallDir"    `
        -DLLVM_ENABLE_ZLIB=OFF                          `
        -DLLVM_USE_CRT_RELEASE=MT                       `
        -DLLVM_USE_CRT_DEBUG=MTd                        `
        -S src/llvm-18.1.8.src

    # Use custom job count if provided, otherwise default to number of processors less 4
    $procs = 1
    if ($Jobs -gt 0) {
        $procs = $Jobs
    } else {
        $procs = (Get-CimInstance Win32_ComputerSystem).NumberOfLogicalProcessors - 4
        if ($procs -lt 1) {
            $procs = 1  # Ensure at least one processor is used.
        }
    }

    # Build
    cmake --build "$BuildDir" --parallel "$procs" --config "$BuildType"

    # Install
    cmake --build "$BuildDir" --target install --config "$BuildType"
}

# Do builds based on flags
if ($buildDebug) {
    BuildLLVM -BuildType "Debug" -BuildDir "build_debug" -InstallDir "debug" -Force:$forceBuild -Partial:$partialBuild -Jobs $jobCount
}

if ($buildRelease) {
    BuildLLVM -BuildType "Release" -BuildDir "build_release" -InstallDir "release" -Force:$forceBuild -Partial:$partialBuild -Jobs $jobCount
}

Exit-Script 0
