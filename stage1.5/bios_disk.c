

#include "16bitreal.h"
#include "bios_disk.h"
#include "mem.h"
#include "alloc.h"
#include "print.h"

#define SUPPORT_RANDOM_READS 0

/******************************************************************************************************
 * int get_ds_reg
 *     return current data segment.
 */
int get_ds_reg();
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

#ifdef DEBUG
	printf("disk_read_sector(0x%x,0x%lx,0x%x\n",bios_drive,sector,dst);
#endif

	dap.packet_size = 0x10;
	dap.reserved0 = dap.reserved1 = 0;
	dap.sectors = 1;
	dap.mem_addr.seg.segment = get_ds_reg();
	dap.mem_addr.seg.offset = (unsigned short)(int)dst;
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
static char *buffer = 0;
static int bps = 0;

int disk_read(unsigned long long abs_address, unsigned short abs_size, void* dst) {

	int ret;

	if(!bps)
		bps = bytes_per_sector(_root_disk);

    // absolute partition address to absolute disk address
	abs_address += _root_sector * bps;

	while(abs_size) {

		unsigned long long sector = abs_address / bps;
		unsigned short offset     = abs_address % bps;
		unsigned short thisread   = bps - offset;

		if(offset || (abs_size > bps)) {
			// not reading a whole sector, we need to buffer, and selectively copy.

			if(!buffer)
				buffer = (char*)alloc(bps);

			if((ret = disk_read_sector( _root_disk, sector, buffer )) != 0)
				return ret;

#ifdef DEBUG
			printf("memcpy(0x%x, 0x%x + 0x%x, 0x%x)\n", dst, buffer , offset, thisread);
			for(;;);
#endif
			memcpy(dst, buffer + offset, thisread);
		}
		else {
			// reading exactly one sector, let the bios write directly to destination.
			if((ret = disk_read_sector( _root_disk, sector, dst )) != 0)
				return ret;
		}

		abs_address += thisread;
		abs_size    -= thisread;
		dst         += thisread;
	}
	return 0;
}

