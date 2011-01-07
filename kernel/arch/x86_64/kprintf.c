/*
 * kprintf.c
 *
 *  Created on: Jan 6, 2011
 *      Author: cds
 */

#include <stdarg.h>
#include <inttypes.h>

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

static int putc(char c) {

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

static int puts(const char *s) {

    short i=0;
    char c;
    while((c = *s)) {
        ++i;
	    putc(c);
	    ++s;
    }
    return i;
}

static int putx(uint32_t n) {

	int      l=0;
    sint16_t s;   // shift
    uint8_t  x;
    for(s=28; s>=0; s-=4)
    	if((x = (n>>s)&15) || l)
            l += putc( x + ((x<10) ? '0' : 'a' ));

    return l;
}

static int putu(uint32_t n) {

    uint32_t   d = 1000000000; // divisor
    sint16_t   h;              // digit
    sint16_t   l = 0;          // wrote length
    for(; d>=1; d/=10)
        if((h = ((n/d)%10)) || l || (d<=1)) {
            ++l;
            putc('0' + h);
	    }
    return l;
}

static int putd(sint32_t n) {

    if(n>=0)
    	return putu((sint32_t)n);

    return putc('-') + putd((sint32_t)(-n));
}

static int putlu(uint64_t n) {

    uint64_t d  = 1000000000;
	         d *= 1000000000;

    sint16_t   h;              // digit
    sint16_t   l = 0;          // wrote length
    for(; d>=1; d/=10)
        if((h = ((n/d)%10)) || l || (d<=1)) {
            ++l;
            putc('0' + h);
	    }
    return l;
}

static int putll(sint64_t n) {

    if(n>=0)
    	return putlu((uint64_t)n);

    return putc('-') + putlu((uint64_t)(-n));
}

int kprintf(const char * format, ... ) {

	va_list va;
	char c;
	int  l=0;
	int special=0;

	va_start(va, format);

    while((c = *format++)) {

        if(special) {
        	special = 0;

        	switch(c) {

        	    case 's':
                    l += puts( va_arg(va, const char*) );
			        break;

        	    case 'd':
        	    case 'i':
        	    	l += putd( va_args(va, sint32_t) );
        	        break;

				case 'u':
					l += putu( va_args(va, uint32_t) );
					break;

				case 'x':
				case 'X':
					l += putx( va_args(va, uint32_t) );
					break;

				case 'l':

					if(*format == 'l') {
						++format;
						l += putll( va_args(va, sint64_t) );
					}

					if(*format == 'u') {
						++format;
						l += putlu( va_args(va, uint64_t) );
					}

                    break;

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

    va_end(va);

	return l;
}

