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
#define S_IFREG	8000	/* regular file */
#define S_IFDIR	4000	/* directory */

/****************************************************************************************/
/********************** SUPERBLOCK FIELD OFFSETS / SIZES ********************************/
/****************************************************************************************/

#define EXT2_SUPERBLOCK_OFFSET 			1024
#define EXT2_SUPERBLOCK_SIZE   			1024
#define EXT2_SB_BLOCKS_OFFSET	    	   0		/*** TOTAL INODES ***/
#define EXT2_SB_BLOCKS_SIZE	     		   4
#define EXT2_SB_INODES_OFFSET	    	   4		/*** TOTAL BLOCKS ***/
#define EXT2_SB_INODES_SIZE	     		   4
#define EXT2_SB_SIG_OFFSET	    		  56		/*** SUPERBLOCK MAGIC ***/
#define EXT2_SB_SIG_SIZE	     		   2
#define EXT2_SB_SIG_VALUE			  0xEF53
#define EXT2_SB_FST_DATA_OFFSET 		  20		/*** SUPERBLOCK FIRST DATA BLOCK ***/
#define EXT2_SB_FST_DATA_SIZE    		   4
#define EXT2_SB_BS_OFFSET       		  24		/*** SUPERBLOCK BLOCK SIZE ***/
#define EXT2_SB_BS_SIZE          		   4
#define EXT2_SB_BPG_OFFSET      		  32		/*** SUPERBLOCK BLOCKS PER GROUP ***/
#define EXT2_SB_BPG_SIZE         		   4
#define EXT2_SB_IPG_OFFSET      		  40		/*** SUPERBLOCK INODES PER GROUP ***/
#define EXT2_SB_IPG_SIZE         		   4
#define EXT2_SB_FST_INO_OFFSET 			  84		/*** SUPERBLOCK FIRST INODE ***/
#define EXT2_SB_FST_INO_SIZE     		   4

/****************************************************************************************/
/******************* GROUP DESCRIPTOR FIELD OFFSETS / SIZES *****************************/
/****************************************************************************************/

#define EXT2_GD_BLK_BMP_OFFSET			   0		/*** GROUP-DESCRIPTOR BLOCK BITMAP ***/
#define EXT2_GD_BLK_BMP_SIZE			   4
#define EXT2_GD_INO_BMP_OFFSET			   4		/*** GROUP-DESCRIPTOR I-NODE BITMAP ***/
#define EXT2_GD_INO_BMP_SIZE			   4
#define EXT2_GD_INO_TBL_OFFSET			   8		/*** GROUP-DESCRIPTOR I-NODE TABLE ***/
#define EXT2_GD_INO_TBL_SIZE			   4
#define EXT2_GD_UDIRS_OFFSET			  16		/*** USED DIRS COUNT ***/
#define EXT2_GD_UDIRS_SIZE			  	   2

/****************************************************************************************/
/************************ INODE FIELD OFFSETS / SIZES ***********************************/
/****************************************************************************************/

#define EXT2_IN_MODE_OFFSET				   0		/*** INODE MODE ***/
#define EXT2_IN_MODE_SIZE				   2
#define EXT2_IN_SIZE_OFFSET				   4		/*** FILE SIZE IN BYTES ***/
#define EXT2_IN_SIZE_SIZE				   4
#define EXT2_IN_BLOCKS_OFFSET			  28		/*** BLOCKS COUNT ***/
#define EXT2_IN_BLOCKS_SIZE				   4
#define EXT2_IN_BPTR_OFFSET				  36		/*** POINTERS TO BLOCKS ( ARRAY OF 15 ) ***/
#define EXT2_IN_BPTR_SIZE				   4
#define EXT2_IN_BPTR_ALEN				  15

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

