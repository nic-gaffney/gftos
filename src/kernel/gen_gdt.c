#include "gdt.h"
#include <stdint.h>

GDT_t *table = 0x0;
extern GDTR_t gdtr;
void get_gdtr() {
    uint32_t base = 0x0000;

    *table = (GDT_t){
        // NULL
        make_descriptor(0, 0, 0),
        // Code segment, read
        make_descriptor(base, 0x8FFFF, make_code(0, 0, 1)),
        // Data segment, read write
        make_descriptor(base, 0x8FFFF, make_data(0, 0, 1)),
    };
    gdtr.size = sizeof(Segment_Descriptor_t) * 3 - 1;
    gdtr.offset = base;
}
