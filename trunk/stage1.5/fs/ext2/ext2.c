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

//static uint32_t ext2_block_size = 0;	/*** block size ***/
//static uint32_t ext2_bpg = 0;			/*** blocks per group ***/
//static uint32_t ext2_ipg = 0;			/*** i-nodes per group ***/

int fs_init() {

    int magic = 0;

    disk_read(EXT2_SUPERBLOCK_OFFSET + EXT2_SB_SIG_OFFSET, EXT2_SB_SIG_SIZE, &magic);

    if(magic != EXT2_SB_SIG_VALUE)
        	halt("cannot find ext2 formatted boot partition");

    /*
    disk_read(EXT2_SUPERBLOCK_OFFSET + EXT2_SB_BS_OFFSET,  EXT2_SB_BS_SIZE,  &ext2_block_size);
    disk_read(EXT2_SUPERBLOCK_OFFSET + EXT2_SB_BPG_OFFSET, EXT2_SB_BPG_SIZE, &ext2_bpg);
    disk_read(EXT2_SUPERBLOCK_OFFSET + EXT2_SB_IPG_OFFSET, EXT2_SB_IPG_SIZE, &ext2_ipg);

    switch(ext2_block_size)
    {
    default:
    case 0:
    	ext2_block_size = 1024;
    	break;
    case 1:
    	ext2_block_size = 1024 * 2;
    	break;
    case 2:
    	ext2_block_size = 1024 * 4;
    	break;
    }
    */

    return 0; // SUCCESS
}

