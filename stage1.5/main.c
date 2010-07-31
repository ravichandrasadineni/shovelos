
#include "16bitreal.h"

extern short* _heap_start;

void __attribute__((noreturn))
  cmain() {

  puts("######################\r\n");
  puts("# ShovelOS Stage 1.5 #\r\n");
  puts("######################\r\n");
 
  
  _16_read_memory_map();
 
  
  puts("HALTING...");
  while(1);
}


