#include "fsod.h"
#include "../drivers/video.h"
#include "../cpu/isr.h"
// FSOD
void fsod(const char *message) {
    clear_screen(); 

    for (int i = 0; i < 80 * 25; i++) {
        char *video = (char *)0xB8000;
        video[i * 2 + 1] = 0x55;
    }

    kprint("*** FATAL SYSTEM ERROR ***", 8, 27, 0x55);
    kprint(message, 10, 27, 0x5F);
    kprint("System halted.", 12, 33, 0x57);

    asm volatile("cli");
    while(1) asm volatile("hlt");
}

