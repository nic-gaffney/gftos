#pragma once
#include <stdint.h>
#define FLAG 0b1100

typedef struct Segment_Descriptor {
    uint16_t lim_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t lim_flag;
    uint8_t base_high;
} __attribute__((packed)) Segment_Descriptor_t;

typedef struct GDTR {
    uint16_t size;
    uint32_t offset;
} __attribute__((packed)) GDTR_t;

typedef struct GDT {
    Segment_Descriptor_t nulldesc;
    Segment_Descriptor_t codedesc;
    Segment_Descriptor_t datadesc;
} __attribute__((packed)) GDT_t;

uint16_t make_code(uint8_t priv, uint8_t dc, uint8_t rw);
uint16_t make_data(uint8_t priv, uint8_t dc, uint8_t rw);

Segment_Descriptor_t make_descriptor(uint32_t base, uint32_t limit,
                                     uint16_t access_flag);
void get_gdtr();
