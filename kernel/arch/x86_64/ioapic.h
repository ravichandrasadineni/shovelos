/*
 * ioapic.h
 *
 *  Created on: Jan 29, 2011
 *      Author: cds
 */

#ifndef IOAPIC_H_
#define IOAPIC_H_

enum ioapic_mm_reg {
	IOREGSEL 	= 0x00,
	IOWIN		= 0x10,
};

enum ioapic_reg {

	IOAPICID	= 0x00,
	IOAPICVER	= 0x01,
	IOAPICARB	= 0x02,
	IOREDTBL00	= 0x10,
	IOREDTBL01	= 0x12,
	IOREDTBL02	= 0x14,
	IOREDTBL03	= 0x16,
	IOREDTBL04	= 0x18,
	IOREDTBL05	= 0x1a,
	IOREDTBL06	= 0x1c,
	IOREDTBL07	= 0x1e,
	IOREDTBL08	= 0x20,
	IOREDTBL09	= 0x22,
	IOREDTBL10	= 0x24,
	IOREDTBL11	= 0x26,
	IOREDTBL12	= 0x28,
	IOREDTBL13	= 0x2a,
	IOREDTBL14	= 0x2c,
	IOREDTBL15	= 0x2e,
	IOREDTBL16	= 0x30,
	IOREDTBL17	= 0x32,
	IOREDTBL18	= 0x34,
	IOREDTBL19	= 0x36,
	IOREDTBL20	= 0x38,
	IOREDTBL21	= 0x3a,
	IOREDTBL22	= 0x3c,
	IOREDTBL23	= 0x3e,
};

static inline void io_apic_write32(void* ioapic_base, enum ioapic_reg reg, uint32_t data) {

	*(uint32_t*)((uint8_t*)ioapic_base + IOREGSEL) = reg;
	*(uint32_t*)((uint8_t*)ioapic_base + IOWIN   ) = data;
}

static inline void io_apic_write64(void* ioapic_base, enum ioapic_reg reg, uint64_t data) {

	*(uint32_t*)((uint8_t*)ioapic_base + IOREGSEL) = reg;
	*(uint32_t*)((uint8_t*)ioapic_base + IOWIN   ) = ((uint32_t*)data)[0];
	*(uint32_t*)((uint8_t*)ioapic_base + IOREGSEL) = reg+1;
	*(uint32_t*)((uint8_t*)ioapic_base + IOWIN   ) = ((uint32_t*)data)[1];
}

static inline uint32_t io_apic_read32(void* ioapic_base, enum ioapic_reg reg) {

	       *(uint32_t*)((uint8_t*)ioapic_base + IOREGSEL) = reg;
	return *(uint32_t*)((uint8_t*)ioapic_base + IOWIN   );
}

static inline uint32_t io_apic_read64(void* ioapic_base, enum ioapic_reg reg) {

	uint64_t reta,retb;

	         *(uint32_t*)((uint8_t*)ioapic_base + IOREGSEL) = reg;
	reta  =  *(uint32_t*)((uint8_t*)ioapic_base + IOWIN   );

             *(uint32_t*)((uint8_t*)ioapic_base + IOREGSEL) = reg+1;
	retb |=  *(uint32_t*)((uint8_t*)ioapic_base + IOWIN   );

	return reta | (retb << 32);
}

#endif /* IOAPIC_H_ */
