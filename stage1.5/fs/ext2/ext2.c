/*
 * ext2.c
 *
 *  Created on: Nov 6, 2010
 *      Author: cds
 */

#include "../../16bitreal.h"
#include "../../inttypes.h"
#include "../../bios_disk.h"
#include "../../mem.h"
#include "../../print.h"
#include "ext2.h"

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
static void parse_superblock() {

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
static void read_block(uint32_t block, void* dst) {

	partition_read(&partition, block * superblock.block_size, superblock.block_size, dst );
}

/*** find the blockgroup containing the given inode ***/
static uint32_t inode_to_blockgroup(uint32_t inode) {

	return (inode-1) / superblock.inodes_per_block_group;
}

/*** find inode table block address from blockgroup ***/
static uint32_t blockgroup_to_inode_table(uint32_t blockgroup) {

	int ret = 0;
	int gd_addr = (superblock.block0 + 1) * superblock.block_size + blockgroup * 32;

    partition_read(&partition, gd_addr + EXT2_GD_INO_TBL_OFFSET, EXT2_GD_INO_TBL_SIZE, &ret);

	return ret;
}

/*** find an inodes index within a blockgroup ***/
static uint32_t inode_to_blockgroup_index(uint32_t inode) {

	return (inode-1) % superblock.inodes_per_block_group;
}

/*** get an inodes on disk address (partition relative) ***/
static uint64_t get_inode_phy_addr64(uint32_t inode) {

	uint64_t block  = blockgroup_to_inode_table(  inode_to_blockgroup(inode) );
	uint64_t inodeidx = inode_to_blockgroup_index(inode);
	uint64_t inodesize = superblock.inode_size;
	uint64_t blocksize = superblock.block_size;

	return block * blocksize + inodeidx * inodesize;
}

static void ext2_read_block(uint32_t block, uint16_t offset, uint16_t size, void *dst) {

	uint64_t addr64  = block;
	         addr64 *= superblock.block_size;
	         addr64 += offset;

	partition_read(&partition, addr64, size, dst);

	return;
}

static uint32_t ext2_read_block_32(uint32_t block, uint16_t offset) {

	uint32_t ret = 0;

	ext2_read_block(block, offset, sizeof ret, &ret);

	return ret;
}

static uint32_t ext2_read_phy_32(uint64_t addr64) {

	uint32_t ret = 0;

	partition_read(&partition, addr64, sizeof ret, &ret);

	return ret;
}

static uint16_t ext2_read_phy_16(uint64_t addr64) {

	uint16_t ret = 0;

	partition_read(&partition, addr64, sizeof ret, &ret);

	return ret;
}

static uint32_t ext2_filesize(uint32_t inode) {

	return ext2_read_phy_32(get_inode_phy_addr64(inode) + EXT2_IN_SIZE_OFFSET);
}

static uint32_t ext2_isdir(uint32_t inode) {

	return ext2_read_phy_16(get_inode_phy_addr64(inode) + EXT2_IN_MODE_OFFSET) & S_IFDIR;
}

static uint32_t ext2_isreg(uint32_t inode) {

	return ext2_read_phy_16(get_inode_phy_addr64(inode) + EXT2_IN_MODE_OFFSET) & S_IFREG;
}

static uint32_t ext2_issym(uint32_t inode) {

	return ext2_read_phy_16(get_inode_phy_addr64(inode) + EXT2_IN_MODE_OFFSET) & S_IFSYM;
}

static void read_inode_block(uint32_t inode, uint32_t block, uint32_t offset, uint16_t size, void* dst) {

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
static void read_inode(uint32_t inode, uint32_t offset, uint16_t size, void* dst) {

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

static uint32_t ext2_find_kernel() {

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

void shuffle_high();

void ext2_shuffle_hi() {

	uint32_t inode = ext2_find_kernel();
	uint32_t ksize = ext2_filesize(inode);
	uint32_t off   = 0;
	uint64_t dst   = 0xFFFF800000000000;
	uint16_t thisread = 0;
	uint64_t temp     = 0;
	uint64_t shuffle_params[4];

	while(ksize > 0) {

		thisread = ksize > superblock.block_size ? superblock.block_size : (uint16_t)ksize;

		read_inode(inode,off,thisread, DISK_BUFFER + disk.sector_bytes);

		/*** soooooo ugly! ***/
		memset(shuffle_params,0,sizeof shuffle_params);
		shuffle_params[0] = dst;
		shuffle_params[1] = (uint64_t)(DISK_BUFFER + disk.sector_bytes);
		shuffle_params[2] = thisread;
		shuffle_params[3] = 0;

		memcpy(ADHOC_COMM, shuffle_params, sizeof shuffle_params);

		shuffle_high();

		dst   += thisread;
		ksize -= thisread;
		off   += thisread;
	}
}


