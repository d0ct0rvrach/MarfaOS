#include "../cpu/isr.h"
#include "timer.h"
#include "io.h"
#include "../cpu/task.h"

volatile unsigned int timer_ticks = 0;

unsigned int timer_handler_wrapper(unsigned int esp) {
    struct registers *r = (struct registers *)esp;
    timer_ticks++;
    return switch_task(esp); // switch task
}

int test_timer() {
    unsigned int before = timer_ticks;
    for (volatile int i = 0; i < 10000000; i++);
    return timer_ticks > before; // ticking — alive
}

void init_timer(unsigned int freq) {
    unsigned int divisor = 1193180 / freq; // frequency divisor

    outb(0x43, 0x36); // PIT command
    outb(0x40, (unsigned char)(divisor & 0xFF));        // low byte
    outb(0x40, (unsigned char)((divisor >> 8) & 0xFF)); // high byte

    register_interrupt_handler(32, timer_handler_wrapper);
}
