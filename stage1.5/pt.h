

#ifndef __PAGE_TRANSLATION_H
#define __PAGE_TRANSLATION_H

#define PAGE_SIZE 0x200000 /* 2meg pages */

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

#endif /*** __PAGE_TRANSLATION_H ***/

