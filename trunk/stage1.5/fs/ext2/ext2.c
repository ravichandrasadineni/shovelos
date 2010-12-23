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

static struct disk disk;
static struct partition partition;

int fs_init() {

	disk = open_disk(_root_disk);
	partition = open_partition(&disk, _root_part);

    int magic = 0;

    partition_read(&partition, EXT2_SUPERBLOCK_OFFSET + EXT2_SB_SIG_OFFSET, EXT2_SB_SIG_SIZE, &magic);

    if(magic != EXT2_SB_SIG_VALUE)
        halt("cannot find ext2 formatted boot partition");

    return 0; // SUCCESS
}

