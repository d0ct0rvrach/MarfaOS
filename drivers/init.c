/*
#include "video.h"
#include "speaker.h"
#include "keyboard.h"
#include "utils/utils.h"

void init_all_drivers() {
    clear_screen();
    kprint("--- Marfa OS SYSTEM BOOT ---", 0, 0, 0x0B);
    wait(1);

    // Проверка Видео
    kprint("VGA Video RAM Test..........", 2, 0, 0x0F);
    if (test_vga()) kprint("[ PASSED ]", 2, 30, 0x0A);
    else kprint("[ FAILED ]", 2, 30, 0x0C);
    wait(1);

    // Проверка Клавиатуры
    kprint("PS/2 Controller Test........", 3, 0, 0x0F);
    if (test_ps2()) kprint("[ READY ]", 3, 30, 0x0A);
    else kprint("[ ERROR ]", 3, 30, 0x0C);
    wait(1);

    // Находим блок звука в drivers/init.c
    kprint("PC Speaker Test.............", 4, 0, 0x0F);
    
    for(volatile int i = 0; i < 80000000; i++);

    if (test_speaker()) {
        // Если вернул 1
        kprint("[ OK ]", 4, 30, 0x0A); 
    } else {
        // Если ты в драйвере поставил return 0 — сработает эта ветка!
        kprint("[ FAIL ]", 4, 30, 0x0C); 
    }

    kprint("All systems GO. Starting Shell...", 6, 0, 0x0E);
    for(volatile int i = 0; i < 1500000000; i++);

    clear_screen();
} 
    */

    // Это старый файл бут лога, он отвратительный. НЕ В КОЕМ СЛУЧАЕ НЕ УБИРАТЬ КОМЕНТИРОВАНИЕ, УМРЕТ СИСТЕМА

    // ну короче эт памятник