#ifndef ISR_H
#define ISR_H

#include <stdint.h>

// registers struct
typedef struct registers {
    unsigned int ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
} registers_t;

typedef unsigned int (*isr_t)(unsigned int);

// functions
void pic_remap();
void register_interrupt_handler(unsigned char n, isr_t handler);

void init_isr_handlers(); 

#endif