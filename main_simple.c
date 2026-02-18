#include "graphics.h"
#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include <time.h>

/* sphere_data[i*2+1].w = metallic (0..1) */
static void scene_set_sphere(Scene *s, int idx, float px, float py, float pz,
    float radius, float r, float g, float b, float metallic) {
    int base = idx * 8;
    s->sphere_data[base+0] = px;  s->sphere_data[base+1] = py;
    s->sphere_data[base+2] = pz;  s->sphere_data[base+3] = radius;
    s->sphere_data[base+4] = r;   s->sphere_data[base+5] = g;
    s->sphere_data[base+6] = b;   s->sphere_data[base+7] = metallic;
}

static void scene_set_light(Scene *s, int idx, float px, float py, float pz,
    float r, float g, float b, float intensity) {
    int base = idx * 8;
    s->light_data[base+0] = px;  s->light_data[base+1] = py;
    s->light_data[base+2] = pz;  s->light_data[base+3] = 0.0f;
    s->light_data[base+4] = r;   s->light_data[base+5] = g;
    s->light_data[base+6] = b;   s->light_data[base+7] = intensity;
}

/* Input state */
volatile int input_forward = 0, input_backward = 0;
volatile int input_left = 0, input_right = 0;
volatile int light_move_left = 0, light_move_right = 0;
volatile int light_move_up = 0, light_move_down = 0;

float current_fov = 60.0f;
float light_intensity = 1.5f;
int show_help = 1;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    (void)scancode; (void)mods;
    int state = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_W) input_forward = state;
    if (key == GLFW_KEY_S) input_backward = state;
    if (key == GLFW_KEY_A) input_left = state;
    if (key == GLFW_KEY_D) input_right = state;
    if (key == GLFW_KEY_LEFT) light_move_left = state;
    if (key == GLFW_KEY_RIGHT) light_move_right = state;
    if (key == GLFW_KEY_UP) light_move_up = state;
    if (key == GLFW_KEY_DOWN) light_move_down = state;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
        show_help = !show_help;
    if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) light_intensity += 0.2f;
    if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) light_intensity -= 0.2f;
    if (light_intensity < 0.1f) light_intensity = 0.1f;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    (void)window; (void)xoffset;
    current_fov -= (float)yoffset * 2.0f;
    if (current_fov < 20.0f) current_fov = 20.0f;
    if (current_fov > 120.0f) current_fov = 120.0f;
}

int main(void) {
    printf("=== Vibe Tracing - GPU Raytracer (Phase 2+3) ===\n");
    printf("Controls:\n");
    printf("  WASD       = Move camera\n");
    printf("  Mouse      = Look around\n");
    printf("  Scroll     = Zoom (FOV)\n");
    printf("  Arrows     = Move light\n");
    printf("  +/-  (Pad) = Light intensity\n");
    printf("  ESC        = Quit\n\n");

    Graphics *graphics = graphics_create(1920, 1080, "Vibe Tracing - GPU");
    if (!graphics) { printf("Failed to create graphics!\n"); return 1; }

    glfwSetKeyCallback(graphics->window, key_callback);
    glfwSetScrollCallback(graphics->window, scroll_callback);
    glfwSetInputMode(graphics->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Scene scene = {0};

    /*                           px     py      pz    rad    r     g     b    metallic */
    /* Shiny metallic red */
    scene_set_sphere(&scene, 0, -1.5f,  0.5f, -3.0f, 0.5f, 0.9f, 0.1f, 0.1f, 0.9f);
    /* Matte green */
    scene_set_sphere(&scene, 1,  0.0f,  0.3f, -2.5f, 0.3f, 0.1f, 0.8f, 0.1f, 0.05f);
    /* Metallic blue */
    scene_set_sphere(&scene, 2,  1.5f,  0.5f, -3.5f, 0.5f, 0.1f, 0.2f, 0.9f, 0.7f);
    /* Gold sphere */
    scene_set_sphere(&scene, 3,  0.5f,  0.8f, -4.0f, 0.8f, 1.0f, 0.84f,0.0f, 0.95f);
    /* Small cyan */
    scene_set_sphere(&scene, 4, -0.5f,  0.15f,-1.8f, 0.15f,0.0f, 0.8f, 0.8f, 0.3f);
    /* Purple */
    scene_set_sphere(&scene, 5,  2.5f,  0.3f, -2.0f, 0.3f, 0.6f, 0.1f, 0.8f, 0.4f);
    /* Orange */
    scene_set_sphere(&scene, 6, -2.0f,  0.25f,-2.0f, 0.25f,1.0f, 0.5f, 0.0f, 0.2f);
    /* Large mirror sphere */
    scene_set_sphere(&scene, 7,  3.0f,  1.0f, -5.0f, 1.0f, 0.95f,0.95f,0.95f,0.98f);
    /* Glass sphere (metallic = -1.0 signals refraction in shader) */
    scene_set_sphere(&scene, 8, -0.3f,  0.5f, -3.5f, 0.5f, 0.95f,0.95f,1.0f, -1.0f);
    /* Ground (matte gray checkerboard handled in shader) */
    scene_set_sphere(&scene, 9,  0.0f,-100.0f,-3.0f,100.0f,0.4f, 0.4f, 0.4f, 0.0f);
    scene.sphere_count = 10;

    /* 2 lights for richer illumination */
    scene_set_light(&scene, 0,  3.0f, 4.0f,  1.0f, 1.0f, 0.95f, 0.9f, 1.5f);
    scene_set_light(&scene, 1, -2.0f, 3.0f, -1.0f, 0.4f, 0.5f,  0.9f, 0.8f);
    scene.light_count = 2;

    /* Camera */
    scene.camera_pos[0] = 0.0f; scene.camera_pos[1] = 1.0f; scene.camera_pos[2] = 3.0f;
    scene.camera_forward[0] = 0.0f; scene.camera_forward[1] = -0.1f; scene.camera_forward[2] = -1.0f;
    scene.camera_up[0] = 0.0f; scene.camera_up[1] = 1.0f; scene.camera_up[2] = 0.0f;

    float yaw = -3.14159f / 2.0f, pitch = -0.1f;
    double last_mx = 0, last_my = 0;
    int first_mouse = 1;

    int frame_count = 0;
    time_t last_time = time(NULL);

    while (!graphics_should_close(graphics)) {
        graphics_poll_events();

        /* Mouse look */
        double mx, my;
        glfwGetCursorPos(graphics->window, &mx, &my);
        if (first_mouse) { last_mx = mx; last_my = my; first_mouse = 0; }
        float dx = (float)(mx - last_mx) * 0.003f;
        float dy = (float)(last_my - my) * 0.003f;
        last_mx = mx; last_my = my;
        yaw += dx; pitch += dy;
        if (pitch > 1.5f) pitch = 1.5f;
        if (pitch < -1.5f) pitch = -1.5f;

        scene.camera_forward[0] = cosf(pitch) * cosf(yaw);
        scene.camera_forward[1] = sinf(pitch);
        scene.camera_forward[2] = cosf(pitch) * sinf(yaw);

        /* WASD movement */
        float speed = 0.05f;
        float fx = scene.camera_forward[0], fz = scene.camera_forward[2];
        float fl = sqrtf(fx*fx + fz*fz); if (fl > 0) { fx/=fl; fz/=fl; }
        if (input_forward)  { scene.camera_pos[0]+=fx*speed; scene.camera_pos[2]+=fz*speed; }
        if (input_backward) { scene.camera_pos[0]-=fx*speed; scene.camera_pos[2]-=fz*speed; }
        if (input_left)     { scene.camera_pos[0]+=fz*speed; scene.camera_pos[2]-=fx*speed; }
        if (input_right)    { scene.camera_pos[0]-=fz*speed; scene.camera_pos[2]+=fx*speed; }

        /* Arrow keys: move light 0 */
        float ls = 0.05f;
        if (light_move_left)  scene.light_data[0] -= ls;
        if (light_move_right) scene.light_data[0] += ls;
        if (light_move_up)    scene.light_data[1] += ls;
        if (light_move_down)  scene.light_data[1] -= ls;

        /* Update light intensity from Phase 3 controls */
        scene.light_data[7] = light_intensity;

        graphics_dispatch_compute_ex(graphics, &scene, current_fov);
        graphics_render_with_overlay(graphics, show_help);
        graphics_swap_buffers(graphics);

        frame_count++;
        time_t now = time(NULL);
        if (now != last_time) {
            char title[256];
            snprintf(title, sizeof(title),
                "Vibe Tracing - GPU | FPS: %d | FOV: %.0f | Light: %.1f",
                frame_count, current_fov, light_intensity);
            glfwSetWindowTitle(graphics->window, title);
            frame_count = 0;
            last_time = now;
        }
    }

    graphics_destroy(graphics);
    printf("Goodbye!\n");
    return 0;
}
