

#ifndef __GDT_H
#define __GDT_H

#include "inttypes.h"

/*** global descriptor ( code segment ) ***/
struct GDT_CS {

	unsigned short seg_lim15_0; 	  /* 16 bits */
	unsigned short seg_base15_0;	  /* 16 bits */
	unsigned char seg_base23_16; 	  /*  8 bits */

	unsigned _ignored2 : 1;  /*  1 bits */
	unsigned R         : 1;  /*  1 bits */
	unsigned C         : 1;  /*  1 bits */
	unsigned type      : 2;  /*  2 bits */
	unsigned DPL       : 2;  /*  2 bits */
	unsigned P         : 1;  /*  1 bits */
	unsigned seg_lim19_16  : 4;   /*  4 bits */
	unsigned _ignored3 : 1;  /*  1 bits */
	unsigned L         : 1;  /*  1 bits */
	unsigned D         : 1;  /*  1 bits */
	unsigned _ignored4 : 1;  /*  1 bits */
	unsigned char seg_base31_24; /* 8 bits */

} __attribute__((packed));

/*** global descriptor ( data segment ) ***/
struct GDT_DS {

	unsigned short seg_lim15_0;  	  /* 16 bits */
	unsigned short seg_base15_0; 	  /* 16 bits */
	unsigned char seg_base23_16; 	  /*  8 bits */

	unsigned _ignored2 : 1;  /*  1 bits */
	unsigned W : 1;  /*  1 bits */
	unsigned _ignored3 : 1;  /*  1 bits */
	unsigned type      : 2;  /*  2 bits */
	unsigned _ignored4 : 2;  /*  2 bits */

	unsigned P         : 1;  /*  1 bits */
	unsigned seg_lim19_16  : 4;   /*  4 bits */
	unsigned _ignored5 : 4;  /*  4 bits */

	unsigned char seg_base31_24; /* 8 bits */

} __attribute__((packed));


struct GDT_REG {

	uint16_t size;
	uint64_t base;

} __attribute__((packed));

void setup_gdt();

#endif /*** __GDT_H ***/

