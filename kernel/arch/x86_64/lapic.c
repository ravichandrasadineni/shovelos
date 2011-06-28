/*
 * lapic.c
 *
 *  Created on: Apr 2, 2011
 *      Author: cds
 */

#include "lapic.h"
#include "msr.h"

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

