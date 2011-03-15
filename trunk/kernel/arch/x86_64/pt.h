/*
 * pt.h
 *
 *  Created on: Jan 10, 2011
 *      Author: cds
 */

#ifndef PT_H_
#define PT_H_

#include<inttypes.h>

uint64_t virt_to_phy(uint64_t virt);

/*** memory page size ***/
#define PAGE_SIZE 0x200000

/*** maximum number of pages supported ***/
#define PAGE_MAX 4096

/*** maximum amount of physical memory supported. ( 8 Gigabytes using above defaults ) ***/
#define MAX_PHY_MEM (PAGE_MAX * PAGE_SIZE)

/*** all available physical memory will be mapped to virtual address (physical + VIRT_OFFSET) ***/
#define VIRT_OFFSET 0xFFFF800000000000

/*** pointer arithmetic ***/
#define PHY_TO_VIRT(phy, _type) ((_type)(((uint8_t*)phy) + VIRT_OFFSET))

#define VIRT_KERNEL_BASE 0xFFFFFFFF80000000

#endif /* PT_H_ */
