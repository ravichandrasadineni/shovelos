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

static int running_processors = 0;

int main(struct mm_phy_reg *reg, uint64_t len, void (*boot_aux_cpu)())  {

	running_processors++;

	// TODO: catch aux cpu threads and spin them for now!
	static BOOL bsp = TRUE;
	if(!bsp)
		for(;;);
	bsp = FALSE;

	kbc_initialise();
	_8259_disable();			/*** kill the legacy pic ***/
	mm_phy_init(reg,len); 		/*** initialise physical memory manager ***/
	pt_initialise(reg,len);		/*** retire boot-loaders page tables ***/
	_x86_64_load_gdt();			/*** retire boot-loaders gdt ***/
	_x86_64_load_idt();			/*** retire boot-loaders idt ***/
	lapic_configure();			/*** configure the local-apic ***/
	ioapic_configure();			/*** configure the io-apic ***/
	hpet_init();				/*** configure HPET ***/

	kprintf("shovelos.kernel - \"HELLO WORLD!\"\n");

	kprintf("#> ");

	for(;;) {

		sint32_t c = kbc_readchar();

		if(c>=0) {
			kprintf("%c",c);
			if(c=='\n')
				kprintf("#> ");
		}
	}
	return 0;
}

/*** FORCE linker script to put this code at the very start of the kernel binary ***/
__asm__ ( ".section .start_text\n"
						".global _start\n"
						"_start:\n"
						"	jmp main\n"	);

