
#include "16bitreal.h"

#include "mmap.h"

void __attribute__((noreturn))
  cmain() {
    
  cls(); 
    
  puts("ShovelOS Stage 1.5\n");
   
  _heap_start = &_heap_start+1;
  
  read_mmap();
  
  puts("HALTING...");
  while(1);
}

