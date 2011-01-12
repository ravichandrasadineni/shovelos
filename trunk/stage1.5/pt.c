
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
void write64( uint32_t addr20, uint64_t value ) {

	printf("write64 addr 0x%x, data 0x%lx\n", addr20, value);
	memcpy((void*)addr20, &value, 8);
}

uint32_t pt_get_addr(uint32_t addr20) {

	uint64_t pt = 0 ;

	printf("pt_get_addr addr 0x%x = ", addr20);

	memcpy( &pt, (const void*)addr20, 8);

	printf(" AD(0x%lx)\n", pt);

	return  (uint32_t)ALIGN_DOWN( pt );
}


/************************************************************************************************************
 * Create a 64bit virtual -> physical page mapping
 * takes 1) virtual page base ( PAGE_SIZE aligned )
 *       2) physical page base ( PAGE_SIZE aligned )
 */
void pt_map_page(uint64_t virt, uint64_t phy, int pdpe_base) {

	volatile uint32_t pml4e = 0x10000; //&_pml4e;
	uint32_t pdpe;

#ifdef DEBUG
	printf("pt_map_page 0x%lx -> 0x%lx...\n",virt,phy);
#endif

	printf("pml4e = 0x%x (0x%x)\n", pml4e, &pml4e);

	pml4e += 8 * (0x1ff & (virt >> 39));

	pdpe = pt_get_addr(pml4e);

	if(pdpe == 0) {

		volatile uint64_t data  = (pdpe = pdpe_base)        |
		                 PT_PRESENT_FLAG           |
		        		 PT_WRITABLE_FLAG          |
		        		 PT_USER_FLAG              |
		        		 PT_WRITE_THROUGH_FLAG     ;


		printf("pml4e = 0x%x (0x%x)\n", pml4e, &pml4e);
		printf("data = 0x%lx (0x%x)\n", data, &data);
		write64(pml4e, data);
		printf("pml4e = 0x%x (0x%x)\n", pml4e, &pml4e);
		printf("data = 0x%lx (0x%x)\n", data, &data);
	}
	pdpe += 8 * (0x1ff & (virt >> 30));

	{
		volatile uint64_t data  = phy                       |
		                 PT_GLOBAL_FLAG            |
				         PT_TERMINAL_FLAG          |
		                 PT_PRESENT_FLAG           |
		        		 PT_WRITABLE_FLAG          |
		        		 PT_USER_FLAG              |
		        		 PT_WRITE_THROUGH_FLAG     ;

		write64(pdpe, data);
	}

#ifdef DEBUG
	printf("done\n",virt,phy);
#endif
}

/************************************************************************************************************
 * Create a page table structure for use in long mode by our bootloader.
 * The first 1 megabyte will be identity mapped.
 * high-mem will be mapped to virtual address 2Meg
 * we will map untill we run out of physical memory, or fill a whole pdpe ( max 1 gig with 2meg pages )
 */







void setup_pt(uint32_t needed_himem) {

	uint64_t pb = 0; // physical base
	uint64_t pl = 0; // physical length
	uint64_t addr64 = 0;
	struct mmap_e820h_reg kernel_reg;

	// get physical memory layout.
	struct mmap_e820h     *mmap = read_mmap();

	/*** identity map memory ***/
	for(uint64_t i=0; i<1/*PAGE_TABLE_SIZE*/; i++) {

		pt_map_page(addr64,addr64,0x11000);
		addr64 += PAGE_SIZE;

	}

	memset(&kernel_reg, 0, sizeof kernel_reg);

	// iterate through physical memory regions.
	for(struct mmap_e820h_reg *mreg = mmap->map; mreg < (mmap->map + mmap->size); mreg++) {

		if(mreg->type != 1)
			continue; // NOT a usable region

		pb = mreg->b.b64; // this region base
		pl = mreg->l.l64; // this region length

		// adjust phyical base/length for PAGE_SIZE alignment.
		if(pb & (_2MEG-1)) {

			uint64_t waste = _2MEG - (pb % _2MEG);
			if(waste > pl)
				continue; // region too small to align, try next region.
			pl -= waste;
			if(pl < _2MEG)
				continue; // aligned region is smaller than one page, try next region.
			pb += waste;
		}

		if(pb && (pl >= needed_himem) && (( pl < kernel_reg.l.l64 ) || !kernel_reg.l.l64 )) {
			kernel_reg.type = 1;
			kernel_reg.b.b64 = pb;
			kernel_reg.l.l64 = pl;
		}
	}

	if(kernel_reg.type == 1)
		pt_map_page(0xFFFFFFFF80000000,kernel_reg.b.b64,0x12000);
	else
		halt("couldnt find a suitable place for the kernel!");

}

