/*
 * pt.c
 *
 *  Created on: Jan 8, 2011
 *      Author: cds
 */

#include <inttypes.h>


#define PAGE_TABLE_SIZE 512                         /* enteries per table */
#define PAGE_TABLE_ALIGNLENT 0x1000

/* mask out bits under page size */
#define ALIGN_DOWN(x)	(x & ~(PAGE_TABLE_ALIGNLENT-1))

/* next page aligned address */
#define ALIGN_UP(x)     ((x & (PAGE_TABLE_ALIGNLENT-1)) ? ALIGN_DOWN(x+PAGE_TABLE_ALIGNLENT) : x)

#define PT_PRESENT_FLAG  		(1<<0)
#define PT_WRITABLE_FLAG 		(1<<1)
#define PT_USER_FLAG     		(1<<2)
#define PT_WRITE_THROUGH_FLAG	(1<<3)
#define PT_TERMINAL_FLAG    	(1<<7)
#define PT_GLOBAL_FLAG        	(1<<8)

/***************************************************************************************************
 *     _x86_64_phy_exists_in_pt - test if a physical address is mapped in the page tables.
 *     This code is used very early during startup to determine how many pages the boot-loader
 *     saw fit to allocate before loading the kernel.
 */
BOOL _x86_64_phy_exists_in_pt(uint64_t phy) {

	uint64_t *pml4e = 0x0000;

	__asm__ __volatile__( "movq %%cr3, %0;"
			             : "=r" (pml4e) );

	for(uint64_t *l4 = pml4e; l4 < pml4e + PAGE_TABLE_SIZE; ++l4)
		if(*l4 & PT_PRESENT_FLAG) {
			uint64_t *base = (uint64_t*)ALIGN_DOWN(*l4);
			for(uint64_t* l3 = base; l3 < base + PAGE_TABLE_SIZE; ++l3)
				if(*l3 & PT_PRESENT_FLAG) {
					uint64_t *base = (uint64_t*)ALIGN_DOWN(*l3);
					for(uint64_t* l2 = base; l2 < base + PAGE_TABLE_SIZE; ++l2)
						if(*l2 & PT_PRESENT_FLAG)
						    if((uint64_t)ALIGN_DOWN(*l2) == phy)
						    	return TRUE;
				}
		}

	return FALSE;
}
