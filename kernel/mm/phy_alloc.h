/*
 * phy_alloc.h
 *
 *  Created on: Jan 8, 2011
 *      Author: cds
 */

#ifndef PHY_ALLOC_H_
#define PHY_ALLOC_H_

#include <mm/mm.h>

static inline uint64_t mm_phy_to_page(uint64_t phy) {

	return phy / PAGE_SIZE;
}

static inline uint64_t mm_page_to_phy(uint64_t page) {

	return page * PAGE_SIZe;
}

static inline uint64_t mm_phy_alloc() {

	return mm_page_to_phy( mm_phy_alloc_page() );
}

static inline void mm_phy_free(uint64_t phy) {

	mm_phy_free_page( mm_phy_to_page( phy ) );
}

#endif /* PHY_ALLOC_H_ */

