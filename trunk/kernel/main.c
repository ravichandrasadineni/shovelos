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

	kprintf("-%s-", "shovelos.kernel -\"HELLO WORLD!\"\n");
	kprintf("-s-", "abcdefg\n");
	kprintf("-s-", "ABCDEFG\n");
	kprintf("-s-", "1234567\n");

	for(;;);
	return 0;
}


