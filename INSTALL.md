# Installation & Build Guide

## Table des Matières
1. [Windows](#windows)
2. [Linux](#linux)
3. [macOS](#macos)
4. [Docker](#docker)

---

## Windows

### Prérequis
- **Visual Studio 2022** (Community Edition gratuite)
- **CMake 3.16+** (https://cmake.org/download/)
- **Git**

### Installation Rapide

1. **Cloner le repo**
```batch
git clone <repo-url>
cd vibe-tracing
```

2. **Build**
```batch
.\build.bat
```

3. **Exécuter**
```batch
.\build\Release\vibe_tracing.exe
```

### Build Manuel (si build.bat échoue)
```batch
mkdir build
cd build
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
cd ..
.\build\Release\vibe_tracing.exe
```

### Troubleshooting Windows
**Erreur: "CMake not found"**
- Installer CMake depuis https://cmake.org
- Ajouter CMake au PATH (case installation)

**Erreur: "Visual Studio not found"**
- Installer Visual Studio 2022 Community
- Relancer CMake

**"GLFW not found"** (moins courant)
```batch
vcpkg install glfw3:x64-windows
cmake -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake ..
```

---

## Linux

### Ubuntu/Debian
```bash
# Installation dépendances
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    libglfw3-dev \
    libgl1-mesa-dev \
    git

# Clone & build
git clone <repo-url>
cd vibe-tracing
chmod +x build.sh
./build.sh

# Run
./build/vibe_tracing
```

### Fedora/RHEL
```bash
sudo dnf install -y \
    gcc \
    cmake \
    glfw-devel \
    mesa-libGL-devel \
    git

git clone <repo-url>
cd vibe-tracing
chmod +x build.sh
./build.sh
```

### Arch Linux
```bash
sudo pacman -S \
    base-devel \
    cmake \
    glfw-x11 \
    mesa \
    git

git clone <repo-url>
cd vibe-tracing
./build.sh
```

### Avec OpenMP (optionnel, pour +speed)
```bash
# Installer OpenMP
sudo apt-get install libomp-dev

# Build avec OpenMP
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DOPENMP=ON ..
make -j$(nproc)
```

---

## macOS

### Via Homebrew (recommandé)
```bash
# Installer dépendances
brew install cmake glfw

# Clone & build
git clone <repo-url>
cd vibe-tracing
chmod +x build.sh
./build.sh

# Run
./build/vibe_tracing
```

### Via MacPorts
```bash
sudo port install cmake glfw
git clone <repo-url>
cd vibe-tracing
./build.sh
```

### M1/M2 Silicon
```bash
# Compiler pour ARM64 (default)
./build.sh

# Ou forcer si problème:
mkdir build && cd build
cmake -DCMAKE_OSX_ARCHITECTURES=arm64 ..
make -j$(nproc)
```

---

## Docker

### Build dans Docker
```bash
docker build -t vibe-tracing .
```

### Run avec Docker
```bash
# Affichage GUI (nécessite X11 forwarding)
docker run -it -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    vibe-tracing /build/vibe_tracing
```

### Dockerfile
```dockerfile
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential cmake git \
    libglfw3-dev libgl1-mesa-dev

WORKDIR /app
COPY . .
RUN chmod +x build.sh && ./build.sh

ENTRYPOINT ["./build/vibe_tracing"]
```

---

## Vérification Installation

### Test basique
```bash
# Check CMake
cmake --version

# Check GCC/Clang
gcc --version

# Check OpenGL (Linux)
glxinfo | grep "OpenGL version"

# Check GLFW
pkg-config --modversion glfw3
```

### Build test
```bash
cd vibe-tracing
mkdir test_build && cd test_build
cmake ..
make
```

---

## Configuration de Développement

### VS Code
1. Installer extensions:
   - C/C++ (Microsoft)
   - CMake (twxs)
   - CMake Tools (Microsoft)

2. `.vscode/settings.json`:
```json
{
    "cmake.configureOnOpen": true,
    "cmake.generator": "Unix Makefiles",
    "[c]": {
        "editor.formatOnSave": true,
        "editor.defaultFormatter": "xaver.clang-format"
    }
}
```

### CLion
- File → Open → Select project directory
- CMake sera auto-détecté
- Run → Run 'vibe_tracing'

### Visual Studio
- File → Open → Folder
- Sélectionner vibe-tracing/
- Select CMake Presets if prompted

---

## Build Options Avancées

### Release optimisé
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DOPENMP=ON ..
make -j$(nproc)
```

### Debug avec symboles
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
gdb ./vibe_tracing
```

### Avec profiling
```bash
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
make
perf record ./vibe_tracing
perf report
```

### Avec ASAN (Memory sanitizer)
```bash
cmake -DCMAKE_C_FLAGS="-fsanitize=address" ..
make
./vibe_tracing  # Détecte memory leaks!
```

---

## Performance Tips

| Option | Impact | Command |
|--------|--------|---------|
| Resolution ↓ | +200% FPS | Edit `config.h`: `SCREEN_WIDTH 512` |
| Ray depth ↓ | +50% FPS | Edit `config.h`: `MAX_RAY_DEPTH 1` |
| OpenMP | +4-8x | Use `-DOPENMP=ON` |
| GPU Compute | +50-100x | TODO: Compute shaders |

---

## Support

| Problème | Solution |
|----------|----------|
| Build échoue | Vérifier CMake, gcc, dépendances |
| "Cannot find glfw3.h" | `sudo apt-get install libglfw3-dev` |
| "Cannot find GL/gl.h" | `sudo apt-get install libgl1-mesa-dev` |
| Fenêtre noire | Vérifier OpenGL drivers |
| Crash au démarrage | Vérifier OpenGL version (besoin 3.3+) |

---

**Prêt? Lancer:** `./build.sh && ./build/vibe_tracing`
