

#ifndef __BIOS_DISK_H
#define __BIOS_DISK_H

/****************************************************************************************************
 * DISK ADDRESS PACKET - for use with bios function INT 13h, AH=42h
 */
struct disk_address_packet {

	unsigned char packet_size;        // 0x10
	unsigned char reserved0;
	char          sectors;            // number of sectors to access
	unsigned char reserved1;
	union {
		unsigned int address;
		struct {
			unsigned short offset;    // memory offset to access
			unsigned short segment;   // memory segment to access
		} seg;
	} mem_addr;
	union {
		unsigned long long sector;    // absolute first sector
		struct {
			unsigned int lower;       // absolute first sector least sig dword
			unsigned int upper;       // absolute first sector most  sig dword
		} sec32;
	} disk_addr;
};

/******************************************************************************************************
 * EXTENDED DRIVE INFO BUFFER - for use with bios function INT 13h, AH=48h
 */
struct ext_drive_param_buffer {

	unsigned short         buffer_size;
	unsigned short         info_flags;
	unsigned int           phy_cylinders;
	unsigned int           phy_heads;
	unsigned int           phy_sectors_per_track;
	unsigned long long int abs_number_of_sectors;
	unsigned short         bytes_per_sector;
	unsigned int           edd_ptr;
};

#endif /*** __BIOS_DISK_H ***/

