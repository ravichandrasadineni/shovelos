

#ifndef __GDT_H
#define __GDT_H

#include "inttypes.h"

/*** global descriptor ( code segment ) ***/
struct GDT_CS {

	uint8_t _ignored1[5];    /* 40 bits */
	unsigned _ignored2 : 2;  /*  2 bits */
	unsigned C         : 1;  /*  1 bits */
	unsigned type      : 2;  /*  2 bits */
	unsigned DPL       : 2;  /*  2 bits */
	unsigned P         : 1;  /*  1 bits */
	unsigned _ignored3 : 5;  /*  5 bits */
	unsigned L         : 1;  /*  1 bits */
	unsigned D         : 1;  /*  1 bits */
	unsigned _ignored4 : 9;  /*  9 bits */

} __attribute__((packed));

/*** global descriptor ( data segment ) ***/
struct GDT_DS {

	uint8_t _ignored1[5];    /* 40 bits */

	unsigned _ignored2 : 3;  /*  3 bits */
	unsigned type      : 2;  /*  2 bits */
	unsigned _ignored3 : 2;  /*  2 bits */

	unsigned P         : 1;  /*  1 bits */
	unsigned _ignored4 : 16; /* 16 bits */

} __attribute__((packed));


struct GDT_REG {

	uint16_t size;
	uint64_t base;

} __attribute__((packed));

void setup_gdt();
void load_gdt();

#endif /*** __GDT_H ***/

