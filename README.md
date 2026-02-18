# Vibe Tracing - Interactive Ray Tracer in C

Un ray tracer **interactif temps réel** haute performance en C pur avec OpenGL.

## 🎬 Démarrage Rapide

### Windows
```batch
.\build.bat
.\build\Release\vibe_tracing.exe
```

### Linux/Mac
```bash
chmod +x build.sh
./build.sh
./build/vibe_tracing
```

## 🎮 Contrôles

| Clé | Action |
|-----|--------|
| **W/A/S/D** | Déplacement (avant/gauche/arrière/droite) |
| **SPACE** | Monter |
| **SHIFT** | Descendre |
| **Right-Click + Souris** | Look around (rotation caméra) |
| **ESC** | Quitter |

## 🏗️ Architecture

```
vibe-tracing/
├── CMakeLists.txt        # Configuration CMake
├── main.c                # Boucle principale GLFW
├── math.c/h              # Vecteurs 3D optimisés
├── raytracer.c/h         # Moteur ray tracing
├── camera.c/h            # Caméra interactive
├── scene.c/h             # Gestion de scène
├── ui.c/h                # Framebuffer
├── graphics.c/h          # OpenGL + GLFW
├── display.vert/frag     # Shaders (héritage)
├── build.bat/sh          # Scripts de build
└── README.md             # Doc
```

## ✨ Caractéristiques

### Ray Tracing
- ✅ Intersection sphere/ray avec précision
- ✅ Éclairage Phong multi-source
- ✅ Profondeur de récursion configurable
- ✅ Matériaux (metallic/roughness)

### Interactivité
- ✅ Caméra libre 6-DOF
- ✅ Rotation fluide (yaw/pitch)
- ✅ Déplacement temps réel
- ✅ Rendu progressif (10 FPS pour démo)

### Performance
- ✅ Rendu CPU multi-sphères
- ✅ Texture OpenGL pour affichage
- ✅ Tone mapping + gamma correction
- ✅ Structure prête pour GPU compute

## 🔧 Dépendances

**Système:**
- CMake 3.16+
- Compilateur C11 (MSVC/GCC/Clang)
- OpenGL 3.3+

**Externes:**
- **GLFW3** - Fenêtrage & input
- **OpenGL** - Rendu GPU

**Installation (Linux):**
```bash
sudo apt-get install cmake libglfw3-dev libgl1-mesa-dev
```

**Installation (Mac):**
```bash
brew install cmake glfw3
```

**Installation (Windows):**
- Installer Visual Studio 2022 Community
- CMake détectera automatiquement OpenGL
- GLFW3 peut être installé via vcpkg:
  ```
  vcpkg install glfw3:x64-windows
  ```

## 📊 Configuration

Éditer dans `main.c`:

```c
#define WIDTH 1024          // Résolution
#define HEIGHT 768

render_interval = 0.1;      // FPS cible (10 FPS)
float speed = 5.0f;         // Vitesse caméra
ray_trace(..., 2);          // Profondeur ray tracing
```

## 🚀 Prochaines Étapes

1. **Multi-threading** - Paralléliser rendu CPU (OpenMP)
2. **GPU Compute Shaders** - Ray tracing sur GPU (10x+ speedup)
3. **BVH Tree** - Accélération spatiale pour +100 objets
4. **Sampling** - Anti-aliasing progressif
5. **Reflections/Refraction** - Matériaux avancés
6. **Bump Mapping** - Textures

## 📝 Notes pour la Soutenance

- Code **lisible et commenté**
- Structure **modulaire** et extensible
- **Pas de dépendances obscures** (juste GLFW + OpenGL)
- Easy to **benchmark** et **profile**
- Prêt pour **parallélisation GPU**

## 🐛 Troubleshooting

**"GLFW not found"**
- Installer glfw3: `sudo apt-get install libglfw3-dev`
- Ou vcpkg: `vcpkg install glfw3`

**"OpenGL headers not found"**
- Linux: `sudo apt-get install libgl1-mesa-dev`

**Low FPS**
- Réduire `WIDTH/HEIGHT` dans `main.c`
- Diminuer profondeur ray tracing (actuellement 2)
- Augmenter `render_interval` (ex: 0.2)

## 📜 License

École projet - Usage libre pour soutenance
