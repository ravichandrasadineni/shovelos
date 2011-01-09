/*
 * pt.c
 *
 *  Created on: Jan 8, 2011
 *      Author: cds
 */

#include <inttypes.h>


#define PAGE_TABLE_SIZE 512                         /* Entries per table */
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
 *     saw fit to allocate before loading and executing the kernel.
 */
BOOL _x86_64_phy_exists_in_pt(uint64_t phy) {

	uint64_t *_pml4e = 0x0000;

	__asm__ __volatile__( "movq %%cr3, %0;"
			             : "=r" (_pml4e) );

	for(uint64_t *pml4e = _pml4e; pml4e < _pml4e + PAGE_TABLE_SIZE; ++pml4e)
		if(*pml4e & PT_PRESENT_FLAG) {
			uint64_t *base = (uint64_t*)ALIGN_DOWN(*pml4e);
			for(uint64_t* pdpe = base; pdpe < base + PAGE_TABLE_SIZE; ++pdpe)
				if(*pdpe & PT_PRESENT_FLAG) {
					uint64_t *base = (uint64_t*)ALIGN_DOWN(*pdpe);
					for(uint64_t* pde = base; pde < base + PAGE_TABLE_SIZE; ++pde)
						if(*pde & PT_PRESENT_FLAG)
						    if((uint64_t)ALIGN_DOWN(*pde) == phy)
						    	return TRUE;
				}
		}

	return FALSE;
}


/***************************************************************************************************
 *     x86_64_virt_to_phy
 *       translate a virtual address to its physical address.
 *         takes 1) virtual memory address.
 *         returns: physical memory address.
 */
uint64_t x86_64_virt_to_phy(uint64_t virt) {

	uint64_t *pml4e = 0x0000;

	/*** page table address */
	__asm__ __volatile__( "movq %%cr3, %0;"
			             : "=r" (pml4e) );

	/*** walk page tables ***/
	pml4e += (PAGE_TABLE_SIZE-1) & (virt >> 39);
	if(*pml4e & PT_PRESENT_FLAG) {
		uint64_t *pdpe = (uint64_t*)ALIGN_DOWN(*pml4e);
		pdpe += (PAGE_TABLE_SIZE-1) & (virt >> 30);
		if(*pdpe & PT_PRESENT_FLAG) {
			uint64_t *pde = (uint64_t*)ALIGN_DOWN(*pdpe);
			pde += (PAGE_TABLE_SIZE-1) & (virt >> 21);
			if(*pde & PT_PRESENT)
				return  ALIGN_DOWN(*pde) + virt & (PAGE_SIZE-1); /* found it*/
		}
	}

	return 0; /* virtual address not mapped */
}



