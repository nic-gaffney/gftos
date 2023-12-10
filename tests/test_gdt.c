#include "gdt.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
int main() {

    uint16_t code_info = make_code(0, 0, 1);
    uint16_t data_info = make_data(0, 0, 1);
    Segment_Descriptor_t null = make_descriptor(0, 0, 0);
    Segment_Descriptor_t code = make_descriptor(0, 0xFFFFF, code_info);
    Segment_Descriptor_t data = make_descriptor(0, 0xFFFFF, data_info);
    GDT_t *table = malloc(sizeof(GDT_t));
    *table = (GDT_t){null, code, data};
    GDTR_t gdtr = {(table + sizeof(GDT_t) - table - 1), (uint32_t)table};

    printf("%zu\n", sizeof(GDT_t));
    printf("0x%.16llX\n", null);
    printf("0x%.16llX\n", code);
    printf("0x%.16llX\n", data);
    printf("0x%.24llX\n", table);
}
