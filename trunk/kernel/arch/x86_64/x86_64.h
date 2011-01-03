/*
 * x86_64.h
 *
 *  Created on: Dec 31, 2010
 *      Author: cds
 */

#ifndef ARCH_X86_64_H_
#define ARCH_X86_64_H_

#include "../../inttypes.h"

void _x86_64_load_gdt();
void _x86_64_asm_lgdt(void* gdtr, uint64_t cs, uint64_t ds);

void _x86_64_asm_lidt(void* gdtr);
void _x86_64_load_idt() ;

#endif /* ARCH_X86_64_H_ */

