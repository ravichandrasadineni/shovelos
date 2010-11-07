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
#define EXT2_SUPERBLOCK_MAGIC_OFFSET	56
#define EXT2_SUPERBLOCK_MAGIC_SIZE		2
#define EXT2_SUPERBLOCK_MAGIC_VALUE		0xEF53

int fs_is_ext2();

#endif /* __STAGE_1_5_FS_EXT2_EXT2_H */

