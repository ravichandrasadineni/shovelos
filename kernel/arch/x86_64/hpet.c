/*
 * hpet.c
 *
 *  Created on: 23 Aug 2011
 *      Author: Chris Stones
 */

#include<arch/arch.h>
#include<lib/lib.h>

struct  event_timer_block_id {

	unsigned hardware_rev_id_1st : 8;
	unsigned comparators_1st : 5;
	unsigned count_size_cap : 1;
	unsigned reserved0 : 1;
	unsigned legacy_replacement_irq_route_cap : 1;
	unsigned pci_vendor_id : 16;

}__attribute__((__packed__));

enum GAS_address_space_id {

	system_mem = 0,
	system_io  = 1,
};

enum page_protection {

	no_guarantee=0,
	page_protected_4k=1,
	page_protected_64k=2,

};

struct generic_address_struct {

	uint8_t 	address_space_id;
	uint8_t  	register_bit_width;
	uint8_t  	register_bit_offset;
	uint8_t  	reserved0;
	uint64_t 	base_address;

}__attribute__((__packed__));

struct hpet_descr_table {

	uint8_t 						signature[4];
	uint32_t 						length;
	uint8_t							revision;
	uint8_t							checksum;
	uint8_t							oem_id[6];
	uint8_t							oem_table_id[8];
	uint32_t 						oem_revision;
	uint32_t						creator_id;
	uint32_t						creator_revision;
	struct  event_timer_block_id 	event_timer_block_id;
	struct  generic_address_struct	base_address;
	uint8_t							hpet_number;
	uint16_t						min_periodic_tick;
	unsigned						page_protection : 4;
	unsigned						oem_attribute : 4;


} __attribute__((__packed__));




static uint8_t sum(const void * _data, uint64_t len) {

	uint8_t sum = 0;
	const uint8_t *data = (const uint8_t *)_data;

	while(len--)
		sum += *data++;

	return sum;
}

uint8_t hpet_init(void) {

	const struct hpet_descr_table* hpet_tab =
			(const struct hpet_descr_table*) acpi_find_first_table("HPET");

	if(!hpet_tab)
		return -1;

	if(sum(hpet_tab,hpet_tab->length) != 0)
		return -1;

	kprintf("hpet %d (%d comparators) @ 0x%lx\n",
			hpet_tab->hpet_number,
			hpet_tab->event_timer_block_id.comparators_1st,
			hpet_tab->base_address.base_address);

	return 0;
}

