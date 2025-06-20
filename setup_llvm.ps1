mkdir deps
mkdir deps\llvm

Set-Location deps/llvm

Install-Module -Name 7Zip4Powershell -Force

Invoke-WebRequest https://github.com/llvm/llvm-project/releases/download/llvmorg-18.1.8/llvm-18.1.8.src.tar.xz -OutFile llvm.tar.xz
Invoke-WebRequest https://github.com/llvm/llvm-project/releases/download/llvmorg-18.1.8/third-party-18.1.8.src.tar.xz -OutFile third-party.tar.xz
Invoke-WebRequest https://github.com/llvm/llvm-project/releases/download/llvmorg-18.1.8/cmake-18.1.8.src.tar.xz -OutFile cmake.tar.xz

Expand-7Zip llvm.tar.xz .
Expand-7Zip third-party.tar.xz .
Expand-7Zip cmake.tar.xz .

Expand-7Zip llvm.tar src
Expand-7Zip third-party.tar src
Expand-7Zip cmake.tar src

Move-Item src/cmake-18.1.8.src src/cmake
Move-Item src/third-party-18.1.8.src src/third-party

New-Item build -ItemType Directory

cmake -B build `
    -DCMAKE_CXX_COMPILER=cl `
    -DCMAKE_C_COMPILER=cl `
    -DCMAKE_BUILD_TYPE=Release `
    -DCMAKE_INSTALL_PREFIX:PATH=.. `
    -DLLVM_ENABLE_ZLIB=OFF `
    -DLLVM_USE_CRT_RELEASE=MT `
    -DLLVM_USE_CRT_DEBUG=MTd `
    -S src/llvm-18.1.8.src

$procs = (Get-CimInstance Win32_ComputerSystem).NumberOfLogicalProcessors - 4
cmake --build build --parallel $procs --config Release

cmake --build build --target install --config Release
