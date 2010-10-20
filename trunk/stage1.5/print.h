
#ifndef __PRINT_H
#define __PRINT_H

#include "inttypes.h"

// clear screen
void cls();

// write a string
int  puts(const char *s);

// write a hex number
int  putnhex(uint64_t n);

// write an unsigned decimal
int  putndecu(uint64_t n);

// write a signed decimal
int  putndec(sint64_t n);

// write fomatted
int  printf(const char * format, int *_args);

#endif /*** __PRINT_H ***/

