/*
 * kernel_malloc.c - kernel memory allocator support routines
 * Copyright 2021 Ruslan Nikolaev <rnikola@psu.edu>
 * Distribution, modification, or usage without explicit author's permission
 * is not allowed.
 */

#include <malloc.h>
#include <types.h>
#include <printf.h>
#include <string.h>

static void *HeapMemoryBrk = NULL;
static void *HeapMemory = NULL;
static void *HeapMemoryEnd = NULL;

void mem_init(void *heapMemory, size_t heapMemorySize)
{
	HeapMemory = heapMemory;
	HeapMemoryBrk = heapMemory;
	HeapMemoryEnd = heapMemory + heapMemorySize;
}

void *mem_sbrk(intptr_t incr)
{
	void *prevBrk = HeapMemoryBrk;
	if (prevBrk + incr > HeapMemoryEnd) {
		printf("ERROR: Allocated too much memory!\n");
		return (void *) -1;
	}
	HeapMemoryBrk += incr;
	return prevBrk;
}

void *mem_heap_lo()
{
	return HeapMemory;
}

void *mem_heap_hi()
{
	return HeapMemoryBrk - 1;
}

void mem_extra_test()
{
	size_t i;
	mm_init();
	void *p1 = malloc(1024);
	if (!p1) {
		printf("Extra Credit: 0/40 points\n\n");
		return;
	}
	memset(p1, 0xAA, 1024);
	void *p2 = malloc(86);
	if (!p2) {
		printf("ERROR: buggy memory allocator\n");
		while (1) {}
	}
	void *p3 = malloc(1024);
	if (!p3) {
		printf("ERROR: buggy memory allocator\n");
		while (1) {}
	}
	free(p2);
	memcpy(p3, p1, 1024);
	free(p1);
	for (i = 0; i < 1024; i++) {
		if (*((unsigned char *)p3 + i) != 0xAA) {
			printf("ERROR: memory is corrupted\n");
			while (1) {}
		}
	}
	free(p3);
	printf("Extra Credit: 40/40 points\n\n");
}
