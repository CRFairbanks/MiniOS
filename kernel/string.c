/*
 * string.c - a substitute for memset and memcpy (Project 2, CMPSC 473)
 *
 * Those are just to make the compiler happy, since they are
 * occassionally used, not optimized for performance anyhow.
 *
 * Copyright 2021 Ruslan Nikolaev <rnikola@psu.edu>
 * Distribution, modification, or usage without explicit author's permission
 * is not allowed.
 */

#include <string.h>

void *memset(void *ptr, int value, size_t num)
{
	unsigned char *p = ptr;
	while (num--)
		*p++ = (unsigned char) value;
	return ptr;
}

void *memcpy(void *dst, const void *src, size_t num)
{
	unsigned char *d = dst;
	const unsigned char *s = src;
	while (num--)
		*d++ = *s++;
	return dst;
}
