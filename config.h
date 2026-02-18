#ifndef CONFIG_H
#define CONFIG_H

/* Display resolution */
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

/* Ray tracing */
#define MAX_RAY_DEPTH 2
#define RENDER_INTERVAL 0.1f

/* Camera */
#define CAMERA_FOV 45.0f
#define CAMERA_SPEED 5.0f
#define CAMERA_SENSITIVITY 0.005f

/* Scene */
#define MAX_SPHERES 100
#define MAX_LIGHTS 10

/* Performance */
#define ENABLE_MULTITHREADING 0
#define NUM_THREADS 4

#endif
