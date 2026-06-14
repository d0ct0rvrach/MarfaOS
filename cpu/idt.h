#ifndef IDT_H
#define IDT_H

#include "drivers/io.h"

/* IDT Entry struct */
struct idt_entry_struct {
    unsigned short base_low;  // lower 16 bits of handler address
    unsigned short sel;       // kernel segment (usually 0x08)
    unsigned char  always0;   // always 0
    unsigned char  flags;     // access flags
    unsigned short base_high; // upper 16 bits of address
} __attribute__((packed));

/* LIDT command struct (load table into CPU) */
struct idt_ptr_struct {
    unsigned short limit;
    unsigned int   base;
} __attribute__((packed));

void init_idt();
void set_idt_gate(int n, unsigned int handler);

#endif