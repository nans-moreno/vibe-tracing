#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "math.h"

typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;

typedef struct {
    float r, g, b;
} Color;

typedef struct {
    Vec3 position;
    float radius;
    Color color;
    float metallic;
    float roughness;
} Sphere;

typedef struct {
    Vec3 position;
    Color color;
    float intensity;
} Light;

typedef struct {
    Sphere *spheres;
    int sphere_count;
    Light *lights;
    int light_count;
    Vec3 background;
} Scene;

/* Ray tracing functions */
Ray ray_create(Vec3 origin, Vec3 direction);
Color ray_trace(Ray ray, Scene *scene, int depth);
Color sphere_hit(Ray ray, Sphere sphere, Vec3 hit_point, Vec3 normal);

#endif
