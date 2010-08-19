
#include "16bitreal.h"

#include "mmap.h"

int check_a20_line();

void __attribute__((noreturn))
  cmain() {
    
  cls(); 
    
  puts("ShovelOS Stage 1.5\n");
   
  _heap_start = &_heap_start+1;
  
  read_mmap();
  
  printf("looks like a20 line is %s.\n", check_a20_line() ? "enabled" : "disabled");

  puts("HALTING...");
  while(1);
}


