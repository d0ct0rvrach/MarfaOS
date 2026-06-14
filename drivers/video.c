#include "video.h"
#include "io.h"

int current_row = 0;
int current_col = 0;
int test_vga();

void put_char(char c, int pos, char color) {
    char *video = (char*) 0xb8000;
    video[pos * 2] = c;
    video[pos * 2 + 1] = color;
}

void kprint_at_cursor(const char *str, char color) {
    kprint(str, current_row, current_col, color);
}

void update_cursor(int row, int col) {
    unsigned short pos = row * 80 + col;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

void clear_screen() {
    char *video = (char*) 0xb8000;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video[i] = ' ';
        video[i + 1] = 0x07;
    }
}

void kprint(const char *str, int row, int col, char color) {
    int pos = row * 80 + col;
    for (int i = 0; str[i] != '\0'; i++) {
        put_char(str[i], pos + i, color);
    }
}

void scroll() {
    char *video = (char*) 0xb8000;
    for (int i = 0; i < 80 * 24 * 2; i++) {
        video[i] = video[i + 80 * 2];
    }
    for (int i = 80 * 24 * 2; i < 80 * 25 * 2; i += 2) {
        video[i] = ' ';
        video[i + 1] = 0x07;
    }
}

int test_vga() {
    volatile char *video = (char*)0xB8000;
    int offsets[] = {0, 1000, 3998}; 
    for (int i = 0; i < 3; i++) {
        int pos = offsets[i];
        char backup = video[pos];
        video[pos] = 0xAA;
        if (video[pos] != (char)0xAA) {
            video[pos] = backup;
            return 0;
        }
        video[pos] = backup;
    }
    return 1;
}

void kprint_at(char *message, int row, int col, char attribute) {
    unsigned char *vidmem = (unsigned char *)0xB8000;
    int offset = (row * 80 + col) * 2;
    while (*message) {
        vidmem[offset++] = *message++;
        vidmem[offset++] = attribute;
    }
}

void kprint_int(int value, int row, int col, char attribute) {
    char str[12]; // buffer for the number (fits -2147483648)
    int i = 0;

    // handle zero
    if (value == 0) {
        kprint("0", row, col, attribute);
        return;
    }

    // integer to string (reversed)
    char temp[12];
    int j = 0;
    while (value > 0) {
        temp[j++] = (value % 10) + '0';
        value /= 10;
    }

    // reverse string to correct order
    char final_str[12];
    int k = 0;
    while (j > 0) {
        final_str[k++] = temp[--j];
    }
    final_str[k] = '\0';

    kprint(final_str, row, col, attribute);
}