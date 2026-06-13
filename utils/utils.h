#ifndef UTILS_H
#define UTILS_H

typedef int (*init_func_ptr)();


typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef int            int32_t;

void sleep(int ms); // Добавили ;
int strcmp(char *s1, char *s2);
void wait(int ticks); // Оставь на всякий случай, если она где-то вызывается
char *skip_command(char *input); //Обьявление функции и возврат указателья
int strncmp(char *s1, char *s2, int n);

#endif