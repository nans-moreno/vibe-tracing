#include "scene.h"
#include <stdlib.h>

Scene scene_create(void) {
    Scene scene = {0};
    
    // Initialize default spheres
    scene.spheres[0] = (SceneSphere){{-1.0f, 0.5f, -3.0f}, 0.5f, {1.0f, 0.0f, 0.0f}};
    scene.spheres[1] = (SceneSphere){{0.5f, -0.2f, -2.5f}, 0.8f, {0.0f, 1.0f, 0.0f}};
    scene.spheres[2] = (SceneSphere){{2.0f, 0.0f, -3.0f}, 0.6f, {0.0f, 0.0f, 1.0f}};
    scene.sphere_count = 3;
    
    // Initialize lights
    scene.lights[0] = (SceneLight){{2.0f, 2.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, 1.0f};
    scene.light_count = 1;
    
    // Initialize camera
    scene.camera_pos = (Vec3){0.0f, 0.5f, 2.0f};
    scene.camera_forward = (Vec3){0.0f, 0.0f, -1.0f};
    scene.camera_right = (Vec3){1.0f, 0.0f, 0.0f};
    scene.camera_up = (Vec3){0.0f, 1.0f, 0.0f};
    
    return scene;
}

void scene_destroy(Scene *scene) {
    (void)scene;
}
