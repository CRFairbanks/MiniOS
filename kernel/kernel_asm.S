/*
 * kernel_asm.S - kernel assembly routines (Project 2, CMPSC 473)
 * Copyright 2021 Ruslan Nikolaev <rnikola@psu.edu>
 * Distribution, modification, or usage without explicit author's permission
 * is not allowed.
 */

.global syscall_entry_asm, user_jump
.code64

.align 64
.type syscall_entry,%function
syscall_entry_asm:
	/* Set up the kernel stack */
	movq %rsp, user_stack(%rip)
	movq kernel_stack(%rip), %rsp

	/* Save SYSCALL/SYSRET registers */
	pushq %rcx
	pushq %r11

	/* Save other registers */
	pushq %rdi
	pushq %rsi
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10

	/* Call the internal handler */
	movq %r10, %rcx			/* r10 is used in lieu of rcx for syscalls */
	cmpq $1024, %rdi
	je 2f
	call syscall_entry

1:
	/* Restore other registers */
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rsi
	popq %rdi

	/* Restore SYSCALL/SYSRET registers */
	popq %r11
	popq %rcx

	movq user_stack(%rip), %rsp
	sysretq	/* Return the value */
2:
	movq kernel_status(%rip), %rax
	jmp 1b

.align 64
.type user_jump,%function
user_jump:
	pushfq 
	pop %r11 /* Will be used for RFLAGS by sysret */
	movq %rdi, %rcx /* Will be used for the instruction pointer by sysret */
	movq user_stack(%rip), %rsp
	sysretq
