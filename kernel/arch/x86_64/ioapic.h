/*
 * ioapic.h
 *
 *  Created on: Jan 29, 2011
 *      Author: cds
 */

#ifndef IOAPIC_H_
#define IOAPIC_H_

#include <inttypes.h>

uint16_t ioapic_detect();

void ioapic_configure();

#endif /* IOAPIC_H_ */
