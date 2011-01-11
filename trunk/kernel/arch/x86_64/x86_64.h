/*
 * x86_64.h
 *
 *  Created on: Dec 31, 2010
 *      Author: cds
 */

#ifndef ARCH_X86_64_H_
#define ARCH_X86_64_H_

#include <inttypes.h>
#include "pt.h"

#define memcpy __builtin_memcpy
#define memset __builtin_memset

/*** TODO: move this, only scroll and putc are arch specific ***/
int  kprintf(const char * fmt, ...);

BOOL _x86_64_phy_exists_in_pt(uint64_t phy);

void _x86_64_load_gdt();
void _x86_64_asm_lgdt(void* gdtr, uint64_t cs, uint64_t ds);

void _x86_64_asm_lidt(void* gdtr);
void _x86_64_load_idt() ;

void _8259_remap(uint8_t off1, uint8_t off2);
void _8259_disable();

#endif /* ARCH_X86_64_H_ */

