#include "gdt.h"
#include <stdint.h>

GDT_t table = 0x0;
extern GDTR_t gdtr;
void get_gdtr() {
    uint32_t base = (uint32_t)table;

    table[0] = make_descriptor(base, 0, 0);
    table[1] = make_descriptor(base, 0x3FFFF, make_code(0, 0, 1));
    table[2] = make_descriptor(base, 0x3FFFF, make_data(0, 0, 1));

    gdtr.size = sizeof(Segment_Descriptor_t) * 3 - 1;
    gdtr.offset = base;
}

uint16_t make_code(uint8_t priv, uint8_t dc, uint8_t rw) {
    uint16_t access = 0;
    access |= (1 << 7);
    access |= (priv << 5);
    access |= (0b11 << 3);
    access |= (dc << 2);
    access |= (rw << 1); // 10
    access <<= 8;
    access |= FLAG;
    return access;
}

uint16_t make_data(uint8_t priv, uint8_t dc, uint8_t rw) {
    uint16_t access = 0;
    access |= (1 << 7);
    access |= (priv << 5);
    access |= (0b10 << 3);
    access |= (dc << 2);
    access |= (rw << 1); // 10
    access <<= 8;
    access |= FLAG;
    return access;
}

Segment_Descriptor_t make_descriptor(uint32_t base, uint32_t limit,
                                     uint16_t access_flag) {
    uint16_t lim_low = limit;
    uint16_t base_low = base;
    uint8_t base_mid = (base >> 16);
    uint8_t access_byte = access_flag >> 8;
    uint8_t lim_flag = ((access_flag << 4) | (limit >> 16));
    uint8_t base_high = (base >> 24);
    return (Segment_Descriptor_t){
        lim_low, base_low, base_mid, access_byte, lim_flag, base_high,
    };
}
