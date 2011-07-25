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

extern uint32_t x,y;

int main(struct mm_phy_reg *reg, uint64_t len)  {

	mm_phy_init(reg,len); 		/*** initialise physical memory manager ***/
	pt_initialise(reg,len);		/*** retire boot-loaders page tables ***/
    //ioapic_configure();

	_x86_64_load_gdt();
	_x86_64_load_idt();
	acpi_init();

	ioapic_configure();

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

//	kprintf("test divide by %lx\n", x/y);

	for(;;) {
	}
	return 0;
}

/*** FORCE linker script to put this code at the very start of the kernel binary ***/
__asm__ ( ".section .start_text\n"
						".global _start\n"
						"_start:\n"
						"	jmp main\n"	);

