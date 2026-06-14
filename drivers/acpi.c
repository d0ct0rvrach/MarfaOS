#include "acpi.h"
#include "video.h"

void off() {
    outw(0x604, 0x2000); // QEMU/VirtualBox
    outw(0xB004, 0x2000); // Bochs

    // if we got here — real hardware, ACPI didn't start
    kprint("It is now safe to turn off your computer.", 24, 0, 0x07);
    asm volatile ("hlt");
}
