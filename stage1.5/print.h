
#ifndef __PRINT_H
#define __PRINT_H

// write a char
void putc(char c);

// write a string
int  puts(const char *s);

// write a hex number
int  putnhex(unsigned int n);

// write an unsigned decimal
int  putndecu(unsigned int n);

// write a signed decimal
int  putndec(int n);

// write fomatted
int  printf(const char * format, int *_args);

#endif /*** __PRINT_H ***/

