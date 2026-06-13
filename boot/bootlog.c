#include "bootlog.h"
#include "../drivers/video.h"
#include "../drivers/keyboard.h"
#include "../drivers/speaker.h"
#include "../drivers/timer.h"
#include "../cpu/idt.h"
#include "../cpu/isr.h"

static int wrap_idt() { init_idt(); return 0; } 
static int wrap_pic() { pic_remap(); return 0; }
static int wrap_tmr() { init_timer(100); return 0; }
static int wrap_kbd() { init_keyboard(); return 0; }
static int wrap_vga() { return !test_vga(); }
static int wrap_ps2() { return !test_ps2(); }
static int wrap_speaker() { return !test_speaker(); }

boot_step_t boot_sequence[] = {
    {"CPU IDT", wrap_idt},   
    {"Hardware PIC", wrap_pic},
    {"Keyboard PS/2", wrap_kbd},
    {"System Timer", wrap_tmr},
    {"VGA Video", wrap_vga},
    {"PS/2 Controller", wrap_ps2},
    {"PC Speaker", wrap_speaker},

    {0, 0}
};

void boot_delay(int loops) {
    for (volatile int i = 0; i < loops * 1000000; i++); 
}

void run_boot_diagnostics() {
    for (int i = 0; boot_sequence[i].init_func != 0; i++) {
        kprint("Initializing: ", current_row, 0, 0x07); 
        kprint(boot_sequence[i].name, current_row, 14, 0x0F); 
        
        boot_delay(40); 

        if (boot_sequence[i].init_func() == 0) {
            kprint(" [ OK ]\n", current_row, 30, 0x0A); current_row++;
        } else {
            kprint(" [ FAIL ]\n", current_row, 30, 0x0C); current_row++;
            while(1); 
        }

        boot_delay(100);
        clear_screen();

    }
}