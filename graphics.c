#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *QUAD_VERT = "#version 330 core\n"
    "layout(location = 0) in vec2 pos;\n"
    "layout(location = 1) in vec2 tex;\n"
    "out vec2 TexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(pos, 0.0, 1.0);\n"
    "    TexCoord = tex;\n"
    "}\n";

const char *QUAD_FRAG = "#version 330 core\n"
    "in vec2 TexCoord;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D screenTexture;\n"
    "void main() {\n"
    "    vec3 col = texture(screenTexture, TexCoord).rgb;\n"
    "    col = col / (col + vec3(1.0));\n"
    "    col = pow(col, vec3(1.0 / 2.2));\n"
    "    FragColor = vec4(col, 1.0);\n"
    "}\n";

unsigned int graphics_compile_shader(const char *src, unsigned int type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "Shader compile error: %s\n", infoLog);
    }
    return shader;
}

unsigned int graphics_create_program(const char *vert_src, const char *frag_src) {
    unsigned int vert = graphics_compile_shader(vert_src, GL_VERTEX_SHADER);
    unsigned int frag = graphics_compile_shader(frag_src, GL_FRAGMENT_SHADER);
    
    unsigned int program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr, "Program link error: %s\n", infoLog);
    }
    
    glDeleteShader(vert);
    glDeleteShader(frag);
    return program;
}

Graphics* graphics_create(int width, int height, const char *title) {
    Graphics *g = malloc(sizeof(Graphics));
    g->width = width;
    g->height = height;
    g->pixels = malloc(width * height * sizeof(unsigned int));
    memset(g->pixels, 0, width * height * sizeof(unsigned int));
    
    if (!glfwInit()) {
        fprintf(stderr, "GLFW init failed\n");
        free(g->pixels);
        free(g);
        return NULL;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    g->window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!g->window) {
        fprintf(stderr, "Window creation failed\n");
        glfwTerminate();
        free(g->pixels);
        free(g);
        return NULL;
    }
    
    glfwMakeContextCurrent(g->window);
    glfwSwapInterval(1);
    
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    
    /* Create shader program */
    g->shaderProgram = graphics_create_program(QUAD_VERT, QUAD_FRAG);
    glUseProgram(g->shaderProgram);
    
    /* Setup quad geometry */
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f
    };
    
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
    
    glGenVertexArrays(1, &g->VAO);
    glGenBuffers(1, &g->VBO);
    glGenBuffers(1, &g->EBO);
    
    glBindVertexArray(g->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, g->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    /* Create texture */
    glGenTextures(1, &g->texture);
    glBindTexture(GL_TEXTURE_2D, g->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    
    int texLoc = glGetUniformLocation(g->shaderProgram, "screenTexture");
    glUniform1i(texLoc, 0);
    
    return g;
}

void graphics_destroy(Graphics *g) {
    if (!g) return;
    glDeleteProgram(g->shaderProgram);
    glDeleteBuffers(1, &g->VBO);
    glDeleteBuffers(1, &g->EBO);
    glDeleteVertexArrays(1, &g->VAO);
    glDeleteTextures(1, &g->texture);
    glfwDestroyWindow(g->window);
    glfwTerminate();
    free(g->pixels);
    free(g);
}

void graphics_update_texture(Graphics *g, const unsigned int *framebuffer) {
    glBindTexture(GL_TEXTURE_2D, g->texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, g->width, g->height, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);
}

void graphics_render_quad(Graphics *g) {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(g->shaderProgram);
    glBindVertexArray(g->VAO);
    glBindTexture(GL_TEXTURE_2D, g->texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int graphics_should_close(Graphics *g) {
    return glfwWindowShouldClose(g->window);
}

void graphics_swap_buffers(Graphics *g) {
    glfwSwapBuffers(g->window);
}

void graphics_poll_events(void) {
    glfwPollEvents();
}
