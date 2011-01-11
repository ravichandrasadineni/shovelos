/*
 * layout.c
 *
 *  Created on: 11 Jan 2011
 *      Author: Chris Stones
 */

struct virt_region {

	const uint64_t start;
	      uint64_t end;
	const uint64_t limit;
	const uint64_t grows_up;
};

/***
 * 0x0500 -> 0x07bff ( free, and not used by our bootloader )
 * 0x7c00 -> 0x07dff ( free, but too small )
 * 0x7e00 -> 0x7ffff ( free, but used by our boot loader )
 */

/*** note: 0x7e00 -> 0x7ffff is free, and identity mapped !***/
static struct virt_region phy_region = {

	0xffff800000000000, /* start              */
	0xffff800000000000, /* current end        */
	0xffff801000000000, /* 16 gigabytes limit */
	0x1                 /* grows up           */
};

static virt_region heap_region = {

	0xffff802000000000, /* start              */
	0xffff802000000000, /* current end        */
	0xffff803000000000, /* 16 gigabytes limit */
	0x01                /* grows up           */
};

static virt_region stack_region = {

	0xffff805000000000, /* start              */
	0xffff805000000000, /* current end        */
	0xffff804000000000, /* 16 gigabytes limit */
	0x00                /* grows down         */
};



