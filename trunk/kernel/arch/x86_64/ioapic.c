/*
 * ioapic.c
 *
 *  Created on: 31 Jan 2011
 *      Author: Chris Stones
 */

#include <inttypes.h>
#include <lib/lib.h>
#include <mm/mm.h>
#include "ioapic.h"
#include "mp.h"
#include "pt.h"
#include "8259.h"

uint32_t x=100;
uint32_t y=0;

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

/*** DESTINATION PARAMETERS ***/
enum ioapic_dest_param {

    IOAPIC_DEST_PHYSICAL	= 0x0, /* dst is APIC ID */
    IOAPIC_DEST_LOGICAL		= 0x1, /* dst is cpu set */
};

enum ioapic_trigger_param {

    IOAPIC_EDGE_TRIGGER		= 0x0,
    IOAPIC_LEVEL_TRIGGER	= 0x1,
};

/*** DELMOD PARAMETERS ***/
enum ioapic_delmod_param {

    IOAPIC_DELMOD_FIXED		= 0x0,
    IOAPIC_DEMLOD_LOWPR		= 0x1,
    IOAPIC_DELMOD_SMI		= 0x2,
    IOAPIC_DELMOD_NMI		= 0x4,
    IOAPIC_DELMOD_INIT		= 0x5,
    IOAPIC_DELMOD_EXTINIT	= 0x7,
};

/*** INTPOL PARAMETERS ***/
enum ioapic_intpol_param {

    IOAPIC_INTPOL_HI		=0x0,
    IOAPIC_INTPOL_LO		=0x1,
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

uint16_t ioapic_detect() {

    uint16_t count = 0;

    /*** todo: there are other ways to find the ioapic ***/
    for (const struct mp_ioapic *ioapic  = mp_find_first_io_apic();
            ioapic != 0;
            ioapic  = mp_find_next_io_apic( ioapic )) {
        ++count;
    }

    return count;
}

static void* ioapic = 0;

static void config(uint64_t phy_addr) {

    uint64_t red_ent = 0;
  //  uint8_t  red_vec = 0;

    if(!ioapic) {

    	uint64_t pages = 1;
   // 	if((PAGE_SIZE <= 0x1000) && (phy_addr % PAGE_SIZE))
   // 		pages = 2;

    	uint64_t vpage = (uint64_t)vmm_alloc_hw( pages*PAGE_SIZE );

    	for(int i=0;i<pages;i++) {
    		mmap((phy_addr + (i*PAGE_SIZE)) & ~PAGE_SIZE, vpage+PAGE_SIZE*i, 0);
    	}

    	ioapic = (void*)(vpage + (phy_addr % PAGE_SIZE));
    }

    /* all interrupts to boot-strap processor
    for ( uint16_t red_tbl = IOREDTBL00; red_tbl <= IOREDTBL23; red_tbl += 2) {

       red_ent = 	IOAPIC_RED_DST		( 0ll 						)|
                    IOAPIC_RED_MASK		( IOAPIC_SET )				|
                    IOAPIC_RED_TRIGGER	( IOAPIC_CLEAR )			| // read only
                    IOAPIC_RED_INPOL	( IOAPIC_INTPOL_LO )		|
                    IOAPIC_RED_DELVIS	( IOAPIC_CLEAR )			| // read only
                    IOAPIC_RED_DESTMOD	( IOAPIC_DEST_PHYSICAL )	|
                    IOAPIC_RED_DELMOD	( IOAPIC_DELMOD_FIXED ) 	|
                    IOAPIC_RED_INTVEC	( red_vec );

//      kprintf("ioapic_write64(0x%lx,%d,0x%lx)\n",ioapic,red_tbl, red_ent);
        io_apic_write64( ioapic, red_tbl, red_ent); // FIXME

        ++red_vec;
    }
    */


    red_ent = 	IOAPIC_RED_DST		( 0ll 						)|
    			IOAPIC_RED_MASK		( IOAPIC_CLEAR )			|
    			IOAPIC_RED_TRIGGER	( IOAPIC_CLEAR )			| // clear is edge, set is level
    			IOAPIC_RED_INPOL	( IOAPIC_INTPOL_HI )		|
    			IOAPIC_RED_DELVIS	( IOAPIC_CLEAR )			| // read only
    			IOAPIC_RED_DESTMOD	( IOAPIC_DEST_PHYSICAL )	|
    			IOAPIC_RED_DELMOD	( IOAPIC_DELMOD_FIXED ) 	|
    			IOAPIC_RED_INTVEC	( 1 );

    io_apic_write64( ioapic, IOREDTBL01 ,red_ent);

}

uint16_t ioapic_configure() {

    uint16_t count = 0;

    for (const struct mp_ioapic *ioapic  = mp_find_first_io_apic();
            ioapic != 0;
            ioapic  = mp_find_next_io_apic( ioapic )) {

    	kprintf("found an IOApic at 0x%x\n",ioapic->mmap_ioapic);

    	++count;

       config( ioapic->mmap_ioapic );
    }

    return count;
}

