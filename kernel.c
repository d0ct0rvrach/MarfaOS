#include "boot/bootlog.h"
#include "cpu/isr.h"
#include "drivers/video.h"
#include "drivers/keyboard.h"
#include "drivers/io.h"
#include "drivers/speaker.h"
#include "shell/shell.h"
#include "utils/utils.h"
#include "cpu/idt.h"
#include "drivers/timer.h"
#include "cpu/task.h"
#include "memory/memory.h"
#include "boot/logo.h"
#include "boot/ramdisk.h"

void run_boot_diagnostics();

extern void irq0();
extern void irq1();
extern void background_clock();

void kmain(unsigned int magic, unsigned int mboot_addr) {

    init_heap(); // heap init

    init_isr_handlers();

    current_row = 0; // screen
    current_col = 0;
    clear_screen();

    run_boot_diagnostics(); // diagnostics

    ramdisk_init(mboot_addr);
;
    init_multitasking(); // multitasking

    asm volatile("sti"); // heartbeat

     print_logo(); // LOGO!!!
    beep();
     create_task(background_clock);

    kprint("MarfaOS: Multitasking enabled.\n", 0,   25, 0x0F);

    // shell
    run_shell();
}
