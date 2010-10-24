

#ifndef __PAGE_TRANSLATION_H
#define __PAGE_TRANSLATION_H

#include "inttypes.h"

#define PAGE_SIZE 		0x200000 					/* 2meg pages */
#define PAGE_TABLE_SIZE 512                         /* enteries per table */
#define PAGE_TABLE_ALIGNLENT 0x1000

/* mask out bits under page size */
#define ALIGN_DOWN(x)	(x & ~(PAGE_TABLE_ALIGNLENT-1))

/* next page aligned address */
#define ALIGN_UP(x)     ((x & (PAGE_TABLE_ALIGNLENT-1)) ? ALIGN_DOWN(x+PAGE_TABLE_ALIGNLENT) : x)

/*** Page map - level 4 offset table ***/
struct PML4E {

	union {

		uint64_t PageDirectoryPtr52;

		struct {
			unsigned P 				: 1;	/*** PRESENT BIT ***/
			unsigned RW 			: 1;	/*** READ/WRITE BIT ***/
			unsigned US 			: 1;	/*** USER/SUPERVISOR Bit ***/
			unsigned PWT 			: 1;	/*** PAGELEVEL WRITETHROUGH BIT***/
			unsigned PCD 			: 1;	/*** PAGELEVEL CACHE DISABLE BIT ***/
			unsigned A 				: 1;	/*** ACCESSED BIT ***/
			unsigned IGN 			: 1;	/*** ??? ***/
			unsigned MBZ 			: 2;	/*** ??? ***/
			unsigned AVL 			: 3;	/*** AVAILABLE TO SOFTWARE ***/
			char     _padding[5];   		/*** 40 BYTES PADDING ***/
			unsigned Available		: 11;	/*** ??? ***/
			unsigned NX 			: 1;	/*** NO EXECUTE BIT ***/
		} attr;
	} bits ;
} __attribute__((packed)) ;

/*** Page directory pointer ***/
struct PDPE  {

	union {
		uint64_t PageDirectory52;

		struct {
			unsigned P 					: 1;	/*** PRESENT BIT ***/
			unsigned RW 				: 1;	/*** READ/WRITE BIT ***/
			unsigned US 				: 1;	/*** USER/SUPERVISOR Bit ***/
			unsigned PWT 				: 1;	/*** PAGELEVEL WRITETHROUGH BIT***/
			unsigned PCD 				: 1;	/*** PAGELEVEL CACHE DISABLE BIT ***/
			unsigned A 					: 1;	/*** ACCESSED BIT ***/
			unsigned IGN 				: 1;	/*** ??? ***/
			unsigned PS					: 1;	/*** PAGE SIZE BIT ***/
			unsigned MBZ 				: 1;	/*** ??? ***/
			unsigned AVL 				: 3;	/*** AVAILABLE TO SOFTWARE ***/
			char     _padding[5];   			/*** 40 BYTES PADDING ***/
			unsigned Available			: 11;	/*** ??? ***/
			unsigned NX 				: 1;	/*** ??? ***/
		} attr;
	} bits ;
}__attribute__((packed));

/*** Page directory ***/
struct PDE {

	union {
		uint64_t PhysicalPage52;

		struct {
			unsigned P 					: 1;	/*** PRESENT BIT ***/
			unsigned RW 				: 1;	/*** READ/WRITE BIT ***/
			unsigned US 				: 1;	/*** USER/SUPERVISOR Bit ***/
			unsigned PWT 				: 1;	/*** PAGELEVEL WRITETHROUGH BIT***/
			unsigned PCD 				: 1;	/*** PAGELEVEL CACHE DISABLE BIT ***/
			unsigned A 					: 1;	/*** ACCESSED BIT ***/
			unsigned D					: 1;	/*** DIRTY BIT ***/
			unsigned PS					: 1;	/*** PAGE SIZE BIT ***/
			unsigned G					: 1;	/*** GLOBAL PAGE BIT ***/
			unsigned AVL 				: 3;	/*** AVAILABLE TO SOFTWARE ***/
			unsigned PAT 				: 1;	/*** PAGE ATTRIBUTE BIT ***/
			unsigned RESERVED_MBZ		: 8;	/*** RESERVED BITS ***/
			char     _padding[5];   			/*** 40 BYTES PADDING ***/
			unsigned Available			: 11;	/*** ??? ***/
			unsigned NX 				: 1; 	/*** NO EXECUTE BIT ***/
		} attr;
	} bits;
}__attribute__((packed));

static inline __attribute__ ((__always_inline__)) struct PDPE* pt_get_pdpe(struct PML4E *pmle4) {

	return (struct PDPE*)(int)ALIGN_DOWN( pmle4->bits.PageDirectoryPtr52 );
}

static inline __attribute__ ((__always_inline__)) struct PDE* pt_get_pde(struct PDPE *pdpe) {

	return (struct PDE*)(int)ALIGN_DOWN( pdpe->bits.PageDirectory52 );
}

void setup_pt();

#endif /*** __PAGE_TRANSLATION_H ***/

