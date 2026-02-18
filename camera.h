#ifndef CAMERA_H
#define CAMERA_H

#include "math.h"

typedef struct {
    Vec3 position;
    Vec3 direction;
    Vec3 up;
    float fov;
    float aspect_ratio;
} Camera;

Camera camera_create(Vec3 position, Vec3 target, Vec3 up, float fov, float aspect);
void camera_move(Camera *cam, Vec3 delta);
void camera_rotate(Camera *cam, float yaw, float pitch);
Vec3 camera_get_ray_direction(Camera *cam, float x, float y);

#endif
