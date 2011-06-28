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

	mm_phy_init(reg,len); 		/*** initialise physical memory manager ***/
	pt_initialise(reg,len);		/*** retire boot-loaders page tables ***/
    ioapic_configure();

	_x86_64_load_gdt();
	_x86_64_load_idt();

	acpi_init();

	for(const struct mp_processor *cpu  = mp_find_first_processor();
								   cpu != 0;
								   cpu  = mp_find_next_processor( cpu )) {

		kprintf("FOUND A CPU (ID %d)\n", cpu->local_apic_id);
	}


	if(cpu_has_cpuid()) {

			kprintf("cpuid is supported\n");
		}
		else {

			kprintf("bootstrap processor does not support cpuid!?\n");
			for(;;);
		}


	kprintf("\nshovelos.kernel - \"HELLO WORLD!\"\n");

	for(;;) {
	}
	return 0;
}

