/*
 * kprintf.c
 *
 *  Created on: Jan 6, 2011
 *      Author: cds
 */

#include "x86_64.h"
#include <stdarg.h>

#define ROWS 25
#define COLS 80

/* fixme */
static void memcpy(void* dst, const void* src, int sz) {

	uint8_t *d = (uint8_t*)dst;
	const uint8_t *s = (const uint8_t*)src;
	while(sz--)
		*d++ = *s++;
}

struct console_mem {
	uint16_t chars[ROWS][COLS];
};

struct console {

    struct console_mem *const mem;
    uint16_t xpos;
    uint16_t ypos;
    uint16_t colour;
};

struct console console = {
    (struct console_mem *const)0xb8000, /* PC video address   */
    0,                                  /* start at left      */
    ROWS-1,                             /* start at bottom    */
    0x0f00,                             /* white on black     */
};

static void scroll() {

	int r,c;

	for(r=0;r<ROWS-1;++r)
		memcpy(console.mem->chars[r],console.mem->chars[r+1], sizeof console.mem->chars[0]);

	for(c=0;c<COLS;++c)
		console.mem->chars[r][c] = console.colour;
}

int putc(char c) {

    switch(c) {
        case '\n':
        	console.xpos = 0;
            if(console.ypos>=ROWS-1)
                scroll();
            else
                ++console.ypos;

            return 1;

         case '\r':
             return 0;

         default:
             console.mem->chars[console.ypos][console.xpos] = console.colour | c;
             ++console.xpos;
             if(console.xpos>=COLS) {
            	 console.xpos = 0;
	             if(console.ypos>=ROWS-1)
	                 scroll();
	             else
	                 ++console.ypos;
             }
             return 1;
     }
}

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


int kprintf(const char * format, ... ) {

	char c;
	int  l=0;
	int special=0;
	char  **args = (char**)(&format+1);

    while((c = *format++)) {

        if(special) {
        	special = 0;

        	switch(c) {
        	case 's':
        	case 'S':
        	{
        		char *str = args*
        	    l += puts(str);
			    break;
        	}
        	default:
        		l += putc(c);
        		break;
        	}
        }
        else if(c=='%')
        	special = 1;
        else
        	l += putc(c);
    }
	return l;
}

