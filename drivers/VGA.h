#ifndef VGA_H
#define VGA_H

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} vga_color_t;

void set_mode_13h();
void vga_set_palette(unsigned char index, vga_color_t color);
void put_pixel(int x, int y, unsigned char color);
void clear_screen_graphics(unsigned char color);

#endif