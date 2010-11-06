
#include "16bitreal.h"
#include "inttypes.h"
#include "pt.h"
#include "print.h"
#include "alloc.h"
#include "fs/ext2/ext2.h"

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

  int isext2 = fs_is_ext2();
  if(isext2 < 0)
	  puts("DISK ERROR\n");
  else if(isext2 > 0)
	  puts("NOT EXT2\n");
  else
	  puts("EXT2 !!!\n");

//  setup_pt();

//  himem();

#if defined(DEBUG)
  printf("FreeMem 0x%x\n", 0xffff - (int)alloc(0));
#endif

  halt("HALTING...");
}


