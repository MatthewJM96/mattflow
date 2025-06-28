# Make sure llvm dir exists in deps, and change to that directory.
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
        [string]$InstallDir
    )

    # Create build and install directories
    New-Item $BuildDir -ItemType Directory -Force
    New-Item $InstallDir -ItemType Directory -Force

    # Configure CMake
    cmake -B $BuildDir                          `
        -DCMAKE_CXX_COMPILER=cl                 `
        -DCMAKE_C_COMPILER=cl                   `
        -DCMAKE_BUILD_TYPE=$BuildType           `
        -DCMAKE_INSTALL_PREFIX:PATH=$InstallDir `
        -DLLVM_ENABLE_ZLIB=OFF                  `
        -DLLVM_USE_CRT_RELEASE=MT               `
        -DLLVM_USE_CRT_DEBUG=MTd                `
        -S src/llvm-18.1.8.src

    # Get number of processors
    $procs = (Get-CimInstance Win32_ComputerSystem).NumberOfLogicalProcessors - 4

    # Build
    cmake --build $BuildDir --parallel $procs --config $BuildType

    # Install
    cmake --build $BuildDir --target install --config $BuildType
}

# Parse command line arguments
$buildDebug = $false
$buildRelease = $false

foreach ($arg in $args) {
    if ($arg -eq "/Debug") {
        $buildDebug = $true
    } elseif ($arg -eq "/Release") {
        $buildRelease = $true
    }
}

# Default behavior: build both if none specified
if (-not $buildDebug -and -not $buildRelease) {
    $buildDebug = $true
    $buildRelease = $true
}

# Do builds based on flags
if ($buildDebug) {
    BuildLLVM -BuildType "Debug" -BuildDir "build_debug" -InstallDir "debug"
}
if ($buildRelease) {
    BuildLLVM -BuildType "Release" -BuildDir "build_release" -InstallDir "release"
}
