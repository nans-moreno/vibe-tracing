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
volatile int light_move_left = 0, light_move_right = 0;
volatile int light_move_up = 0, light_move_down = 0;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    int state = (action != GLFW_RELEASE);
    
    if (key == GLFW_KEY_W) input_forward = state;
    if (key == GLFW_KEY_S) input_backward = state;
    if (key == GLFW_KEY_A) input_left = state;
    if (key == GLFW_KEY_D) input_right = state;
    if (key == GLFW_KEY_SPACE) input_up = state;
    if (key == GLFW_KEY_LEFT_SHIFT) input_down = state;
    if (key == GLFW_KEY_LEFT) light_move_left = state;
    if (key == GLFW_KEY_RIGHT) light_move_right = state;
    if (key == GLFW_KEY_UP) light_move_up = state;
    if (key == GLFW_KEY_DOWN) light_move_down = state;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(void) {
    printf("=== Vibe Tracing - GPU Ray Tracer ===\n");
    printf("Controls:\n");
    printf("  WASD - Move\n");
    printf("  SPACE - Up\n");
    printf("  SHIFT - Down\n");
    printf("  Left Click + Mouse - Look around\n");
    printf("  Arrow Keys - Move light\n");
    printf("  ESC - Exit\n");
    printf("Resolution: %dx%d (GPU)\n\n", SCREEN_WIDTH, SCREEN_HEIGHT);
    
    Graphics *graphics = graphics_create(SCREEN_WIDTH, SCREEN_HEIGHT, "Vibe Tracing - GPU");
    if (!graphics) return 1;
    
    UI ui = ui_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    Scene scene = scene_create();
    
    scene_add_sphere(&scene, vec3(0, 0, -5), 1.0f, (Color){1.0f, 0.3f, 0.3f});
    scene_add_sphere(&scene, vec3(-3, 0, -8), 1.5f, (Color){0.3f, 1.0f, 0.3f});
    scene_add_sphere(&scene, vec3(3, 0, -8), 1.2f, (Color){0.3f, 0.3f, 1.0f});
    scene_add_sphere(&scene, vec3(0, -100, 0), 98.0f, (Color){0.8f, 0.8f, 0.8f});
    
    scene_add_light(&scene, vec3(5, 10, 0), (Color){1.0f, 1.0f, 1.0f}, 1.5f);
    scene_add_light(&scene, vec3(-5, 5, 5), (Color){0.8f, 0.8f, 1.0f}, 0.8f);
    
    Camera camera = camera_create(
        vec3(0, 2, 0),
        vec3(0, 0, -5),
        vec3(0, 1, 0),
        CAMERA_FOV,
        (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT
    );
    
    glfwSetKeyCallback(graphics->window, key_callback);
    glfwSetInputMode(graphics->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    double last_mouse_x = 0, last_mouse_y = 0;
    glfwGetCursorPos(graphics->window, &last_mouse_x, &last_mouse_y);
    
    double last_time = glfwGetTime();
    int frame_count = 0;
    double last_fps_time = last_time;
    
    printf("GPU Rendering started...\n");
    
    while (!graphics_should_close(graphics)) {
        double current_time = glfwGetTime();
        double delta = current_time - last_time;
        last_time = current_time;
        
        /* Camera rotation - always active */
        double mouse_x, mouse_y;
        glfwGetCursorPos(graphics->window, &mouse_x, &mouse_y);
        
        float yaw = (float)(mouse_x - last_mouse_x) * CAMERA_SENSITIVITY;
        float pitch = (float)(mouse_y - last_mouse_y) * CAMERA_SENSITIVITY;
        if (yaw != 0 || pitch != 0) {
            camera_rotate(&camera, yaw, pitch);
        }
        
        last_mouse_x = mouse_x;
        last_mouse_y = mouse_y;
        
        Vec3 movement = {0, 0, 0};
        if (input_forward) movement = vec3_add(movement, vec3_mul(camera.direction, CAMERA_SPEED * delta));
        if (input_backward) movement = vec3_sub(movement, vec3_mul(camera.direction, CAMERA_SPEED * delta));
        
        Vec3 right = vec3_normalize(vec3_cross(camera.direction, camera.up));
        if (input_right) movement = vec3_add(movement, vec3_mul(right, CAMERA_SPEED * delta));
        if (input_left) movement = vec3_sub(movement, vec3_mul(right, CAMERA_SPEED * delta));
        
        if (input_up) movement = vec3_add(movement, vec3_mul(camera.up, CAMERA_SPEED * delta));
        if (input_down) movement = vec3_sub(movement, vec3_mul(camera.up, CAMERA_SPEED * delta));
        
        camera_move(&camera, movement);
        
        /* Move light with arrow keys */
        float light_speed = 5.0f * (float)delta;
        if (light_move_left) scene.lights[0].position.x -= light_speed;
        if (light_move_right) scene.lights[0].position.x += light_speed;
        if (light_move_up) scene.lights[0].position.y += light_speed;
        if (light_move_down) scene.lights[0].position.y -= light_speed;
        
        /* Compute raytracing on GPU */
        glUseProgram(graphics->computeProgram);
        glUniform3f(glGetUniformLocation(graphics->computeProgram, "camera_pos"), 
                    camera.position.x, camera.position.y, camera.position.z);
        glUniform3f(glGetUniformLocation(graphics->computeProgram, "camera_dir"), 
                    camera.direction.x, camera.direction.y, camera.direction.z);
        glUniform3f(glGetUniformLocation(graphics->computeProgram, "camera_up"), 
                    camera.up.x, camera.up.y, camera.up.z);
        glUniform1f(glGetUniformLocation(graphics->computeProgram, "fov"), CAMERA_FOV);
        glUniform1f(glGetUniformLocation(graphics->computeProgram, "aspect"), 
                    (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
        
        graphics_dispatch_compute(graphics, &scene);
        graphics_render_quad(graphics);
        graphics_swap_buffers(graphics);
        graphics_poll_events();
        
        frame_count++;
        if (current_time - last_fps_time >= 1.0) {
            char title[256];
            snprintf(title, sizeof(title), "Vibe Tracing - GPU | FPS: %d", frame_count);
            glfwSetWindowTitle(graphics->window, title);
            printf("FPS: %d\n", frame_count);
            frame_count = 0;
            last_fps_time = current_time;
        }
    }
    
    graphics_destroy(graphics);
    ui_destroy(&ui);
    
    return 0;
}
