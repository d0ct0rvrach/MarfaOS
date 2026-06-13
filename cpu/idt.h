#ifndef IDT_H
#define IDT_H

#include "drivers/io.h"

/* Структура элемента таблицы прерываний (IDT Entry) */
struct idt_entry_struct {
    unsigned short base_low;  // Младшие 16 бит адреса функции
    unsigned short sel;       // Сегмент ядра (обычно 0x08)
    unsigned char  always0;   // Всегда 0
    unsigned char  flags;     // Флаги доступа
    unsigned short base_high; // Старшие 16 бит адреса
} __attribute__((packed));

/* Структура для команды LIDT (загрузка таблицы в процессор) */
struct idt_ptr_struct {
    unsigned short limit;
    unsigned int   base;
} __attribute__((packed));

void init_idt();
void set_idt_gate(int n, unsigned int handler);

#endif