#ifndef UTILS_H
#define UTILS_H

typedef int (*init_func_ptr)();


typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef int            int32_t;

void sleep(int ms); // added ;
int strcmp(char *s1, char *s2);
void wait(int ticks); // keep just in case it's called somewhere
char *skip_command(char *input); // function declaration, returns pointer to arg
int strncmp(char *s1, char *s2, int n);

#endif