
#include "16bitreal.h"

#include "mmap.h"


/***********************************************************
 * read memory map from BIOS ( use int 15h EAX=e820h )
 * takes: nothing
 * writes: array to bottom of heap.
 ***********************************************************/
void _bios_15h_e820h();

__asm__("_bios_15h_e820h:\n"

  "push %edi\n"
  "push %ebx\n"
  "push %ecx\n"
  "push $0x0018\n"				  // alloc parameter

  "xorl %ebx,         %ebx\n"     // clear ebx

".rmm_next:\n"

  "call alloc \n"
  "movw %ax, %di \n"

  "movl $0x00000001, 20(%di)\n"   // insitialise bytes 20..24 (incase the bios doesnt)
  "movl $0x534D4150,  %edx\n"
  "movl $0x0000e820,  %eax\n"
  "movl $0x00000018,  %ecx\n"
  "int  $0x00000015\n"            // call bios

  "jc   .rmm_exit\n"              // carry flag set? error
  "cmpl $0x534d4150,  %eax\n"     // eax not magic? error
  "jne  .rmm_exit\n"

  "cmpl $0, %ebx\n"               // should be non-zero. may be reset to zero after reading last entry ?
  "je  .rmm_exit\n"
  "jmp .rmm_next\n"               // next next region
".rmm_exit:\n"

  "pop %ecx\n" // pop alloc parameter
  "pop %ecx\n" // pop ecx
  "pop %ebx\n" // pop ebx
  "pop %edi\n" // pop edi

  "ret\n");


 // global storage for memory map. ( contains heap ptr )
 struct mmap_e820h mem = {0,0};

 static const char * types[] ={
   "UNKNOWN",
   "USABLE",
   "RESERVED",
   "ACPI RECLAIMABLE",
   "ACPI NVS",
   "BAD",
 };

 struct mmap_e820h *read_mmap() {

   if(mem.map == 0) {

	   mem.map = (struct mmap_e820h_reg*)alloc(0);

	   _bios_15h_e820h();

	   mem.size = (alloc(0) - (int)mem.map) / sizeof(struct mmap_e820h_reg);

	   if(mem.size) {
		 int i=0;

		 puts("memory map:\n");

		 for(i=0;i<mem.size;i++) {
		   if(mem.map[i].type > 5)
			  mem.map[i].type = 0;

		   printf(" %d) base {0x%lx} len {0x%lx} type %s\n",
				  i,
				  mem.map[i].b.b64,
				  mem.map[i].l.l64,
				  types[mem.map[i].type]);
		 }
	   }
	   else {
		 halt("failed to read memory map!\n");
	   }
   }

   return &mem;
 }


