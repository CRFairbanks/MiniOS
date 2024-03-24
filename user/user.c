/*
 * user.c - a simple application (Project 2, CMPSC 473)
 * Copyright 2021 Ruslan Nikolaev <rnikola@psu.edu>
 * Distribution, modification, or usage without explicit author's permission
 * is not allowed.
 */

#include <syscall.h>

static int check_var = 0;

void user_start(void)
{
	__syscall1(1, (long) "This message is from user space!\n");

	long check_page_table = __syscall0(1024);
	if (check_page_table == 2 && (long) &check_var < 0) {
		__syscall1(1, (long) "SYSCALLS (Q2): YES\nPAGE_TABLES (Q3): YES\nUSER_SPACE (Q4): YES\n\nFinal: 100/100 points\n");
	} else if (check_page_table > 0) {
		__syscall1(1, (long) "SYSCALLS (Q2): YES\nPAGE_TABLES (Q3): YES\nUSER_SPACE (Q4): NO\n\nFinal: 80/100 points\n");
	} else {
		__syscall1(1, (long) "SYSCALLS (Q2): YES\nPAGE_TABLES (Q3): NO\nUSER_SPACE (Q4): NO\n\nFinal: 30/100 points\n");
	}

	/* Never exit */
	while (1) {};
}
