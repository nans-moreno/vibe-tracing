#!/bin/bash

# Vibe Tracing Build Script for Linux/Mac

echo "=== Vibe Tracing Build System ==="
echo ""

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo "ERROR: CMake not found. Please install it:"
    echo "  Ubuntu: sudo apt-get install cmake"
    echo "  Mac: brew install cmake"
    exit 1
fi

# Check if glfw3 is installed
if ! pkg-config --exists glfw3; then
    echo "WARNING: GLFW3 not found. Installing..."
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        sudo apt-get install libglfw3-dev
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        brew install glfw3
    fi
fi

# Create build directory
mkdir -p build
cd build

echo "Configuring with CMake..."
cmake -DCMAKE_BUILD_TYPE=Release ..
if [ $? -ne 0 ]; then
    echo "ERROR: CMake configuration failed"
    cd ..
    exit 1
fi

echo ""
echo "Building project..."
make -j$(nproc)
if [ $? -ne 0 ]; then
    echo "ERROR: Build failed"
    cd ..
    exit 1
fi

echo ""
echo "Build successful!"
echo "Executable: ./vibe_tracing"
echo ""
echo "To run: ./vibe_tracing"
cd ..
