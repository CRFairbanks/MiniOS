#pragma once

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

bool mm_init();
void* malloc(size_t size);
void free(void* ptr);

void mem_init(void *heapMemory, size_t heapMemorySize);
void mem_extra_test();

void *mem_sbrk(intptr_t incr);
void *mem_heap_lo();
void *mem_heap_hi();

#ifdef __cplusplus
}
#endif
