
#include "16bitreal.h"

/*******************************************************************************
 * rread: real read
 *  int rread(int seg, int offset);
 *    read int from seg:offset
 ******************************************************************************/
__asm__(

    "rread:\n"
        "pushl    %ds         \n" // backup data segment
		"movl  12(%esp), %eax \n" // read offset from stack
		"movw   8(%esp), %ds  \n" // read segment to data segment
		"movl    (%eax), %eax \n" // read value
		"popl     %ds         \n"
        "ret                  \n" );

/*******************************************************************************
 * rwrites: real write ( short )
 *  void rwrites(int seg, int offset, int data);
 *    write given short to seg:offset
 ******************************************************************************/
__asm__(

    "rwrites:\n"
        "pushl    %ds         \n" // backup data segment
		"pushl    %ebx        \n" // backup data segment
		"movl  20(%esp), %ebx \n" // read data from stack
		"movl  16(%esp), %eax \n" // read offset from stack
		"movw  12(%esp), %ds  \n" // read segment to data segment
		"movw      %bx ,(%eax)\n" // write value
		"popl     %ebx        \n"
		"popl     %ds         \n"
        "ret                  \n" );



/***************************************************************************************
 * Check if the A20 line is enabled. ( test if address wraps around after 1 megabyte )
 *   returns zero is a20 line is not enabled, non-zero if it is enabled.
 **************************************************************************************/
#define S1 0x0000
#define O1 0x7dfe
#define S2 0xFFFF
#define O2 0x7e0e
int check_a20_line() {

	int bootsig   = (rread(S1,O1) & 0xffff); // read MBR boot sig ( 0xaa55 )
	int bootsig1M = (rread(S2,O2) & 0xffff); // read MBR boot sig + 1M ( will wrap around if A20 line is disabled )

	printf("0x%x\n", bootsig);
	printf("0x%x\n", bootsig1M);

	if(bootsig == bootsig1M) {

		rwrites(S1,O1, ~bootsig);               // change boot sig
		bootsig   = (rread(S1,O1) & 0xffff);    // re-read boot sig
		bootsig1M = (rread(S2,O2) & 0xffff);    // re-read boot sig +1M

		printf("0x%x\n", bootsig);
		printf("0x%x\n", bootsig1M);
	}

	return (bootsig1M - bootsig);                  // if different, a20 line must be enabled
}


