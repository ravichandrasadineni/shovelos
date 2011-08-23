/*
 * acpi.h
 *
 *  Created on: Jan 23, 2011
 *      Author: cds
 */

#ifndef ACPI_H_
#define ACPI_H_

void* acpi_find_first_table(const char * header);
void* acpi_find_next_table(void *last, const char * header);

#endif /* ACPI_H_ */

