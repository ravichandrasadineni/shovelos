/*
 * main.c
 *
 *  Created on: Dec 27, 2010
 *      Author: cds
 */

#include <inttypes.h>
#include <arch/arch.h>
#include <boot/boot.h>
#include <mm/mm.h>
#include <arch/arch.h>

int _start() {

	_x86_64_load_idt();
	_x86_64_load_gdt();

	kprintf("shovelos.kernel -\"HELLO WORLD!\"\n");

	/*** initialise the physical memory allocator ***/
	mm_phy_init(
		&_shovboot_phy_exists,
		&_x86_64_phy_exists_in_pt);

	for(;;) {
//		int key = port_inb(0x60);
//		if(key)
//			kprintf("%c",key);
	}
	return 0;
}

