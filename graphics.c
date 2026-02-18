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
    "    FragColor = texture(screenTexture, TexCoord);\n"
    "}\n";

/* Compute shader loading */
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

unsigned int graphics_load_compute_shader(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        char fullpath[256];
        snprintf(fullpath, sizeof(fullpath), "./%s", filename);
        f = fopen(fullpath, "r");
        if (!f) {
            fprintf(stderr, "Cannot open compute shader: %s or %s\n", filename, fullpath);
            return 0;
        }
    }
    
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char *src = malloc(size + 1);
    fread(src, 1, size, f);
    src[size] = 0;
    fclose(f);
    
    unsigned int shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, (const char**)&src, NULL);
    glCompileShader(shader);
    
    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        fprintf(stderr, "Compute shader error: %s\n", infoLog);
        free(src);
        return 0;
    }
    
    unsigned int program = glCreateProgram();
    glAttachShader(program, shader);
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        fprintf(stderr, "Compute program link error: %s\n", infoLog);
    }
    
    glDeleteShader(shader);
    free(src);
    return program;
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
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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
    
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "GLEW init failed\n");
        glfwDestroyWindow(g->window);
        glfwTerminate();
        free(g->pixels);
        free(g);
        return NULL;
    }
    
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    
    /* Load compute shader */
    g->computeProgram = graphics_load_compute_shader("raytrace.comp");
    if (!g->computeProgram) {
        fprintf(stderr, "Failed to load compute shader\n");
        return NULL;
    }
    printf("✓ Compute shader loaded\n");
    
    /* Create output texture */
    glGenTextures(1, &g->outputTexture);
    glBindTexture(GL_TEXTURE_2D, g->outputTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindImageTexture(0, g->outputTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    printf("✓ Output texture created\n");
    
    /* Create display shader */
    g->shaderProgram = graphics_create_program(QUAD_VERT, QUAD_FRAG);
    glUseProgram(g->shaderProgram);
    
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
    
    glGenTextures(1, &g->texture);
    glBindTexture(GL_TEXTURE_2D, g->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    
    int texLoc = glGetUniformLocation(g->shaderProgram, "screenTexture");
    glUniform1i(texLoc, 0);
    
    /* Create SSBOs for scene data */
    glGenBuffers(1, &g->sphereBuffer);
    glGenBuffers(1, &g->lightBuffer);
    
    printf("✓ Graphics initialized (GPU mode)\n");
    return g;
}

void graphics_dispatch_compute(Graphics *g, Scene *scene) {
    glUseProgram(g->computeProgram);
    
    /* Pack spheres as vec4 pairs: (pos.xyz, radius), (color.rgb, 0) */
    float *gpu_spheres = malloc(scene->sphere_count * 8 * sizeof(float));
    for (int i = 0; i < scene->sphere_count; i++) {
        gpu_spheres[i * 8 + 0] = scene->spheres[i].position.x;
        gpu_spheres[i * 8 + 1] = scene->spheres[i].position.y;
        gpu_spheres[i * 8 + 2] = scene->spheres[i].position.z;
        gpu_spheres[i * 8 + 3] = scene->spheres[i].radius;
        gpu_spheres[i * 8 + 4] = scene->spheres[i].color.r;
        gpu_spheres[i * 8 + 5] = scene->spheres[i].color.g;
        gpu_spheres[i * 8 + 6] = scene->spheres[i].color.b;
        gpu_spheres[i * 8 + 7] = 0.0f;
    }
    
    /* Pack lights as vec4 pairs: (pos.xyz, 0), (color.rgb, intensity) */
    float *gpu_lights = malloc(scene->light_count * 8 * sizeof(float));
    for (int i = 0; i < scene->light_count; i++) {
        gpu_lights[i * 8 + 0] = scene->lights[i].position.x;
        gpu_lights[i * 8 + 1] = scene->lights[i].position.y;
        gpu_lights[i * 8 + 2] = scene->lights[i].position.z;
        gpu_lights[i * 8 + 3] = 0.0f;
        gpu_lights[i * 8 + 4] = scene->lights[i].color.r;
        gpu_lights[i * 8 + 5] = scene->lights[i].color.g;
        gpu_lights[i * 8 + 6] = scene->lights[i].color.b;
        gpu_lights[i * 8 + 7] = scene->lights[i].intensity;
    }
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, g->sphereBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, scene->sphere_count * 8 * sizeof(float), gpu_spheres, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, g->sphereBuffer);
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, g->lightBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, scene->light_count * 8 * sizeof(float), gpu_lights, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, g->lightBuffer);
    
    free(gpu_spheres);
    free(gpu_lights);
    
    /* Bind output texture */
    glBindImageTexture(0, g->outputTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    
    /* Set uniforms */
    glUniform1i(glGetUniformLocation(g->computeProgram, "sphere_count"), scene->sphere_count);
    glUniform1i(glGetUniformLocation(g->computeProgram, "light_count"), scene->light_count);
    glUniform3f(glGetUniformLocation(g->computeProgram, "background"), 
                scene->background.x, scene->background.y, scene->background.z);
    
    /* Dispatch */
    glDispatchCompute((g->width + 15) / 16, (g->height + 15) / 16, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
}

void graphics_destroy(Graphics *g) {
    if (!g) return;
    glDeleteProgram(g->shaderProgram);
    glDeleteProgram(g->computeProgram);
    glDeleteBuffers(1, &g->VBO);
    glDeleteBuffers(1, &g->EBO);
    glDeleteBuffers(1, &g->sphereBuffer);
    glDeleteBuffers(1, &g->lightBuffer);
    glDeleteVertexArrays(1, &g->VAO);
    glDeleteTextures(1, &g->texture);
    glDeleteTextures(1, &g->outputTexture);
    glfwDestroyWindow(g->window);
    glfwTerminate();
    free(g->pixels);
    free(g);
}

void graphics_update_texture(Graphics *g, const unsigned int *framebuffer) {
    glBindTexture(GL_TEXTURE_2D, g->texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, g->width, g->height, GL_RGBA, GL_UNSIGNED_BYTE, framebuffer);
}

void graphics_render_quad(Graphics *g) {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(g->shaderProgram);
    glBindVertexArray(g->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g->outputTexture);
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
