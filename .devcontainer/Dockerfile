FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04
LABEL maintainer "Matthew Marshall <matthew.marshall090396@gmail.com>"

# Install build tools and LLVM dependency.
RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository universe && \
    export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends zsh build-essential cmake git gcc-14 \
        g++-14 clang-18 llvm-18 llvm-18-dev gdb python3.12 python3.12-venv \
        python3-pip graphviz && \
    rm -rf /var/lib/apt/lists/*

# Replace any existing clang/clang++/gcc/g++ links with the versions we prefer.
RUN ln -f -s /usr/bin/clang-18 /usr/bin/clang && \
    ln -f -s /usr/bin/clang++-18 /usr/bin/clang++ && \
    ln -f -s /usr/bin/gcc-14 /usr/bin/gcc && \
    ln -f -s /usr/bin/g++-14 /usr/bin/g++

# Explicitly set folder in which to install virtual env for conan.
ENV VIRTUAL_ENV=/opt/venv
# Explicitly add virtual env to path, the main point of activating the virtualenv.
ENV PATH="$VIRTUAL_ENV/bin:$PATH"

# Install conan dependency manager for other deps.
RUN python3 -m venv $VIRTUAL_ENV && \
    pip3 install conan==2.8.0 pre-commit==3.8.0 && \
    conan profile detect
