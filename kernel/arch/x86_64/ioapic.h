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

/** for building 64bit IOREDTBL## registers ***/
#define IOAPIC_RED_DST(x)  		((x & 0xff) << 56)
#define IOAPIC_RED_MASK(x) 		((x & 0x01) << 16)
#define IOAPIC_RED_TRIGGER(x)	((x & 0x01) << 15)
#define IOAPIC_RED_RIRR(x)		((x & 0x01) << 14)
#define IOAPIC_RED_INPOL(x)		((x & 0x01) << 13)
#define IOAPIC_RED_DELVIS(x)	((x & 0x01) << 12)
#define IOAPIC_RED_DESTMOD(x)	((x & 0x01) << 11)
#define IOAPIC_RED_DELMOD(x)	((x & 0x07) <<  8)
#define IOAPIC_RED_INTVEC(x)	((x & 0xff) <<  0)

/*** DELMOD PARAMETERS ***/
enum ioapic_delmod_param {

	IOAPIC_DELMOD_FIXED		= 0x0,
	IOAPIC_DEMLOD_LOWPR		= 0x1,
	IOAPIC_DELMOD_SMI		= 0x2,
	IOAPIC_DELMOD_NMI		= 0x4,
	IOAPIC_DELMOD_INIT		= 0x5,
	IOAPIC_DELMOD_EXTINIT	= 0x7,
};

/*** DESTINATION PARAMETERS ***/
enum ioapic_dest_param {

	IOAPIC_DEST_PHYSICAL	= 0x0,
	IOAPIC_DEST_LOGICAL		= 0x1,
};

/*** other parameters ***/
enum ioapic_param {

	IOAPIC_CLEAR = 0x0,
	IOAPIC_SET	= 0x1,
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
