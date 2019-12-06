// On-disk file system format.
// Both the kernel and user programs use this header file.

#ifndef __FS_H
# define __FS_H

#ifndef FALSE
# define FALSE 0
#endif // FALSE
#ifndef TRUE
# define TRUE 1
#endif // TRUE

#define ROOTINO 1  // root i-number
#define BSIZE 512  // block size

// Disk layout:
// [ boot block | super block | log | inode blocks |
//                                          free bit map | data blocks]
//
// mkfs computes the super block and builds an initial file system. The
// super block describes the disk layout:
struct superblock {
  uint size;         // Size of file system image (blocks)
  uint nblocks;      // Number of data blocks
  uint ninodes;      // Number of inodes.
  uint nlog;         // Number of log blocks
  uint logstart;     // Block number of first log block
  uint inodestart;   // Block number of first inode block
  uint bmapstart;    // Block number of first free map block
};

#ifdef BIGFILE
//# error need to change the NDIRECT

// The following macro is used in a couple places in the fs.[ch] code
//   as well as in the file.h file and mkfs.c file.
// Instead of working around all those uses, we will just make it 1
//   smaller and then adapt when we use the double indirect block
//   allocation.
# define NDIRECT 11
#else // BIGFILE
# define NDIRECT 12
#endif // BIGFILE

#define NINDIRECT (BSIZE / sizeof(uint))

#ifdef BIGFILE

//# error need to change and add a few things in here

// The index in the addrs data member in the dinode structure where
//   the double indirect disk pointer block is stored.
# define DIDIRECT (NDIRECT + 1)

// This is the number of data blocks that can be stored in using the
//   double indirect pointers.
# define NDINDIRECT (NINDIRECT * NINDIRECT)

// This is the total numb er of data blocks that can be used by a
//   single file.
//   11 (= direct) + 128 (= indirect) + (128 * 128) (= double indirect) = 16523
# define MAXFILE (NDIRECT + NINDIRECT + NDINDIRECT)

#else // BIGFILE

// this is the maximum number of blocks a file can use.
# define MAXFILE (NDIRECT + NINDIRECT)

#endif // BIGFILE

// On-disk inode structure
struct dinode {
  short type;           // File type
  short major;          // Major device number (T_DEV only)
  short minor;          // Minor device number (T_DEV only)
  short nlink;          // Number of links to inode in file system
  uint size;            // Size of file (bytes)
#ifdef BIGFILE
//# error need to change the addrs data member
    // the number of direct blocks is NDIRECT
    // the first +1 is for the indirect block
    // the second +1 is for the double indirect blocks
  uint addrs[NDIRECT + 1 + 1];   // Data block addresses
#else // BIGFILE
  uint addrs[NDIRECT + 1];   // Data block addresses
#endif // BIGFILE
};

// Inodes per block.
#define IPB           (BSIZE / sizeof(struct dinode))

// Block containing inode i
#define IBLOCK(i, sb)     ((i) / IPB + sb.inodestart)

// Bitmap bits per block
#define BPB           (BSIZE*8)

// Block of free map containing bit for block b
#define BBLOCK(b, sb) (b/BPB + sb.bmapstart)

// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 14

struct dirent {
  ushort inum;
  char name[DIRSIZ];
};

#endif // __FS_H
