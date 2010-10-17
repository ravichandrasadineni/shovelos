
/************************************************************************************************************
 * Setup long-mode page tables.
 * Identity map the first 2meg.
 */

#include "16bitreal.h"
#include "inttypes.h"
#include "mem.h"
#include "pt.h"

static struct PML4E *pmle4 = (struct PML4E *)alloc(512 * sizeof(struct PMLE4));
static struct PDPE  *pdpe  = (struct PDPE  *)alloc(512 * sizeof(struct PDPE ));
static struct PDE 	 *pde  = (struct PDE   *)alloc(512 * sizeof(struct PDE  ));

void pt_map(uint32_t virt, uint32 phy) {

	struct PML4E *ppml4e = pmle4 + ((virt >> 39) & 0x1ff);
	struct PDPE  *ppdpe  = ppml4e.PageDirectoryPtr &

	int pdpe_idx  = (virt >> 30) & 0x1ff);
	int pde_idx   = (virt >> 21) & 0x1ff);

}

void setup_pt() {

	pmle4 = (struct PML4E *)zalloc_align(0x200000, 512 * sizeof(struct PMLE4));
	pdpe  = (struct PDPE  *)zalloc_align(0x200000, 512 * sizeof(struct PDPE ));
	pde   = (struct PDE   *)zalloc_align(0x200000, 512 * sizeof(struct PDE  ));

	pmle4[0].PageDirectoryPtr = pdpe + 0;
	pmle4[0].attr.P  = 1; // present
	pmle4[0].arrt.RW = 1; // writable

	pdpe[0].PageDirectory = pde + 0;
	pdpe[0].attr.P  = 1; // present
	pdpe[0].arrt.RW = 1; // writable

	pde[0].attr.P =  1; // present
	pde[0].attr.RW = 1; // writable
}

