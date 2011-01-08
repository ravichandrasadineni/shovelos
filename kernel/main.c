/*
 * main.c
 *
 *  Created on: Dec 27, 2010
 *      Author: cds
 */

#include <inttypes.h>
#include <arch/arch.h>

int main() {

	_x86_64_load_idt();
	_x86_64_load_gdt();

	kprintf("shovelos.kernel -\"HELLO WORLD!\"\n");

	for(;;) {
//		int key = port_inb(0x60);
//		if(key)
//			kprintf("%c",key);
	}
	return 0;
}


