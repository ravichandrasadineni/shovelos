

#ifndef __PAGE_TRANSLATION_H
#define __PAGE_TRANSLATION_H

/*** Page map - level 4 offset table ***/
struct PML4E {

	unsigned P 					: 1;	/*** PRESENT BIT ***/
	unsigned RW 				: 1;	/*** READ/WRITE BIT ***/
	unsigned US 				: 1;	/*** USER/SUPERVISOR Bit ***/
	unsigned PWT 				: 1;	/*** PAGELEVEL WRITETHROUGH BIT***/
	unsigned PCD 				: 1;	/*** PAGELEVEL CACHE DISABLE BIT ***/
	unsigned A 					: 1;	/*** ACCESSED BIT ***/
	unsigned IGN 				: 1;	/*** ??? ***/
	unsigned MBZ 				: 2;	/*** ??? ***/
	unsigned AVL 				: 3;	/*** AVAILABLE TO SOFTWARE ***/
	unsigned PageDirectoryPtr	: 40;	/*** ??? ***/
	unsigned Available			: 11;	/*** ??? ***/
	unsigned NX 				: 1;	/*** NO EXECUTE BIT ***/
};

/*** Page directory pointer ***/
struct PDPE {

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
	unsigned PageDirectory		: 40;	/*** ??? ***/
	unsigned Available			: 11;	/*** ??? ***/
	unsigned NX 				: 1;	/*** ??? ***/
};

/*** Page directory ***/
struct PDE {

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
	unsigned PhysicalPage		: 31;	/*** ??? ***/
	unsigned Available			: 11;	/*** ??? ***/
	unsigned NX 				: 1; 	/*** NO EXECUTE BIT ***/
};

#endif /*** __PAGE_TRANSLATION_H ***/

