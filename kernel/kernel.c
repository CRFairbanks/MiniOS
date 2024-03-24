/*
 * boot.c - a simple kernel (Project 2, CMPSC 473)
 * Copyright 2021 Ruslan Nikolaev <rnikola@psu.edu>
 * Distribution, modification, or usage without explicit author's permission
 * is not allowed.
 */

#include <kernel.h>
#include <types.h>
#include <msr.h>
#include <malloc.h>
#include <fb.h>
#include <printf.h>

void *kernel_stack; /* Initialized in kernel_entry.S */
void *syscall_entry_ptr; /* Points to syscall_entry_asm(), initialized in kernel_entry.S; workarounds a linker bug */

static void syscall_init(void)
{
	/* Enable SYSCALL/SYSRET */
	wrmsr(MSR_EFER, rdmsr(MSR_EFER) | 0x1);

	/* GDT descriptors for SYSCALL/SYSRET (USER descriptors are implicit) */
	wrmsr(MSR_STAR, ((uint64_t) GDT_KERNEL_DATA << 48) | ((uint64_t) GDT_KERNEL_CODE << 32));

	/* A system call entry point */
	wrmsr(MSR_LSTAR, (uint64_t) syscall_entry_ptr);

	/* Disable interrupts (IF) while in a syscall */
	wrmsr(MSR_SFMASK, 1U << 9);
}

#define KERNEL_HEAP_SIZE (1U << 20) /* 1MB */

/* Used internally, do not modify */
long kernel_status = 0;
void *kernel_memory = NULL;
void *kernel_memory_end = NULL;

extern void *user_program;

/* The main program starts here */
void kernel_start(void *kstack, void *ustack, framebuffer_t *fb,
		  void *ucode, void *memory, size_t memorySize)
{
	fb_init(fb->addr, fb->width, fb->height);
	syscall_init();
	kernel_memory = memory + KERNEL_HEAP_SIZE;
	kernel_memory_end = memory + memorySize;
	mem_init(memory, KERNEL_HEAP_SIZE);
	kernel_init(ustack, ucode, memory + KERNEL_HEAP_SIZE, memorySize - KERNEL_HEAP_SIZE);
	user_jump(user_program);

	/* Never exit! */
	while (1) {};
}
