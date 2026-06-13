#include "idt.h"

extern void irq0();
extern void irq1();

struct idt_entry_struct idt_entries[256];
struct idt_ptr_struct   idt_ptr;

void set_idt_gate(int n, unsigned int handler) {
    idt_entries[n].base_low  = handler & 0xFFFF;
    idt_entries[n].sel       = 0x08; // сегмент ядра
    idt_entries[n].always0   = 0;
    idt_entries[n].flags     = 0x8E;
    idt_entries[n].base_high = (handler >> 16) & 0xFFFF;
}

extern void idt_flush(unsigned int);

void init_idt() {
    idt_ptr.limit = sizeof(struct idt_entry_struct) * 256 - 1;
    idt_ptr.base  = (unsigned int)&idt_entries;

    for (int i = 0; i < 256; i++) set_idt_gate(i, 0); // чистим таблицу

    set_idt_gate(32, (unsigned int)irq0); // таймер
    set_idt_gate(33, (unsigned int)irq1); // клавиатура
    
    extern void isr0();
    set_idt_gate(0, (unsigned int)isr0); // деление на ноль, смотрите shell.c - crash

    idt_flush((unsigned int)&idt_ptr);
}
