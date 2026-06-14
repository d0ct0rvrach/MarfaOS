/*
#include "video.h"
#include "speaker.h"
#include "keyboard.h"
#include "utils/utils.h"

void init_all_drivers() {
    clear_screen();
    kprint("--- Marfa OS SYSTEM BOOT ---", 0, 0, 0x0B);
    wait(1);

    // Video check
    kprint("VGA Video RAM Test..........", 2, 0, 0x0F);
    if (test_vga()) kprint("[ PASSED ]", 2, 30, 0x0A);
    else kprint("[ FAILED ]", 2, 30, 0x0C);
    wait(1);

    // Keyboard check
    kprint("PS/2 Controller Test........", 3, 0, 0x0F);
    if (test_ps2()) kprint("[ READY ]", 3, 30, 0x0A);
    else kprint("[ ERROR ]", 3, 30, 0x0C);
    wait(1);

    // Sound block in drivers/init.c
    kprint("PC Speaker Test.............", 4, 0, 0x0F);
    
    for(volatile int i = 0; i < 80000000; i++);

    if (test_speaker()) {
        // returned 1
        kprint("[ OK ]", 4, 30, 0x0A);
    } else {
        // if you put return 0 in the driver — this branch fires!
        kprint("[ FAIL ]", 4, 30, 0x0C);
    }

    kprint("All systems GO. Starting Shell...", 6, 0, 0x0E);
    for(volatile int i = 0; i < 1500000000; i++);

    clear_screen();
} 
    */

    // This is the old boot log file, it's disgusting. DO NOT REMOVE THE COMMENT, THE SYSTEM WILL DIE

    // basically this is a monument