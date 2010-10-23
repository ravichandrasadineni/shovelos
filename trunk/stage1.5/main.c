
#include "16bitreal.h"

#include "mmap.h"
#include "inttypes.h"
#include "pt.h"
#include "gdt.h"
#include "print.h"
#include "mode.h"

int enable_a20_line();

void __attribute__((noreturn))
  halt(char *msg) {

	puts(msg);
	for(;;);
}

void __attribute__((noreturn))
  cmain() {

  cls();

  alloc(0); //delete me

  puts("ShovelOS Stage 1.5\n");

  alloc(0); //delete me

  enable_a20_line();

  alloc(0); //delete me

  read_mmap();

  setup_pt();

  setup_gdt();

  puts("trying to enter long mode!!!\n");

  enter_long_mode();

  halt("HALTING...");
}


