@echo off
REM QUICK START SCRIPT - Vibe Tracing (Windows)

echo.
echo ╔════════════════════════════════════════╗
echo ║   Vibe Tracing - Quick Start (Windows)  ║
echo ╚════════════════════════════════════════╝
echo.

REM Check CMake
where cmake >nul 2>nul
if errorlevel 1 (
    echo ✗ CMake not found
    echo   Download from: https://cmake.org/download/
    echo   Add to PATH during installation
    pause
    exit /b 1
)
echo ✓ CMake found

REM Check Visual Studio
if exist "C:\Program Files\Microsoft Visual Studio\2022" (
    echo ✓ Visual Studio 2022 found
) else (
    echo ⚠ Visual Studio 2022 not found
    echo   Download Community Edition: https://visualstudio.microsoft.com
    echo.
)

echo.
echo Step 1: Building...
echo ════════════════════════════════════════════════
call build.bat
if errorlevel 1 (
    echo.
    echo ❌ Build failed
    echo.
    pause
    exit /b 1
)

echo.
echo Step 2: Running...
echo ════════════════════════════════════════════════
echo.
echo   Controls:
echo   - WASD: Move camera
echo   - SPACE: Move up
echo   - SHIFT: Move down
echo   - Right-Click + Drag: Look around
echo   - ESC: Exit
echo.
timeout /t 2

if exist "build\Release\vibe_tracing.exe" (
    .\build\Release\vibe_tracing.exe
) else (
    echo ❌ Executable not found
    pause
    exit /b 1
)
