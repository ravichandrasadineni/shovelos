/*
 * lapic.c
 *
 *  Created on: Apr 2, 2011
 *      Author: cds
 */

#include "lapic.h"
#include "msr.h"
#include "pt.h"

enum apic_base_msr {

	APIC_BSP_FLAG = (1<<8),
	APIC_GLOBAL_ENABLE_FLAG = (1<<11),
};

typedef struct _128bit_aligned_uint32 {

	union {
		uint32_t _register;
		uint64_t _128bits[2];
	};
};

struct local_apic_struct {

	const	_128bit_aligned_uint32		reserved_00[2];
	const	_128bit_aligned_uint32		id;
	const	_128bit_aligned_uint32		version;
	const	_128bit_aligned_uint32		reserved_01[4];
			_128bit_aligned_uint32		task_priority;
	const	_128bit_aligned_uint32		arbitration_priority;
	const	_128bit_aligned_uint32		processor_priority;
			_128bit_aligned_uint32		end_of_interrupt;
	const	_128bit_aligned_uint32		remote_read;
			_128bit_aligned_uint32		local_destination;
			_128bit_aligned_uint32		destination_format;
			_128bit_aligned_uint32		spurious_interrupt_vector;
	const	_128bit_aligned_uint32		in_service[8];
	const	_128bit_aligned_uint32		trigger_mode[8];
	const	_128bit_aligned_uint32		interrupt_request[8];
	const	_128bit_aligned_uint32		error_status;
	const	_128bit_aligned_uint32		reserved_02[6];
			_128bit_aligned_uint32		lvt_cmci;
			_128bit_aligned_uint32		interrupt_command[2];
			_128bit_aligned_uint32		lvt_timer;
			_128bit_aligned_uint32		lvt_thermal_sensor;
			_128bit_aligned_uint32		lvt_performance_monitoring_counters;
			_128bit_aligned_uint32		lvt_lint0;
			_128bit_aligned_uint32		lvt_lint1;
			_128bit_aligned_uint32		lvt_error;
			_128bit_aligned_uint32		initial_count;
	const	_128bit_aligned_uint32		current_count;
	const	_128bit_aligned_uint32		reserved_03[4];
			_128bit_aligned_uint32		divide_configuration;
	const	_128bit_aligned_uint32		reserved_04;
};

struct local_apic_struct *lapic = 0;

static void lapic_global_enable() {

	uint64_t msr = cpu_rdmsr64( IA32_APIC_BASE );

	msr |= APIC_GLOBAL_ENABLE_FLAG;

	cpu_wrmsr64(IA32_APIC_BASE, msr);
}

static void lapic_global_disable() {

	uint64_t msr = cpu_rdmsr64( IA32_APIC_BASE );

	msr &= ~APIC_GLOBAL_ENABLE_FLAG;

	cpu_wrmsr64(IA32_APIC_BASE, msr);
}

static uint64_t lapic_get_phy_address() {

	uint64_t msr = cpu_rdmsr64( IA32_APIC_BASE );

	return msr & 0xFFFFFF000;
}

static uint64_t lapic_set_phy_address(uint64_t phy) {

	if(phy & ~0xFFFFFF000)
		return 0; // cant relocate lapic to given address

	uint64_t msr = cpu_rdmsr64( IA32_APIC_BASE );

	msr &= ~0xFFFFFF000;
	msr |= phy;

	cpu_wrmsr64( IA32_APIC_BASE, msr );

	return lapic_get_phy_address();
}

static uint64_t* lapic_mmap() {

	uint64_t phy = lapic_get_phy_address();
	uint64_t off = phy & (PAGE_SIZE-1);

	uint64_t pages  = sizeof(struct local_apic_struct) / PAGE_SIZE;
					+(sizeof(struct local_apic_struct) % PAGE_SIZE) ? 1 : 0;

	uint64_t virt = vmm_alloc_hw(pages);

	for(uint64_t i=0; i<pages; i++)
		mmap((phy + i*PAGE_SIZE ) & ~(PAGE_SIZE-1), virt + PAGE_SIZE*i, 0);

	return (uint64_t*)(virt + (phy & (PAGE_SIZE-1)));
}

void lapic_configure() {

	if(!lapic)
		lapic = lapic_mmap();

	lapic_global_enable();

	krpintf("found lapic id:%d. version:%d\n", lapic->id._register, lapic->version._register);
}


