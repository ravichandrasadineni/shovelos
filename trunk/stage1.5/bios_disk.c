

#include "16bitreal.h"
#include "bios_disk.h"


__asm__("get_ds_reg:\n"
		"  xorl %eax, %eax    \n"
		"  movw  %ds,  %ax    \n"
		"  ret "
		);


static struct disk_address_packet g_dap;

void disk_read(
		unsigned char disk,
		unsigned long long address,
		unsigned long long size,
		unsigned short mseg,
		unsigned short moff ) {

	g_dap.packet_size = 0x10;
	d_dap.reserved0 = d_dap.reserved1 = 0;
	g_dap.sectors = 1;
	g_dap.mem_addr.seg.segment = mseg;
	g_dap.mem_addr.seg.offset = moff;
	g_dap.disk_addr.sector = address / 512; /* fixme */

}
