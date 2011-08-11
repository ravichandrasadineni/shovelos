/*
 * acpi.c
 *
 *  Created on: Jan 23, 2011
 *      Author: cds
 */

#include<mm/mm.h>
#include<arch/arch.h>
#include<lib/string.h>

struct rsdp_header {

	struct {

		sint8_t signature[8];
		uint8_t checksum;
		sint8_t oemid[6];
		uint8_t revision;
		uint32_t rsdt_address;
	} ver1;

	struct {

		uint32_t length;
		uint64_t xsdt_address;
		uint8_t  extended_checksum;
		uint8_t  reserved[3];
	} ver2;

} __attribute__((packed));


static uint8_t sum(const void * _data, uint64_t len) {

	uint8_t sum = 0;
	const uint8_t *data = (const uint8_t *)_data;

	while(len--)
		sum += *data++;

	return sum;
}

static sint8_t validate(const struct rsdp_header* header) {

	if(memcmp(header,"RSD PTR ",8)!=0)
		return -1; // no magic!

	if(sum(&(header->ver1), sizeof header->ver1) != 0)
		return -1; // corrupt or invalid.

      if(header->ver1.revision && (sum(&(header->ver2), sizeof header->ver2) != 0))
            return -1; // corrupt of invalid extended data.

	return 0; // all good!
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
static const struct rsdp_header* scan_for_rsdp(const uint8_t *_addr, uint64_t size) {

	for(const uint8_t* addr = _addr; addr < _addr + size; addr+=0x10)
		if(validate((const struct rsdp_header*)addr)==0)
			return (const struct rsdp_header*)addr;

	return (const struct rsdp_header*)0;
}

/***
 * search for root system descriptor pointer.
 */
static const struct rsdp_header* find_rsdp() {

	const struct rsdp_header* rsdp;

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
	if((rsdp = scan_for_rsdp(PHY_TO_VIRT(0xE0000, const uint8_t*), 0x20000)))
		return rsdp;

	return (const struct rsdp_header*)0;
}

sint8_t acpi_init() {

	const struct rsdp_header * rsdp;

	if(!(rsdp = find_rsdp())) {
		return -1;
	}

	return 0;
}


