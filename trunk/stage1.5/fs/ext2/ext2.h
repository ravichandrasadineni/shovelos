/*
 * ext2.h
 *
 *  Created on: Nov 6, 2010
 *      Author: cds
 */

#ifndef __STAGE_1_5_FS_EXT2_EXT2_H
#define __STAGE_1_5_FS_EXT2_EXT2_H

#include <inttypes.h>

int fs_init();

typedef struct {

	uint32_t inode;
} FILE;

struct stat {
	uint32_t st_ino;
	uint32_t st_size;
};

#endif /* __STAGE_1_5_FS_EXT2_EXT2_H */

