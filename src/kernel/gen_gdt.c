#include "gdt.h"

GDT_t *table = 0x0;
GDTR_t gdtr = {0, 0};
void get_gdtr() {
    *table = (GDT_t){make_descriptor(0, 0, 0),
                     make_descriptor(0x0000, 0xFFFFF, make_code(0, 0, 1)),
                     make_descriptor(0x0000, 0xFFFFF, make_data(0, 0, 1))};
    gdtr.size = sizeof(Segment_Descriptor_t) * 3 - 1;
    gdtr.offset = 0x0;
}
