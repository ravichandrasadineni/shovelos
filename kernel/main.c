/*
 * main.c
 *
 *  Created on: Dec 27, 2010
 *      Author: cds
 */

#include <inttypes.h>
#include <arch/arch.h>
#include <mm/mm.h>
#include <arch/arch.h>


int _start(struct mm_phy_reg *reg, uint64_t len) {

	mm_phy_init(reg,len);

	_x86_64_load_gdt();
	_x86_64_load_idt();

	acpi_init();

	kprintf("shovelos.kernel - \"HELLO WORLD!\"\n");


	for(;;) {
	}
	return 0;
}

