#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "math.h"
#include <GLFW/glfw3.h>

typedef struct {
    GLFWwindow *window;
    unsigned int VAO, VBO, EBO;
    unsigned int shaderProgram;
    unsigned int texture;
    int width;
    int height;
    unsigned int *pixels;
} Graphics;

Graphics* graphics_create(int width, int height, const char *title);
void graphics_destroy(Graphics *g);
void graphics_update_texture(Graphics *g, const unsigned int *framebuffer);
void graphics_render_quad(Graphics *g);
int graphics_should_close(Graphics *g);
void graphics_swap_buffers(Graphics *g);
void graphics_poll_events(void);

unsigned int graphics_compile_shader(const char *src, unsigned int type);
unsigned int graphics_create_program(const char *vert_src, const char *frag_src);

#endif
