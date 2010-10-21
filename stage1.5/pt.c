
/************************************************************************************************************
 * Setup long-mode page tables.
 * Identity map the first 2meg.
 */

#include "16bitreal.h"
#include "inttypes.h"
#include "mem.h"
#include "pt.h"
#include "mmap.h"
#include "alloc.h"

static struct PML4E *g_pmle4 = 0;

/************************************************************************************************************
 * Create a 64bit virtual -> physical page mapping
 * takes 1) virtual page base ( PAGE_SIZE aligned )
 *       2) physical page base ( PAGE_SIZE aligned )
 */
void pt_map_page(uint64_t virt, uint64_t phy) {

	struct PML4E *pmle4 = g_pmle4;
	struct PDPE  *pdpe;
	struct PDE   *pde;

	if(pmle4 != 0) {
		g_pmle4 =
		pmle4	= (struct PML4E *)zalloc_align(PAGE_TABLE_ALIGNLENT, PAGE_TABLE_SIZE * sizeof(struct PML4E));
	}
	pmle4 += 0x1ff & (virt >> 39);

	pdpe = pt_get_pdpe(pmle4);
	if(pdpe != 0) {
		pmle4->bits.PageDirectoryPtr52 = (int)(pdpe = (struct PDPE*)zalloc_align(PAGE_TABLE_ALIGNLENT, PAGE_TABLE_SIZE * sizeof(struct PDPE)));
		pmle4->bits.attr.P  = 1; // present
		pmle4->bits.attr.RW = 1; // writable
	}
	pdpe += 0x1ff & (virt >> 30);

	pde = pt_get_pde(pdpe);
	if(pde != 0) {
		pdpe->bits.PageDirectory52 = (int)(pde = (struct PDE*)zalloc_align(PAGE_TABLE_ALIGNLENT, PAGE_TABLE_SIZE * sizeof(struct PDE)));
		pdpe->bits.attr.P  = 1; // present
		pdpe->bits.attr.RW = 1; // writable
	}
	pde += 0x1ff & (virt >> 21);

	pde->bits.PhysicalPage52 = phy;
	pde->bits.attr.P  = 1; // present
	pde->bits.attr.RW = 1; // writable
	pde->bits.attr.PS = 1; // terminal table
}


/************************************************************************************************************
 * Create a page table structure for use in long mode by our bootloader.
 * The first 1 megabyte will be identity mapped.
 * high-mem will be mapped to virtual address 0x8000000000
 * we will map untill we run out of physical memory, or fill a whole pdpe ( max 1 gig with 2meg pages )
 */
void setup_pt() {

	uint64_t vh = 0x8000000000; // virtual hi-mem address
	uint64_t vl = 0x0000000000; // virtual lo-mem address
	uint64_t pb = 0; // physical base
	uint64_t pl = 0; // physical length
	uint64_t total_hi = 0; // total hi-mem mapped.

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
	    		pt_map_page(vl,pb);
	    		vl += PAGE_SIZE;
	    	}
	    	else {
	    		/*** map high-mem ***/
	    		pt_map_page(vh,pb);
	    		vh += PAGE_SIZE;
	    		total_hi += PAGE_SIZE;

	    		// mapping more than 1 gig high-mem would require another pdpe table on our very limited heap!
	    		// and 1 is WAY more than enough for a bootloader.
	    		// provides upto 1 gig with a 2Meg pages!
	    		if(total_hi >= (PAGE_SIZE * PAGE_TABLE_SIZE))
	    			return;
	    	}
	    	pb += PAGE_SIZE;
	    	if(pl > PAGE_SIZE)
	    		pl -= PAGE_SIZE;
	    	else
	    		pl = 0;
	    }
	}
}

