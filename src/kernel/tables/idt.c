#include "idt.h"
#include "gdt.h"
#include "isr.h"
#include <stdint.h>

__attribute__((aligned(0x10))) static Gate_Descriptor_t idt[256];
extern GDTR_t idtr;
extern void *isr_stub_table;

void get_idtr() {
    idtr.offset = (uintptr_t)&idt[0];
    idtr.size = sizeof(Gate_Descriptor_t) * 256 - 1;
    for (uint8_t vec = 0; vec < 32; vec++) {
        make_gate(((void **)(isr_stub_table))[vec], 0x8E, vec);
    }
    make_gate(wrap_keyboard_handler, 0x8E, 0x21);
}

void make_gate(void *isr, uint8_t attrs, uint8_t vec) {
    Gate_Descriptor_t *descriptor = &idt[vec];

    descriptor->offset_low = (uint32_t)isr & 0xFFFF;
    descriptor->attrs = (0b1000 << 4) | attrs;
    descriptor->offset_high = (uint32_t)isr >> 16;
    descriptor->segment_selector = 0x08;
    descriptor->reserved = 0;
}
