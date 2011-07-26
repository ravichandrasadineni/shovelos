

#ifndef __ARCH_X86_64_CPUID_H
#define __ARCH_X86_64_CPUID_H

#include "rflags.h"
#include<inttypes.h>

struct cpu_cpuid_return_registers {

	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
};

static inline void cpu_cpuid(uint32_t eax, struct cpu_cpuid_return_registers *reg) {

	__asm__ __volatile__	(	"movl %4, %%eax;"
								"cpuid;"
								"movl %%eax, %0;"
								"movl %%ebx, %1;"
								"movl %%ecx, %2;"
								"movl %%edx, %3;"
							:	"=g" (reg->eax),
							 	"=g" (reg->ebx),
							 	"=g" (reg->ecx),
							 	"=g" (reg->edx)
							:	"D"  (eax)
							:	"eax", "ebx", "ecx", "edx" );
}

/*****
 * test if the cpu supports the cpuid instruction.
 * 	if the ID field of the frlags register is writable, then cpuid is supported.
 */
static inline BOOL cpu_has_cpuid() {

	uint64_t r1,r2;

	r1 = cpu_read_rflags();

	cpu_write_rflags( r1 ^ RFLAG_ID );

	r2 = cpu_read_rflags();

	if((r1 & RFLAG_ID) != (r2 & RFLAG_ID))
		return TRUE;

	return FALSE;
}

#define CPU_READ_REG(_reg_) \
		static inline uint64_t cpu_read_ ## _reg_() { \
		uint64_t ret = 0; \
		__asm__ __volatile__( "movq %%" #_reg_ ", %0" : "=r" (ret) ); \
		return (ret); \
}

CPU_READ_REG(cr0) /* static inline uint64_t cpu_read_cr0() */
CPU_READ_REG(cr1) /* static inline uint64_t cpu_read_cr1() */
CPU_READ_REG(cr2) /* static inline uint64_t cpu_read_cr2() */
CPU_READ_REG(cr3) /* static inline uint64_t cpu_read_cr3() */


static inline void cpu_invlpg(uint64_t *mem) {

	__asm__ __volatile(
			"invlpg %0;"
		:	/* no output */
		:	"m" (mem)
		);
}

#endif /*** __ARCH_X86_64_CPUID_H ***/


