/*
 * ext2.h
 *
 *  Created on: Nov 6, 2010
 *      Author: cds
 */

#ifndef __STAGE_1_5_FS_EXT2_EXT2_H
#define __STAGE_1_5_FS_EXT2_EXT2_H

int fs_init();

/*** the only two inode mode fields we are interested in ***/
#define S_IFREG	0x8000	/* regular file */
#define S_IFDIR	0x4000	/* directory */
#define S_IFSYM 0xA000  /* symbolic link */

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

#endif /* __STAGE_1_5_FS_EXT2_EXT2_H */

