/*
 * shov.c
 *
 *  Created on: Jan 9, 2011
 *      Author: cds
 *
 *      function for accessing boot info prepared for us by the ShovelOS boot-loader.
 */

#include <inttypes.h>
#include <arch/arch.h>

#define MB_MMAP     ((uint32_t*)0x40000) /* THIS ADDRESS CURRENTLY ALSO HARD CODED INTO THE BOOT-LOADER */

enum types {
	UNKNOWN=0,
	USABLE,
	RESERVED,
	ACPI_RECLAIMABLE,
	ACPI_NVS,
	BAD,
};

struct mem_reg {

	uint64_t base;
	uint64_t len;
	uint32_t type;
} __attribute__((packed)) ;

static BOOL exists(struct mem_reg *reg, uint64_t phy) {

	if((reg->type == USABLE) &&
	   (reg->base <= phy   ) &&
	   (reg->base + reg->len >= phy + PAGE_SIZE))
			return TRUE;

	return FALSE;
}

BOOL _shovboot_phy_exists(uint64_t phy) {

	uint32_t size = *MB_MMAP;
	struct mem_reg *reg_base = (struct mem_reg*)(MB_MMAP+1);

	for(struct mem_reg *r = reg_base; r < reg_base + size; ++r)
		if(exists(r, phy) == TRUE)
			return TRUE;

	return FALSE;
}

