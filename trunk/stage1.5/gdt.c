
#include "16bitreal.h"
#include "inttypes.h"
#include "alloc.h"
#include "mem.h"
#include "print.h"
#include "gdt.h"

struct GDT_REG *gdt_reg = 0;

void setup_gdt() {

	uint64_t *gdt_base = 0;
	struct GDT_CS *cs_long;
	struct GDT_CS *cs_compat;
	struct GDT_DS *ds;

	if(gdt_reg != 0)
		return;

	gdt_reg = (struct GDT_REG*)zalloc(sizeof(struct GDT_REG));

	gdt_reg->size = 8 * 4; /* space for null, code_long, code_compat and data descriptors */
	gdt_base      = (uint64_t*)zalloc(8 * 4);
	gdt_reg->base = (int)gdt_base;

	cs_long   = (struct GDT_CS*)gdt_base+1;
	cs_compat = (struct GDT_CS*)gdt_base+2;
	ds        = (struct GDT_DS*)gdt_base+3;

	/*** CODE DESCRIPTOR (long mode) ***/
	cs_long->type 			= 3; 			/*** code segment ***/
	cs_long->R   			= 1; 			/*** readable ***/
	cs_long->C   			= 0; 			/*** non conforming ***/
	cs_long->DPL  			= 0; 			/*** descriptor privilege level ***/
	cs_long->P   			= 1; 			/*** present ***/
	cs_long->L    			= 1; 			/*** long mode ***/
	cs_long->D    			= 0; 			/*** default size ( reserved in long mode ) ***/
	cs_long->seg_lim15_0  	= 0xffff;
	cs_long->seg_lim19_16 	= 0xf;

	/*** CODE DESCRIPTOR (compatibility mode) ***/
	cs_compat->type 			= 3; 			/*** code segment ***/
	cs_compat->R   				= 1; 			/*** readable ***/
	cs_compat->C   				= 0; 			/*** non conforming ***/
	cs_compat->DPL  			= 0; 			/*** descriptor privilege level ***/
	cs_compat->P   				= 1; 			/*** present ***/
	cs_compat->L    			= 0; 			/*** long mode ***/
	cs_compat->D    			= 0; 			/*** default size ( reserved in long mode ) ***/
	cs_compat->seg_lim15_0  	= 0xffff;
	cs_compat->seg_lim19_16 	= 0xf;

	/*** DATA DESCRIPTOR ***/
	ds->type = 2; /*** data segment ***/
	ds->P    = 1; /*** present ***/
	ds->W    = 1; /*** writable ***/
	ds->seg_lim15_0  	= 0xffff;
	ds->seg_lim19_16 	= 0xf;
}

