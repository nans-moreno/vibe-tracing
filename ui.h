#ifndef UI_H
#define UI_H

#include <stdint.h>

typedef struct {
    uint8_t r, g, b, a;
} Pixel;

typedef struct {
    int width, height;
    unsigned int *framebuffer;
    int mouse_x, mouse_y;
    int mouse_buttons[3];
} UI;

typedef struct {
    float x, y, z;
} Vec3;

UI ui_create(int width, int height);
void ui_destroy(UI *ui);
uint32_t ui_color_to_uint(float r, float g, float b);
void ui_set_pixel(UI *ui, int x, int y, float r, float g, float b);
float fclamp(float val, float min, float max);

#endif // UI_H
