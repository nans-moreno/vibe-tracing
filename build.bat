@echo off
setlocal enabledelayedexpansion

set "GLFW3_DIR=C:\dependencies\glfw-3.3.10.bin.WIN64"
set "GLEW_DIR=C:\dependencies\glew-2.2.0"

if exist build (
    rmdir /s /q build
)

mkdir build
cd build

cmake -G "Visual Studio 17 2022" -A x64 ^
  -DGLFW3_DIR="%GLFW3_DIR%" ^
  -DGLEW_DIR="%GLEW_DIR%" ^
  ..

if errorlevel 1 (
    echo CMake generation failed
    exit /b 1
)

cmake --build . --config Release

if errorlevel 1 (
    echo Build failed
    exit /b 1
)

echo Build successful!
cd ..
