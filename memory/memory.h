#ifndef MEMORY_H
#define MEMORY_H // защита х2

void init_heap(); // вызов
void *kmalloc(unsigned int size); //запрос байта и возвращает указатель
void kfree(void *ptr); // пока заглушка

#endif
