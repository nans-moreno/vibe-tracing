#ifndef CONFIG_H
#define CONFIG_H

/* Display resolution */
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

/* Ray tracing */
#define MAX_RAY_DEPTH 2
#define RENDER_INTERVAL 0.5f

/* Camera */
#define CAMERA_FOV 45.0f
#define CAMERA_SPEED 5.0f
#define CAMERA_SENSITIVITY 0.005f

/* Scene */
#define MAX_SPHERES 100
#define MAX_LIGHTS 10

/* Performance */
#define ENABLE_MULTITHREADING 1
#define NUM_THREADS 4

#endif
