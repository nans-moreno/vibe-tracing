#ifndef MATH_H
#define MATH_H

#include <math.h>

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    float x, y, z, w;
} Vec4;

typedef struct {
    Vec3 col[3];
} Mat3;

/* Vector operations */
Vec3 vec3(float x, float y, float z);
Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_mul(Vec3 v, float s);
Vec3 vec3_div(Vec3 v, float s);
Vec3 vec3_cross(Vec3 a, Vec3 b);
float vec3_dot(Vec3 a, Vec3 b);
float vec3_length(Vec3 v);
Vec3 vec3_normalize(Vec3 v);
Vec3 vec3_lerp(Vec3 a, Vec3 b, float t);

/* Utilities */
float fclamp(float x, float min, float max);
float flerp(float a, float b, float t);

#endif
