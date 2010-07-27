
#include "16bitreal.h"
#include "print.h"

static const char num[] = "0123456789ABCDEF";

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
short puts(const char *s) {

    short i=0;
    while(*s++) {
        ++i;
        putc(*s);
    }
    return i;
}

/***********************************************************************
    putnhex
      write a number to the screen, in hex
***********************************************************************/
short putnhex(int n) {
    short s;         // shift
    short h;         // hex nibble
    short l=0;       // wrote length
    for(s=28; s>=0; s-=4)
        if((h = ((n>>s)&15)) || l || !s) {
            ++l;
            putc(num[h]);
        }
    return l;
}



