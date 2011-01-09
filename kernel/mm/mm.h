/*
 * mm.h
 *
 *  Created on: Jan 8, 2011
 *      Author: cds
 */

#ifndef MM_H_
#define MM_H_

#include <inttypes.h>

/*** memory page size ***/
#ifndef PAGE_SIZE
#define PAGE_SIZE 0x200000 /*** default 2meg pages ***/
#endif

/*** maximum number of pages supported ***/
#ifndef PAGE_MAX
#define PAGE_MAX 4096 /*** default maximum number of pages ***/
#endif

/*** maximum amount of physical memory supported. ( 8 Gigabytes using above defaults ) ***/
#define MAX_PHY_MEM (PAGE_MAX * PAGE_SIZE)

typedef BOOL(*mm_phy_detect_func)(uint64_t page);

#endif /* MM_H_ */
