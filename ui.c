#include "ui.h"
#include <stdlib.h>

UI ui_create(int width, int height) {
    UI ui = {
        width, height,
        malloc(width * height * sizeof(unsigned int)),
        0, 0, {0}
    };
    return ui;
}

void ui_destroy(UI *ui) {
    free(ui->framebuffer);
}

unsigned int ui_color_to_uint(float r, float g, float b) {
    unsigned char cr = (unsigned char)(r * 255) & 0xFF;
    unsigned char cg = (unsigned char)(g * 255) & 0xFF;
    unsigned char cb = (unsigned char)(b * 255) & 0xFF;
    return (0xFF << 24) | (cb << 16) | (cg << 8) | cr;
}

void ui_set_pixel(UI *ui, int x, int y, float r, float g, float b) {
    if (x < 0 || x >= ui->width || y < 0 || y >= ui->height) return;
    r = fclamp(r, 0.0f, 1.0f);
    g = fclamp(g, 0.0f, 1.0f);
    b = fclamp(b, 0.0f, 1.0f);
    ui->framebuffer[y * ui->width + x] = ui_color_to_uint(r, g, b);
}
