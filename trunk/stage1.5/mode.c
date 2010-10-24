
#include "16bitreal.h"
#include "mode.h"
#include "print.h"
#include "pt.h"
#include "gdt.h"

__asm__(".global enter_long_mode   \n"
		"msg:                      \n"
		".asciz \"hello\"          \n"
		"enter_long_mode:          \n"
        "  push %ebp               \n"
		"  mov  %esp, %ebp         \n"

		"  mov $0xa0, %eax   \n"    // Set PAE and PGE
		"  mov %eax,  %cr4   \n"

  //      "  call load_pt      \n"    // load page table.
		"    movl g_pmle4, %edx    \n"
		"    movl %edx,    %cr3    \n"

		"  mov $0xC0000080, %ecx \n" //	Specify EFER MSR
		"  rdmsr				\n"
		"  or $0x00000100, %eax  \n"
		"  wrmsr                \n" // 	Enable Long Mode



		"  movl %cr0,%ebx		\n" //	Activate long mode
		"  orl  $0x80000001, %ebx  \n" //	by enabling paging and protection simultaneously
		"  movl %ebx, %cr0       \n"	//	skipping protected mode entirely

		"    movw gdt_reg, %eax \n"
		"    lgdt (%eax)   \n"

		"  jmp $8, $long_main  \n" // long jump to long mode (code segment descriptor at offset 8)

		"  pop %ebp          \n"
		"  ret               \n"

//		".code64 \n"
		"  cli                        \n"
		"long_main:                   \n"
	//	" movl $16, %eax              \n"
	//	" movl %eax, %ds              \n"
//		"  push $msg                 \n"
//		"  call halt                  \n"
		"  jmp   long_main            \n");

