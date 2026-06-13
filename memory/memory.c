#include "memory.h"

extern unsigned int kernel_end; // метка из линкера

static unsigned int heap_ptr = 0; //указатель на свободный байт
static unsigned int heap_end = 0; //кучка, 4 мб Дальше не идти

void init_heap() {
    heap_ptr = (unsigned int)&kernel_end; // свободная озу перед едром ставить
    heap_end = 0x400000;
}

void *kmalloc(unsigned int size) {
    if (size == 0) return 0; // защита от дибила

    if (heap_ptr % 4 != 0)
        heap_ptr += 4 - (heap_ptr % 4); // округление вверх

    if (heap_ptr + size > heap_end) return 0; // если место занято вернем нолик, ил жопа

    void *addr = (void *)heap_ptr; //адрес и пере-им указатель вперед
    heap_ptr += size;
    return addr;
}

void kfree(void *ptr) {
    (void)ptr; // заглушка дял компилятора, пиздим что переменая используется чтобы небыло опаности т-е варнинга
}
