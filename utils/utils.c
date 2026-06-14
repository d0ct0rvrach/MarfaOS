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
    unsigned int end_ticks = timer_ticks + (ms / 10); // 100Hz = 10ms per tick
    while (timer_ticks < end_ticks) {
        asm volatile("hlt"); // wait for interrupt, don't eat CPU
    }
}
char *skip_command(char *input) {
    while (*input != ' ' && *input != '\0') input++; // walk string until space
    if (*input == ' ') input++; // step forward
    return input; // return pointer to argument start
}
int strncmp(char *s1, char *s2, int n) {
    while (n > 0 && *s1 && (*s1 == *s2)) { // go while limit not exhausted
        s1++; s2++; n--;
    }
    if (n == 0) return 0; // all n chars matched, return zero
    return *(unsigned char *)s1 - *(unsigned char *)s2; // diverged, return difference
}
