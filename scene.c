#include "scene.h"
#include <stdlib.h>

Scene scene_create(void) {
    Scene scene = {0};
    scene.spheres = malloc(100 * sizeof(Sphere));
    scene.lights = malloc(10 * sizeof(Light));
    scene.background = (Vec3){0.1f, 0.1f, 0.2f};
    return scene;
}

void scene_add_sphere(Scene *scene, Vec3 pos, float radius, Color color) {
    if (scene->sphere_count >= 100) return;
    scene->spheres[scene->sphere_count++] = (Sphere){
        pos, radius, color, 0.5f, 0.3f
    };
}

void scene_add_light(Scene *scene, Vec3 pos, Color color, float intensity) {
    if (scene->light_count >= 10) return;
    scene->lights[scene->light_count++] = (Light){pos, color, intensity};
}

void scene_destroy(Scene *scene) {
    free(scene->spheres);
    free(scene->lights);
}
