/*
 * Scene Examples - Vibe Tracing
 * 
 * Différentes configurations de scène pour tester/démontrer
 * les capacités du ray tracer
 */

#ifndef SCENES_H
#define SCENES_H

#include "scene.h"

/* Scene 1: Three Colored Spheres */
static inline void scene_demo_spheres(Scene *scene) {
    scene_add_sphere(scene, vec3(0, 0, -5), 1.0f, (Color){1.0f, 0.3f, 0.3f});
    scene_add_sphere(scene, vec3(-3, 0, -8), 1.5f, (Color){0.3f, 1.0f, 0.3f});
    scene_add_sphere(scene, vec3(3, 0, -8), 1.2f, (Color){0.3f, 0.3f, 1.0f});
    scene_add_sphere(scene, vec3(0, -100, 0), 98.0f, (Color){0.8f, 0.8f, 0.8f});
    
    scene_add_light(scene, vec3(5, 10, 0), (Color){1.0f, 1.0f, 1.0f}, 1.5f);
    scene_add_light(scene, vec3(-5, 5, 5), (Color){0.8f, 0.8f, 1.0f}, 0.8f);
}

/* Scene 2: Sphere Array */
static inline void scene_sphere_grid(Scene *scene) {
    float colors[][3] = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 1.0f},
    };
    
    int color_idx = 0;
    for (int x = -2; x <= 2; x++) {
        for (int z = -4; z <= -1; z++) {
            Color c = {colors[color_idx][0], colors[color_idx][1], colors[color_idx][2]};
            scene_add_sphere(scene, vec3(x * 2.0f, 1.0f, z * 2.0f), 0.8f, c);
            color_idx = (color_idx + 1) % 6;
        }
    }
    
    scene_add_sphere(scene, vec3(0, -100, 0), 98.0f, (Color){0.7f, 0.7f, 0.7f});
    
    scene_add_light(scene, vec3(0, 10, 0), (Color){1.0f, 1.0f, 1.0f}, 2.0f);
    scene_add_light(scene, vec3(10, 5, 0), (Color){1.0f, 0.5f, 0.5f}, 1.0f);
}

/* Scene 3: Single Large Sphere */
static inline void scene_single_sphere(Scene *scene) {
    scene_add_sphere(scene, vec3(0, 0, -5), 2.5f, (Color){0.9f, 0.9f, 0.9f});
    scene_add_sphere(scene, vec3(0, -100, 0), 98.0f, (Color){0.5f, 0.5f, 0.5f});
    
    scene_add_light(scene, vec3(5, 10, 0), (Color){1.0f, 1.0f, 1.0f}, 2.0f);
    scene_add_light(scene, vec3(-5, 5, -5), (Color){1.0f, 0.8f, 0.6f}, 1.5f);
    scene_add_light(scene, vec3(0, 2, 5), (Color){0.5f, 0.7f, 1.0f}, 0.8f);
}

/* Scene 4: Minimal (performance test) */
static inline void scene_minimal(Scene *scene) {
    scene_add_sphere(scene, vec3(0, 0, -3), 1.0f, (Color){1.0f, 1.0f, 1.0f});
    scene_add_sphere(scene, vec3(0, -100, 0), 98.0f, (Color){0.8f, 0.8f, 0.8f});
    
    scene_add_light(scene, vec3(0, 5, 0), (Color){1.0f, 1.0f, 1.0f}, 1.0f);
}

/* Pyramide de sphères */
static inline void scene_pyramid(Scene *scene) {
    float y_base = 0.5f;
    float spacing = 1.5f;
    
    for (int row = 0; row < 4; row++) {
        float y = y_base + row * spacing;
        for (int col = -row; col <= row; col++) {
            float x = col * spacing;
            Color color = {
                0.5f + 0.5f * (row / 4.0f),
                0.3f + 0.7f * (col + row) / (2.0f * row + 1.0f),
                0.8f - 0.5f * (row / 4.0f)
            };
            scene_add_sphere(scene, vec3(x, y, -6.0f - row * 0.5f), 0.6f, color);
        }
    }
    
    scene_add_sphere(scene, vec3(0, -100, 0), 98.0f, (Color){0.7f, 0.7f, 0.7f});
    
    scene_add_light(scene, vec3(0, 15, 0), (Color){1.0f, 1.0f, 1.0f}, 2.0f);
    scene_add_light(scene, vec3(10, 8, 0), (Color){1.0f, 0.6f, 0.6f}, 1.0f);
}

#endif
