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

#include "pt.h"

#define PAGE_TABLE_SIZE 512                         /* Entries per table */
#define PAGE_TABLE_BYTES 0x1000
#define PAGE_TABLE_ALIGNLENT 0x1000

/* mask out bits under page size */
#define ALIGN_DOWN(x)	((x) & ~(PAGE_TABLE_ALIGNLENT-1))

/* next page aligned address */
#define ALIGN_UP(x)     (((x) & (PAGE_TABLE_ALIGNLENT-1)) ? ALIGN_DOWN(x+PAGE_TABLE_ALIGNLENT) : (x))


struct page_table_mem kernel_page_tables = {

	{0,0,0}, /* lock */
	NULL,  /* pml4e */
	0,     /* stake top */
	0      /* stack bottom */
};

/*** get a physical page for use in the page tables ***/
static uint64_t* get_phy_page_table( struct page_table_mem *tab) {

	uint64_t ret = 0;

	if((tab->table_stack_bottom + PAGE_TABLE_BYTES) > tab->table_stack_top) {

		uint64_t phy_page = mm_phy_alloc_page();

		if(!phy_page)
			goto done;

		tab->table_stack_bottom = mm_page_to_phy( phy_page );
		tab->table_stack_top    = tab->table_stack_bottom + PAGE_SIZE;
	}
	tab->table_stack_top -= PAGE_TABLE_BYTES;

	ret = tab->table_stack_top;

done:

	if(ret)
		memset( PHY_TO_VIRT(ret,void*), 0x00, PAGE_TABLE_BYTES);

	return (uint64_t*)ret;
}

/*** fixme - ugly!
 * used at startup to determine where the boot-loader put the kernel.
 */
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

uint64_t debug_virt(uint64_t virt, uint64_t *_pml4e) {

	uint64_t *_pdpe  = 0x0000;
	uint64_t *_pde   = 0x0000;

	if(!_pml4e)
	{
		__asm__ __volatile__( "movq %%cr3, %0;"
					        : "=r" (_pml4e) );
	}

	kprintf("debug virtual 0x%lx, pml4e = phy 0x%lx\n",virt,_pml4e);

	_pml4e  += (0x1ff & (virt >> 39));

	kprintf("  _mpl4e[%d] = 0x%lx\n", 0x1ff & (virt >> 39), *PHY_TO_VIRT(_pml4e, uint64_t*));

	_pml4e  = PHY_TO_VIRT(_pml4e, uint64_t*);

	if(!(*_pml4e & PT_PRESENT_FLAG))
		return 0;

	_pdpe  = (uint64_t*)ALIGN_DOWN(*_pml4e);
	_pdpe += (0x1ff & (virt >> 30));

	kprintf("  _pdpe[%d] = 0x%lx\n", 0x1ff & (virt >> 30), *PHY_TO_VIRT(_pdpe, uint64_t*));

	_pdpe  = PHY_TO_VIRT(_pdpe, uint64_t*);


	if(!(*_pdpe & PT_PRESENT_FLAG))
		return 0;

	_pde  = (uint64_t*)ALIGN_DOWN(*_pdpe);
	_pde += (0x1ff & (virt >> 21));

	kprintf("  _pde[%d] = 0x%lx\n", 0x1ff & (virt >> 21), *PHY_TO_VIRT(_pde, uint64_t*));

	_pde  = PHY_TO_VIRT(_pde, uint64_t*);

	if(!(*_pde & PT_PRESENT_FLAG))
		return 0;

	return (uint64_t)ALIGN_DOWN(*_pde);
}

static BOOL debug_flag;

static sint64_t _mmap(uint64_t phy, uint64_t virt, struct page_table_mem *tab) {

	uint64_t *pml4e;
	uint64_t *pdpe;
	uint64_t *pde;

	if(debug_flag) kprintf("_mmap 0x%lx,0x%lx\n",phy,virt);

	/*** find or allocate the pml4e ***/
	if(!(pml4e = tab->pml4e) && !(pml4e = tab->pml4e = get_phy_page_table(tab)))
		return -1; // out of memory

	// seek in pml4e to pdpe offset.
	if(debug_flag) kprintf("pml4e += (%d)\n",0x1ff & ( virt >> 39 ));
	pml4e += (0x1ff & ( virt >> 39 ));

	/*** find or allocate the pdpe ***/
	if(!(*PHY_TO_VIRT(pml4e,uint64_t*) & PT_PRESENT_FLAG)) {
		if(!(*PHY_TO_VIRT(pml4e,uint64_t*) = (uint64_t)(pdpe = get_phy_page_table(tab))))
			return -1; // out of memory
		if(debug_flag) kprintf("new pdpe at 0x%lx\n",pdpe);
	}
	else {
		pdpe = (uint64_t*)ALIGN_DOWN(*PHY_TO_VIRT(pml4e,uint64_t*) );
		if(debug_flag) kprintf("existing pdpe at 0x%lx\n",pdpe);
	}

	// point pml4e to pdpe
	*PHY_TO_VIRT(pml4e,uint64_t*) |= PT_PRESENT_FLAG | PT_WRITABLE_FLAG;
	if(debug_flag) kprintf("pml4e[%d] = 0x%lx\n",0x1ff & ( virt >> 39 ),*PHY_TO_VIRT(pml4e,uint64_t*) );

	// seek in pdpe to pde
	pdpe += (0x1ff & (virt >> 30 ));
	if(debug_flag) kprintf("pdpe += (%d)\n",0x1ff & ( virt >> 30 ));

	/*** find or allocate the pde ***/
	if(!(*PHY_TO_VIRT(pdpe,uint64_t*) & PT_PRESENT_FLAG)) {
		if(!(*PHY_TO_VIRT(pdpe,uint64_t*) = (uint64_t)(pde = get_phy_page_table(tab))))
			return -1; // out of memory
		if(debug_flag) kprintf("new pde at 0x%lx\n",pde);
	}
	else {
		pde = (uint64_t*)ALIGN_DOWN( *PHY_TO_VIRT(pdpe,uint64_t*) );
		if(debug_flag) kprintf("existing pde at 0x%lx\n",pde);
	}

	// point pdpe to pde
	*PHY_TO_VIRT(pdpe,uint64_t*) |= PT_PRESENT_FLAG | PT_WRITABLE_FLAG;
	if(debug_flag) kprintf("pdpe[%d] = 0x%lx\n",0x1ff & ( virt >> 30 ),*PHY_TO_VIRT(pdpe,uint64_t*) );

	// seek in pde to pte
	pde += (0x1ff & (virt >> 21 ));
	if(debug_flag) kprintf("pde += (%d)\n",0x1ff & ( virt >> 21 ));

	// point pde to physical address.
	*PHY_TO_VIRT(pde,uint64_t*) = phy | PT_PRESENT_FLAG | PT_WRITABLE_FLAG | PT_TERMINAL_FLAG;
	if(debug_flag) kprintf("pde[%d] = 0x%lx\n",0x1ff & ( virt >> 21 ),*PHY_TO_VIRT(pde,uint64_t*) );

	return 0;
}


sint64_t mmap(uint64_t phy, uint64_t virt, struct page_table_mem *tab) {

	sint32_t success;

	if(!tab)
		tab = &kernel_page_tables;

	ticket_lock_wait( &tab->lock );

	success = _mmap(phy,virt,tab);

	ticket_lock_signal( &tab->lock );

	if(success != 0)
		kprintf("MMAP FAILED!!!!\n");

	return success;
}

/************
 * generate a new page table structure in kernel space,
 * and retire the one generated for us by the boot loader.
 */
void pt_initialise(struct mm_phy_reg *regs, uint64_t regnum) {

	/*** force mapping of first megabyte ***/
	for(uint64_t b = 0; b<PAGE_SIZE * 512; b+=PAGE_SIZE) {

		mmap( b, PHY_TO_VIRT(b,uint64_t), &kernel_page_tables); // at offset

		mmap( b, b, &kernel_page_tables);						// identity map
	}


	/*** map all physical memory at virtual = physical + VIRT_OFFSET ***/
	for(struct mm_phy_reg* r=regs; r<regs+regnum; r++) {

		if(r->type != MM_PHY_USABLE)
			continue;

		uint64_t b = r->base & (PAGE_SIZE-1) ? ((r->base + PAGE_SIZE) & PAGE_SIZE)  : r->base;
		uint64_t l = r->base & (PAGE_SIZE-1) ? ( r->len  - (r->base   & PAGE_SIZE)) : r->len;

		for(; l>=PAGE_SIZE; l-=PAGE_SIZE, b+=PAGE_SIZE)
			mmap( b, PHY_TO_VIRT(b,uint64_t), &kernel_page_tables);
	}

	/*** map in the kernel ***/
	for(uint64_t v = VIRT_KERNEL_BASE; ; v += PAGE_SIZE) {

		uint64_t p = virt_to_phy(v);

		if(p)
			mmap( p, v, &kernel_page_tables);
		else
			break;
	}

	/*** identity map low memory ***
	 * we still need access to structures created by the boot loader
	 * ( the stack being the most important one! )
	 */
	for(uint64_t b = 0; b<0x100000; b+=PAGE_SIZE)
		mmap( b, b, &kernel_page_tables);


	/*** load new page tables ***/
	__asm__ __volatile__ (
        "movq      %0,   %%rax;"
		"movq   %%rax,   %%cr3;"
		: /* no output */
		: "m" (kernel_page_tables.pml4e));

}



