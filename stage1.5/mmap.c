
#include "16bitreal.h"

#include "mmap.h"


/***********************************************************
 * read memory map from BIOS ( use int 15h EAX=e820h )
 * takes: nothing
 * writes: array to _heap_start
 * returns: end of array address
 ***********************************************************/
short *_bios_15h_e820h();

__asm__("_bios_15h_e820h:\n"

  "push %edi\n"
  "push %ebx\n"
  "push %ecx\n"

  "xorl %ebx,         %ebx\n"     // clear ebx
  "xorl %edi,         %edi\n"     // clear ebx
  "movw _heap_start,  %di\n"

".rmm_next:\n"
  "movl $0x00000001, 20(%di)\n"   // insitialise bytes 20..24 (incase the bios doesnt)
  "movl $0x534D4150,  %edx\n"
  "movl $0x0000e820,  %eax\n"
  "movl $0x00000018,  %ecx\n"
  "push %di\n"
  "int  $0x00000015\n"            // call bios
  "pop %di\n"

  "jc   .rmm_exit\n"              // carry flag set? error
  "cmpl $0x534d4150,  %eax\n"     // eax not magic? error
  "jne  .rmm_exit\n"
  "addw $0x0018, %di\n"           // inc array address

  "cmpl $0, %ebx\n"               // should be non-zero. may be reset to zero after reading last entry ?
  "je  .rmm_exit\n"
  "jmp .rmm_next\n"               // next next region
".rmm_exit:\n"
  "movl $0, %eax\n"               // return end of struct
  "movw %di, %ax\n"

  "pop %ecx\n"
  "pop %ebx\n"
  "pop %edi\n"

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

 void read_mmap() {

   int end_of_map;

   end_of_map = (int)_bios_15h_e820h();

   mem.size = ((short)end_of_map - (short)_heap_start) / sizeof(struct mmap_e820h_reg);

   if(mem.size) {
     int i;
     mem.map = (struct mmap_e820h_reg *)_heap_start;
     _heap_start = (short*)end_of_map;

     puts("memory map:\r\n");
     for(i=0;i<mem.size;i++) {
       if(mem.map[i].type > 5)
	     mem.map[i].type = 0;

       printf(" %d) base {0x%x%x} len {0x%x%x} type %s\r\n",
	      i,
	      mem.map[i].base.msi,
	      mem.map[i].base.lsi,
	      mem.map[i].length.msi,
	      mem.map[i].length.lsi,
	      types[mem.map[i].type]);
     }
   }
   else {
     halt("failed to read memory map!\r\n");
   }
 }


