/*
 * mp.h
 *
 *  Created on: Jan 25, 2011
 *      Author: cds
 */

/*** DROPPED IN FAVOR OF APCI ***/
#if(0)

#ifndef MP_H_
#define MP_H_

#include<inttypes.h>

struct mp_processor {

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

struct mp_bus {

	uint8_t entry_type;
	uint8_t bus_id;
	sint8_t bus_type_string[6];

} __attribute__ ((packed));

struct mp_ioapic {

	uint8_t entry_type;
	uint8_t ioapic_id;
	uint8_t ioapic_version;
	unsigned flag_en : 1;
	unsigned reserved0 : 7;
	uint32_t mmap_ioapic;

} __attribute__ ((packed));

struct mp_io_interrupt_assignment {

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

struct mp_local_interrupt_assignment {

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

const struct mp_processor*						mp_find_first_processor			(  );
const struct mp_bus* 							mp_find_first_bus				(  );
const struct mp_ioapic* 						mp_find_first_io_apic			(  );
const struct mp_io_interrupt_assignment* 		mp_find_first_io_interrupt		(  );
const struct mp_local_interrupt_assignment* 	mp_find_first_local_interrupt	(  );

const struct mp_processor* 						mp_find_next_processor			( const struct mp_processor 					*last );
const struct mp_bus* 							mp_find_next_bus				( const struct mp_bus 							*last );
const struct mp_ioapic* 						mp_find_next_io_apic			( const struct mp_ioapic 						*last );
const struct mp_io_interrupt_assignment* 		mp_find_next_io_interrupt		( const struct mp_io_interrupt_assignment 		*last );
const struct mp_local_interrupt_assignment* 	mp_find_next_local_interrupt	( const struct mp_local_interrupt_assignment 	*last );

#endif /* MP_H_ */

#endif



