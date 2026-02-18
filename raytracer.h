#ifndef RAYTRACER_H
#define RAYTRACER_H

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    float r, g, b;
} Color;

typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;

typedef struct {
    Vec3 center;
    float radius;
    Color color;
} Sphere;

typedef struct {
    Vec3 position;
    Color color;
    float intensity;
} Light;

#endif // RAYTRACER_H
