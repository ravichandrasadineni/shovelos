
#include "16bitreal.h"
#include "print.h"

/***********************************************************************
    void putc(char c);
      write a character the the screen using bios.
      clobbers ax
***********************************************************************/
__asm__("putc:\n"\
            "push %bp\n"
            "movw %sp, %bp\n"
            "movb 4(%bp), %al\n"      /* parameter (char) from stack  */
            "movb $0x0E, %ah\n"       /* function 0x0e ( print char ) */
            "int  $0x10\n"            /* call bios */
            "pop  %bp\n"
                "ret");

/***********************************************************************
    puts
      write a string to the the screen using bios.
***********************************************************************/
void puts(const char *s) {

    while(*s++)
        putc(*s);
}



