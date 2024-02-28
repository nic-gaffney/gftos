#include "paging.h"
#include "exceptions.h"
#include <stdint.h>
extern uint32_t endkernel;

Page_Directory make_pagedir(Page_Table *addr, uint8_t flags) {
    return ((uint32_t)addr & 0xFFFFF000) | flags;
}

Page_Table make_pagetable(uint32_t addr, uint8_t flags) {
    return ((uint32_t)addr & 0xFFFFF000) | flags;
}

Page_Directory page_dir[1024] __attribute__((aligned(4096)));
Page_Table pgtable[1024] __attribute__((aligned(4096)));

void idpaging(uint32_t *first_pte, uint32_t from, int size) {
    for (; size > 0; from += 4096, size -= 4096, first_pte++) {
        *first_pte = make_pagetable(from, 3); // mark page present.
    }
}

void genDirs() {
    for (int i = 0; i < 1024; i++) {
        page_dir[i] = make_pagedir(0x0, 2);
        for (int j = 0; j < 1024; j++)
            ((uint32_t *)(page_dir[i] & 0xFFFFF000))[j] &= 0xFFFFFFFE;
    }

    page_dir[1023] = make_pagetable((uint32_t)page_dir, 3);

    idpaging(&pgtable[0], 0x0, 1024 * 0x800);
    page_dir[0] = make_pagedir(pgtable, 3);
}

void *get_physaddr(void *virtualaddr) {
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;

    unsigned long *pd = (unsigned long *)page_dir;
    if (!(pd[pdindex] & 1))
        panic("Unimplemented - getphysaddr - pd[%d]", pdindex);

    unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);
    if (!(pt[ptindex] & 1))
        panic("Unimplemented - getphysaddr - pt[%d]", ptindex);

    return (void *)((pt[ptindex] & ~0xFFF) +
                    ((unsigned long)virtualaddr & 0xFFF));
}

void map_page(void *physaddr, void *virtualaddr, uint8_t flags) {
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;

    unsigned long *pd = (unsigned long *)page_dir;
    if (!(pd[pdindex] & 1)) {
        pd[pdindex] =
            make_pagedir((uint32_t *)0xFFC00000 + (0x400 * pdindex), 3);
    }

    unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);
    if (pt[ptindex] & 1)
        panic("Unimplemented - map_page - pt[%d]", ptindex);

    pt[ptindex] = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01; // Present
}
