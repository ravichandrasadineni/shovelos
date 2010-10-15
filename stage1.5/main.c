
#include "16bitreal.h"

#include "mmap.h"
#include "inttypes.h"

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

  read_mmap();

  enable_a20_line();

  // test_math64();

  halt("HALTING...");
}


