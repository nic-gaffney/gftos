#include "print.h"
#include "asm.h"
#include "str.h"
#include <stdarg.h>
#include <stdint.h>
// Constants
static const size_t NUM_COLS = 80;
static const size_t NUM_ROWS = 25;

// Char struct
struct Char {
    uint8_t character;
    uint8_t color;
};

// Set video mem buffer
struct Char *buffer = (struct Char *)0xb8000;
// Set current row and col
size_t col = 0;
size_t row = 0;

// Default colors
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

// clear row
void clear_row(size_t row) {
    // Define empty character
    struct Char empty = (struct Char){
        ' ',
        color,
    };

    // For every col in row, set vmem to empty
    for (size_t col = 0; col < NUM_COLS; col++) {
        buffer[col + NUM_COLS * row] = empty;
    }
}

void delete_char() {
    if (col == 0) {
        row--;
        col = NUM_COLS;
    }
    col--;
    buffer[col + NUM_COLS * row] = (struct Char){
        ' ',
        color,
    };
}

// Call clear_row for every row
void print_clear() {
    for (size_t i = 0; i < NUM_ROWS; i++) {
        clear_row(i);
    }
    col = 0;
    row = 0;
}

// newline
void print_newline() {
    // Reset col and iterate row if not at bottom of screen
    col = 0;
    if (row < NUM_ROWS - 1) {
        row++;
        return;
    }

    // If at bottom of screen, move all the text up one row
    for (size_t row = 1; row < NUM_ROWS; row++) {
        for (size_t col = 0; col < NUM_COLS; col++) {
            struct Char character = buffer[col + NUM_COLS * row];
            buffer[col + NUM_COLS * (row - 1)] = character;
        }
    }

    // Clear
    clear_row(NUM_ROWS - 1);
}

void update_cursor() {
    uint16_t pos = col + NUM_COLS * row;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// printchar
void print_char(char character) {
    // If \n, call newline
    if (character == '\n') {
        print_newline();
        return;
    }

    if (character == '\t') {
        print_str("    \0");
        return;
    }

    // if cols is too large, overflow
    if (col >= NUM_COLS) {
        print_newline();
    }

    // set video memeory to requested character
    buffer[col + NUM_COLS * row] = (struct Char){
        (uint8_t)character,
        color,
    };

    // iterate col
    col++;
}

// print_str
int print_str(char *str) {
    // while character != '\0' print char
    size_t i;
    for (i = 0; str[i] != '\0'; ++i)
        print_char(str[i]);
    return i;
}

// Set color byte to requested colors using color enum from print.h
void print_set_color(uint8_t foreground, uint8_t background) {
    color = foreground + (background << 4);
}

int printf_logic(const char *str, va_list args) {
    int len = 0;
    char temp_str[256] = "";
    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] == '%') {
            switch (str[++i]) {
            case 'i':
            case 'd':
                len += print_str(itoa(va_arg(args, int), temp_str, 10));
                break;
            case 'x':
                len += print_str("0x");
                len += print_str(itoa(va_arg(args, int), temp_str, 16));
                break;
            case 's':
                len += print_str(va_arg(args, char *));
                break;
            case 'c':
                print_char(va_arg(args, int));
                len++;
                break;
            case '%':
                print_char('%');
                len++;
            case 'n':
                break;
            }
            continue;
        }
        print_char(str[i]);
    }
    return len;
}

int printf(const char *str, ...) {
    va_list args;
    va_start(args, str);
    int len = printf_logic(str, args);
    va_end(args);
    return len;
}
