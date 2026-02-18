#!/bin/bash

# Vibe Tracing - Build Verification Script
# Vérifier que le projet compile et fonctionne

set -e

echo "=========================================="
echo "Vibe Tracing - Build Verification"
echo "=========================================="
echo ""

# Check basic tools
echo "1. Checking required tools..."
if ! command -v cmake &> /dev/null; then
    echo "❌ CMake not found. Install it:"
    echo "   Ubuntu: sudo apt-get install cmake"
    echo "   Mac: brew install cmake"
    exit 1
fi
echo "✓ CMake $(cmake --version | head -1)"

if ! command -v gcc &> /dev/null && ! command -v clang &> /dev/null; then
    echo "❌ C compiler not found"
    exit 1
fi
echo "✓ Compiler found: $(gcc --version 2>/dev/null | head -1 || clang --version | head -1)"

# Check GLFW
echo ""
echo "2. Checking GLFW3..."
if pkg-config --exists glfw3; then
    echo "✓ GLFW3 $(pkg-config --modversion glfw3) found"
else
    echo "❌ GLFW3 not found. Install it:"
    echo "   Ubuntu: sudo apt-get install libglfw3-dev"
    echo "   Mac: brew install glfw3"
    exit 1
fi

# Check OpenGL
echo ""
echo "3. Checking OpenGL..."
if pkg-config --exists gl; then
    echo "✓ OpenGL found"
else
    echo "❌ OpenGL not found. Install it:"
    echo "   Ubuntu: sudo apt-get install libgl1-mesa-dev"
    exit 1
fi

# Check source files
echo ""
echo "4. Checking source files..."
files=(
    "main.c" "math.c" "raytracer.c" "camera.c" 
    "scene.c" "ui.c" "graphics.c"
    "math.h" "raytracer.h" "camera.h" 
    "scene.h" "ui.h" "graphics.h"
)

for file in "${files[@]}"; do
    if [ -f "$file" ]; then
        echo "✓ $file"
    else
        echo "❌ Missing: $file"
        exit 1
    fi
done

# Check CMakeLists.txt
if [ -f "CMakeLists.txt" ]; then
    echo "✓ CMakeLists.txt"
else
    echo "❌ Missing: CMakeLists.txt"
    exit 1
fi

# Build
echo ""
echo "5. Building project..."
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release .. || {
    echo "❌ CMake configuration failed"
    exit 1
}

make -j$(nproc) || {
    echo "❌ Build failed"
    exit 1
}

echo "✓ Build successful"
cd ..

# Verify executable
echo ""
echo "6. Verifying executable..."
if [ -f "build/vibe_tracing" ]; then
    echo "✓ Executable created: $(ls -lh build/vibe_tracing | awk '{print $5 " - " $9}')"
else
    echo "❌ Executable not found"
    exit 1
fi

# Test executable exists and has correct format
if file build/vibe_tracing | grep -q "ELF"; then
    echo "✓ Valid ELF binary"
elif file build/vibe_tracing | grep -q "Mach-O"; then
    echo "✓ Valid Mach-O binary (macOS)"
fi

echo ""
echo "=========================================="
echo "✓ All checks passed!"
echo "=========================================="
echo ""
echo "To run the project:"
echo "  ./build/vibe_tracing"
echo ""
echo "To clean build files:"
echo "  rm -rf build"
echo ""
