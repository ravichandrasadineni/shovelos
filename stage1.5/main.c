
#include "16bitreal.h"
#include "inttypes.h"
#include "print.h"
#include "alloc.h"
#include <fs/fs.h>
#include <disk/disk.h>
#include "mem.h"
#include "pt.h"

extern void* _pml4e;

void shuffle_high();

void __attribute__((noreturn))
  cmain() {

  cls();

  puts("ShovelOS Stage 1.5\n");

  fs_init();

  printf("FreeMem 0x%x\n", 0xffff - (int)alloc(0));

  /*** soooooo ugly! ***/
  {
	  poke64(ADHOC_COMM+0x00, 0);
	  poke64(ADHOC_COMM+0x08, 0);
	  poke64(ADHOC_COMM+0x10, 0);
	  poke64(ADHOC_COMM+0x18, 1);
	  shuffle_high();
  }

  halt("HALTING...");
}


