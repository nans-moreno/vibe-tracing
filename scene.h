#ifndef SCENE_H
#define SCENE_H

#include "raytracer.h"

Scene scene_create(void);
void scene_add_sphere(Scene *scene, Vec3 pos, float radius, Color color);
void scene_add_light(Scene *scene, Vec3 pos, Color color, float intensity);
void scene_destroy(Scene *scene);

#endif
