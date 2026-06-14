#include "task.h"
#include "../utils/utils.h"

// task queue
task_t *current_task = 0;
task_t *ready_queue = 0;
int next_pid = 1;

// ticks
unsigned int total_system_ticks = 0;

void init_multitasking() {
    task_t *task = (task_t *)0x500000;

    task->id = next_pid++;
    task->ticks = 0;
    task->priority = 1;
    task->state = STATE_RUNNING;

    task->next = task;
    ready_queue = task;
    current_task = task; 
}

void create_task(void (*entry_point)()) {
    task_t *new_task = (task_t *)(0x500000 + (next_pid * 0x1000));

    unsigned int *stack = (unsigned int *)(0x600000 + (next_pid * 0x1000));
    unsigned int *esp = stack;

    // stack
    *(--esp) = 0x0202;
    *(--esp) = 0x08;
    *(--esp) = (unsigned int)entry_point;
    *(--esp) = 0;
    *(--esp) = 0;

    for(int i = 0; i < 8; i++) {
        *(--esp) = 0;
    }

    *(--esp) = 0x10;

    new_task->id = next_pid++;
    new_task->esp = (unsigned int)esp;
    new_task->ticks = 0;
    new_task->priority = 1;
    new_task->state = STATE_RUNNING;
    new_task->next = ready_queue->next;
    ready_queue->next = new_task;
}

// scheduler — every 10ms
unsigned int switch_task(unsigned int last_esp) {
    if (current_task == 0) return last_esp;

    current_task->esp = last_esp;

    if (current_task->state == STATE_RUNNING) {
        current_task->ticks++;
    }
    total_system_ticks++;

    current_task = current_task->next;

    return current_task->esp;
}
