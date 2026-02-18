#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- 5x7 bitmap font for ASCII 32-127 ---- */
static const unsigned char font5x7[][7] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* 32 space */
    {0x04,0x04,0x04,0x04,0x00,0x04,0x00}, /* ! */
    {0x0A,0x0A,0x00,0x00,0x00,0x00,0x00}, /* " */
    {0x0A,0x1F,0x0A,0x1F,0x0A,0x00,0x00}, /* # */
    {0x04,0x0F,0x14,0x0E,0x05,0x1E,0x04}, /* $ */
    {0x18,0x19,0x02,0x04,0x08,0x13,0x03}, /* % */
    {0x08,0x14,0x14,0x08,0x15,0x12,0x0D}, /* & */
    {0x04,0x04,0x00,0x00,0x00,0x00,0x00}, /* ' */
    {0x02,0x04,0x08,0x08,0x08,0x04,0x02}, /* ( */
    {0x08,0x04,0x02,0x02,0x02,0x04,0x08}, /* ) */
    {0x00,0x0A,0x04,0x1F,0x04,0x0A,0x00}, /* * */
    {0x00,0x04,0x04,0x1F,0x04,0x04,0x00}, /* + */
    {0x00,0x00,0x00,0x00,0x04,0x04,0x08}, /* , */
    {0x00,0x00,0x00,0x1F,0x00,0x00,0x00}, /* - */
    {0x00,0x00,0x00,0x00,0x00,0x04,0x00}, /* . */
    {0x01,0x01,0x02,0x04,0x08,0x10,0x10}, /* / */
    {0x0E,0x11,0x13,0x15,0x19,0x11,0x0E}, /* 0 */
    {0x04,0x0C,0x04,0x04,0x04,0x04,0x0E}, /* 1 */
    {0x0E,0x11,0x01,0x06,0x08,0x10,0x1F}, /* 2 */
    {0x0E,0x11,0x01,0x06,0x01,0x11,0x0E}, /* 3 */
    {0x02,0x06,0x0A,0x12,0x1F,0x02,0x02}, /* 4 */
    {0x1F,0x10,0x1E,0x01,0x01,0x11,0x0E}, /* 5 */
    {0x06,0x08,0x10,0x1E,0x11,0x11,0x0E}, /* 6 */
    {0x1F,0x01,0x02,0x04,0x08,0x08,0x08}, /* 7 */
    {0x0E,0x11,0x11,0x0E,0x11,0x11,0x0E}, /* 8 */
    {0x0E,0x11,0x11,0x0F,0x01,0x02,0x0C}, /* 9 */
    {0x00,0x04,0x00,0x00,0x04,0x00,0x00}, /* : */
    {0x00,0x04,0x00,0x00,0x04,0x04,0x08}, /* ; */
    {0x02,0x04,0x08,0x10,0x08,0x04,0x02}, /* < */
    {0x00,0x00,0x1F,0x00,0x1F,0x00,0x00}, /* = */
    {0x08,0x04,0x02,0x01,0x02,0x04,0x08}, /* > */
    {0x0E,0x11,0x01,0x06,0x04,0x00,0x04}, /* ? */
    {0x0E,0x11,0x17,0x15,0x17,0x10,0x0E}, /* @ */
    {0x0E,0x11,0x11,0x1F,0x11,0x11,0x11}, /* A */
    {0x1E,0x11,0x11,0x1E,0x11,0x11,0x1E}, /* B */
    {0x0E,0x11,0x10,0x10,0x10,0x11,0x0E}, /* C */
    {0x1E,0x11,0x11,0x11,0x11,0x11,0x1E}, /* D */
    {0x1F,0x10,0x10,0x1E,0x10,0x10,0x1F}, /* E */
    {0x1F,0x10,0x10,0x1E,0x10,0x10,0x10}, /* F */
    {0x0E,0x11,0x10,0x17,0x11,0x11,0x0F}, /* G */
    {0x11,0x11,0x11,0x1F,0x11,0x11,0x11}, /* H */
    {0x0E,0x04,0x04,0x04,0x04,0x04,0x0E}, /* I */
    {0x07,0x02,0x02,0x02,0x02,0x12,0x0C}, /* J */
    {0x11,0x12,0x14,0x18,0x14,0x12,0x11}, /* K */
    {0x10,0x10,0x10,0x10,0x10,0x10,0x1F}, /* L */
    {0x11,0x1B,0x15,0x15,0x11,0x11,0x11}, /* M */
    {0x11,0x19,0x15,0x13,0x11,0x11,0x11}, /* N */
    {0x0E,0x11,0x11,0x11,0x11,0x11,0x0E}, /* O */
    {0x1E,0x11,0x11,0x1E,0x10,0x10,0x10}, /* P */
    {0x0E,0x11,0x11,0x11,0x15,0x12,0x0D}, /* Q */
    {0x1E,0x11,0x11,0x1E,0x14,0x12,0x11}, /* R */
    {0x0E,0x11,0x10,0x0E,0x01,0x11,0x0E}, /* S */
    {0x1F,0x04,0x04,0x04,0x04,0x04,0x04}, /* T */
    {0x11,0x11,0x11,0x11,0x11,0x11,0x0E}, /* U */
    {0x11,0x11,0x11,0x11,0x0A,0x0A,0x04}, /* V */
    {0x11,0x11,0x11,0x15,0x15,0x1B,0x11}, /* W */
    {0x11,0x11,0x0A,0x04,0x0A,0x11,0x11}, /* X */
    {0x11,0x11,0x0A,0x04,0x04,0x04,0x04}, /* Y */
    {0x1F,0x01,0x02,0x04,0x08,0x10,0x1F}, /* Z */
    {0x0E,0x08,0x08,0x08,0x08,0x08,0x0E}, /* [ */
    {0x10,0x10,0x08,0x04,0x02,0x01,0x01}, /* \ */
    {0x0E,0x02,0x02,0x02,0x02,0x02,0x0E}, /* ] */
    {0x04,0x0A,0x11,0x00,0x00,0x00,0x00}, /* ^ */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x1F}, /* _ */
    {0x08,0x04,0x00,0x00,0x00,0x00,0x00}, /* ` */
    {0x00,0x00,0x0E,0x01,0x0F,0x11,0x0F}, /* a */
    {0x10,0x10,0x1E,0x11,0x11,0x11,0x1E}, /* b */
    {0x00,0x00,0x0E,0x11,0x10,0x11,0x0E}, /* c */
    {0x01,0x01,0x0F,0x11,0x11,0x11,0x0F}, /* d */
    {0x00,0x00,0x0E,0x11,0x1F,0x10,0x0E}, /* e */
    {0x06,0x08,0x1E,0x08,0x08,0x08,0x08}, /* f */
    {0x00,0x00,0x0F,0x11,0x0F,0x01,0x0E}, /* g */
    {0x10,0x10,0x1E,0x11,0x11,0x11,0x11}, /* h */
    {0x04,0x00,0x0C,0x04,0x04,0x04,0x0E}, /* i */
    {0x02,0x00,0x06,0x02,0x02,0x12,0x0C}, /* j */
    {0x10,0x10,0x12,0x14,0x18,0x14,0x12}, /* k */
    {0x0C,0x04,0x04,0x04,0x04,0x04,0x0E}, /* l */
    {0x00,0x00,0x1A,0x15,0x15,0x11,0x11}, /* m */
    {0x00,0x00,0x1E,0x11,0x11,0x11,0x11}, /* n */
    {0x00,0x00,0x0E,0x11,0x11,0x11,0x0E}, /* o */
    {0x00,0x00,0x1E,0x11,0x1E,0x10,0x10}, /* p */
    {0x00,0x00,0x0F,0x11,0x0F,0x01,0x01}, /* q */
    {0x00,0x00,0x16,0x19,0x10,0x10,0x10}, /* r */
    {0x00,0x00,0x0F,0x10,0x0E,0x01,0x1E}, /* s */
    {0x08,0x08,0x1E,0x08,0x08,0x09,0x06}, /* t */
    {0x00,0x00,0x11,0x11,0x11,0x13,0x0D}, /* u */
    {0x00,0x00,0x11,0x11,0x11,0x0A,0x04}, /* v */
    {0x00,0x00,0x11,0x11,0x15,0x15,0x0A}, /* w */
    {0x00,0x00,0x11,0x0A,0x04,0x0A,0x11}, /* x */
    {0x00,0x00,0x11,0x11,0x0F,0x01,0x0E}, /* y */
    {0x00,0x00,0x1F,0x02,0x04,0x08,0x1F}, /* z */
};

static void draw_char(unsigned int *pixels, int w, int h, int cx, int cy, char ch, unsigned int color) {
    int idx = ch - 32;
    if (idx < 0 || idx >= 91) return;
    for (int row = 0; row < 7; row++) {
        int py = (h - 1) - (cy + row);
        if (py < 0 || py >= h) continue;
        unsigned char bits = font5x7[idx][row];
        for (int col = 0; col < 5; col++) {
            int px = cx + col;
            if (px < 0 || px >= w) continue;
            if (bits & (0x10 >> col))
                pixels[py * w + px] = color;
        }
    }
}

static void draw_string(unsigned int *pixels, int w, int h, int x, int y, const char *str, unsigned int color, int scale) {
    while (*str) {
        /* Draw scaled character */
        int idx = *str - 32;
        if (idx >= 0 && idx < 91) {
            for (int row = 0; row < 7; row++) {
                unsigned char bits = font5x7[idx][row];
                for (int col = 0; col < 5; col++) {
                    if (bits & (0x10 >> col)) {
                        for (int sy = 0; sy < scale; sy++)
                            for (int sx = 0; sx < scale; sx++) {
                                int px = x + col*scale + sx;
                                int py = (h - 1) - (y + row*scale + sy);
                                if (px >= 0 && px < w && py >= 0 && py < h)
                                    pixels[py * w + px] = color;
                            }
                    }
                }
            }
        }
        x += 6 * scale;
        str++;
    }
}

void graphics_draw_overlay(Graphics *g) {
    int w = g->width, h = g->height;
    int panel_w = 220, panel_h = 220;
    int px0 = 10, py0 = 10;

    /* Semi-transparent dark background */
    for (int y = py0; y < py0 + panel_h && y < h; y++)
        for (int x = px0; x < px0 + panel_w && x < w; x++) {
            int flipped_y = (h - 1) - y;
            if (flipped_y >= 0 && flipped_y < h)
                g->pixels[flipped_y * w + x] = 0xBB000000;
        }

    unsigned int white = 0xFFFFFFFF;
    unsigned int yellow = 0xFF00DDFF;
    unsigned int cyan = 0xFFFFDD00;
    int sc = 2;
    int lh = 18;
    int tx = px0 + 8, ty = py0 + 8;

    draw_string(g->pixels, w, h, tx, ty,        "CONTROLS",     yellow, sc); ty += lh + 4;
    draw_string(g->pixels, w, h, tx, ty,        "WASD   Move",  white, sc);  ty += lh;
    draw_string(g->pixels, w, h, tx, ty,        "Mouse  Look",  white, sc);  ty += lh;
    draw_string(g->pixels, w, h, tx, ty,        "Scroll Zoom",  white, sc);  ty += lh;
    draw_string(g->pixels, w, h, tx, ty,        "Arrows Light", white, sc);  ty += lh;
    draw_string(g->pixels, w, h, tx, ty,        "+/-    Power", white, sc);  ty += lh;
    draw_string(g->pixels, w, h, tx, ty,        "H   HideHelp", white, sc);  ty += lh;
    draw_string(g->pixels, w, h, tx, ty,        "ESC    Quit",  white, sc);  ty += lh + 4;
    draw_string(g->pixels, w, h, tx, ty,        "PBR+SHADOWS",  cyan, sc);   ty += lh;
    draw_string(g->pixels, w, h, tx, ty,        "REFLECT+AA",   cyan, sc);
}

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
    graphics_dispatch_compute_ex(g, scene, 60.0f);
}

void graphics_dispatch_compute_ex(Graphics *g, Scene *scene, float fov) {
    glUseProgram(g->computeProgram);

    /* Upload sphere SSBO */
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, g->sphereBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, scene->sphere_count * 2 * 4 * sizeof(float), scene->sphere_data, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, g->sphereBuffer);

    /* Upload light SSBO */
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, g->lightBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, scene->light_count * 2 * 4 * sizeof(float), scene->light_data, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, g->lightBuffer);

    /* Bind output texture */
    glBindImageTexture(0, g->outputTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

    /* Set all uniforms */
    glUniform1i(glGetUniformLocation(g->computeProgram, "sphere_count"), scene->sphere_count);
    glUniform1i(glGetUniformLocation(g->computeProgram, "light_count"), scene->light_count);
    glUniform3f(glGetUniformLocation(g->computeProgram, "camera_pos"),
                scene->camera_pos[0], scene->camera_pos[1], scene->camera_pos[2]);
    glUniform3f(glGetUniformLocation(g->computeProgram, "camera_dir"),
                scene->camera_forward[0], scene->camera_forward[1], scene->camera_forward[2]);
    glUniform3f(glGetUniformLocation(g->computeProgram, "camera_up"),
                scene->camera_up[0], scene->camera_up[1], scene->camera_up[2]);
    glUniform3f(glGetUniformLocation(g->computeProgram, "background"), 0.05f, 0.05f, 0.15f);
    glUniform1f(glGetUniformLocation(g->computeProgram, "fov"), fov);
    glUniform1f(glGetUniformLocation(g->computeProgram, "aspect"), (float)g->width / (float)g->height);

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

void graphics_render_with_overlay(Graphics *g, int show_help) {
    glClear(GL_COLOR_BUFFER_BIT);
    /* Draw raytraced scene */
    glUseProgram(g->shaderProgram);
    glBindVertexArray(g->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g->outputTexture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    if (show_help) {
        /* Draw overlay on top with blending */
        memset(g->pixels, 0, g->width * g->height * sizeof(unsigned int));
        graphics_draw_overlay(g);

        glBindTexture(GL_TEXTURE_2D, g->texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, g->width, g->height,
                        GL_RGBA, GL_UNSIGNED_BYTE, g->pixels);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, g->texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDisable(GL_BLEND);
    }
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
