

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
 *   extended_read_drive_parameters
 *     call bios extended read sectors function.
 *     takes 1) BIOS disk ( 0x80 for C:, etc )
 *           2) result buffer (struct ext_drive_param_buffer*) .
 *     returns 0 on success, non-zero on error.
 */
int extended_read_drive_parameters(unsigned char bios_drive, struct ext_drive_param_buffer* out);

__asm__("extended_read_drive_parameters:      \n"
		"  pushl   %edx                       \n"
		"  pushl   %esi                       \n"
	    "  movb    $0x48,      %ah            \n"
		"  movb    12(%esp),  %dl            \n" // parameter 1, bios drive
		"  movw    16(%esp),  %si            \n" // parameter 2, ptr to result buffer
		"  int     $0x13                      \n" // call bios 13h
	    "  xorl    %eax ,     %eax            \n" // clear return var
		"  jnc     erdp.ret                   \n" // exit on no error
		"  movb    $1 ,       %al             \n" // set error return flag
		"erdp.ret:                            \n"
		"  popl    %esi                       \n"
		"  popl    %edx                       \n"
		"  ret" );



/******************************************************************************************************
 *   extended_read_sectors_from_drive
 *     call bios extended read sectors function.
 *     takes 1) BIOS disk ( 0x80 for C:, etc )
 *           2) initialised disk address packet.
 *     returns 0 on success, non-zero on error.
 */
int extended_read_sectors_from_drive(unsigned char bios_drive, struct disk_address_packet *dap);

__asm__("extended_read_sectors_from_drive:       \n"
		"  pushl      %edx                       \n"
		"  pushl      %esi                       \n"
		"  movb    12(%esp),   %dl               \n" // bios disk to dl
		"  movw    16(%esp),   %si               \n" // D.A.P address ( DS:SI )
		"  movb      $0x42 ,   %ah               \n" // bios function 42h
		"  int       $0x13                       \n" // call bios 13h
		"  xorl       %eax ,   %eax              \n" // clear return var
		"  jnc        ersfd.ret                  \n" // exit on no error
		"  movb         $1 ,   %al               \n" // set error return flag
        "ersfd.ret:                                   \n"
		"  popl   %esi                           \n"
		"  popl   %edx                           \n"
		"  ret");


/****************************************************************************************************
 * bytes_per_sector
 *     query the number of bytes per sector. ( probably 512 )
 *     takes 1) bios drive
 *     returns bytes per sector, or zero on error.
 */
int bytes_per_sector(unsigned char bios_disk) {

	struct ext_drive_param_buffer edpb;
	memset(&edpb, 0, sizeof edpb);
	edpb.buffer_size = 0x1e;

	if(extended_read_drive_parameters(bios_disk, &edpb) != 0)
		return 0;

	return edpb.bytes_per_sector;
}

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
	dap.mem_addr.seg.offset = (unsigned short)dst;
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
	int bps = bytes_per_sector(bios_drive);
	char *buffer = (char*)malloc();

	while(abs_size) {

		unsigned long long sector = abs_address / bps;
		unsigned short offset     = abs_address % bps;
		unsigned short thisread   = bps - offset;

		if(offset || (abs_size < bps)) {
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

