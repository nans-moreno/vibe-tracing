#include "raytracer.h"
#include "camera.h"
#include "scene.h"
#include "ui.h"
#include "graphics.h"
#include "config.h"
#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>

volatile int input_forward = 0, input_backward = 0;
volatile int input_left = 0, input_right = 0;
volatile int input_up = 0, input_down = 0;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    int state = (action != GLFW_RELEASE);
    
    if (key == GLFW_KEY_W) input_forward = state;
    if (key == GLFW_KEY_S) input_backward = state;
    if (key == GLFW_KEY_A) input_left = state;
    if (key == GLFW_KEY_D) input_right = state;
    if (key == GLFW_KEY_SPACE) input_up = state;
    if (key == GLFW_KEY_LEFT_SHIFT) input_down = state;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(void) {
    printf("=== Vibe Tracing - Interactive Ray Tracer ===\n");
    printf("Controls:\n");
    printf("  WASD - Move\n");
    printf("  SPACE - Up\n");
    printf("  SHIFT - Down\n");
    printf("  Mouse - Look around\n");
    printf("  ESC - Exit\n");
    
    /* Create graphics */
    Graphics *graphics = graphics_create(SCREEN_WIDTH, SCREEN_HEIGHT, "Vibe Tracing");
    if (!graphics) return 1;
    
    /* Initialize UI */
    UI ui = ui_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    /* Create scene */
    Scene scene = scene_create();
    
    /* Add geometry */
    scene_add_sphere(&scene, vec3(0, 0, -5), 1.0f, (Color){1.0f, 0.3f, 0.3f});
    scene_add_sphere(&scene, vec3(-3, 0, -8), 1.5f, (Color){0.3f, 1.0f, 0.3f});
    scene_add_sphere(&scene, vec3(3, 0, -8), 1.2f, (Color){0.3f, 0.3f, 1.0f});
    scene_add_sphere(&scene, vec3(0, -100, 0), 98.0f, (Color){0.8f, 0.8f, 0.8f});
    
    /* Add lights */
    scene_add_light(&scene, vec3(5, 10, 0), (Color){1.0f, 1.0f, 1.0f}, 1.5f);
    scene_add_light(&scene, vec3(-5, 5, 5), (Color){0.8f, 0.8f, 1.0f}, 0.8f);
    
    /* Create camera */
    Camera camera = camera_create(
        vec3(0, 2, 0),
        vec3(0, 0, -5),
        vec3(0, 1, 0),
        CAMERA_FOV,
        (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT
    );
    
    /* Input setup */
    glfwSetKeyCallback(graphics->window, key_callback);
    glfwSetInputMode(graphics->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    /* Main loop */
    double last_time = glfwGetTime();
    double last_render = last_time;
    int frame_count = 0;
    
    printf("Starting render loop...\n");
    
    while (!graphics_should_close(graphics)) {
        double current_time = glfwGetTime();
        double delta = current_time - last_time;
        last_time = current_time;
        
        /* Input handling */
        if (glfwGetMouseButton(graphics->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            double mouse_x, mouse_y;
            glfwGetCursorPos(graphics->window, &mouse_x, &mouse_y);
            static double last_x = 0, last_y = 0;
            static int first = 1;
            
            if (!first) {
                float yaw = (mouse_x - last_x) * CAMERA_SENSITIVITY;
                float pitch = (mouse_y - last_y) * CAMERA_SENSITIVITY;
                camera_rotate(&camera, yaw, pitch);
            }
            first = 0;
            last_x = mouse_x;
            last_y = mouse_y;
        }
        
        Vec3 movement = {0, 0, 0};
        
        if (input_forward) movement = vec3_add(movement, vec3_mul(camera.direction, CAMERA_SPEED * delta));
        if (input_backward) movement = vec3_sub(movement, vec3_mul(camera.direction, CAMERA_SPEED * delta));
        
        Vec3 right = vec3_normalize(vec3_cross(camera.direction, camera.up));
        if (input_right) movement = vec3_add(movement, vec3_mul(right, CAMERA_SPEED * delta));
        if (input_left) movement = vec3_sub(movement, vec3_mul(right, CAMERA_SPEED * delta));
        
        if (input_up) movement = vec3_add(movement, vec3_mul(camera.up, CAMERA_SPEED * delta));
        if (input_down) movement = vec3_sub(movement, vec3_mul(camera.up, CAMERA_SPEED * delta));
        
        camera_move(&camera, movement);
        
        /* Render every N ms */
        if (current_time - last_render >= RENDER_INTERVAL) {
            for (int y = 0; y < SCREEN_HEIGHT; y++) {
                for (int x = 0; x < SCREEN_WIDTH; x++) {
                    float px = (float)x / SCREEN_WIDTH;
                    float py = (float)y / SCREEN_HEIGHT;
                    
                    Vec3 ray_dir = camera_get_ray_direction(&camera, px, py);
                    Ray ray = ray_create(camera.position, ray_dir);
                    
                    Color col = ray_trace(ray, &scene, MAX_RAY_DEPTH);
                    ui_set_pixel(&ui, x, y, col.r, col.g, col.b);
                }
            }
            
            graphics_update_texture(graphics, ui.framebuffer);
            last_render = current_time;
            frame_count++;
            
            if (frame_count % 10 == 0) {
                printf("Frame %d rendered\n", frame_count);
            }
        }
        
        graphics_render_quad(graphics);
        graphics_swap_buffers(graphics);
        graphics_poll_events();
    }
    
    printf("Total frames: %d\n", frame_count);
    
    /* Cleanup */
    ui_destroy(&ui);
    scene_destroy(&scene);
    graphics_destroy(graphics);
    
    printf("Done!\n");
    return 0;
}
