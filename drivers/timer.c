#include "../cpu/isr.h"
#include "timer.h"
#include "io.h"
#include "../cpu/task.h"

volatile unsigned int timer_ticks = 0;

unsigned int timer_handler_wrapper(unsigned int esp) {
    struct registers *r = (struct registers *)esp;
    timer_ticks++;
    return switch_task(esp); // переключаем задачу
}

int test_timer() {
    unsigned int before = timer_ticks;
    for (volatile int i = 0; i < 10000000; i++);
    return timer_ticks > before; // тикает — живой
}

void init_timer(unsigned int freq) {
    unsigned int divisor = 1193180 / freq; // делитель частоты

    outb(0x43, 0x36); // команда PIT
    outb(0x40, (unsigned char)(divisor & 0xFF));        // младший байт
    outb(0x40, (unsigned char)((divisor >> 8) & 0xFF)); // старший байт

    register_interrupt_handler(32, timer_handler_wrapper);
}
