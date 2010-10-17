
/************************************************************************************************************
 * Setup long-mode page tables.
 * Identity map the first 2meg.
 */

#include "16bitreal.h"
#include "inttypes.h"
#include "mem.h"
#include "pt.h"

static struct PML4E *pmle4 = 0;
static struct PDPE  *pdpe  = 0;
static struct PDE 	 *pde  = 0;

void pt_map_page(uint32_t virt, uint32 phy) {

	if(!pmle4)
		pmle4 = (struct PML4E *)zalloc_align(PAGE_SIZE, 512 * sizeof(struct PMLE4));

	struct PML4E *pmle4_off = pmle4 + ((virt >> 39) & 0x1ff );

	if(!pmle4_off->P)


	//	pdpe  = (struct PDPE  *)zalloc_align(PAGE_SIZE, 512 * sizeof(struct PDPE ));
		//pde   = (struct PDE   *)zalloc_align(PAGE_SIZE, 512 * sizeof(struct PDE  ));
}

void setup_pt() {

	pt_map_page(0,0); // identity map first 2 meg.

	pmle4[0].PageDirectoryPtr = pdpe + 0;
	pmle4[0].attr.P  = 1; // present
	pmle4[0].arrt.RW = 1; // writable

	pdpe[0].PageDirectory = pde + 0;
	pdpe[0].attr.P  = 1; // present
	pdpe[0].arrt.RW = 1; // writable

	pde[0].attr.P =  1; // present
	pde[0].attr.RW = 1; // writable
}

