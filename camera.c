#include "camera.h"
#include <math.h>

Camera camera_create(Vec3 position, Vec3 target, Vec3 up, float fov, float aspect) {
    Vec3 direction = vec3_normalize(vec3_sub(target, position));
    return (Camera){position, direction, up, fov, aspect};
}

void camera_move(Camera *cam, Vec3 delta) {
    cam->position = vec3_add(cam->position, delta);
}

void camera_rotate(Camera *cam, float yaw, float pitch) {
    float cy = cosf(yaw);
    float sy = sinf(yaw);
    float cp = cosf(pitch);
    float sp = sinf(pitch);
    
    Vec3 right = vec3_cross(cam->direction, cam->up);
    right = vec3_normalize(right);
    
    Vec3 new_up = vec3_cross(right, cam->direction);
    new_up = vec3_normalize(new_up);
    
    Vec3 dir = cam->direction;
    dir = vec3_normalize(vec3_add(
        vec3_mul(right, sy * cp),
        vec3_add(vec3_mul(dir, cy * cp), vec3_mul(new_up, sp))
    ));
    
    cam->direction = dir;
    cam->up = new_up;
}

Vec3 camera_get_ray_direction(Camera *cam, float x, float y) {
    float fov_rad = cam->fov * 3.14159f / 180.0f;
    float h = tanf(fov_rad / 2.0f);
    float w = h * cam->aspect_ratio;
    
    Vec3 right = vec3_normalize(vec3_cross(cam->direction, cam->up));
    Vec3 up = vec3_normalize(vec3_cross(right, cam->direction));
    
    Vec3 ray = vec3_add(
        cam->direction,
        vec3_add(
            vec3_mul(right, (2.0f * x - 1.0f) * w),
            vec3_mul(up, (2.0f * (1.0f - y) - 1.0f) * h)
        )
    );
    
    return vec3_normalize(ray);
}
