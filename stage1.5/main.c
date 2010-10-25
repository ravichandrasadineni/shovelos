
#include "16bitreal.h"

#include "mmap.h"
#include "inttypes.h"
#include "pt.h"
#include "gdt.h"
#include "print.h"
#include "mode.h"
#include "alloc.h"

int enable_a20_line();

void __attribute__((noreturn))
  halt(char *msg) {

	puts(msg);
	for(;;);
}

extern int _heap_start;

void __attribute__((noreturn))
  cmain() {

  _heap_start = (&_heap_start)+1;

  cls();

  alloc(0); //delete me

  puts("ShovelOS Stage 1.5\n");

  alloc(0); //delete me

  enable_a20_line();

  alloc(0); //delete me

  read_mmap();

  setup_pt();

  setup_gdt();

  printf("FreeMem 0x%x\n", 0xffff - (int)alloc(0));

  //enter_long_mode();

  halt("HALTING...");
}


