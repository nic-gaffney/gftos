#include "idt.h"
#include "gdt.h"
#include <stdint.h>

static Gate_Descriptor_t idt[256];
extern GDTR_t idtr;

void get_idtr() {
    idtr.offset = (uintptr_t)&idt[0];
    idtr.size = sizeof(Gate_Descriptor_t) * 256 - 1;
    for (uint8_t vec = 0; vec < 32; vec++) {
        make_gate(stub_table[vec], 0x8E, vec);
    }
}

void make_gate(uint32_t offset, uint8_t attrs, uint8_t vec) {
    Gate_Descriptor_t *descriptor = &idt[vec];

    descriptor->offset_low = offset & 0xFFFF;
    descriptor->attrs = (0b1000 << 4) | attrs;
    descriptor->offset_high = offset >> 16, descriptor->segment_selector = 0x08;
    descriptor->reserved = 0;
}
