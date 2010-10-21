
#include "16bitreal.h"

#include "mmap.h"
#include "inttypes.h"
#include "pt.h"

int enable_a20_line();

void __attribute__((noreturn))
  halt(char *msg) {

	puts(msg);
	for(;;);
}

void __attribute__((noreturn))
  cmain() {

  cls();

  puts("ShovelOS Stage 1.5\n");

  enable_a20_line();

  read_mmap();

  setup_pt();

  halt("HALTING...");
}


