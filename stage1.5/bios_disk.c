

#include "16bitreal.h"
#include "bios_disk.h"


/******************************************************************************************************
 * int get_ds_reg
 *     return current data segment.
 */
__asm__("get_ds_reg:\n"
		"  xorl %eax, %eax    \n"
		"  movw  %ds,  %ax    \n"
		"  ret "
		);

/******************************************************************************************************
 *   extended_read_sectors_from_drive
 *     call bios extended read sectors function.
 *     takes 1) BIOS disk ( 0x80 for C:, etc )
 *           2) initialised disk address packet.
 *     returns 0 on success, non-zero on error.
 */
int extended_read_sectors_from_drive(unsigned char bios_drive, struct disk_address_packet *dap);

__asm__("extended_read_sectors_from_drive:\n"
		"  pushl      %edx                       \n"
		"  pushl      %esi                       \n"
		"  movb    $8(%esp),   %dl               \n" // bios disk to dl
		"  movw   $12(%esp),   %si               \n" // D.A.P address ( DS:SI )
		"  movb      $0x42 ,   %ah               \n" // bios function 42h
		"  int       $0x13                       \n" // call bios 13h
		"  xorl       $eax ,   %eax              \n" // clear return var
		"  jnc        .ret                       \n" // exit on no error
		"  movb         $1 ,   %al               \n" // set error return flag
".ret:"
		"  popl   %esi                           \n"
		"  popl   %edx                           \n"
		"  ret"

/******************************************************************************************************
 *  disk_read_sector
 *     call bios extended read sectors function.
 *     takes 1) BIOS disk ( 0x80 for C:, etc )
 *           2) initialised disk address packet.
 *           3) read to address ( in data segment )
 *     returns 0 on success, non-zero on error.
 */
int disk_read_sector( unsigned char bios_drive, unsigned long long sector, void *dst ) {

	static struct disk_address_packet dap;

	dap.packet_size = 0x10;
	dap.reserved0 = dap.reserved1 = 0;
	dap.sectors = 1;
	dap.mem_addr.seg.segment = get_ds_reg();
	dap.mem_addr.seg.offset = dst;
	dap.disk_addr.sector = sector;

	return extended_read_sectors_from_drive(bios_drive, &dap);
}

/******************************************************************************************************
 *  disk_read
 *     read absolute disk address to given address
 *     takes 1) BIOS disk ( 0x80 for C:, etc )
 *           2) initialised disk address packet.
 *           3) read to address ( in data segment )
 *     returns 0 on success, non-zero on error.
 */
int disk_read( unsigned char bios_drive, unsigned long long abs_address, unsigned short abs_size, void* dst) {

	int ret;
	static char buffer[512]; // TODO:

	while(abs_size) {

		unsigned long long sector = abs_address / 512;
		unsigned short offset     = abs_address % 512;
		unsigned short thisread   = 512 - offset;

		if(offset || (abs_size < 512)) {
			// not reading a whole sector, we need to buffer, and selectivly copy.
			if((ret = disk_read_sector( bios_drive, sector, buffer )) != 0)
				return ret;

			memcpy(dst, buffer + offset, thisread);
		}
		else {
			// reading exactly one sector, let the bios write directly to destination.
			if((ret = disk_read_sector( bios_drive, sector, dst )) != 0)
				return ret;
		}

		abs_address += thisread;
		abs_size    -= thisread;
		dst         += thisread;
	}
	return 0;
}

