#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define MAX_SPHERES 256
#define MAX_LIGHTS 16

/* Shared scene structure - used by main and graphics */
typedef struct {
    /* Spheres: 2 vec4 per sphere → [i*8+0..3] = (px,py,pz,radius), [i*8+4..7] = (r,g,b,0) */
    float sphere_data[MAX_SPHERES * 2 * 4];
    int sphere_count;

    /* Lights: 2 vec4 per light → [i*8+0..3] = (px,py,pz,0), [i*8+4..7] = (r,g,b,intensity) */
    float light_data[MAX_LIGHTS * 2 * 4];
    int light_count;

    float camera_pos[3];
    float camera_forward[3];
    float camera_up[3];
} Scene;

typedef struct {
    GLFWwindow *window;
    unsigned int VAO, VBO, EBO;
    unsigned int shaderProgram;
    unsigned int computeProgram;
    unsigned int outputTexture;
    unsigned int sphereBuffer;
    unsigned int lightBuffer;
    unsigned int texture;
    int width;
    int height;
    unsigned int *pixels;
} Graphics;

Graphics* graphics_create(int width, int height, const char *title);
void graphics_destroy(Graphics *g);
void graphics_update_texture(Graphics *g, const unsigned int *framebuffer);
void graphics_render_quad(Graphics *g);
void graphics_render_with_overlay(Graphics *g, int show_help);
void graphics_dispatch_compute(Graphics *g, Scene *scene);
void graphics_dispatch_compute_ex(Graphics *g, Scene *scene, float fov);
void graphics_draw_overlay(Graphics *g);
int graphics_should_close(Graphics *g);
void graphics_swap_buffers(Graphics *g);
void graphics_poll_events(void);

unsigned int graphics_compile_shader(const char *src, unsigned int type);
unsigned int graphics_create_program(const char *vert_src, const char *frag_src);

#endif
