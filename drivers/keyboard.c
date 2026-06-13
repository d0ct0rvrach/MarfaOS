#include "../cpu/isr.h"
#include "keyboard.h"
#include "video.h"    
#include "io.h"       
#include "../utils/utils.h"


int test_ps2();

// --- НОВОЕ: Кольцевой буфер для символов ---
static char key_buffer[256];
static int head = 0; // Индекс для записи (куда кладёт прерывание)
static int tail = 0; // Индекс для чтения (откуда забирает Шелл)
static int extended = 0;

void keyboard_callback() {
    unsigned char scancode = inb(0x60);
    if (scancode == 0xE0) {
    extended = 1;
    return;
    }

    if (extended) {
        extended = 0;
        if (scancode == 0x48) { key_buffer[head] = '\x01'; head = (head + 1) % 256; }
        if (scancode == 0x50) { key_buffer[head] = '\x02'; head = (head + 1) % 256; }
    return;
    }

    if (scancode < 0x80) { // Только нажатие
        char c = scan_to_char(scancode);
        if (c != 0) {
            // Кладём символ в твой кольцевой буфер
            key_buffer[head] = c;
            head = (head + 1) % 256;
        }
    }
}

unsigned int keyboard_handler_wrapper(unsigned int esp) {
    keyboard_callback(); 
    return esp;          
}

// Эту функцию будет вызывать Шелл вместо прямого опроса порта
char get_char() {
    if (head == tail) return 0; // Буфер пуст
    
    char c = key_buffer[tail];
    tail = (tail + 1) % 256;
    return c;
}
// -------------------------------------------

char scan_to_char(unsigned char scancode) {
    switch(scancode) {
        case 0x1E: return 'a'; case 0x30: return 'b'; case 0x2E: return 'c';
        case 0x20: return 'd'; case 0x12: return 'e'; case 0x21: return 'f';
        case 0x22: return 'g'; case 0x23: return 'h'; case 0x17: return 'i';
        case 0x24: return 'j'; case 0x25: return 'k'; case 0x26: return 'l';
        case 0x32: return 'm'; case 0x31: return 'n'; case 0x18: return 'o';
        case 0x19: return 'p'; case 0x10: return 'q'; case 0x13: return 'r';
        case 0x1F: return 's'; case 0x14: return 't'; case 0x16: return 'u';
        case 0x2F: return 'v'; case 0x11: return 'w'; case 0x2D: return 'x';
        case 0x15: return 'y'; case 0x2C: return 'z'; case 0x39: return ' ';
        case 0x02: return '1'; case 0x03: return '2'; case 0x04: return '3';
        case 0x1C: return '\n'; case 0x0E: return '\b';
        case 0x34: return '.';
        case 0x35: return '/';
        case 0x0C: return '-';
        default: return 0;
    }
}


int test_ps2() {
    while (inb(0x64) & 1) { inb(0x60); } 
    outb(0x64, 0xAA);
    wait(1);
    return (inb(0x60) == 0x55);
}

void init_keyboard() {
    test_ps2();
    register_interrupt_handler(33, keyboard_handler_wrapper);
}

void check_cpu() {
    unsigned int ebx, ecx, edx;
    char vendor[13];
    asm volatile("cpuid" : "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(0));
    *((int*)(vendor)) = ebx;
    *((int*)(vendor + 4)) = edx;
    *((int*)(vendor + 8)) = ecx;
    vendor[12] = '\0';
    kprint("CPU Vendor ID: ", 3, 0, 0x0F);
    kprint(vendor, 3, 15, 0x0E);
}