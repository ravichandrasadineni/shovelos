/*
 * ext2.c
 *
 *  Created on: Nov 6, 2010
 *      Author: cds
 */

#include "../../16bitreal.h"
#include "../../inttypes.h"
#include "../../bios_disk.h"
#include "ext2.h"

int fs_is_ext2() {

    int magic = 0xFFFF;

    if(disk_read(EXT2_SUPERBLOCK_OFFSET + EXT2_SUPERBLOCK_MAGIC_OFFSET,  EXT2_SUPERBLOCK_MAGIC_SIZE, &magic) != 0)
    	return -1; // DISK ERROR

    if(magic != EXT2_SUPERBLOCK_MAGIC_VALUE)
    	return 1; // NOT EXT2

    return 0; // FS IS EXT2
}

