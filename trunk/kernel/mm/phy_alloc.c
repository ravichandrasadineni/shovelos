/*
 * phy_alloc.c
 *
 *  Created on: Jan 8, 2011
 *      Author: cds
 */

#include <mm/mm.h>
#include <mm/phy_alloc.h>
#include <arch/arch.h>

static uint64_t phy_bitmap[(PAGE_MAX/64)+1];

#define PAGES_PER_GROUP ((sizeof phy_bitmap[0]) * 8)

void mm_phy_init(mm_phy_detect_func present, mm_phy_detect_func mapped) {

	/* set all pages to used / unavailable */
	memset(phy_bitmap, 0xff, sizeof phy_bitmap);

	/* set pages that physically exist, but are not presently in use as free */
	for(uint64_t p=1; p<PAGE_MAX; ++p) {
		uint64_t phy = mm_page_to_phy(p);
		if(detect( phy ) && !mapped( phy ))
			mm_phy_free_page(p);
	}
}

void mm_phy_free_page(uint64_t page) {

	phy_bitmap[page/64] &= ~(1 << (page % 64));
}

/****************************************************
 * mm_phy_alloc_page: Allocate the first free page.
 * returns page number, or zero on out of memory.
 */
uint64_t mm_phy_alloc_page() {

	for(uint64_t g=0; g< sizeof phy_bitmap / sizeof phy_bitmap[0]; ++g)
		if(phy_bitmap != 0xffffffffffffffff) {
			uint64_t _g = phy_bitmap[g];
			for(uint8_t p = 0; p< (sizeof phy_bitmap[0]) * 8; ++p)
				if((_g & (1 << p)) == 0) {

					phy_bitmap[g] |= 1 << p;

					return (g * PAGES_PER_GROUP) + p;
				}
		}
	return 0; /* no free pages  */
}

