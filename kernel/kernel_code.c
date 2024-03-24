/*
 * kernel_code.c - Project 2, CMPSC 473
 * Copyright 2021 Ruslan Nikolaev <rnikola@psu.edu>
 * Distribution, modification, or usage without explicit author's permission
 * is not allowed.
 */

#include <kernel.h>
#include <types.h>
#include <printf.h>
#include <malloc.h>
#include <string.h>

void *page_table = NULL; /* TODO: Must be initialized to the page table address */
void *user_stack = NULL; /* TODO: Must be initialized to a user stack virtual address */
void *user_program = NULL; /* TODO: Must be initialized to a user program virtual address */

	// information on structs and types from class slides
	typedef unsigned long long u64;

	struct page_pte{
		u64 present:1;		
		u64 writable:1;	
		u64 user_mode:1;				
		u64 write_through:1;
		u64 cache_disable:1;
		u64 accessed:1;
		u64 dirty:1;
		u64 pat:1;
		u64 g:1;
		u64 avl:3;
		u64 page_address:40;
		u64 avail:7;
		u64 pke:4;
		u64 nonexecute:1;
	};

	struct page_pde{
		u64 present:1;
		u64 writable:1;
		u64 user_mode:1;
		u64 write_through:1;
		u64	cache_disable:1;
		u64 accessed:1;
		u64 ign1:1;
		u64 zero:1;
		u64 ign2:1;
		u64 avl:3;
		u64 page_address:40;
		u64 avail:11;
		u64 nonexecute:1;
	};


void kernel_init(void *ustack, void *uprogram, void *memory, size_t memorySize)
{
	// printf statement to indicate user submission
	printf("Hello from sgroup47, CRFairbanks! \n");

	// 'memory' points to the place where memory can be used to create
	// page tables (assume 1:1 initial virtual-to-physical mappings here)
	// 'memorySize' is the maximum allowed size, do not exceed that (given just in case)

	// CREATE THE INITIAL 1 TO 1 FIRST LEVEL OF THE PAGE TABLE (PTE)
	struct page_pte *p1 = memory;

	// INITIALIZE THE BITS FOR EACH OF THE ENTRIES
	for(int i = 0; i < 1048576; i++){
		p1[i].page_address = (uint64_t) i;
		p1[i].present = 1;
		p1[i].writable = 1;
		p1[i].user_mode = 0;
		p1[i].write_through= 0;
		p1[i].cache_disable = 0;
		p1[i].accessed = 0;
		p1[i].dirty = 0;
		p1[i].pat = 0;
		p1[i].g = 0;
		p1[i].avl = 0;
		p1[i].avail = 0;
		p1[i].pke = 0;
		p1[i].nonexecute = 0;
	}


	// CREATE LEVEL 2 (PDE)
	struct page_pde *p2 = (struct page_pde *)(p1 + 1048576);

	// INITIALIZE BITS BITS
	for (int j = 0; j < 2048; j++){
		struct page_pte *start_pte = (p1 + (512 * j) );
		p2[j].page_address = (uint64_t)start_pte >> 12;
		p2[j].present = 1;
		p2[j].writable = 1;
		p2[j].user_mode = 0;
		p2[j].write_through = 0;
		p2[j].cache_disable = 0;
		p2[j].accessed = 0;
		p2[j].ign1 = 0;
		p2[j].zero = 0;
		p2[j].ign2 = 0;
		p2[j].avl = 0;
		p2[j].avail = 0;
		p2[j].nonexecute = 0;
	}


	// CREATE LEVEL 3 (PDPE)
	struct page_pde *p3 = (struct page_pde *)(p2 + 2048);

	// INITIALIZE BITS
	for(int k = 0; k < 4; k++){
		struct page_pde *start_pde = p2 + (512 * k);
		p3[k].page_address = (uint64_t)start_pde >> 12;
		p3[k].present = 1;
		p3[k].writable = 1;
		p3[k].user_mode = 0;
		p3[k].write_through = 0;
		p3[k].cache_disable = 0;
		p3[k].accessed = 0;
		p3[k].ign1 = 0;
		p3[k].zero = 0;
		p3[k].ign2 = 0;
		p3[k].avl = 0;
		p3[k].avail = 0;
		p3[k].nonexecute = 0;
	}


	// CREATE LEVEL 4 (PMLE4E)
	struct page_pde *p4 = (struct page_pde *)(p3 + 512);

	// INITIALIZE BITS
	for( int l = 0; l < 512; l++){
		struct page_pde *start_pdpe = p3;
		if(l == 0){
			p4[l].page_address = (uint64_t)start_pdpe >> 12;
			p4[l].present = 1;
			p4[l].writable = 1;
		}
		else{
			p4[l].page_address = (uint64_t) 0;
			p4[l].present = 0;
			p4[l].writable = 0;
		}
		p4[l].user_mode = 0;
		p4[l].write_through = 0;
		p4[l].cache_disable = 0;
		p4[l].accessed = 0;
		p4[l].ign1 = 0;
		p4[l].zero = 0;
		p4[l].ign2 = 0;
		p4[l].avl = 0;
		p4[l].avail = 0;
		p4[l].nonexecute = 0;
		}

	// CREATE THE ACTUAL PAGE TABLE
	page_table = (void*)p4;

	// USER_STACK AND PROGRAM TO BE IMPLEMENTED FOR Q4

	/* PERSONAL Q4 NOTES FROM LECTURE

	- MAKE NEW PAGE TABLE FOR USER SPACE REFERENCED FROM OLD P4[511] -> NEW P2[511] -> NEW P1[511] -> NEW_TABLE (LAST ENTRIES OF EACH LVL)
	- FIRST 511 ENTRIES OF NEWP2 AND NEWP1 INITIALIZED TO ZERO
	- USER_STACK AND USER_PROG POINTED TO BY ANY 2 ENTIRES IN THE NEW TABLE

	*/

	/* ATTEMPT AT Q4 THAT DIDNT WORK - COMMENTED OUT TO PREVENT ISSUES - LINES 173-217 */

/*
	void *new_page_table = NULL;


	struct page_pte *newp1 = memory;

	// INITIALIZE THE BITS FOR EACH OF THE ENTRIES
	for(int i = 0; i < 512; i++) {
		newp1[i].page_address = (uint64_t) i;
		newp1[i].present = 1;
		newp1[i].writable = 1;
		newp1[i].user_mode = 1;
		newp1[i].write_through= 0;
		newp1[i].cache_disable = 0;
		newp1[i].accessed = 0;
		newp1[i].dirty = 0;
		newp1[i].pat = 0;
		newp1[i].g = 0;
		newp1[i].avl = 0;
		newp1[i].avail = 0;
		newp1[i].pke = 0;
		newp1[i].nonexecute = 0;
	}

	struct page_pde *newp2 = (struct page_pde *)(newp1 + 1048576);

	// INITIALIZE BITS BITS
	for (int j = 0; j < 512; j++){
		struct page_pte *start_pte = (newp1 + (512 * j) );
		newp2[j].page_address = (uint64_t)start_pte >> 12;
		newp2[j].present = 1;
		newp2[j].writable = 1;
		newp2[j].user_mode = 1;
		newp2[j].write_through = 0;
		newp2[j].cache_disable = 0;
		newp2[j].accessed = 0;
		newp2[j].ign1 = 0;
		newp2[j].zero = 0;
		newp2[j].ign2 = 0;
		newp2[j].avl = 0;
		newp2[j].avail = 0;
		newp2[j].nonexecute = 0;
	}
	
*/

	
	user_stack = ustack; // 0xFFFFFFFFFFE00000 to 0xFFFFFFFFFFFFFFFF depending on new_page_table placement

	user_program = uprogram; // 0xFFFFFFFFFFE00000 to 0xFFFFFFFFFFFFFFFF depending on new_page_table placement


	// The remaining portion just loads the page table,
	// this does not need to be changed:
	// load 'page_table' into the CR3 register
	const char *err = load_page_table(page_table);
	if (err != NULL) {
		printf("ERROR: %s\n", err);
	}


	// The extra credit assignment
	mem_extra_test();
}

/* The entry point for all system calls */
long syscall_entry(long n, long a1, long a2, long a3, long a4, long a5)
{
	// CALL FOR N = 1
	if(n == 1){
		printf("%s\n", (char *)a1);
		return 0;
	}

	return -1; /* Success: 0, Failure: -1 */
}