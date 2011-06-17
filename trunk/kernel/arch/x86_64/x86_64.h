/*
 * x86_64.h
 *
 *  Created on: Dec 31, 2010
 *      Author: cds
 */

#ifndef ARCH_X86_64_H_
#define ARCH_X86_64_H_

#include <inttypes.h>
#include "msr.h"
#include "8259.h"
#include "lock.h"
#include "ticket_lock.h"
#include "pt.h"
#include "semaphore.h"
#include "acpi.h"
#include "mp.h"
#include "console.h"
#include "ioapic.h"
#include "lapic.h"

/*** todo: better home ***/
typedef void (*main_func)(int argc, char **argv);
void call_main(main_func, void* stack, int argc, char **argv);


int  kprintf(const char * fmt, ...);

void _x86_64_load_gdt();
void _x86_64_asm_lgdt(void* gdtr, uint64_t cs, uint64_t ds);

void _x86_64_asm_lidt(void* gdtr);
void _x86_64_load_idt() ;

void _8259_remap(uint8_t off1, uint8_t off2);
void _8259_disable();

#endif /* ARCH_X86_64_H_ */

