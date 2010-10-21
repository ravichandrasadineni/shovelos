

#ifndef __PAGE_TRANSLATION_H
#define __PAGE_TRANSLATION_H

#define PAGE_SIZE 		0x200000 					/* 2meg pages */
#define PAGE_TABLE_SIZE 512                         /* enteries per table */

/* mask out bits under page size */
#define ALIGN_DOWN(x)	(x & ~(PAGE_SIZE-1))

/* next page aligned address */
#define ALIGN_UP(x)     ((x & (PAGE_SIZE-1)) ? ALIGN_DOWN(x+PAGE_SIZE) : x)

/*** Page map - level 4 offset table ***/
struct PML4E __attribute__ ((packed)) {

	union {
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
		} attr;
		unsigned PageDirectoryPtr	: 52;	/*** Page Directory Pointer ***/
	};
	unsigned Available			: 11;	/*** ??? ***/
	unsigned NX 				: 1;	/*** NO EXECUTE BIT ***/
};

/*** Page directory pointer ***/
struct PDPE __attribute__ ((packed)) {

	union {
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
		} attr;
		unsigned PageDirectory			: 52;	/*** Page Directory ***/
	};
	unsigned Available			: 11;	/*** ??? ***/
	unsigned NX 				: 1;	/*** ??? ***/
};

/*** Page directory ***/
struct PDE __attribute__ ((packed)) {

	union {
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
		};
		unsigned PhysicalPage		: 52;	/*** Physical Page ***/
	};

	unsigned Available			: 11;	/*** ??? ***/
	unsigned NX 				: 1; 	/*** NO EXECUTE BIT ***/
};

static inline __attribute__ ((__always_inline__)) struct PDPE* pt_get_pdpe(struct PML4E *pmle4) {

	return (struct PDPE*)ALIGN_DOWN( pmle4->PageDirectoryPtr );
}

static inline __attribute__ ((__always_inline__)) struct PDE* pt_get_pde(struct PDPE *pdpe) {

	return (struct PDE*)ALIGN_DOWN( pdpe->PageDirectory );
}

#endif /*** __PAGE_TRANSLATION_H ***/

