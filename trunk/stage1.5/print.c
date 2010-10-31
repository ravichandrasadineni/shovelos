
#include "16bitreal.h"

#include "print.h"
#include "inttypes.h"

static const char num[] = "0123456789ABCDEF";

static char screen_x=0;
static char screen_y=0;


/***********************************************************************
    void putc_vmem(char c, char x, char y);
      write a character the the screen WITHOUT using bios.
***********************************************************************/
void putc_vmem(int c, int x, int y);
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
void scroll();
__asm__("scroll:\n"
            "push %es\n"
            "push %ds\n"
            "push %edi\n"
            "push %esi\n"
            "push %ecx\n"
            "cld\n"

            "movw $0xb800, %ax\n"
            "movw %ax, %es\n"                 /* set extra segment to start of video memory */
            "movw %ax, %ds\n"                 /* set data segment to start of video memory */
            "movl $0x000000a0, %esi\n"        /* src = start of line 1 */
            "movl $0x00000000, %edi\n"        /* dst = start of line 0 */
            "movl $0x00000780, %ecx\n"
            "rep  \n"                         /* 24 lines, times 80 columns */
            "movsw\n"                         /* scroll! */
            "movw $0x0f20, %ax\n"             /* white on black space */
            "movl $0x00003160, %edi\n"        /* dst = start of line 79 */
            "movl $0x00000050, %ecx\n"
            "rep  \n"                         /* 80 columns */
            "stosw\n"                         /* blank last line */

            "pop %ecx\n"
            "pop %esi\n"
            "pop %edi\n"
            "pop %ds\n"
            "pop %es\n"
            "ret");

/***********************************************************************
    void cls()
      clear screen
***********************************************************************/
void cls();
__asm__(".global cls\n"
            "cls:\n"
            "push %es\n"
            "push %edi\n"
            "push %esi\n"
            "push %ecx\n"
            "cld\n"

            "movb $0, screen_x\n"            /* reset screen cords */
            "movb $0, screen_y\n"

            "movw $0xb800, %ax\n"
            "movw %ax, %es\n"                 /* set extra segment to start of video memory */
            "movl $0x00000000, %edi\n"        /* dst = start of line 0 */
            "movw $0x0f20, %ax\n"             /* white on black space */
            "movl $0x000007D0, %ecx\n"
            "rep  \n"                         /* 25 lines, times 80 columns */
            "stosw\n"                         /* cls! */

            "pop %ecx\n"
            "pop %esi\n"
            "pop %edi\n"
            "pop %es\n"
            "ret");

/***********************************************************************
    putc
      write a character to the the screen
***********************************************************************/
void putc(char c) {

  switch(c) {
    case '\n':
      screen_x = 0;
      if(screen_y>=24)
        scroll();
      else
        ++screen_y;
    case '\r':
      break;
    default:
      putc_vmem(c,screen_x,screen_y);
      ++screen_x;
      if(screen_x>=80) {
        screen_x = 0;
	if(screen_y>=24)
	  scroll();
	else
	  ++screen_y;
    }
  }
}

/***********************************************************************
    puts
      write a string to the the screen
***********************************************************************/
int puts(const char *s) {

    short i=0;
    char c;
    while((c = *s)) {
        ++i;
	    putc(c);
	    ++s;
    }
    return i;
}

/***********************************************************************
    putnhex
      write a number to the screen, in hex
***********************************************************************/
int putnhex(uint64_t n, int longmode) {
#if(DEBUG)
    short s;         // shift
    for(s=longmode ? 60 : 28; s>=0; s-=4)
        putc(num[(n>>s)&15]);
#endif
    return 8;
}

/***********************************************************************
    putndecu
      write an unsigned number to the screen, in dec
***********************************************************************/
int putndecu(uint64_t n) {
#if(DEBUG)
	uint64_t   s=1000000000;
	s          *=1000000000; // s = 100000000000000000 generates compiler warning.
    short h;         	  // digit
    short l=0;            // wrote length
    for(; s>=1; s/=10)
        if((h = ((n/s)%10)) || l || (s<=1)) {
            ++l;
            putc(num[h]);
        }
    return l;
#endif
    return 0;
}

/***********************************************************************
    putndec
      write an unsigned number to the screen, in dec
***********************************************************************/
int putndec(sint64_t n) {

    if(n>=0)
        return putndecu((uint64_t)n);

    putc('-');

    return putndecu((uint64_t)(-1 * n));
}


/***********************************************************************
    printf ( minimal, only supports %s,x,d,u - no floats, no padding )
      write a formatted string to the screen
***********************************************************************/
int printf(const char * format, ... /*int *_args*/) {

    char c;                // current char
    int special=0;        // special flag ( '%' )
    int longflag=0;       // special flag ( 'l' )
    int  **args = (int**)(&format+1);  // ptr to args array
    uint64_t longargs;
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
              case 'l':
              case 'L':
            	  special=longflag = 1;
            	  break;
              case 'd':
              case 'D':
              {
                _64_DWORD_LO(longargs) = (int)(*(args++));
                _64_DWORD_HI(longargs) = longflag ? (int)(*(args++)) : 0;

            	putndec((sint64_t)longargs);
                break;
              }
              case 'u':
              case 'U':
              {
            	_64_DWORD_LO(longargs) = (int)(*(args++));
            	_64_DWORD_HI(longargs) = longflag ? (int)(*(args++)) : 0;

                putndecu((uint64_t)longargs);
                break;
              }
              case 'x':
              case 'X':
              {
            	_64_DWORD_LO(longargs) = (int)(*(args++));
            	_64_DWORD_HI(longargs) = longflag ? (int)(*(args++)) : 0;
                putnhex((sint64_t)longargs,longflag);
                break;
              }
            }
        }
        else if(c == '%') {
          longflag = 0;
          special  = 1;
        }
        else
          putc(c);
    }
    return l;
}

