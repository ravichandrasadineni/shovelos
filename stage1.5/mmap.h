#ifndef __MMAP_H
#define __MMAP_H

extern short * _heap_start;

struct mmap_e820h_reg {
  
  struct {
    int lsi;
    int msi;
  } base;
  
  struct {
    int lsi;
    int msi;
  } length;
  
  int type;
  int extended;
};

struct mmap_e820h {
  
  short size;
  struct mmap_e820h_reg *map;
};

#endif /*** __MMAP_H ***/

