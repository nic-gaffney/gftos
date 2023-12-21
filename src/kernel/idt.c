#include "idt.h"
#include "gdt.h"
#include <stdint.h>

IDT_t idt = (IDT_t)0x7fffe;
extern GDTR_t idtr;

void get_idtr() {
    int base = 0x7fffe;
    uint8_t code_seg = (0b1000 << 3) | 0b000;

    // idtr.size = sizeof(Gate_Descriptor_t) * n - 1;
    idtr.size = 0;
    idtr.offset = base;
}

Gate_Descriptor_t make_gate(uint32_t offset, uint8_t segment_selector,
                            uint8_t type) {
    Gate_Descriptor_t ret = (Gate_Descriptor_t){
        .offset_low = offset,
        .reserved = 0,
        .attrs = (0b1000 << 4) | type,
        .offset_high = offset >> 16,
        .segment_selector = segment_selector,
    };

    return ret;
}
