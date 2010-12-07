
#include "16bitreal.h"
#include "mem.h"

#define _286_memcpy
#define _286_memset
#define _286_strcmp

#ifdef _286_memcpy

	__asm__(".global memcpy        \n"
			"memcpy:               \n"
			"pushl   %edi          \n"
			"pushl   %esi          \n"
			"movl 12(%esp), %edi   \n"
			"movl 16(%esp), %esi   \n"
			"movl 20(%esp), %ecx   \n"
			"rep                   \n"
			"movsb                 \n"
			"popl    %esi          \n"
			"popl    %edi          \n"
			"ret");
#endif

#ifdef _286_memset

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
#endif

#ifdef _286_strcmp

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

#endif

#ifdef _C_memcpy
	void *memcpy(void *dst,const void* src, int size) {

		while(size--)
			((char*)dst)[size] = ((char*)src)[size];

		return dst;
	}

#endif

#ifdef _C_memset
	void *memset(void* dst, int c, int size) {
		while(size--)
			((char*)dst)[size] = (char)c;

		return dst;
	}
#endif

#ifdef _C_strcmp
	int strcmp(const char *s1, const char *s2) {

		int index,compare;

		for(index=0; s1[index] || s2[index]; index++)
			if((compare = (s1[index] - s2[index])))
				return compare;

		return 0;
	}
#endif

