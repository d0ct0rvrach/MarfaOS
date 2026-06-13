#include "io.h"

void outb(unsigned short port, unsigned char val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

void outw(unsigned short port, unsigned short data) {
    asm volatile ("outw %0, %1" : : "a"(data), "nd"(port));
}

unsigned char inb(unsigned short port) {
    unsigned char res;
    asm volatile("inb %1, %0" : "=a"(res) : "Nd"(port));
    return res;
}