/*
 * pt.h
 *
 *  Created on: Jan 10, 2011
 *      Author: cds
 */

#ifndef PT_H_
#define PT_H_

/*** memory page size ***/
#define PAGE_SIZE 0x200000

/*** maximum number of pages supported ***/
#define PAGE_MAX 4096

/*** maximum amount of physical memory supported. ( 8 Gigabytes using above defaults ) ***/
#define MAX_PHY_MEM (PAGE_MAX * PAGE_SIZE)

/*** all available physical memory will be mapped to virtual address (physical + VIRT_OFFSET) ***/
#define VIRT_OFFSET 0xFFFF800000000000

#endif /* PT_H_ */
