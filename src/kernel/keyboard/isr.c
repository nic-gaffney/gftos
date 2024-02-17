#include "asm.h"
#include "exceptions.h"
#include "print.h"
#include "str.h"
#include <stdint.h>
static uint8_t shift_held = 0;
static uint8_t capslock = 0;

void keyboard_handler() {
    const char *scancodes =
        "??1234567890-=?\tqwertyuiop[]\n?asdfghjkl;'`?\\zxcvbnm,./?*? ";
    const char *SCANCODES =
        "\?\?!@#$%^&*()_+?\tQWERTYUIOP{}\n?ASDFGHJKL:\"~?|ZXCVBNM<>??*? ";

    unsigned char sc = inb(0x60);

    uint8_t isheld = shift_held != 0;
    char c = (isheld ^ capslock) ? SCANCODES[sc] : scancodes[sc];

    if (capslock & (!isheld) && !isalpha(c))
        c = scancodes[sc];
    if (isheld && !isalpha(c))
        c = SCANCODES[sc];

    if (sc < 0x59 && scancodes[sc] != '?')
        printf("%c", c);

    switch (sc) {
    case 0x2A:
        shift_held |= 0b10;
        break;
    case 0x36:
        shift_held |= 0b01;
        break;
    case 0xAA:
        shift_held &= 0b01;
        break;
    case 0xB6:
        shift_held &= 0b10;
        break;
    case 0x0E:
        delete_char();
        break;
    case 0x3A:
        capslock = !capslock;
        break;
    }

    if (shift_held && sc == 0xE0)
        panic("Test panic");

    outb(0x20, 0x20);
}
