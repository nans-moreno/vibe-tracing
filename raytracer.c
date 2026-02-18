#include "raytracer.h"
#include <stdlib.h>
#include <math.h>

/* Fast inverse square root approximation */
static inline float fast_sqrt(float x) {
    if (x <= 0.0f) return 0.0f;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;
    y = y * (1.5f - (x * 0.5f * y * y));
    return x * y;
}

Ray ray_create(Vec3 origin, Vec3 direction) {
    return (Ray){origin, vec3_normalize(direction)};
}

float ray_sphere_intersect(Ray ray, Sphere sphere) {
    Vec3 oc = vec3_sub(ray.origin, sphere.position);
    float b = 2.0f * vec3_dot(oc, ray.direction);
    float c = vec3_dot(oc, oc) - sphere.radius * sphere.radius;
    
    float discriminant = b * b - 4.0f * c;
    if (discriminant < 0) return -1.0f;
    
    /* Avoid sqrt by comparing discriminant directly */
    /* Use fast sqrt approximation instead of sqrtf */
    float sqrt_disc = fast_sqrt(discriminant);
    float t1 = (-b - sqrt_disc) * 0.5f;
    if (t1 > 0.001f) return t1;
    
    float t2 = (-b + sqrt_disc) * 0.5f;
    if (t2 > 0.001f) return t2;
    
    return -1.0f;
}

Color color_mul(Color c, float s) {
    return (Color){c.r * s, c.g * s, c.b * s};
}

Color color_add(Color a, Color b) {
    return (Color){a.r + b.r, a.g + b.g, a.b + b.b};
}

Color ray_trace(Ray ray, Scene *scene, int depth) {
    if (depth <= 0) return (Color){0, 0, 0};
    
    float closest = 1e10f;
    int hit_sphere = -1;
    Vec3 hit_point = {0, 0, 0};
    Vec3 normal = {0, 0, 0};
    
    /* Find closest sphere intersection */
    for (int i = 0; i < scene->sphere_count; i++) {
        float t = ray_sphere_intersect(ray, scene->spheres[i]);
        if (t > 0 && t < closest) {
            closest = t;
            hit_sphere = i;
            hit_point = vec3_add(ray.origin, vec3_mul(ray.direction, t));
            normal = vec3_normalize(vec3_sub(hit_point, scene->spheres[i].position));
        }
    }
    
    if (hit_sphere == -1) {
        Color c = {scene->background.x, scene->background.y, scene->background.z};
        return c;
    }
    
    Sphere sphere = scene->spheres[hit_sphere];
    Color result = {0, 0, 0};
    
    /* Simple lighting */
    for (int i = 0; i < scene->light_count; i++) {
        Light light = scene->lights[i];
        Vec3 to_light = vec3_normalize(vec3_sub(light.position, hit_point));
        float intensity = fclamp(vec3_dot(normal, to_light), 0, 1) * light.intensity;
        result = color_add(result, color_mul(sphere.color, intensity));
    }
    
    return result;
}
