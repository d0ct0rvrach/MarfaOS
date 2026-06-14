#include "speaker.h"
#include "io.h"

void play_sound(uint32_t nFrequence) {
    if (nFrequence == 0) return;

    uint32_t Div = 1193180 / nFrequence; // PIT divisor

    outb(0x43, 0xB6); // channel 2, mode 3
    outb(0x42, (uint8_t)(Div));
    outb(0x42, (uint8_t)(Div >> 8));

    uint8_t tmp = inb(0x61);
    if (tmp != (tmp | 3)) outb(0x61, tmp | 3); // enable speaker
}

void stop_sound() {
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp); // disable speaker
}

void beep() {
    play_sound(1000);
    for(volatile int i = 0; i < 20000000; i++);
    stop_sound();
}

int test_speaker() {
    beep();
    return 1; // beeped — alive
}
