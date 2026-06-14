#include "../drivers/video.h"
#include "../utils/utils.h"

// variable from timer.c to count seconds
extern volatile unsigned int timer_ticks;

void background_clock() {
    while(1) {
        // print simple indicator in top-right corner
        // using kprint_at so we don't mess up Shell cursor
        kprint_at("[ SYSTEM ACTIVE ]", 0, 60, 0x0A);

        // little "pulse" animation
        kprint_at("*", 0, 78, 0x0F);
        sleep(500);
        kprint_at(" ", 0, 78, 0x0F);
        sleep(500);
    }
}