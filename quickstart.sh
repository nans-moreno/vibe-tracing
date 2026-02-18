#!/bin/bash
# QUICK START SCRIPT - Vibe Tracing

echo "╔════════════════════════════════════════╗"
echo "║   Vibe Tracing - Quick Start Guide     ║"
echo "╚════════════════════════════════════════╝"
echo ""

# Detect OS
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="Linux"
    EXEC="./build/vibe_tracing"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macOS"
    EXEC="./build/vibe_tracing"
elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "cygwin" ]]; then
    OS="Windows"
    EXEC=".\build\Release\vibe_tracing.exe"
else
    OS="Unknown"
fi

echo "Detected OS: $OS"
echo ""

# Step 1: Check dependencies
echo "Step 1: Checking dependencies..."
if command -v cmake &> /dev/null; then
    echo "  ✓ CMake found ($(cmake --version | head -1 | cut -d' ' -f3))"
else
    echo "  ✗ CMake not found - Install from https://cmake.org"
    exit 1
fi

if command -v gcc &> /dev/null || command -v clang &> /dev/null; then
    echo "  ✓ C Compiler found"
else
    echo "  ✗ C Compiler not found"
    exit 1
fi

if pkg-config --exists glfw3 2>/dev/null; then
    echo "  ✓ GLFW3 found"
else
    echo "  ⚠ GLFW3 might not be installed"
    echo "    Ubuntu: sudo apt-get install libglfw3-dev"
    echo "    Mac: brew install glfw3"
fi

echo ""
echo "Step 2: Building..."
chmod +x build.sh
./build.sh || {
    echo ""
    echo "❌ Build failed. Try:"
    echo "   Linux: sudo apt-get install cmake libglfw3-dev libgl1-mesa-dev"
    echo "   Mac: brew install cmake glfw3"
    echo "   Windows: Install Visual Studio 2022 Community + CMake"
    exit 1
}

echo ""
echo "Step 3: Running..."
echo "================================================"
echo ""
echo "  Controls:"
echo "  - WASD: Move camera"
echo "  - SPACE: Move up"
echo "  - SHIFT: Move down"
echo "  - Right-Click + Drag: Look around"
echo "  - ESC: Exit"
echo ""
echo "  Starting in 2 seconds..."
echo ""
echo "================================================"
sleep 2

if [ -f "$EXEC" ]; then
    exec "$EXEC"
else
    echo "❌ Executable not found at $EXEC"
    exit 1
fi
