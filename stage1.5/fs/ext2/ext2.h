/*
 * ext2.h
 *
 *  Created on: Nov 6, 2010
 *      Author: cds
 */

#ifndef __STAGE_1_5_FS_EXT2_EXT2_H
#define __STAGE_1_5_FS_EXT2_EXT2_H

#define EXT2_SUPERBLOCK_OFFSET 			1024
#define EXT2_SUPERBLOCK_SIZE   			1024

/*** SUPERBLOCK MAGIC ***/
#define EXT2_SB_SIG_OFFSET	56
#define EXT2_SB_SIG_SIZE	 2
#define EXT2_SB_SIG_VALUE	0xEF53

/*** SUPERBLOCK BLOCK SIZE ***/
#define EXT2_SB_BS_OFFSET 24
#define EXT2_SB_BS_SIZE    4

/*** SUPERBLOCK BLOCKS PER GROUP ***/
#define EXT2_SB_BPG_OFFSET 32
#define EXT2_SB_BPG_SIZE    4

/*** SUPERBLOCK INODES PER GROUP ***/
#define EXT2_SB_IPG_OFFSET 40
#define EXT2_SB_IPG_SIZE    4



int fs_init();

#endif /* __STAGE_1_5_FS_EXT2_EXT2_H */

