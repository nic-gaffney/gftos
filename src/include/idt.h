#pragma once
#include <stdint.h>

typedef struct Gate_Descriptor {
    uint16_t offset_low;
    uint16_t segment_selector;
    uint8_t reserved;
    uint8_t attrs;
    uint16_t offset_high;
} __attribute__((packed)) Gate_Descriptor_t;

void make_gate(uint32_t offset, uint8_t attrs, uint8_t vec);

void get_idtr();
