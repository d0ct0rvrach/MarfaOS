#include "utils.h"

void wait(int ticks) {
    for (volatile int i = 0; i < ticks * 4000000; i++);
}

int strcmp(char *s1, char *s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

extern volatile unsigned int timer_ticks;

void sleep(int ms) {
    unsigned int end_ticks = timer_ticks + (ms / 10); // 100Гц = 10мс на тик
    while (timer_ticks < end_ticks) {
        asm volatile("hlt"); // ждём прерывания, не жрём CPU
    }
}
char *skip_command(char *input) { 
    while (*input != ' ' && *input != '\0') input++; // Хход по строке до встрпечи пробела
    if (*input == ' ') input++; //шаг вперед
    return input; // возврат в начало аргумерта (указателя)
}
int strncmp(char *s1, char *s2, int n) {
    while (n > 0 && *s1 && (*s1 == *s2)) { // боже идем пока не кончится лимит
        s1++; s2++; n--;
    }
    if (n == 0) return 0; // если прошли все х символы и все совпало дай бог возвразаем нолик
    return *(unsigned char *)s1 - *(unsigned char *)s2; //емли господи упаси разошлись возврат разницы не нолик(
}
