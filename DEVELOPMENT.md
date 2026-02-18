# Guide de Développement - Vibe Tracing

## Avant de Modifier le Code

### Check de Compilation
```bash
# Windows
.\build.bat

# Linux/Mac
./build.sh
```

### Validation
- ✅ Pas d'erreur compilation
- ✅ Pas de warning (activer `-Wall -Wextra`)
- ✅ Pas de memory leaks (valgrind)
- ✅ Code lint clean

## Conventions de Code

### Nommage
```c
// Variables
int pixel_count;           // snake_case
float camera_speed;

// Fonctions
void camera_rotate(...);   // snake_case, verbe first
Color ray_trace(...);

// Types
typedef struct { ... } Vec3;     // PascalCase
typedef struct { ... } Camera;

// Constants
#define MAX_SPHERES 100    // UPPER_SNAKE_CASE
#define RENDER_INTERVAL 0.1f
```

### Spacing & Formatting
```c
// Good
Vec3 result = vec3_add(a, vec3_mul(b, 2.0f));
if (x > 0) {
    do_something();
}

// Avoid
Vec3 result=vec3_add(a,vec3_mul(b,2.0f));
if(x>0){do_something();}
```

### Comments
```c
// Use comments sparingly - code should be self-documenting
// Only comment WHY, not WHAT

// GOOD: Explains non-obvious logic
Vec3 normal = vec3_normalize(vec3_sub(hit_point, sphere.position));

// AVOID: Obvious from code
Vec3 normal = vec3_normalize(...);  // Normalize the vector
```

## Ajout d'une Nouvelle Feature

### Exemple: Ajouter une nouvelle forme (Cube)

1. **Header** (`raytracer.h`)
```c
typedef struct {
    Vec3 min, max;
    Color color;
} Cube;

float ray_cube_intersect(Ray ray, Cube cube);
```

2. **Implementation** (`raytracer.c`)
```c
float ray_cube_intersect(Ray ray, Cube cube) {
    // AABB intersection
    // ...
    return closest_t;
}

// Modify ray_trace() to check cubes:
float t_cube = ray_cube_intersect(ray, scene->cubes[i]);
if (t_cube > 0 && t_cube < closest) { ... }
```

3. **Test**
```bash
./build.sh
./vibe_tracing  # Verify no crash
```

## Debugging Tips

### Visual Debugging
```c
// Add debug visualization
if (x == 512 && y == 384) {  // Center pixel
    printf("Ray: origin=(%f,%f,%f) dir=(%f,%f,%f)\n",
        ray.origin.x, ray.origin.y, ray.origin.z,
        ray.direction.x, ray.direction.y, ray.direction.z);
}
```

### GDB Debugging (Linux/Mac)
```bash
gdb ./vibe_tracing
(gdb) break main
(gdb) run
(gdb) print camera.position
(gdb) step
```

### Valgrind Memory Check
```bash
valgrind --leak-check=full ./vibe_tracing
```

### Printf Debugging
```c
#define DEBUG 1
#if DEBUG
    fprintf(stderr, "Debug: %f\n", value);
#endif
```

## Performance Profiling

### Linux/Mac - perf
```bash
perf record ./vibe_tracing
perf report
```

### Clock Timing
```c
clock_t start = clock();
// ... do work ...
clock_t end = clock();
double time = (double)(end - start) / CLOCKS_PER_SEC;
printf("Time: %.3f s\n", time);
```

### FPS Counter
```c
static double fps_time = 0;
static int fps_frames = 0;
fps_frames++;
if (glfwGetTime() - fps_time > 1.0) {
    printf("FPS: %d\n", fps_frames);
    fps_frames = 0;
    fps_time = glfwGetTime();
}
```

## Common Pitfalls

### ❌ Uninitialized Variables
```c
// BAD
Vec3 color;
color.r = 1.0f;
// color.g, color.b uninitialized!

// GOOD
Vec3 color = {0, 0, 0};
color.r = 1.0f;
```

### ❌ Buffer Overflows
```c
// BAD
Sphere spheres[10];
for (int i = 0; i < 20; i++) {  // Oops!
    spheres[i] = ...;
}

// GOOD
for (int i = 0; i < MAX_SPHERES && i < 20; i++) {
    if (scene->sphere_count >= MAX_SPHERES) break;
    scene_add_sphere(...);
}
```

### ❌ Division by Zero
```c
// BAD
float avg = sum / count;  // count could be 0

// GOOD
float avg = (count > 0) ? (sum / count) : 0.0f;
```

### ❌ Memory Leaks
```c
// BAD
Graphics *g = graphics_create(...);
// ... forgot to graphics_destroy(g)
return 1;

// GOOD
Graphics *g = graphics_create(...);
// ... use g ...
graphics_destroy(g);  // Always cleanup
return 0;
```

## Testing Checklist

Before pushing to git:

- [ ] Code compiles without warnings
- [ ] No memory leaks (valgrind)
- [ ] Features work as intended
- [ ] Camera movement is smooth
- [ ] No crashes on exit
- [ ] FPS is reasonable
- [ ] Comments are clear
- [ ] No debug printf left

## Git Workflow

```bash
# Feature branch
git checkout -b feature/new-shape

# Make changes
git add *.c *.h

# Commit with message
git commit -m "Add cube ray intersection"

# Merge back
git checkout main
git merge feature/new-shape
```

## Documentation

When adding new functions:

```c
// ✅ GOOD: Clear documentation
/**
 * Calculate intersection between ray and sphere
 * @param ray Input ray
 * @param sphere Target sphere
 * @return Distance to intersection, or -1.0 if no hit
 */
float ray_sphere_intersect(Ray ray, Sphere sphere) {
    // ...
}

// ❌ AVOID: Missing info
float ray_sphere_intersect(Ray ray, Sphere sphere) {
    // ...
}
```

## Performance Goals

For school project:

| Resolution | Target FPS | Priority |
|------------|-----------|----------|
| 1024×768  | 10        | Interactivity |
| 800×600   | 20        | Smoothness |
| 512×384   | 50+       | Responsiveness |

**Trade-offs:**
- Lower resolution for higher FPS
- Lower ray depth for faster render
- CPU vs GPU: GPU for 100x speedup
