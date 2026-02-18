# Architecture Technique - Vibe Tracing

## Stack Technique

```
┌─────────────────────────────────────┐
│      OpenGL Quad Renderer           │
│   (graphics.c - shader + texture)   │
├─────────────────────────────────────┤
│      Framebuffer Management         │
│      (ui.c - pixel buffer)          │
├─────────────────────────────────────┤
│      Ray Tracing Engine             │
│  (raytracer.c - intersection,       │
│   lighting, recursion)              │
├─────────────────────────────────────┤
│      Scene Management               │
│   (scene.c - geometry + lights)     │
├─────────────────────────────────────┤
│      Camera System                  │
│  (camera.c - projection, rotation)  │
├─────────────────────────────────────┤
│      Math Library                   │
│  (math.c - Vec3, operations)        │
├─────────────────────────────────────┤
│      GLFW + OpenGL Core             │
│   (Input, windowing, rendering)     │
└─────────────────────────────────────┘
```

## Modules

### math.c/h
**Responsabilités:**
- Opérations vectorielles 3D
- Produit scalaire/vectoriel
- Normalisation, interpolation

**Performance Notes:**
- Pas d'allocation dynamique
- Compilable avec SIMD flags (`-mavx2` ou `/arch:AVX2`)
- Inline-able pour la plupart des fonctions

### raytracer.c/h
**Responsabilités:**
- Ray-sphere intersection (quadratic equation)
- Shading (Phong model)
- Recursive ray tracing

**Pipeline:**
```
1. ray_trace(ray, scene, depth)
   └─> Pour chaque sphère:
       ├─> Calcul intersection
       └─> Intersection la plus proche?
           ├─> Calcul normale
           └─> Éclairage multi-lumière
   └─> Retour: Color
```

### camera.c/h
**Responsabilités:**
- Position & orientation 3D
- Rotation (yaw/pitch) fluide
- Génération rayons depuis écran

**Projection:**
```
Screen UV (0-1) → 
  Camera space (FOV + aspect ratio) →
    World space ray
```

### graphics.c/h (NEW)
**Responsabilités:**
- Fenêtre GLFW
- Shader compilation OpenGL
- Texture management
- Input polling

**Pipeline rendering:**
```
CPU framebuffer (RGBA32) →
  Update texture (GPU) →
    Render full-screen quad →
      Tone mapping + gamma (shader) →
        Display
```

### scene.c/h
**Responsabilités:**
- Allocation sphères/lumières
- Gestion compte
- Material properties

**Limites actuelles:**
- Max 100 sphères
- Max 10 lumières
(Facilement extensible)

### ui.c/h
**Responsabilités:**
- Allocation framebuffer CPU
- Conversion Color → RGBA32
- Clamping/wrapping

## Flux de Données

```
Frame Loop:
├─1. Input (glfwGetKey, glfwGetCursorPos)
│
├─2. Update (camera_move, camera_rotate)
│
├─3. Render (if time > interval):
│   ├─ For each pixel (x, y):
│   │  ├─ camera_get_ray_direction()
│   │  ├─ ray_trace()
│   │  │  └─ For each sphere:
│   │  │     └─ ray_sphere_intersect()
│   │  │     └─ Calculate lighting
│   │  └─ ui_set_pixel()
│   │
│   └─ graphics_update_texture()
│
├─4. Display (OpenGL):
│   ├─ graphics_render_quad()
│   └─ graphics_swap_buffers()
│
└─5. Poll Events
```

## Performance Bottlenecks & Solutions

### Actuel (CPU Ray Tracing)
```
Resolution    | FPS  | Notes
────────────────────────────────
1024x768      | ~10  | Interactive
 800x600      | ~20  | Smooth
 512x384      | ~50+ | Very smooth
```

### Optimisations Faciles

1. **Résolution réduite (512x384)** → 2-3x speedup
2. **Ray depth 1** (au lieu de 2) → 1.5x speedup
3. **OpenMP parallelization** → 4-8x speedup (CPU)

### GPU Acceleration (Compute Shader)
```glsl
// Pseudocode
layout(local_size_x = 16, local_size_y = 16) in;
void main() {
    vec2 uv = vec2(gl_GlobalInvocationID) / imageSize(outTexture);
    vec4 color = ray_trace(uv);  // GLSL ray tracer
    imageStore(outTexture, gl_GlobalInvocationID, color);
}
```
**Expected speedup:** 10-100x

## Configuration Tuning

Éditer `config.h`:

```c
/* Pour démo fluide */
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_RAY_DEPTH 1
#define RENDER_INTERVAL 0.05f  // 20 FPS

/* Pour qualité */
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
#define MAX_RAY_DEPTH 3
#define RENDER_INTERVAL 0.5f   // 2 FPS
```

## Memory Layout

```
Framebuffer:   1024 × 768 × 4 bytes = ~3.1 MB
Scene:
  - Spheres:   100 × ~40 bytes = ~4 KB
  - Lights:     10 × ~28 bytes = ~280 B
Temp:          Stack (Vec3, Ray, etc.)
─────────────────────────────────
Total:         ~3.1 MB (très efficace!)
```

## Threading Strategy (Future)

### Current: Single-threaded CPU
- Simple, déterministe
- Pas de race conditions

### Proposed: Tile-based OpenMP
```c
#pragma omp parallel for collapse(2)
for (int ty = 0; ty < NUM_TILES_Y; ty++) {
    for (int tx = 0; tx < NUM_TILES_X; tx++) {
        RenderTile tile = ...;
        render_tile(&tile);  // Independent
    }
}
```
**Avantage:** Pas de synchronisation, load balancing auto

## Extension Points

1. **New primitives:** Ajouter triangle mesh
2. **Materials:** Améliorer shading (Cook-Torrance)
3. **Sampling:** Anti-aliasing, depth of field
4. **Acceleration:** BVH tree, spatial hashing
5. **GPU:** Compute shaders GLSL

## Building for Release

```bash
# Linux/Mac - avec optimisations + OpenMP
cmake -DCMAKE_BUILD_TYPE=Release -DOPENMP=ON ..
make -j8

# Windows - MSVC
cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

**Flags recommandés:**
- `-O3 -march=native` (GCC/Clang)
- `/O2 /arch:AVX2` (MSVC)
- `-fopenmp` (GCC/Clang)
