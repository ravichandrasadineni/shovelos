
#include "16bitreal.h"
#include "inttypes.h"
#include "pt.h"
#include "print.h"
#include "alloc.h"
#include "fs/ext2/ext2.h"

extern void* _pml4e;

void __attribute__((noreturn))
  cmain() {

  cls();

  puts("ShovelOS Stage 1.5\n");

  /*** hack alert -  we should clear ALL ram... for now, just hard coded page table address ***/
  memset(0x10000,0,0x5000);

  fs_init();

  setup_pt();

  himem();

#if defined(DEBUG)
  printf("FreeMem 0x%x\n", 0xffff - (int)alloc(0));
#endif

  halt("HALTING...");
}


