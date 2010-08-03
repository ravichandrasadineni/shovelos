
#include "16bitreal.h"

#include "print.h"

static const char num[] = "0123456789ABCDEF";

/***********************************************************************
    void putc(char c);
      write a character the the screen using bios.
      clobbers ax
***********************************************************************/
__asm__("putc:\n"
            "pushw %bp\n"
            "movw %sp, %bp\n"
            "movb 6(%bp), %al\n"      /* parameter (char) from stack  */
            "movb $0x0E, %ah\n"       /* function 0x0e ( print char ) */
            "int  $0x10\n"            /* call bios */
            "pop  %bp\n"
                "ret");


/***********************************************************************
    void putc_vmem(char c, char x, char y);
      write a character the the screen WITHOUT using bios.
***********************************************************************/
__asm__("putc_vmem:\n"
        
        "push %ebp\n"
        "movl %esp, %ebp\n"               /* enter */
        
        "push %es\n"                      /* save non-volatiles */
        "push %edi\n"
        
        "movw $0xb800, %ax\n"
        "movw %ax, %es\n"                 /* set extra segment to start of video memory */
        
        "xor    %eax, %eax\n"             /* read y coord from stack */
        "addb 16(%ebp), %al\n"
        "imul  $0xa0,  %ax\n"
        "movw    %ax,  %di\n"
        
        "xorl  %eax, %eax\n"              /* read x coord from stack */
        "movb 12(%bp), %al\n"
        "imul   $2,   %ax\n"
        "addw  %ax,   %di\n"

        "xorw   %ax, %ax\n"
        "movb $0x0f, %ah\n"               /* white on black attribute */
        "movb 8(%bp),%al\n"               /* character from stack */
        
        "stosw\n"                         /* ax to es:di */

        "pop  %edi\n"                     /* restore */
        "pop   %es\n"                     /* restore */
        "pop  %ebp\n"                     /* leave */
        "ret");


/***********************************************************************
    void scroll()
      scroll the screen down one line (80x25)
***********************************************************************/
__asm__("scroll:\n"
            
            "movw $0xb800, %ax\n"
            "movw %ax, %es\n"                 /* set extra segment to start of video memory */
            "movw %ax, %ds\n"                 /* set extra segment to start of video memory */
            
".scroll_x:\n"
            "subw $2, %ecx\n"
            "subw $2, %edi\n"
            "movw (%ecx,%ebx,2), %ax\n"
            "stosw\n"
            "cmpw $0,%ecx\n"
            "jne .scroll_x\n"
            
            
            
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
    for(s=28; s>=0; s-=4)
        putc(num[(n>>s)&15]);
    return 8;
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

