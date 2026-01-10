#!/bin/bash
# Build Windows executable from macOS using MinGW

set -e

echo "Building Windows executable from macOS..."
echo "Configuring with MinGW toolchain..."

cmake -B build-windows \
    -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    .

echo "Compiling..."
cmake --build build-windows --config Release

echo "Windows build complete!"
echo "Executable: ./build-windows/Dungeons.exe"
