# ❓ Vibe Tracing - FAQ (Frequently Asked Questions)

## Installation & Setup

### Q: "CMake not found on Windows"
**A:** Download from https://cmake.org and **make sure to check "Add CMake to PATH"** during installation. Then restart your terminal.

### Q: "GLFW3 not found - how to install?"
**A:**
- **Ubuntu/Debian:** `sudo apt-get install libglfw3-dev`
- **Fedora:** `sudo dnf install glfw-devel`
- **Mac:** `brew install glfw3`
- **Windows:** Use vcpkg: `vcpkg install glfw3:x64-windows`

### Q: "Can I use make instead of CMake?"
**A:** Yes! Use `make` instead:
```bash
make              # Compile
make run          # Compile and run
make clean        # Remove build artifacts
make DEBUG=1      # Compile with debug symbols
```

### Q: "Which operating systems are supported?"
**A:** All three:
- ✅ Windows (Visual Studio 2022)
- ✅ Linux (GCC/Clang)
- ✅ macOS (Clang)

---

## Building & Compilation

### Q: "Build is slow - how to speed it up?"
**A:**
- Use parallel build: `make -j$(nproc)` or `cmake --build . -j 8`
- Use faster compiler (clang > gcc usually)
- Use precompiled headers (advanced)

### Q: "I get compiler warnings - is this normal?"
**A:** No! This project compiles with 0 warnings. If you see warnings:
- Check you're not mixing C99 vs C11
- Check CFLAGS: should include `-Wall -Wextra`
- Update your compiler

### Q: "How do I compile with optimization flags?"
**A:**
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..  # Use O3 optimizations
make -j$(nproc)
```

### Q: "Can I use a different build system?"
**A:** Yes:
- CMake (primary - cross-platform)
- Makefile (fast, simple)
- Visual Studio (Windows only)
- XCode (macOS only)

---

## Running & Controls

### Q: "The window opens but it's black"
**A:** 
- Check OpenGL drivers are updated
- Might need to wait 5+ seconds for first render
- Try reducing resolution in `config.h`

### Q: "Controls don't work"
**A:**
- Right-click for mouse look (not left-click)
- WASD for movement (caps lock OFF)
- Check your keyboard layout (QWERTY assumed)

### Q: "How do I adjust controls sensitivity?"
**A:** Edit `config.h`:
```c
#define CAMERA_SENSITIVITY 0.005f  // Lower = less sensitive
#define CAMERA_SPEED 5.0f          // Camera movement speed
```

### Q: "How to record/screenshot the output?"
**A:** Use OBS or your OS screenshot tool. Currently no in-app screenshot (could add to Roadmap).

---

## Performance

### Q: "Why is it running slow?"
**A:** Likely causes:
- GPU drivers outdated (update them)
- Resolution too high (reduce in `config.h`)
- Ray depth too high (set `MAX_RAY_DEPTH 1`)
- CPU not powerful enough (reduce resolution more)

### Q: "Can I enable multi-threading?"
**A:** Yes, with OpenMP (gcc/clang only):
```bash
cmake -DOPENMP=ON ..
make
```

### Q: "Why is CPU version slow and GPU is fast?"
**A:** Ray tracing is embarrassingly parallel. GPU has 1000+ cores, CPU has 4-16. To fix: Use GPU compute shader (in development).

### Q: "How do I profile performance?"
**A:** Use `perf` (Linux):
```bash
perf record ./build/vibe_tracing
perf report
```

### Q: "Target FPS is 10 - why so low?"
**A:** Because:
- Full-screen 1024x768 resolution
- Ray depth 2 (recursive)
- Single-threaded CPU
To improve: Reduce resolution or use GPU

---

## Code & Development

### Q: "How do I add a new primitive (cube, triangle)?"
**A:**
1. Add to `raytracer.h`:
```c
typedef struct { ... } Cube;
float ray_cube_intersect(Ray ray, Cube cube);
```
2. Implement in `raytracer.c`
3. Call in `ray_trace()` loop
4. Build & test

### Q: "How to add multi-light shadows?"
**A:** Modify `ray_trace()` to cast shadow rays:
```c
// For each light, check if point is shadowed
float shadow = 1.0f;
Ray shadow_ray = ray_create(hit_point, to_light);
if (ray_intersects_anything(shadow_ray, scene)) {
    shadow = 0.2f;  // In shadow
}
// Use shadow factor in lighting
```

### Q: "Can I use the GPU?"
**A:** Yes! Implement compute shader in `compute_shader.h`. It's stubbed out with pseudocode already.

### Q: "How do I add textures?"
**A:** 
1. Load texture with `stb_image.h`
2. Store UV coordinates on sphere
3. Lookup texture color in ray_trace()
4. Return texture color instead of material color

### Q: "Is the code commented?"
**A:** Yes, but minimal (code should be self-documenting). See `DEVELOPMENT.md` for commenting standards.

---

## Features & Limitations

### Q: "What primitives does it support?"
**A:** Currently only **spheres** (fastest to intersect). Can add:
- Planes
- Triangles (slow)
- Cubes (AABBs)
- More in future

### Q: "Why no reflections/refractions?"
**A:** Added complexity + performance hit. In roadmap for Phase 2 (GPU).

### Q: "Can I load 3D models?"
**A:** Not yet. Would need:
1. OBJ loader
2. Triangle intersection
3. BVH acceleration (slow without it)
4. UV mapping

### Q: "How many lights/objects max?"
**A:** Currently:
- Max 100 spheres
- Max 10 lights
Edit `config.h` to increase (or decrease for speed)

### Q: "Can I save rendered images?"
**A:** Not yet. Could add:
1. PPM export (simple)
2. PNG export (need libpng)
3. Screenshot hotkey

---

## Troubleshooting

### Q: "Program crashes on startup"
**A:** Most likely:
- OpenGL version < 3.3 (you need 3.3+)
- GLFW not installed properly
- Missing graphics driver

**Fix:** 
```bash
glxinfo | grep "OpenGL version"  # Check version
./verify.sh                       # Run dependency checker
```

### Q: "Compilation errors with GL headers"
**A:** Missing OpenGL development files:
```bash
# Linux
sudo apt-get install libgl1-mesa-dev

# Mac
# Comes with XCode

# Windows
# Comes with Visual Studio
```

### Q: "Symbol error on Linux: 'glCreateShader@GLVND_1.0.0' not found"
**A:** Likely GLVND issue:
```bash
export LD_LIBRARY_PATH=/usr/lib/nvidia-driver:$LD_LIBRARY_PATH
```

### Q: "How do I debug the program?"
**A:** Use GDB (Linux/Mac):
```bash
gdb ./build/vibe_tracing
(gdb) break main
(gdb) run
(gdb) next
(gdb) print camera.position
```

### Q: "Memory leak - how to find?"
**A:** Use Valgrind:
```bash
valgrind --leak-check=full ./build/vibe_tracing
```

---

## Academic & Presentation

### Q: "Is this suitable for a school project?"
**A:** Yes! Perfect for:
- Graphics programming course
- Performance optimization project
- Real-time rendering demo
- Computer vision foundation

### Q: "How do I cite this in my paper?"
**A:**
```
Vibe Tracing: Interactive Ray Tracing Engine in C
https://github.com/...
Written in C11 with GLFW3 and OpenGL 3.3
```

### Q: "Can I use this commercially?"
**A:** Check LICENSE (assumed MIT/open for school project). Ask your professor.

### Q: "How do I present this to my class?"
**A:** See `PRESENTATION.md` for slide outline + talking points.

### Q: "Is the code ready for production?"
**A:** Almost! For production you'd add:
- Error handling
- Resource leaks checking
- Thread safety
- More extensive testing

---

## Extensions & Advanced

### Q: "Can I implement GPU ray tracing?"
**A:** Yes! Use compute shaders. See `compute_shader.h` for stub + pseudocode.

### Q: "How do I parallelize the CPU version?"
**A:** Use OpenMP:
```c
#pragma omp parallel for collapse(2)
for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++)
        // Render pixel
```

### Q: "Can I add path tracing?"
**A:** Yes! Modify `ray_trace()` to:
1. Sample random direction
2. Trace ray
3. Accumulate color over many frames
4. Denoise result

### Q: "How do I add BVH acceleration?"
**A:** 
1. Implement BVH tree builder
2. Modify ray intersection to traverse BVH
3. Much faster for 1000+ objects

---

## Philosophy & Design

### Q: "Why C and not C++?"
**A:**
- Simpler, more educational
- Forces you to understand memory
- Faster compilation
- Portable C11 standard

### Q: "Why not use a game engine?"
**A:**
- Learn graphics fundamentals
- Full control over implementation
- Educational value
- Smaller codebase (understand everything)

### Q: "Why OpenGL and not Vulkan?"
**A:**
- Simpler for beginners
- Better cross-platform support
- Easier to learn before Vulkan

### Q: "Why CPU ray tracing first?"
**A:**
- Understand algorithm before GPU
- Debug easier on CPU
- GPU version is logical next step

---

## Getting Help

### Q: "I'm stuck, where do I look?"
**A:** In order:
1. Check `README.md` for your question
2. Look in `INSTALL.md` for setup issues
3. Review `DEVELOPMENT.md` for code questions
4. Check `ARCHITECTURE.md` for design
5. Read source code comments

### Q: "The documentation has an error"
**A:** Great catch! Please report it (or fix and submit PR).

### Q: "Can I contribute?"
**A:** Yes!
1. Fork repo
2. Create feature branch
3. Follow `DEVELOPMENT.md`
4. Submit PR
5. We'll review + merge

---

## Last Resort

### Q: "Nothing works!"
**A:** Try the nuclear option:
```bash
rm -rf build/              # Remove old build
rm -rf CMakeCache.txt
cmake .
make clean
make -j$(nproc)
./build/vibe_tracing
```

If still broken:
1. Run `./verify.sh` to check deps
2. Review `INSTALL.md` again
3. Check compiler: `gcc --version`
4. Check GLFW: `pkg-config --list-all | grep glfw`

---

**Still have questions? Read `INDEX.md` for doc map or ask in comments!** 💬

---

**Remember: This is a learning project. If something doesn't work, that's a great opportunity to debug and learn!** 🎓
