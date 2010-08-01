
#include "16bitreal.h"

#include "mmap.h"

/***********************************************************
 * read memory map from BIOS ( use int 15h EAX=e820h )
 * takes: nothing
 * writes: array to _heap_start
 * returns: end of array address
 ***********************************************************/
__asm__("_bios_15h_e820h:\n"

  "pusha\n"

  "movw $_heap_start, %di\n"
  "xorl %ebx,         %ebx\n"     // clear ebx
".rmm_next:\n"
  "movl $0x00000001, 20(%di)\n"   // insitialise bytes 20..24 (incase the bios doesnt)
  "movl $0x534D4150,  %edx\n"     
  "movl $0x0000e820,  %eax\n"
  "movl $0x00000018,  %ecx\n"
  "int  $0x00000015\n"            // call bios
  "jc   .rmm_exit\n"              // carry flag set? error
  "cmpl $0x534d4150,  %eax\n"     // eax not magic? error
  "jne  .rmm_exit\n"
  "addw $0x0018, %di\n"           // inc array address
  "cmpl $0, %ebx\n"               // should be non-zero. may be reset to zero after reading last entry ?
  "je  .rmm_exit\n"
  "jmp .rmm_next\n"               // next next region
".rmm_exit:\n"
  "movl $0, %eax\n"               // return end of struct
  "movw %si, %ax\n"
  "popa\n"
  "ret\n");

  
 struct mmap_e820h read_mmap() {
    
   struct mmap_e820h mem = {0,0};
   short *new_heap_start = (short*)_bios_15h_e820h();
     
   mem.size = (new_heap_start - _heap_start) / sizeof(struct mmap_e820h_reg);
     
   if(mem.size) {
     int i;
     mem.map = (struct mmap_e820h_reg *)_heap_start;
     _heap_start = new_heap_start;
     
     puts("MAP:\r\n");
     for(i=0;i<mem.size;i++)
       printf("  base {%x,%x} len {%x,%x} type %d\r\n",
	      mem.map[i].base.msi,
	      mem.map[i].base.lsi,
	      mem.map[i].length.msi,
	      mem.map[i].length.lsi,
	      mem.map[i].type);
   }
   else
     puts("failed to read memory map!\r\n");
   
   return mem;
 }
 
 