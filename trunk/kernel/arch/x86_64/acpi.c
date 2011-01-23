/*
 * acpi.c
 *
 *  Created on: Jan 23, 2011
 *      Author: cds
 */

#include<mm/mm.h>
#include<arch/arch.h>


static uint8_t memcmp(const void* s1, const void* s2, uint64_t len) {

	const uint8_t *a = (const uint8_t*)s1;
	const uint8_t *b = (const uint8_t*)s2;
	uint8_t ret;

	while(len--)
		if((ret = ((*a++) - (*b++))))
			return ret;

	return 0;
}

/***
 * find the extended BIOS data area
 */
static const uint8_t* ebda_addr() {

	uint64_t seg_addr = PHY_TO_VIRT(0x40e, const uint16_t*)[0];
	         seg_addr <<= 4;

	return  PHY_TO_VIRT(seg_addr, const uint8_t*);
}

/***
 * search given range for root system descriptor pointer.
 */
static const uint8_t* scan_for_rsdp(const uint8_t *_addr, uint64_t size) {

	for(const uint8_t* addr = _addr; addr < _addr + size; addr+=0x10)
		if(memcmp(addr,"RSD PTR ",8)==0)
			return addr;

	return 0;
}

/***
 * search for root system descriptor pointer.
 */
static const uint8_t* find_rsdp() {

	const uint8_t* rsdp;

	/*** search extended BIOS data area ***/
	{
		const uint8_t* ebda = ebda_addr();
		if(((uint64_t)ebda) & 0x0f) {
			ebda +=  0x10;
			ebda -= (((uint64_t)ebda) & 0x0f);
		}

		if((rsdp = scan_for_rsdp(ebda, 0x400)))
			return rsdp;
	}

	/*** search BIOS main area ***/
	if((rsdp = scan_for_rsdp((const uint8_t*)0xE0000, 0x20000)))
		return rsdp;

	return 0;
}

sint8_t acpi_init() {

	const uint8_t* rsdp;

	if(!(rsdp = find_rsdp())) {
		kprintf("ACPI: no hardware support\n");
		return -1;
	}
	else
		kprintf("ACPI: RSDP @ 0x%016lx\n", rsdp);

	return 0;
}


