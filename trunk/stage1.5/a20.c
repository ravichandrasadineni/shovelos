
#include "16bitreal.h"
#include "print.h"

/*******************************************************************************
 * rread: real read
 *  short rread(int seg, int offset);
 *    read int from seg:offset
 ******************************************************************************/
short rreads(int seg, int offset);
__asm__(

    "rreads:\n"
        "pushl    %ds         \n" // backup data segment
		"movl  12(%esp), %eax \n" // read offset from stack
		"movw   8(%esp), %ds  \n" // read segment to data segment
		"movw    (%eax), %ax  \n" // read value
		"andl   $0xffff, %eax \n"
		"popl     %ds         \n"
        "ret                  \n" );

/*******************************************************************************
 * rwrites: real write ( short )
 *  void rwrites(int seg, int offset, int data);
 *    write given short to seg:offset
 ******************************************************************************/
void rwrites(int seg, int offset, int data);
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

/*******************************************************************************
 * attempt to enable a20 line using the bios.
 *  int a20_enable_bios();
 *  returns 1 on reported success, 0 on failure.
 ******************************************************************************/
int a20_enable_bios();
__asm__(

    "a20_enable_bios:\n"
        "movl $0x2401, %eax   \n"
		"int $0x15            \n"
		"movl $1, %eax        \n"
		"jnc .success         \n"
		"xorl %eax, %eax      \n"
	".success:                \n"
		"ret                  \n");

/***************************************************************************************
 * Check if the A20 line is enabled. ( test if address wraps around after 1 megabyte )
 *   returns zero is a20 line is not enabled, non-zero if it is enabled.
 **************************************************************************************/
int check_a20_line() {

	// overwrite bootsig with NOT contents of bootsig address + 1M
	rwrites(0x0000,0x7dfe, ~rreads(0xFFFF,0x7e0e));

	// if contents of bootsig are different from bootsig + 1M, then a20 MUST be enabled.
	return rreads(0x0000,0x7dfe) != rreads(0xFFFF,0x7e0e);
}

/***************************************************************************************
 * Attempt to enable the a20 line.
 **************************************************************************************/
void enable_a20_line() {

	if(!check_a20_line()) {
		a20_enable_bios();
		if(!check_a20_line())
			halt("ERR: a20\n");
	}
}

