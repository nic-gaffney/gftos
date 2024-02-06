#pragma once
#include <stdint.h>
void PIC_init(int offset1, int offset2);
void PIC_disable(void);
uint16_t PIC_irr(void);
uint16_t PIC_isr(void);
void PIC_send(uint8_t irq, uint16_t cmd);
void IRQ_mask(uint8_t line);
void IRQ_unmask(uint8_t line);
void wrap_keyboard_handler();
