#ifndef SCENE_H
#define SCENE_H

#include "raytracer.h"

#define MAX_SPHERES 256
#define MAX_LIGHTS 16

typedef struct {
    Vec3 position;
    float radius;
    Color color;
} SceneSphere;

typedef struct {
    Vec3 position;
    Color color;
    float intensity;
} SceneLight;

typedef struct {
    SceneSphere spheres[MAX_SPHERES];
    int sphere_count;
    
    SceneLight lights[MAX_LIGHTS];
    int light_count;
    
    Vec3 camera_pos;
    Vec3 camera_forward;
    Vec3 camera_right;
    Vec3 camera_up;
} Scene;

#endif // SCENE_H
