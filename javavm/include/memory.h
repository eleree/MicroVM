#ifndef __MICRO_VM_MEMORY_H__
#define __MICRO_VM_MEMORY_H__

void * vmMalloc(size_t size);

void * vmCalloc(size_t count, size_t size); 

void vmFree(void * mem);

uint64_t vmMemoryUsed(void);

uint64_t vmMaxMemoryUsed(void);

#endif