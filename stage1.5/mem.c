
#include "16bitreal.h"
#include "mem.h"




/********************************************************************
 * memcpy(dst,src,size)
 *   1) dst ( 20bit address ) ( 0x00000 -> 0xfffff )
 *   2) src ( 20bit address ) ( 0x00000 -> 0xfffff )
 *   3) size in bytes
 */
__asm__(".global memcpy_far       \n"
		"memcpy_far:              \n"
		"pushl   %edi             \n"
		"pushl   %esi             \n"
		"pushl   %ds              \n"
		"pushl   %es              \n"
							     	  // setup destination (es:edi)
		"movl 20(%esp),    %edi   \n" // edi = dst
		"movl %edi,        %es    \n" // es  = dst
		"andl $0xffff,     %edi   \n" // edi &= 0xffff
		"andl $0ffff0000,  %es    \n" // es  &= 0xffff0000
		"shll $12,         %es    \n" // es  <<= 12;

								       // setup source (ds::esi)
		"movl 24(%esp),     %esi   \n" // esi = src
		"movl %esi,         %ds    \n" // ds  = src
		"andl $0xffff,      %esi   \n" // esi &= 0xffff
		"andl $0ffff0000,   %ds    \n" // ds  &= 0xffff0000
		"shll $12,          %ds    \n" // ds  <<= 12;

		"xorl %ecx,         %ecx   \n"
    ".loop:                        \n"
		"cmpl   %ecx,    28(%esp)  \n"	// increment counter and test for exit
		"je    .eloop              \n"
		"incl   %ecx               \n"

	    "ds movb (%esi),     %al   \n"	// copy byte
		"es movb    %al,   (%edi)  \n"

										// INCREMENT DST seg:offset
		"incl %edi                 \n"
		"cmpl $0x10000, %edi       \n"
		"jne .done_edi             \n"
		"addl $0x1000,   %es        \n"
		"xorl %edi,     %edi       \n"
	".done_edi:                    \n"

										// INCREMENT SRC seg:offset
		"incl %esi                 \n"
		"cmpl $0x10000, %esi       \n"
		"jne .done_esi             \n"
		"addl $0x1000,  %ed        \n"
		"xorl %esi,     %esi       \n"
	".done_esi:                    \n"

		"jmp .loop                 \n"
	".eloop:                       \n"

		"popl    %es               \n"
		"popl    %ds               \n"
		"popl    %esi              \n"
		"popl    %edi              \n"
		"ret");


	__asm__(".global memset        \n"
			"memset:               \n"
			"pushl   %edi          \n"
			"movl  8(%esp), %edi   \n"
			"movl 12(%esp), %eax   \n"
			"movl 16(%esp), %ecx   \n"
			"rep                   \n"
			"stosb                 \n"
			"popl    %edi          \n"
			"ret");


	__asm__(".global strcmp        \n"
			"strcmp:               \n"

			"xorl %ecx, %ecx       \n"
			"xorl %eax, %eax       \n"

"._286_strcmp_loop:                \n"

			"movb 4(%esp,%ecx,1), %ah  \n"
			"movb 8(%esp,%ecx,1), %al  \n"

			"cmpb %al,%ah              \n" /* different? return non-zero */
			"jnz  ._286_strcmp_ret     \n"

			"cmpw $0, %ax              \n" /* both zero? return zero */
			"jz   ._286_strcmp_ret     \n"

			"inc %ecx                  \n" /* same, not terminal, continue */
			"jmp  ._286_strcmp_loop    \n"

"._286_strcmp_ret:                     \n"
			"ret");



