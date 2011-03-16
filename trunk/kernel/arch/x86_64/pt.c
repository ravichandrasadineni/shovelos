/*
 * pt.c
 *
 *  Created on: Jan 8, 2011
 *      Author: cds
 */

#include <inttypes.h>
#include <mm/mm.h>
#include <arch/arch.h>
#include <lib/string.h>

#define PAGE_TABLE_SIZE 512                         /* Entries per table */
#define PAGE_TABLE_BYTES 0x1000
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


static uint64_t get_phy_page_table() {

	static TICKET_LOCK( lock );

	static uint64_t stack_top = 0x0000000000000000;
	static uint64_t stack_bot = 0x0000000000000000;

	uint64_t ret = 0;

	ticket_lock_wait( &lock );

	if((stack_bot + PAGE_TABLE_BYTES) > stack_top) {

		uint64_t phy_page = mm_phy_alloc_page();

		if(!phy_page)
			goto done;

		stack_bot = mm_page_to_phy( phy_page );
		stack_top = stack_bot + PAGE_SIZE;
	}
	stack_top -= PAGE_TABLE_BYTES;
	ret = stack_top;

done:

	ticket_lock_signal( &lock );

	if(ret)
		memset( PHY_TO_VIRT(ret,void*), 0x00, PAGE_TABLE_BYTES);

	return ret;
}

sint64_t mmap(uint64_t phy, uint64_t virt, uint64_t *_pml4e) {

	uint64_t *_pdpe  = 0x0000;
	uint64_t *_pde   = 0x0000;

	if(!_pml4e) {
		__asm__ __volatile__( 
		  "movq %%cr3, %0;"
		  : "=r" (_pml4e) );
	}

	_pml4e  += (0x1ff & (virt >> 39));
	_pml4e  = PHY_TO_VIRT(_pml4e, uint64_t*);
	_pdpe   = (uint64_t*)ALIGN_DOWN(*_pml4e);

	if(!(*_pml4e & PT_PRESENT_FLAG)) {

		_pdpe = (uint64_t*)get_phy_page_table();
		if(!(*_pml4e = (uint64_t)_pdpe))
			return -1; // out of memory!

		*_pml4e |= PT_PRESENT_FLAG;
	}

	_pdpe += (0x1ff & (virt >> 30));
	_pdpe  = PHY_TO_VIRT(_pdpe, uint64_t*);
	_pde   = (uint64_t*)ALIGN_DOWN(*_pdpe);

	if(!(*_pdpe & PT_PRESENT_FLAG)) {

		_pde = (uint64_t*)get_phy_page_table();
		if(!(*_pdpe = (uint64_t)_pde))
			return -1; // out of memory!

		*_pdpe |= PT_PRESENT_FLAG;
	}

	_pde += (0x1ff & (virt >> 21));
	_pde  = PHY_TO_VIRT(_pde, uint64_t*);

	*_pde = phy |
			PT_PRESENT_FLAG  |
			PT_TERMINAL_FLAG ;

	return 0;
}

uint64_t virt_to_phy(uint64_t virt) {

	uint64_t *_pml4e = 0x0000;
	uint64_t *_pdpe  = 0x0000;
	uint64_t *_pde   = 0x0000;

	__asm__ __volatile__( "movq %%cr3, %0;"
				        : "=r" (_pml4e) );

	_pml4e  += (0x1ff & (virt >> 39));
	_pml4e  = PHY_TO_VIRT(_pml4e, uint64_t*);

	if(!(*_pml4e & PT_PRESENT_FLAG))
		return 0;

	_pdpe  = (uint64_t*)ALIGN_DOWN(*_pml4e);
	_pdpe += (0x1ff & (virt >> 30));
	_pdpe  = PHY_TO_VIRT(_pdpe, uint64_t*);

	if(!(*_pdpe & PT_PRESENT_FLAG))
		return 0;

	_pde  = (uint64_t*)ALIGN_DOWN(*_pdpe);
	_pde += (0x1ff & (virt >> 21));
	_pde  = PHY_TO_VIRT(_pde, uint64_t*);

	if(!(*_pde & PT_PRESENT_FLAG))
		return 0;

	return (uint64_t)ALIGN_DOWN(*_pde);
}

