#include "../drivers/video.h"
#include "../utils/utils.h"

// Переменная из timer.c, чтобы считать секунды
extern volatile unsigned int timer_ticks;

void background_clock() {
    while(1) {
        // Выводим простую индикацию в правый верхний угол
        // Мы используем kprint_at, чтобы не сбивать курсор Шелла
        kprint_at("[ SYSTEM ACTIVE ]", 0, 60, 0x0A);
        
        // Маленькая анимация "пульса"
        kprint_at("*", 0, 78, 0x0F);
        sleep(500);
        kprint_at(" ", 0, 78, 0x0F);
        sleep(500);
    }
}