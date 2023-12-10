#include "print.h"

// Check if the compiler thinks we are targeting the wrong OS
#if defined(__linux__)
#error "Not using cross compiler!"
#endif

// Only works for 32 bit ix86 target
#if !defined(__i386__)
#error "Must use ix86-elf compiler"
#endif

void kernel_main(void) {
    print_clear();
    for (int r = 0; r < 24; r++) {
        for (int c = 0; c < 80; c += 3) {
            int color = (r + c) % 15 + 1;
            print_set_color(color, PRINT_COLOR_BLACK);
            printf(":3 ");
        }
        printf("%n");
    }
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    printf("printf\n\tint: %d\n\tstring: %s\n\tchar: %c\n\tpercent: "
           "%%\n\tnothing: %n",
           99, "World! ", 't');

    // printf("\n0x%d\n", *((int *)(0x0010)));
}
