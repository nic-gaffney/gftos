#include "print.h"

__attribute__((noreturn)) void exception_handler(void);
void exception_handler() {
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    print_clear();
    printf("KERNEL EXCEPTION");
    __asm__ volatile("cli; hlt");
}
