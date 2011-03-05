/*
 * ext2.c
 *
 *  Created on: Nov 6, 2010
 *      Author: cds
 */

#include "../../16bitreal.h"
#include "../../inttypes.h"
#include <disk/disk.h>
#include "../../mem.h"
#include "../../print.h"
#include "../../pt.h"
#include "ext2.h"

/*** the only two inode mode fields we are interested in ***/
#define S_IFREG	0x8000	/* regular file */
#define S_IFDIR	0x4000	/* directory */
#define S_IFSYM 0xA000  /* symbolic link */

#define EXT2_ROOT_INODE					  2

/****************************************************************************************/
/********************** SUPERBLOCK FIELD OFFSETS / SIZES ********************************/
/****************************************************************************************/
#define EXT2_SUPERBLOCK_OFFSET 			1024
#define EXT2_SUPERBLOCK_SIZE   			1024
#define EXT2_SB_INODES_OFFSET	    	   0		/*** total i-nodes ***/
#define EXT2_SB_INODES_SIZE	     		   4
#define EXT2_SB_BLOCKS_OFFSET	    	   4		/*** total blocks ***/
#define EXT2_SB_BLOCKS_SIZE	     		   4
#define EXT2_SB_BLOCK0_OFFSET             20        /*** block number of first superblock ***/
#define EXT2_SB_BLOCK0_SIZE                4
#define EXT2_SB_BS_OFFSET       		  24		/*** log2(block_size)-10 ***/
#define EXT2_SB_BS_SIZE          		   4
#define EXT2_SB_BPG_OFFSET      		  32		/*** blocks per group ***/
#define EXT2_SB_BPG_SIZE         		   4
#define EXT2_SB_IPG_OFFSET      		  40		/*** i-nodes per group ***/
#define EXT2_SB_IPG_SIZE         		   4
#define EXT2_SB_SIG_OFFSET	    		  56		/*** SUPERBLOCK MAGIC ***/
#define EXT2_SB_SIG_SIZE	     		   2
#define EXT2_SB_SIG_VALUE			  0xEF53
#define EXT2_SB_MAJVER_OFFSET             76		/*** ext2 major version ***/
#define EXT2_SB_MAJVER_SIZE                4
/*** ext2 version 1.0 and above ***/
#define EXT2_SB_INODE_SIZE_OFFSET         88		/*** inode structure size***/
#define EXT2_SB_INODE_SIZE_SIZE            2


/****************************************************************************************/
/******************* GROUP DESCRIPTOR FIELD OFFSETS / SIZES *****************************/
/****************************************************************************************/

#define EXT2_GD_INO_TBL_OFFSET			   8		/*** starting block address of i-node table ***/
#define EXT2_GD_INO_TBL_SIZE			   4

/****************************************************************************************/
/************************ INODE FIELD OFFSETS / SIZES ***********************************/
/****************************************************************************************/

#define EXT2_IN_MODE_OFFSET				   0		/*** INODE MODE ***/
#define EXT2_IN_MODE_SIZE				   2
#define EXT2_IN_SIZE_OFFSET				   4		/*** FILE SIZE IN BYTES ***/
#define EXT2_IN_SIZE_SIZE				   4
#define EXT2_IN_DIRECT_PTR		          40		/*** pointers to direct blocks, array of 12 ***/
#define EXT2_IN_INDIRECT_PTR		      88		/*** pointers to indirect blocks ***/
#define EXT2_IN_INDIRECT2_PTR		      92		/*** pointers to double indirect blocks ***/
#define EXT2_IN_INDIRECT3_PTR		      96		/*** pointers to triple indirect blocks ***/
#define EXT2_IN_PTR_SIZE			       4
#define EXT2_IN_PTR_ARRAY_LEN		      12

/****************************************************************************************/
/********************** DIRECTORY FIELD OFFSETS / SIZES *********************************/
/****************************************************************************************/

#define EXT2_DR_INODE_OFFSET			   0
#define EXT2_DR_INODE_SIZE				   4
#define EXT2_DR_RECLEN_OFFSET			   4
#define EXT2_DR_RECLEN_SIZE				   2
#define EXT2_DR_NAMLEN_OFFSET			   6
#define EXT2_DR_NAMLEN_SIZE				   1

static struct disk disk;
static struct partition partition;

struct superblock {
	int block_size;
	int total_inodes;
	int total_blocks;
	int blocks_per_group;
	int inodes_per_block_group;
	uint16_t inode_size;
	uint32_t block0;
} superblock ;

struct dirent {
	uint32_t inode;
	uint16_t size;
	uint16_t  _padding;    /*** Ignoring these fields as they are just optimisations, and depend of feature flags. ***/
	char     name[256];
} dirent;

static void parse_superblock();
void ext2_shuffle_hi();

int fs_init() {

	disk = open_disk(_root_disk);
	partition = open_partition(&disk, _root_part);

    int magic = 0;

    partition_read(&partition, EXT2_SUPERBLOCK_OFFSET + EXT2_SB_SIG_OFFSET, EXT2_SB_SIG_SIZE, &magic);

    if(magic != EXT2_SB_SIG_VALUE)
        halt("cannot find ext2 formatted boot partition");

    parse_superblock();

    ext2_shuffle_hi();

    return 0; // SUCCESS
}

/********************************************************************************
 * parse_superblock
 *   read needed fields from superblock
 */
void parse_superblock() {

	uint32_t version_major = 0;

	partition_read(&partition,EXT2_SUPERBLOCK_OFFSET + EXT2_SB_MAJVER_OFFSET, EXT2_SB_MAJVER_SIZE, &version_major);
	partition_read(&partition,EXT2_SUPERBLOCK_OFFSET + EXT2_SB_INODES_OFFSET, EXT2_SB_INODES_SIZE, &superblock.total_inodes);
	partition_read(&partition,EXT2_SUPERBLOCK_OFFSET + EXT2_SB_BLOCKS_OFFSET, EXT2_SB_BLOCKS_SIZE, &superblock.total_blocks);
	partition_read(&partition,EXT2_SUPERBLOCK_OFFSET + EXT2_SB_BPG_OFFSET,    EXT2_SB_BPG_SIZE,    &superblock.blocks_per_group);
	partition_read(&partition,EXT2_SUPERBLOCK_OFFSET + EXT2_SB_IPG_OFFSET,    EXT2_SB_IPG_SIZE,    &superblock.inodes_per_block_group);
	partition_read(&partition,EXT2_SUPERBLOCK_OFFSET + EXT2_SB_BLOCK0_OFFSET, EXT2_SB_BLOCK0_SIZE, &superblock.block0 );

	partition_read(&partition,EXT2_SUPERBLOCK_OFFSET + EXT2_SB_BS_OFFSET,     EXT2_SB_BS_SIZE,     &superblock.block_size );
	superblock.block_size = 1024 << superblock.block_size;

	superblock.inode_size = 128; // fixed in version < 1.0
	if(version_major >= 1)
		partition_read(&partition,EXT2_SUPERBLOCK_OFFSET + EXT2_SB_INODE_SIZE_OFFSET, EXT2_SB_INODE_SIZE_SIZE, &superblock.inode_size );

}

/*** read an ext2 block ***/
void read_block(uint32_t block, void* dst) {

	partition_read(&partition, block * superblock.block_size, superblock.block_size, dst );
}

/*** find the blockgroup containing the given inode ***/
uint32_t inode_to_blockgroup(uint32_t inode) {

	return (inode-1) / superblock.inodes_per_block_group;
}

/*** find inode table block address from blockgroup ***/
uint32_t blockgroup_to_inode_table(uint32_t blockgroup) {

	int ret = 0;
	int gd_addr = (superblock.block0 + 1) * superblock.block_size + blockgroup * 32;

    partition_read(&partition, gd_addr + EXT2_GD_INO_TBL_OFFSET, EXT2_GD_INO_TBL_SIZE, &ret);

	return ret;
}

/*** find an inodes index within a blockgroup ***/
uint32_t inode_to_blockgroup_index(uint32_t inode) {

	return (inode-1) % superblock.inodes_per_block_group;
}

/*** get an inodes on disk address (partition relative) ***/
uint64_t get_inode_phy_addr64(uint32_t inode) {

	uint64_t block  = blockgroup_to_inode_table(  inode_to_blockgroup(inode) );
	uint64_t inodeidx = inode_to_blockgroup_index(inode);
	uint64_t inodesize = superblock.inode_size;
	uint64_t blocksize = superblock.block_size;

	return block * blocksize + inodeidx * inodesize;
}

void ext2_read_block(uint32_t block, uint16_t offset, uint16_t size, void *dst) {

	uint64_t addr64  = block;
	         addr64 *= superblock.block_size;
	         addr64 += offset;

	partition_read(&partition, addr64, size, dst);

	return;
}

uint32_t ext2_read_block_32(uint32_t block, uint16_t offset) {

	uint32_t ret = 0;

	ext2_read_block(block, offset, sizeof ret, &ret);

	return ret;
}

uint32_t ext2_read_phy_32(uint64_t addr64) {

	uint32_t ret = 0;

	partition_read(&partition, addr64, sizeof ret, &ret);

	return ret;
}

uint16_t ext2_read_phy_16(uint64_t addr64) {

	uint16_t ret = 0;

	partition_read(&partition, addr64, sizeof ret, &ret);

	return ret;
}

uint32_t ext2_filesize(uint32_t inode) {

	return ext2_read_phy_32(get_inode_phy_addr64(inode) + EXT2_IN_SIZE_OFFSET);
}

uint32_t ext2_isdir(uint32_t inode) {

	return ext2_read_phy_16(get_inode_phy_addr64(inode) + EXT2_IN_MODE_OFFSET) & S_IFDIR;
}

uint32_t ext2_isreg(uint32_t inode) {

	return ext2_read_phy_16(get_inode_phy_addr64(inode) + EXT2_IN_MODE_OFFSET) & S_IFREG;
}

uint32_t ext2_issym(uint32_t inode) {

	return ext2_read_phy_16(get_inode_phy_addr64(inode) + EXT2_IN_MODE_OFFSET) & S_IFSYM;
}

void read_inode_block(uint32_t inode, uint32_t block, uint32_t offset, uint16_t size, void* dst) {

	uint64_t direct0 = 12;
	uint64_t indirect1 = (superblock.block_size/4);
	uint64_t indirect2 = indirect1 * indirect1;
	uint64_t indirect3 = indirect1 * indirect2;
	uint64_t phy = get_inode_phy_addr64(inode);

	if(block < direct0) {

		uint32_t blk = ext2_read_phy_32(phy + EXT2_IN_DIRECT_PTR + block * 4);

		partition_read(&partition, (uint64_t)blk * superblock.block_size + offset, size, dst);

		return;
	}

	block -= 12;

	if(block < indirect1) {

		uint32_t blk = ext2_read_phy_32  (phy + EXT2_IN_INDIRECT_PTR);
		         blk = ext2_read_block_32(blk, block * 4);

		partition_read(&partition, (uint64_t)blk * superblock.block_size + offset, size, dst );

		return;
	}
	block -= indirect1;


	if(block < indirect2) {

		uint32_t blk = ext2_read_phy_32  (phy + EXT2_IN_INDIRECT2_PTR);
		         blk = ext2_read_block_32(blk, (block / indirect1) * 4); block %= indirect1;
		         blk = ext2_read_block_32(blk, (block            ) * 4);

		partition_read(&partition, (uint64_t)blk * superblock.block_size + offset, size, dst );

		return;
	}
	block -= indirect2;

	if(block < indirect3) {

		uint32_t blk = ext2_read_phy_32  (phy + EXT2_IN_INDIRECT2_PTR);
				 blk = ext2_read_block_32(blk, (block / indirect3) * 4);  block %= indirect3;
				 blk = ext2_read_block_32(blk, (block / indirect2) * 4);  block %= indirect2;
				 blk = ext2_read_block_32(blk, (block            ) * 4);

		partition_read(&partition, (uint64_t)blk * superblock.block_size + offset, size, dst );

		return;
	}
	halt("read_inode_block - overflow");
}

/*** FIXME - size limited to superblock.block_size ***/
void read_inode(uint32_t inode, uint32_t offset, uint16_t size, void* dst) {

	uint32_t block = offset / superblock.block_size;
	uint32_t off   = offset % superblock.block_size;

	if(size > superblock.block_size)
		halt("FIXME: read_inode");

	if((off + size) > superblock.block_size) {
		read_inode_block(inode,block,off,superblock.block_size - off,dst);
		read_inode_block(inode,block+1,0,size-(superblock.block_size - off),dst);
	}
	else {
		read_inode_block(inode,block,off,size,dst);
	}
}

uint32_t ext2_find_kernel() {

	uint32_t offset=0;
	uint32_t size = ext2_filesize(2);

	while(offset < size) {

		read_inode(2,offset,sizeof dirent, &dirent);

		if(strcmp("shovelos.kernel", dirent.name) == 0) {

			if(!ext2_isreg(dirent.inode))
				halt("shovelos.kernel is not a regular file!");

			return dirent.inode;
		}

		offset+=dirent.size;
	}

	halt("cannot find /shovelos.kernel");
}

#if(TODO)
sint32_t lstat(const char *path, struct stat *stat) {

	stat->st_ino  = EXT2_ROOT_INODE;
	stat->st_size = ext2_filesize(stat->st_ino);

	{
		// copy the section of filename we are looking for into the DISK_BUFFER
		++path; // skip '/'
		int i;
		for(i=0; *path && (*path != '/'); ++i) {
			memset(DISK_BUFFER+i, path++, 1 );
		}
		memset(DISK_BUFFER+i, 0, 1 );
	}

	{
		// search for name
		uint32_t offset = 0;
		while(offset < stat->st_size) {

			read_inode(stat->st_ino,offsset,sizeof dirent, &dirent);

			if(strcmp(dirent.name, DISK_BUFFER) == 0) {



			}
		}
	}
}
#endif





void shuffle_high();

void ext2_shuffle_hi() {

	uint32_t inode = ext2_find_kernel();
	uint32_t ksize = ext2_filesize(inode);
	uint32_t off   = 0;
	uint64_t dst   = 0xFFFFFFFF80000000;
	uint16_t thisread = 0;
	uint64_t shuffle_params[4];

	/*** setup page tables, identity map lower memory,
	     and allocate enough hi-memory for kernel ***/

	setup_pt( ksize );

	while(ksize > 0) {

		thisread = ksize > superblock.block_size ? superblock.block_size : (uint16_t)ksize;

		read_inode(inode,off,thisread, DISK_BUFFER + disk.sector_bytes);

		/*** soooooo ugly! ***/
		memset(shuffle_params,0,sizeof shuffle_params);
		shuffle_params[0] = dst;
		shuffle_params[1] = (uint32_t)(DISK_BUFFER + disk.sector_bytes);
		shuffle_params[2] = thisread;
		shuffle_params[3] = 0;

		memcpy(ADHOC_COMM, shuffle_params, sizeof shuffle_params);

		shuffle_high();

		dst   += thisread;
		ksize -= thisread;
		off   += thisread;
	}
}


