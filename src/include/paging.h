#ifndef __PAGING_H
#define __PAGING_H
#include <stdint.h>
typedef uint32_t Page_Directory;
typedef uint32_t Page_Table;

Page_Directory make_pagedir(Page_Table *addr, uint8_t flags);
Page_Directory make_pagetable(uint32_t addr, uint8_t flags);
void *get_physaddr(void *virtualaddr);
void map_page(void *physaddr, void *virtualaddr, uint8_t flags);
void genDirs();
void *kalloc_init();
#endif // !__PAGING_H
