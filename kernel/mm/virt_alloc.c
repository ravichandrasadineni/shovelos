/*
 * virt_alloc.c
 *
 *  Created on: March 19, 2011
 *      Author: cds
 */

#include<inttypes.h>
#include<arch/arch.h>
#include "mm.h"

#if(0)

static uint8_t *cur_heap_top = (uint8_t*)VMM_HEAP_BASE;
static TICKET_LOCK( lock );


void *morecore(uint64_t size) {

	void *ret = NULL;

	ticket_lock_wait( &lock );

	if(((uint64_t)cur_heap_top + size <= VMM_HEAP_TOP) ) {

		ret = cur_heap_top;
		cur_heap_top += size;
	}

	ticket_lock_signal( &lock );

	return ret;
}

uint64_t vmm_heap_size() {

	return (uint64_t)cur_heap_top - VMM_HEAP_BASE;
}

static uint8_t *cur_hw_top = (uint8_t*)VMM_HWMMAP_BASE;
static TICKET_LOCK( hw_lock );

void *vmm_alloc_hw(uint64_t size) {

	void *ret = NULL;

	if(size & (PAGE_SIZE-1))
		size = (size + PAGE_SIZE) & (PAGE_SIZE-1);

	ticket_lock_wait( &hw_lock );

	if(((uint64_t)cur_hw_top + size <= VMM_HWMMAP_TOP) ) {

		ret = cur_hw_top;
		cur_hw_top += size;
	}

	ticket_lock_signal( &hw_lock );

	return ret;
}

#endif


