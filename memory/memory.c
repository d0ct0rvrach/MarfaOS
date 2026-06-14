#include "memory.h"

extern unsigned int kernel_end; // linker symbol

static unsigned int heap_ptr = 0; // pointer to the next free byte
static unsigned int heap_end = 0; // heap limit, 4 MB, do not cross

void init_heap() {
    heap_ptr = (unsigned int)&kernel_end; // start right after the kernel in RAM
    heap_end = 0x400000;
}

void *kmalloc(unsigned int size) {
    if (size == 0) return 0; // guard against idiot callers

    if (heap_ptr % 4 != 0)
        heap_ptr += 4 - (heap_ptr % 4); // align up

    if (heap_ptr + size > heap_end) return 0; // out of space, return null

    void *addr = (void *)heap_ptr; // save address, advance pointer
    heap_ptr += size;
    return addr;
}

void kfree(void *ptr) {
    (void)ptr; // stub for the compiler so it doesn't warn about unused variable
}
