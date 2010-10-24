
#include "16bitreal.h"
#include "inttypes.h"
#include "alloc.h"
#include "mem.h"
#include "print.h"
#include "gdt.h"

struct GDT_REG *gdt_reg = 0;

void setup_gdt() {

	uint64_t *gdt_base = 0;
	struct GDT_CS *cs;
	struct GDT_DS *ds;

	if(gdt_reg != 0)
		return;

	gdt_reg = (struct GDT_REG*)zalloc(sizeof(struct GDT_REG));

	gdt_reg->size = 8 * 3; /* space for null, code and data descriptors */
	gdt_base      = (uint64_t*)zalloc(8 * 3);
	gdt_reg->base = (int)gdt_base;

	cs = (struct GDT_CS*)gdt_base+1;
	ds = (struct GDT_DS*)gdt_base+2;

	/*** CODE DESCRIPTOR ***/
	cs->type = 3; /*** code segment ***/
	cs->C    = 0; /*** non conforming ***/
	cs->DPL  = 0; /*** descriptor privilege level ***/
	cs->P    = 1; /*** present ***/
	cs->L    = 1; /*** long mode ***/
	cs->D    = 0; /*** default size ( reserved in long mode ) ***/

	/*** DATA DESCRIPTOR ***/
	ds->type = 2; /*** data segment ***/
	ds->P    = 1; /*** present ***/
}

