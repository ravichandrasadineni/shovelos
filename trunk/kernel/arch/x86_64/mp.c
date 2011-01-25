/*
 * mp.c
 *
 *  Created on: 25 Jan 2011
 *      Author: Chris Stones
 */

#include <inttypes.h>

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


