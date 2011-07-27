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

int main(struct mm_phy_reg *reg, uint64_t len)  {

	_8259_disable();			/*** kill the legacy pic ***/
	mm_phy_init(reg,len); 		/*** initialise physical memory manager ***/
	pt_initialise(reg,len);		/*** retire boot-loaders page tables ***/
	_x86_64_load_gdt();			/*** retire boot-loaders gdt ***/
	_x86_64_load_idt();			/*** retire boot-loaders idt ***/
	acpi_init();				/*** parse acpi tables ***/
	ioapic_configure();			/*** configure the io-apic ***/

	/*** enumerate processors ***/
	for(const struct mp_processor *cpu  = mp_find_first_processor();
								   cpu != 0;
								   cpu  = mp_find_next_processor( cpu )) {

		kprintf("found a cpu (ID %d)\n", cpu->local_apic_id);
	}

	kprintf("shovelos.kernel - \"HELLO WORLD!\"\n");

	for(;;) {
	}
	return 0;
}

/*** FORCE linker script to put this code at the very start of the kernel binary ***/
__asm__ ( ".section .start_text\n"
						".global _start\n"
						"_start:\n"
						"	jmp main\n"	);

