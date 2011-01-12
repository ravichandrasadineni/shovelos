
/************************************************************************************************************
 * Setup long-mode page tables.
 */

#include "16bitreal.h"
#include "inttypes.h"
#include "mem.h"
#include "pt.h"
#include "mmap.h"
#include "alloc.h"
#include "print.h"

extern int _pml4e;
extern int _pdpe_ident;
extern int _pde_ident;
extern int _pdpe_hi;
extern int _pde_hi;


/*** write a 64bit page table entry to a far address ***/
static void write64( uint32_t addr20, uint64_t value ) {

	memcpy((void*)addr20, &value, 8);
}

static uint32_t pt_get_addr(uint32_t addr20) {

	uint64_t pt = 0 ;

	memcpy( &pt, (const void*)addr20, 8);

	return  (uint32_t)ALIGN_DOWN( pt );
}

/************************************************************************************************************
 * Create a 64bit virtual -> physical page mapping
 * takes 1) virtual page base ( PAGE_SIZE aligned )
 *       2) physical page base ( PAGE_SIZE aligned )
 */
static void pt_map_page(uint64_t virt, uint64_t phy, int pdpe_base, int pde_base) {

	uint32_t pml4e = 0x10000; //&_pml4e;
	uint32_t pdpe;

#if(PAGE_SIZE==_2MEG)
	uint32_t pde;
#endif

#if(PAGE_SIZE == _4K)
	#error 4k_pages_not_implemented
#endif

#ifdef DEBUG
	printf("pt_map_page 0x%lx -> 0x%lx\n",virt,phy);
#endif

	pml4e += 8 * (0x1ff & (virt >> 39));

	pdpe = pt_get_addr(pml4e);

	if(pdpe == 0) {

		uint64_t data  = (pdpe = pdpe_base)        |
		                 PT_PRESENT_FLAG           |
		        		 PT_WRITABLE_FLAG          |
		        		 PT_USER_FLAG              |
		        		 PT_WRITE_THROUGH_FLAG     ;

		write64(pml4e, data);
	}
	pdpe += 8 * (0x1ff & (virt >> 30));
	pde = pt_get_addr(pdpe);

#if(PAGE_SIZE==_2MEG)
	if(pde == 0) {
		uint64_t data  = (pde = pde_base)          |
#endif
#if(PAGE_SIZE==_1GIG)
	{
		uint64_t data  = phy                       |
		                 PT_GLOBAL_FLAG            |
				         PT_TERMINAL_FLAG          |
#endif
		                 PT_PRESENT_FLAG           |
		        		 PT_WRITABLE_FLAG          |
		        		 PT_USER_FLAG              |
		        		 PT_WRITE_THROUGH_FLAG     ;

		write64(pdpe, data);
	}

#if(PAGE_SIZE==_2MEG)
	pde += 8 * (0x1ff & (virt >> 21));
	{
        uint64_t data = phy             |
        		PT_PRESENT_FLAG         |
        		PT_WRITABLE_FLAG        |
        		PT_USER_FLAG            |
        		PT_WRITE_THROUGH_FLAG   |
        		PT_TERMINAL_FLAG        |
        		PT_GLOBAL_FLAG          ;

        write64(pde, data);
	}
#endif /*** 2MEG PAGES ***/
}

/************************************************************************************************************
 * Create a page table structure for use in long mode by our bootloader.
 * The first 1 megabyte will be identity mapped.
 * high-mem will be mapped to virtual address 2Meg
 * we will map untill we run out of physical memory, or fill a whole pdpe ( max 1 gig with 2meg pages )
 */
void setup_pt(uint32_t needed_himem) {

	uint64_t vh = 0xFFFFFFFF80000000; // virtual hi-mem address
	uint64_t vl = 0x0000000000000000; // virtual lo-mem address
	uint64_t pb = 0; // physical base
	uint64_t pl = 0; // physical length
	uint64_t total = 0; // total mem mapped.
	uint64_t total_hi = 0;

	// get physical memory layout.
	struct mmap_e820h     *mmap = read_mmap();

	// iterate through physical memory regions.
	for(struct mmap_e820h_reg *mreg = mmap->map; mreg < (mmap->map + mmap->size); mreg++) {

		if(mreg->type != 1)
			continue; // NOT a usable region

		pb = mreg->b.b64; // this region base
		pl = mreg->l.l64; // this region length

		// adjust phyical base/length for PAGE_SIZE alignment.
		if(pb & (PAGE_SIZE-1)) {

			uint64_t waste = PAGE_SIZE - (pb % PAGE_SIZE);
			if(waste > pl)
				continue; // region too small to align, try next region.
			pl -= waste;
			if(pl < PAGE_SIZE)
				continue; // aligned region is smaller than one page, try next region.
			pb += waste;
		}

		// map region.
	    while((pl >= PAGE_SIZE) || (pb==0)) {

	    	if(pb < 0x100000) {
	    		/*** identity map low-mem ***/
	    		pt_map_page(vl,pb,0x11000,0x12000);
	    		vl += PAGE_SIZE;
	    	}
	    	else {

	    		/*** map high-mem ***/
	    		pt_map_page(vh,pb,0x13000,0x14000);
	    		vh += PAGE_SIZE;
	    		total_hi += PAGE_SIZE;
	    	}
	    	pb += PAGE_SIZE;
	    	if(pl > PAGE_SIZE)
	    		pl -= PAGE_SIZE;
	    	else
	    		pl = 0;

	    	/* allocated all we need! */
	    	if(total_hi >= needed_himem)
	    		return;

	    	total += PAGE_SIZE;

	    	/* allocated all we can! */
 			if(total >= (PAGE_SIZE * PAGE_TABLE_SIZE))
	    		return;
	    }
	}
}

