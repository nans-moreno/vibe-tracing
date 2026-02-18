@echo off
REM Vibe Tracing Build Script for Windows

echo === Vibe Tracing Build System ===
echo.

REM Check if CMake is installed
where cmake >nul 2>nul
if errorlevel 1 (
    echo ERROR: CMake not found. Please install CMake from https://cmake.org
    exit /b 1
)

REM Create build directory
if not exist build mkdir build
cd build

echo Configuring with CMake...
cmake -G "Visual Studio 17 2022" ..
if errorlevel 1 (
    echo ERROR: CMake configuration failed
    cd ..
    exit /b 1
)

echo.
echo Building project...
cmake --build . --config Release
if errorlevel 1 (
    echo ERROR: Build failed
    cd ..
    exit /b 1
)

echo.
echo Build successful!
echo Executable: build\Release\vibe_tracing.exe
echo.
echo To run: .\build\Release\vibe_tracing.exe
cd ..
