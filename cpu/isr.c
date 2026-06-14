#include "isr.h"
#include "drivers/io.h"
#include "cpu/task.h"
#include "../fsod/fsod.h"

typedef unsigned int (*isr_t)(unsigned int esp);

isr_t interrupt_handlers[256] = {0};

void register_interrupt_handler(unsigned char n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

unsigned int irq_handler(unsigned int esp) {
    struct registers *r = (struct registers *)esp;

    if (r->int_no >= 40) outb(0xA0, 0x20); // second PIC
    outb(0x20, 0x20); // first PIC — EOI signal

    if (interrupt_handlers[r->int_no] != 0) {
        isr_t handler = interrupt_handlers[r->int_no];
        esp = handler(esp);
    }

    return esp;
}

void pic_remap() {
    // ICW1 — initialization
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2 — offset: IRQ0-7 → 32-39, IRQ8-15 → 40-47
    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    // ICW3 — cascade
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // ICW4 — 8086 mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // masks — all enabled
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

unsigned int isr0_handler(unsigned int esp) {
    fsod("Division by zero"); // guard against division by zero
    return esp;
}

unsigned int isr14_handler(unsigned int esp) {
    fsod("Page fault"); // guard against wrong memory access
    return esp;
}

void init_isr_handlers() { // registers both handlers
    register_interrupt_handler(0, isr0_handler);
    register_interrupt_handler(14, isr14_handler); // handler called on interrupt
}
