#ifndef MEMORY_H
#define MEMORY_H // double guard

void init_heap(); // call
void *kmalloc(unsigned int size); // request bytes, returns pointer
void kfree(void *ptr); // stub for now

#endif
