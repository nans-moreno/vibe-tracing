#include "math.h"

Vec3 vec3(float x, float y, float z) {
    return (Vec3){x, y, z};
}

Vec3 vec3_add(Vec3 a, Vec3 b) {
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 vec3_mul(Vec3 v, float s) {
    return vec3(v.x * s, v.y * s, v.z * s);
}

Vec3 vec3_div(Vec3 v, float s) {
    return vec3(v.x / s, v.y / s, v.z / s);
}

Vec3 vec3_cross(Vec3 a, Vec3 b) {
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

float vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vec3_length(Vec3 v) {
    return sqrtf(vec3_dot(v, v));
}

Vec3 vec3_normalize(Vec3 v) {
    float len = vec3_length(v);
    if (len < 1e-6f) return vec3(0, 0, 0);
    return vec3_div(v, len);
}

Vec3 vec3_lerp(Vec3 a, Vec3 b, float t) {
    return vec3_add(a, vec3_mul(vec3_sub(b, a), t));
}

float fclamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

float flerp(float a, float b, float t) {
    return a + (b - a) * t;
}
