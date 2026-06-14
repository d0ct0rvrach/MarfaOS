#include "drivers/vga.h"
#include "drivers/io.h"

#define VGA_OFFSET_ADDR 0xA0000
#define VGA_SCREEN_WIDTH 320
#define VGA_SCREEN_HEIGHT 200

// palette ports
#define PALETTE_INDEX 0x3C8
#define PALETTE_DATA  0x3C9
#define ATTRIBUTE_CTRL_RESET 0x3DA
#define ATTRIBUTE_CTRL_WRITE 0x3C0

// tested register array
unsigned char mode_13h_regs[] = {
    0x63, 0x03, 0x01, 0x0F, 0x00, 0x0E, 0x5F, 0x4F, 0x50, 0x82, 
    0x54, 0x80, 0xBF, 0x1F, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF, 0x00, 
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 
    0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x00, 0x0F, 0x00, 0x00
};

void vga_set_palette(unsigned char index, vga_color_t color) {
    outb(PALETTE_INDEX, index);
    outb(PALETTE_DATA, color.r);
    outb(PALETTE_DATA, color.g);
    outb(PALETTE_DATA, color.b);
}

static void apply_pipboy_theme() {
    vga_color_t black = {0, 0, 0};
    vga_color_t dark_green = {0, 20, 0};
    vga_color_t light_green = {0, 63, 0};

    vga_set_palette(0, black);        // background
    vga_set_palette(1, dark_green);   // alternative background (in case of offset)
    vga_set_palette(2, dark_green);   // main background
    vga_set_palette(10, light_green); // text and lines
}

void set_mode_13h() {
    unsigned char *regs = mode_13h_regs;
    
    // 1. Set base registers
    outb(0x3C2, *regs++);
    for(int i = 0; i < 5; i++) { outb(0x3C4, i); outb(0x3C5, *regs++); }
    outb(0x3D4, 0x03); outb(0x3D5, inb(0x3D5) | 0x80);
    outb(0x3D4, 0x11); outb(0x3D5, inb(0x3D5) & ~0x80);
    for(int i = 0; i < 25; i++) { outb(0x3D4, i); outb(0x3D5, *regs++); }
    for(int i = 0; i < 9; i++) { outb(0x3CE, i); outb(0x3CF, *regs++); }
    
    // 2. Configure attribute controller with reset
    for(int i = 0; i < 21; i++) {
        inb(ATTRIBUTE_CTRL_RESET); 
        outb(ATTRIBUTE_CTRL_WRITE, i);
        outb(ATTRIBUTE_CTRL_WRITE, *regs++);
    }
    
    // 3. Enable display (Bit 5)
    inb(ATTRIBUTE_CTRL_RESET);
    outb(ATTRIBUTE_CTRL_WRITE, 0x20);

    // 4. Load our theme
    apply_pipboy_theme();
}

void put_pixel(int x, int y, unsigned char color) {
    if (x >= 0 && x < VGA_SCREEN_WIDTH && y >= 0 && y < VGA_SCREEN_HEIGHT) {
        unsigned char* screen = (unsigned char*)VGA_OFFSET_ADDR;
        screen[y * VGA_SCREEN_WIDTH + x] = color;
    }
}

void clear_screen_graphics(unsigned char color) {
    unsigned char* screen = (unsigned char*)VGA_OFFSET_ADDR;
    for(int i = 0; i < VGA_SCREEN_WIDTH * VGA_SCREEN_HEIGHT; i++) {
        screen[i] = color;
    }
}