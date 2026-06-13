#ifndef BOOTLOG_H
#define BOOTLOG_H

typedef struct {
    char* name;
    int (*init_func)();
} boot_step_t;

void run_boot_diagnostics();
void boot_delay(int loops);

#endif