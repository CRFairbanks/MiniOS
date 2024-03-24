/*
 * kernel_extra.c - Project 2 Extra Credit, CMPSC 473
 * Copyright 2021 Ruslan Nikolaev <rnikola@psu.edu>
 * Distribution, modification, or usage without explicit author's permission
 * is not allowed.
 */

#include <malloc.h>
#include <types.h>
#include <string.h>
#include <printf.h>

// Your mm_init(), malloc(), free() code from mm.c here
// You can only use mem_sbrk(), mem_heap_lo(), mem_heap_hi() and
// Project 2's kernel headers provided in 'include' such
// as memset and memcpy.
// No other files from Project 1 are allowed!


/* What is the correct alignment? */

#define ALIGNMENT 16

/* rounds up to the nearest multiple of ALIGNMENT */
static size_t align(size_t x)
{
    return ALIGNMENT * ((x+ALIGNMENT-1)/ALIGNMENT);
}


// Refrenced idea of doubly linked list online from geeksforgeeks.org
struct block{
    // Use either prev or next
    size_t size;
    // Free = 1, Used = 0
    int free;
    struct block *prev;
    struct block *next;
};

static struct block* flist = NULL;

bool mm_init()
{
	return false;
	 /* IMPLEMENT THIS */
    /* Initialize memory */
    void *header =  mem_sbrk(align(sizeof(struct block)));
    if(header == (void *)(-1)){
       return false;
    }
    // Initialize free list to header
    flist = header;
    flist->next = NULL;
    flist->prev = NULL;
    flist->size = sizeof(struct block);
    flist->free = 0;
    return true;
}

void *malloc(size_t size)
{
	return NULL;
	    /* IMPLEMENT THIS */
    if(size <= 0){
        return NULL;
    }
    /* Not efficent just makes  sure there is enough space and expands the heap */
    size_t adjusted_size =  align(size);
    // Find best-fit that is free in the current list that is of big enough size
    // Return if fits otherwise return
    void *pointer = mem_sbrk(adjusted_size);
    if(pointer == (void *)(-1)){
        return NULL;
    }
    // create newBlock and initialize it to the beginning of the heap returned by mem_sbrk
    struct block *newBlock = (struct block *)pointer;
    // initalize the newBlock as free, the beginning and end of the linked, with the adjusted size
    newBlock->size = adjusted_size;
    newBlock->free = 1;
    newBlock->next =  NULL;
    newBlock->prev = NULL;
    return  pointer;
}


void free(void *ptr)
{
	    // if its null, do nothing
    if(ptr == NULL){
        return;
    }
    /* Find ptr in heap if not in heap return nothing */
    struct block* a = (struct block*)ptr;
    // if its already free, just return
    if(a->free == 1){
        return;
    }
    // if its not free, label it as free and call combine in order to combine it with adjacent free memory in list
    else{
        a->free = 1;
    }
    return;
}