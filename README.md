# Mattflow

[![Linux CI](https://github.com/MatthewJM96/mattflow/actions/workflows/linux-ci.yml/badge.svg)](https://github.com/MatthewJM96/mattflow/actions/workflows/linux-ci.yml) [![MacOS CI](https://github.com/MatthewJM96/mattflow/actions/workflows/macos-ci.yml/badge.svg)](https://github.com/MatthewJM96/mattflow/actions/workflows/macos-ci.yml) [![Windows CI](https://github.com/MatthewJM96/mattflow/actions/workflows/windows-ci.yml/badge.svg)](https://github.com/MatthewJM96/mattflow/actions/workflows/windows-ci.yml)

Yet another language. Who doesn't try making one these days?

## Building

Below are build instructions for Linux, MacOS and Windows. It is notable that the process is easiest on Linux, comparable (though subject to the whims of Apple) on MacOS, and truly painful on Windows. A helper script is provided to make life a bit easier on Windows.

In all cases, pre-requisites are that you have Git, CMake, Conan, and a valid compiler installed. A final pre-requisite which is discussed in detail for Windows, is to have LLVM installed.

### Linux / MacOS

Installation of all dependencies can be easily achieved on Linux and MacOS using your favourite package manager.

At this point you would compile directly in VSCode or your favourite IDE via CMake integrations, but if you prefer the terminal experience the usual commands work:

```sh
cmake -B build -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=Release -S .

cmake --build build --parallel 4 --config Release
```

Tests can be ran with:

```sh
ctest --test-dir build --build-config Release --output-on-failure
```

### Windows

On Windows the option exists to use the WSL, in which case the same applies as in the Linux section. This is the recommended way to develop this codebase on Windows. However, you may also set up the project to compile on Windows.

An additional requirement for Windows is that Visual Studio 2022 be installed, this provides the necessary compilers to build the project, so make sure to install it with at least the generic C++ components.

Most of the dependencies may be installed with installers provided on each dependency's home page, however it is somewhat harder to obtain LLVM. A helper script `setup_llvm.ps1` is provided, and it is assumed whether you use this or not that LLVM may be found at the path `deps\llvm` from the root of this project.

To run the helper script, you will likely need to run a powershell session as administrator and run

```ps1
Set-ExecutionPolicy unrestricted
.\setup_llvm.ps1 /Help
Set-ExecutionPolicy restricted
```

On Windows, the same CMake commands as for Linux / MacOS will work, and again VSCode and other decent IDEs will integrate with CMake for convenience.
