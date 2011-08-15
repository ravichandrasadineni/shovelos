/*
 * lapic.h
 *
 *  Created on: Apr 2, 2011
 *      Author: cds
 */

#ifndef __ARCH_X86_64_LAPIC_H
#define __ARCH_X86_64_LAPIC_H

#include <inttypes.h>

void lapic_configure();
void lapic_eoi(uint32_t vector);
void lapic_ipi_start(uint8_t lapic_id, void* address);

#endif /*** __ARCH_X86_64_LAPIC_H ***/

