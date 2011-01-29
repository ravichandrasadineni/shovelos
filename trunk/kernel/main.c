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

//	acpi_init();

	for(const struct mp_ioapic *ioapic  = mp_find_first_io_apic();
								ioapic != 0;
								ioapic  = mp_find_next_io_apic( ioapic )) {

		void* base = (void*)(uint64_t)ioapic->mmap_ioapic;
		uint8_t ver = io_apic_read32(base, IOAPICVER) & 0xff;
		uint16_t max = io_apic_read32(base, IOAPICVER) >> 16;

		kprintf("FOUND AN IOAPIC ID %d (ver %d, max %d) @ 0x%8x\n", ioapic->ioapic_id, ver, max ,ioapic->mmap_ioapic);

	}

	for(const struct mp_processor *cpu  = mp_find_first_processor();
								   cpu != 0;
								   cpu  = mp_find_next_processor( cpu )) {

		kprintf("FOUND A CPU (ID %d)\n", cpu->local_apic_id);
	}

	kprintf("\nshovelos.kernel - \"HELLO WORLD!\"\n");

//	*((uint32_t*)(0xfec00000)) = 0;

	for(;;) {
	}
	return 0;
}

