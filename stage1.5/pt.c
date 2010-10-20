
/************************************************************************************************************
 * Setup long-mode page tables.
 * Identity map the first 2meg.
 */

#include "16bitreal.h"
#include "inttypes.h"
#include "mem.h"
#include "pt.h"

static struct PML4E *g_pmle4 = 0;

void pt_map_page(uint64_t virt, uint64_t phy) {

	struct PML4E *pmle4 = g_pmle4;
	struct PDPE  *pdpe;
	struct PDP   *pde;

	if(!pmle4) {
		g_pmle4 =
		pmle4	= (struct PML4E *)zalloc_align(PAGE_SIZE, 512 * sizeof(struct PMLE4));
	}
	pmle4 += 0x1ff & (virt >> 39);

	pdpe = pt_get_pdpe(pmle4);
	if(!pdpe) {
		pmle4->PageDirectoryPtr = pdpe = (struct PDPE*)zalloc_align(PAGE_SIZE, 512 * sizeof(struct PDPE));
		pmle4->attr.P  = 1; // present
		pmle4->attr.RW = 1; // writable
	}
	pdpe += 0x1ff & (virt >> 30);

	pde = pg_get_pde(pdpe);
	if(!pde) {
		pdpe->PageDirectory = pde = (struct PDE*)zalloc_align(PAGE_SIZE, 512 * sizeof(struct PDE));
		pdpe->attr.P  = 1; // present
		pdpe->attr.RW = 1; // writable
	}
	pde += 0x1ff & (virt >> 21);

	pde->PhysicalPage = phy;
	pde->attr.P  = 1; // present
	pde->attr.RW = 1; // writable
	pde->attr.PS = 1; // terminal table
}

void setup_pt() {

	pt_map_page(0,0); // identity map first 2 meg.

	uint64_t v = 0x8000000000;
	uint64_t p = 0/** TODO: GET START OF PHY HIMEM ***/

	// map upto a gig at virtual
	for(int i=0; i<512; i++) {

		pt_map_page(v,p);
		v+=PAGE_SIZE;
		p+=PAGE_SIZE;
	}
}

