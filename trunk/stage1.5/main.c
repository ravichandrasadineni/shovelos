
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

  cls();

  puts("ShovelOS Stage 1.5\n");

  enable_a20_line();

  read_mmap();

  setup_pt();

  setup_gdt();

  himem();

#if defined(DEBUG)
  printf("FreeMem 0x%x\n", 0xffff - (int)alloc(0));
#endif

  halt("HALTING...");
}


