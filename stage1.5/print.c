
#include "16bitreal.h"

#include "print.h"

static const char num[] = "0123456789ABCDEF";

/***********************************************************************
    void putc(char c);
      write a character the the screen using bios.
      clobbers ax
***********************************************************************/
__asm__("putc:\n"\
            "pushw %bp\n"
            "movw %sp, %bp\n"
            "movb 6(%bp), %al\n"      /* parameter (char) from stack  */
            "movb $0x0E, %ah\n"       /* function 0x0e ( print char ) */
            "int  $0x10\n"            /* call bios */
            "pop  %bp\n"
                "ret");

/***********************************************************************
    puts
      write a string to the the screen using bios.
***********************************************************************/
int puts(const char *s) {

    short i=0;
    while(*s) {
        ++i;
        putc(*s++);
    }
    return i;
}

/***********************************************************************
    putnhex
      write a number to the screen, in hex
***********************************************************************/
int putnhex(unsigned int n) {
    short s;         // shift
//    short h;         // hex nibble
    short l=0;       // wrote length
    for(s=28; s>=0; s-=4) {
//      if((h = ((n>>s)&15)) || l || !s) {
        
        ++l;
        putc(num[(n>>s)&15]);
    }
    return l;
}

/***********************************************************************
    putndecu
      write an unsigned number to the screen, in dec
***********************************************************************/
int putndecu(unsigned int n) {
    int   s;         // divisor
    short h;         // digit
    short l=0;       // wrote length
    for(s=1000000000; s>=1; s/=10)
        if((h = ((n/s)%10)) || l || (s<=1)) {
            ++l;
            putc(num[h]);
        }
    return l;
}

/***********************************************************************
    putndec
      write an unsigned number to the screen, in dec
***********************************************************************/
int putndec(int n) {

    if(n>=0)
        return putndecu((unsigned int)n);

    putc('-');

    return putndecu((unsigned int)(-1 * n));
}


/***********************************************************************
    printf ( minimal, only supports %s,x,d,u - no floats, no padding )
      write a formatted string to the screen
***********************************************************************/
int printf(const char * format, int *_args) {

    char c;                // current char
    char special=0;        // special flag ( '%' )
    int  **args = &_args;  // ptr to args array
    short l=0;
    
    if(!format)
         return 0;

    while((c = *format++)) {
        ++l;
        if(special) {
            special=0;
            switch(c) {
              default:
                putc('%');
                putc(c);
                break;
              case 's':
              case 'S':
                puts((const char*)*args++);
                break;
              case 'd':
              case 'D':
                putndec((int)*args++);
                break;
              case 'u':
              case 'U':
                putndecu((unsigned int)*args++);
                break;
              case 'x':
              case 'X':
                putnhex((unsigned int)*args++);
                break;
            }
        }
        else if(c == '%')
          special = 1;
        else
          putc(c);
    }
    return l;
}

