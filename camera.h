#ifndef CAMERA_H
#define CAMERA_H

#include "math.h"

typedef struct {
    Vec3 position;
    Vec3 forward;
    Vec3 right;
    Vec3 up;
} Camera;

Camera camera_create(Vec3 pos, Vec3 forward);
void camera_move(Camera *cam, Vec3 offset);
void camera_rotate(Camera *cam, float yaw, float pitch);

#endif // CAMERA_H
