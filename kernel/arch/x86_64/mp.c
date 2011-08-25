/*
 * mp.c - Parse the PC multiprocessor tables.
 *
 *  Created on: 25 Jan 2011
 *  Author: cds ( chris.stones _AT_ gmail.com )
 */

#include<mm/mm.h>
#include<arch/arch.h>
#include<lib/string.h>

#include "mp.h"

/*** multiprocessor floating pointer ***/
struct mp_fp {

	sint8_t  signature[4]; /*** "_MP_" ***/
	uint32_t phy_address;
	uint8_t  length;
	uint8_t  spec_revision;
	uint8_t  checksum;
	uint8_t  default_config;
	unsigned reserved0 : 7;
	unsigned imcrp : 1;
	unsigned reserved1 : 24;

} __attribute__ ((packed));

/*** multiprocessor configuration table header ***/
struct mp_header {

	sint8_t 	signature[4]; /*** "PCMP" ***/
	uint16_t 	base_table_length;
	uint8_t		spec_revision;
	uint8_t		checksum;
	sint8_t		oem_id[8];
	sint8_t		product_id[12];
	uint32_t	oem_table_ptr;
	uint16_t	oem_table_size;
	uint16_t	entry_count;
	uint32_t	mmap_lapic;
	uint16_t	extended_table_length;
	uint8_t		extended_table_checksum;
	uint8_t		reserved;

} __attribute__ ((packed));

enum entry_type {

	Processor = 0,
	Bus = 1,
	IOApic = 2,
	IOInterruptAssignment=3,
	LocalInterruptAssignment=4,
};

static inline uint16_t sizeof_type(enum entry_type et) {

	if(et == Processor)
		return 20;

	return 8;
}

/*** mp table checksum ***/
static uint8_t sum(const void * _data, uint64_t len) {

	uint8_t sum = 0;
	const uint8_t *data = (const uint8_t *)_data;

	while(len--)
		sum += *data++;

	return sum;
}

/*** validate floating pointer  ***/
static sint8_t validate(const struct mp_fp* fp) {

	if(memcmp(fp->signature,"_MP_",4)!=0)
		return -1; // no magic!

	if(sum(fp, sizeof *fp) != 0)
		return -1; // corrupt or invalid.

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
 * search given range for multi-processor floating pointer.
 */
static const struct mp_fp* scan_for_fp(const uint8_t *_addr, uint64_t size) {

	for(const uint8_t* addr = _addr; addr < _addr + size; addr+=0x01)
		if(validate((const struct mp_fp*)addr)==0)
			return (const struct mp_fp*)addr;

	return (const struct mp_fp*)0;
}

/***
 * search for multi-processor floating pointer.
 */
static const struct mp_fp* find_fp() {

	const struct mp_fp* fp;

	/*** search extended BIOS data area ***/
	if((fp = scan_for_fp(ebda_addr(), 0x400)))
			return fp;

	/*** search BIOS main area ***/
	if((fp = scan_for_fp(PHY_TO_VIRT(0xE0000, const uint8_t*), 0x20000)))
		return fp;

	return (const struct mp_fp*)0;
}

/***
 * get a multiprocessor header from the given floating pointer
 */
static const struct mp_header *find_header(const struct mp_fp* fp) {

	if(fp) {

		const struct mp_header *h;

		if((h = (const struct mp_header *)(uint64_t)fp->phy_address)) {

			h = PHY_TO_VIRT(h, const struct mp_header *);

			if( (memcmp(h->signature, "PCMP", sizeof h->signature) == 0) &&
				(sum(h, h->base_table_length) == 0)) {

				return h;
			}
		}
	}
	return (const struct mp_header *)0;
}

/***
 * iterate the multiprocessor tables.
 * Use the type-safe wrappers below.
 */
static const void* mp_find_next_(enum entry_type type, const void* last) {

	const struct mp_header *header = find_header( find_fp() );

	if(header) {

		uint8_t next = last ? 0 : 1;

		const uint8_t* entry = (const uint8_t*)(header+1);

		for(uint16_t count=0; count<header->entry_count; ++count) {

			if(*entry == type) {
				if(next)
					return entry;
				else if(entry == last)
					next = 1;
			}

			entry += sizeof_type(*entry);
		}
	}

	return 0;
}

const struct mp_processor* mp_find_first_processor(  ) {

	return (const struct mp_processor*)mp_find_next_(Processor, 0);
}

const struct mp_processor* mp_find_next_processor( const struct mp_processor *last ) {

	return (const struct mp_processor*)mp_find_next_(Processor, last);
}

const struct mp_bus* mp_find_first_bus(  ) {

	return (const struct mp_bus*)mp_find_next_(Bus, 0);
}

const struct mp_bus* mp_find_next_bus( const struct mp_bus *last ) {

	return (const struct mp_bus*)mp_find_next_(Bus, last);
}

const struct mp_ioapic* mp_find_first_io_apic( ) {

	return (const struct mp_ioapic*)mp_find_next_(IOApic, 0);
}

const struct mp_ioapic* mp_find_next_io_apic( const struct mp_ioapic *last ) {

	return (const struct mp_ioapic*)mp_find_next_(IOApic, last);
}

const struct mp_io_interrupt_assignment* mp_find_first_io_interrupt( ) {

	return (const struct mp_io_interrupt_assignment*)mp_find_next_(IOInterruptAssignment, 0);
}

const struct mp_io_interrupt_assignment* mp_find_next_io_interrupt( const struct mp_io_interrupt_assignment *last ) {

	return (const struct mp_io_interrupt_assignment*)mp_find_next_(IOInterruptAssignment, last);
}

const struct mp_local_interrupt_assignment* mp_find_first_local_interrupt(  ) {

	return (const struct mp_local_interrupt_assignment*)mp_find_next_(LocalInterruptAssignment, 0);
}

const struct mp_local_interrupt_assignment* mp_find_next_local_interrupt( const struct mp_local_interrupt_assignment *last ) {

	return (const struct mp_local_interrupt_assignment*)mp_find_next_(LocalInterruptAssignment, last);
}








