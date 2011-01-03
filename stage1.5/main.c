
#include "16bitreal.h"
#include "inttypes.h"
#include "pt.h"
#include "print.h"
#include "alloc.h"
#include "fs/ext2/ext2.h"
#include "bios_disk.h"
#include "mem.h"

extern void* _pml4e;

void himem();

void __attribute__((noreturn))
  cmain() {

  cls();

  puts("ShovelOS Stage 1.5\n");

  setup_pt();

  fs_init();

#if defined(DEBUG)
  printf("FreeMem 0x%x\n", 0xffff - (int)alloc(0));
#endif

  /*** soooooo ugly! ***/
  {
	  uint64_t shuffle_params[4];
	  memset(shuffle_params,0,sizeof shuffle_params);
	  shuffle_params[3] = 1;
	  memcpy(ADHOC_COMM, shuffle_params, sizeof shuffle_params);
	  shuffle_high();
  }

  halt("HALTING...");
}


