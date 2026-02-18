#ifndef UI_H
#define UI_H

#include "math.h"

typedef struct {
    int width;
    int height;
    unsigned int *framebuffer;
    int mouse_x;
    int mouse_y;
    int keys[512];
} UI;

UI ui_create(int width, int height);
void ui_destroy(UI *ui);
void ui_set_pixel(UI *ui, int x, int y, float r, float g, float b);
unsigned int ui_color_to_uint(float r, float g, float b);

#endif
