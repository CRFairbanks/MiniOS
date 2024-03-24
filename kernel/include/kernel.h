#pragma once

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void *kernel_stack; /* the kernel stack */
extern void *user_stack; /* the user stack */
extern void *user_program; /* the user program */
void user_jump(void * addr); /* an initial jump to user mode, addr is a VIRTUAL address of user's _start */

/*
 * A pointer to syscall_entry(),
 * do not expose syscall_entry_asm() directly to avoid linker
 * issues/bugs due to the use of a 'pure binary';
 * this pointer is already initialized to syscall_entry_asm() in kernel_entry.S
 */
extern void *syscall_entry_ptr;

/* the system call handler */
long syscall_entry(long n, long a1, long a2, long a3, long a4, long a5);

/* kernel initialization */
void kernel_init(void *ustack, void *uprogram, void *memory, size_t memorySize);

/* check and load page table */
const char *load_page_table(void *page_table);

typedef struct framebuffer_s {
	uint32_t *addr;
	uint32_t width;
	uint32_t height;
} framebuffer_t;

#ifdef __cplusplus
}
#endif
