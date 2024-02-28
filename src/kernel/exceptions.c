#include "print.h"
#include <stdarg.h>

__attribute__((noreturn)) void panic(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    print_set_color(PRINT_COLOR_BLACK, PRINT_COLOR_RED);
    print_clear();
    printf("PANIC!!\n\t");
    printf_logic(msg, args);
    va_end(args);
    __asm__ volatile("cli; hlt");
    while (1)
        ;
}

__attribute__((noreturn)) void exception_handler(void);
void exception_handler() { panic("KERNEL EXCEPTION"); }
