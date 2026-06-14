#ifndef TASK_H
#define TASK_H
#define STATE_RUNNING 0
#define STATE_WAITING 1

struct cpu_context {
    unsigned int ds;
    unsigned int edi, esi, ebp, esp_dummy, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
};

// task struct (process passport)
typedef struct task {
    int id;
    unsigned int esp;
    unsigned int ticks;
    unsigned int priority;
    int state;             // <-- NEW FIELDf
    struct task *next;
} task_t;

// task management function declarations
void init_multitasking();
void create_task(void (*entry_point)());
unsigned int switch_task(unsigned int esp);

#endif