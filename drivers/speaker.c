#include "speaker.h"
#include "io.h"

void play_sound(uint32_t nFrequence) {
    if (nFrequence == 0) return;

    uint32_t Div = 1193180 / nFrequence; // делитель для PIT

    outb(0x43, 0xB6); // канал 2, режим 3
    outb(0x42, (uint8_t)(Div));
    outb(0x42, (uint8_t)(Div >> 8));

    uint8_t tmp = inb(0x61);
    if (tmp != (tmp | 3)) outb(0x61, tmp | 3); // включаем пищалку
}

void stop_sound() {
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp); // выключаем пищалку
}

void beep() {
    play_sound(1000);
    for(volatile int i = 0; i < 20000000; i++);
    stop_sound();
}

int test_speaker() {
    beep();
    return 1; // пискнул — живой
}
