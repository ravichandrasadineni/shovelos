/*
 * virt_alloc.h
 *
 *  Created on: March 19, 2011
 *      Author: cds
 */

#ifndef __MM_VIRT_ALLOC_H
#define __MM_VIRT_ALLOC_H

#include<arch/arch.h>

#define VMM_PHY_BASE 		(0xffff800000000000)
#define VMM_PHY_TOP  		(0xffff900000000000 - PAGE_SIZE)
#define VMM_HWMMAP_BASE 	(0xffff900000000000)
#define VMM_HWMMAP_TOP		(0xffffa00000000000 - PAGE_SIZE)
#define VMM_HEAP_BASE 		(0xffffa00000000000)
#define VMM_HEAP_TOP		(0xffffb00000000000 - PAGE_SIZE)
#define VMM_STACK_BASE 		(0xffffb00000000000)
#define VMM_STACK_TOP		(0xffffc00000000000)
#define VMM_KERNEL_IMG		(0xFFFFFFFF80000000)

void *morecore(uint64_t size);
uint64_t vmm_heap_size();

void* vmm_alloc_hw(uint64_t size);

#endif /*** __MM_VIRT_ALLOC_H ***/
