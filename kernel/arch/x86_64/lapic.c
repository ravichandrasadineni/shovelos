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
			_128bit_aligned_uint32		interrupt_command;

};

enum local_apic_register {




	LAPICIRR0 = 0x00000200, /* Read Only  - Interrupt Request Register (IRR); bits 31:0 */
	LAPICIRR1 = 0x00000210, /* Read Only  - Interrupt Request Register (IRR); bits 63:32 */
	LAPICIRR2 = 0x00000220, /* Read Only  - Interrupt Request Register (IRR); bits 95:64 */
	LAPICIRR3 = 0x00000230, /* Read Only  - Interrupt Request Register (IRR); bits 127:96 */
	LAPICIRR4 = 0x00000240, /* Read Only  - Interrupt Request Register (IRR); bits 159:128 */
	LAPICIRR5 = 0x00000250, /* Read Only  - Interrupt Request Register (IRR); bits 191:160 */
	LAPICIRR6 = 0x00000260, /* Read Only  - Interrupt Request Register (IRR); bits 223:192 */
	LAPICIRR7 = 0x00000270, /* Read Only  - Interrupt Request Register (IRR); bits 255:224 */

	LAPICESR  = 0x00000280, /* Read Only  - Error Status Register */

	LAPICLVT  = 0x000002F0, /* Read/Write - LVT CMCI Register */
	LAPICICR  = 0x00000300, /* Read/Write - Interrupt Command Register (ICR); bits 0-31 */

	/*** TODO - MORE ***/
};

