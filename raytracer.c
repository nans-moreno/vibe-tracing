#include "raytracer.h"
#include <stdlib.h>

Ray ray_create(Vec3 origin, Vec3 direction) {
    return (Ray){origin, vec3_normalize(direction)};
}

float ray_sphere_intersect(Ray ray, Sphere sphere) {
    Vec3 oc = vec3_sub(ray.origin, sphere.position);
    float a = vec3_dot(ray.direction, ray.direction);
    float b = 2.0f * vec3_dot(oc, ray.direction);
    float c = vec3_dot(oc, oc) - sphere.radius * sphere.radius;
    
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return -1.0f;
    
    float t1 = (-b - sqrtf(discriminant)) / (2 * a);
    if (t1 > 0.001f) return t1;
    
    float t2 = (-b + sqrtf(discriminant)) / (2 * a);
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
    
    if (hit_sphere == -1) return scene->background;
    
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
