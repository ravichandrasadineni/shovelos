/*
 * mp.c
 *
 *  Created on: 25 Jan 2011
 *      Author: Chris Stones
 */

#include<mm/mm.h>
#include<arch/arch.h>
#include<lib/string.h>

/*** multi-processor floating pointer ***/
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

/*** multi-processor configuration table header ***/
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

struct processor {

	uint8_t entry_type;
	uint8_t local_apic_id;
	uint8_t local_apic_version;
	unsigned flag_en : 1;
	unsigned flag_bp : 1;
	unsigned reserved0 : 6;
	uint32_t cpu_signature;
	uint32_t feature_flags;
	uint64_t reserved;

} __attribute__ ((packed));

struct bus {

	uint8_t entry_type;
	uint8_t bus_id;
	sint8_t bus_type_string[6];

} __attribute__ ((packed));

struct ioapic {

	uint8_t entry_type;
	uint8_t ioapic_id;
	uint8_t ioapic_version;
	unsigned flag_en : 1;
	unsigned reserved0 : 7;
	uint32_t mmap_ioapic;

} __attribute__ ((packed));

struct io_inturrupt_assignment {

	uint8_t entry_type;
	uint8_t interrupt_type;
	unsigned flag_po : 1;
	unsigned flag_el : 1;
	unsigned reserved0 : 6;
	uint8_t src_bus_id;
	uint8_t src_bus_irq;
	uint8_t dst_ioapic_id;
	uint8_t dst_ioapic_intin;

} __attribute__ ((packed));

struct local_inturrupt_assignment {

	uint8_t entry_type;
	uint8_t interrupt_type;
	unsigned flag_po : 1;
	unsigned flag_el : 1;
	unsigned reserved0 : 6;
	uint8_t src_bus_id;
	uint8_t src_bus_irq;
	uint8_t dst_lapic_id;
	uint8_t dst_llapic_intin;

} __attribute__ ((packed));

static inline uint16_t get_var_table_length(enum entry_type e) {

	if(e == Processor)
		return 20;
	return 8;
}

static uint8_t sum(const void * _data, uint64_t len) {

	uint8_t sum = 0;
	const uint8_t *data = (const uint8_t *)_data;

	while(len--)
		sum += *data++;

	return sum;
}

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
 * search given range for root system descriptor pointer.
 */
static const struct mp_fp* scan_for_rsdp(const uint8_t *_addr, uint64_t size) {

	for(const uint8_t* addr = _addr; addr < _addr + size; addr+=0x01)
		if(validate((const struct mp_fp*)addr)==0)
			return (const struct mp_fp*)addr;

	return (const struct mp_fp*)0;
}

/***
 * search for root system descriptor pointer.
 */
static const struct mp_fp* find_rsdp() {

	const struct mp_fp* fp;

	/*** search extended BIOS data area ***/
	if((fp = scan_for_rsdp(ebda_addr(), 0x400)))
			return fp;

	/*** search BIOS main area ***/
	if((fp = scan_for_rsdp(PHY_TO_VIRT(0xE0000, const uint8_t*), 0x20000)))
		return fp;

	return (const struct mp_fp*)0;
}

sint8_t mp_init() {

	const struct mp_fp * fp;

	if(!(fp = find_rsdp())) {
		kprintf("MP: multi-processor support\n");
		return -1;
	}

	kprintf("MP:   MPFP @ 0x%016lx\n", fp);
	kprintf("      version %d\n", fp->spec_revision);


	return 0;
}



