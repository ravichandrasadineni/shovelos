
#include "16bitreal.h"

#include "mmap.h"
#include "inttypes.h"

int enable_a20_line();

void __attribute__((noreturn))
  halt(char *msg) {

	puts(msg);
	for(;;);
}



void _test_math64(uint64_t a, uint64_t b) {

	uint64_t c;
	uint32_t x,y;

	c = a/b;
	x = (c>>32) & 0xffffffff;
	y = (c>> 0) & 0xffffffff;

	printf("div 0x%x%x\n", x,y);

	c = a%b;
	x = (c>>32) & 0xffffffff;
	y = (c>> 0) & 0xffffffff;

	printf("mod 0x%x%x\n", x,y);
}

void test_math64() {

	_test_math64(0xffffffffffffffff, 0x0000000000000001);
}

void __attribute__((noreturn))
  cmain() {

  cls();

  puts("ShovelOS Stage 1.5\n");

  _heap_start = (int)(&_heap_start+1);

  read_mmap();

  enable_a20_line();

  test_math64();

  halt("HALTING...");
}


