#include "asm.h"
#include <stdint.h>
#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

void PIC_init(int offset1, int offset2) {
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, 0x11); // Init PIC1
    io_wait();
    outb(PIC2_COMMAND, 0x11); // Init PIC2
    io_wait();
    outb(PIC1_DATA, offset1); // ICW2: Master PIC vector offset
    io_wait();
    outb(PIC2_DATA, offset2); // ICW2: Slave PIC vector offset
    io_wait();
    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();
    outb(PIC1_DATA, 0x01);
    io_wait();
    outb(PIC2_DATA, 0x01);
    io_wait();

    outb(PIC1_DATA, a1); // restore saved masks.
    outb(PIC2_DATA, a2);
}

void IRQ_mask(uint8_t line) {
    uint16_t port = line < 8 ? PIC1_DATA : PIC2_DATA;
    line = line > 8 ? line - 8 : line;
    uint8_t val = inb(port) | (1 << line);
    outb(port, val);
}

void IRQ_unmask(uint8_t line) {
    uint16_t port = line < 8 ? PIC1_DATA : PIC2_DATA;
    line = line > 8 ? line - 8 : line;
    uint8_t val = inb(port) & ~(1 << line);
    outb(port, val);
}

void PIC_disable(void) {
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

uint16_t PIC_irr(void) {
    outb(PIC1_COMMAND, 0x0a);
    outb(PIC2_COMMAND, 0x0a);
    return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}

uint16_t PIC_isr(void) {
    outb(PIC1_COMMAND, 0x0b);
    outb(PIC2_COMMAND, 0x0b);
    return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}
