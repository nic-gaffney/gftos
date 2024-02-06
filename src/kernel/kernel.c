#include "kernel.h"
#include "print.h"

#if defined(__linux__)
#error "Not using cross compiler!"
#endif

// Only works for 32 bit ix86 target
#if !defined(__i386__)
#error "Must use ix86-elf compiler"
#endif

void motd() {
    char *empty_row = "                                                        "
                      "                        \n";
    empty_row[0] = (char)186;
    empty_row[79] = (char)186;

    printf("%c", 201);
    for (int i = 0; i < 78; i++)
        printf("%c", 205);
    printf("%c\n", 187);

    printf(empty_row);

    printf("%c", 186);
    for (int i = 0; i < 78 / 2 - 17 / 2; i++)
        printf(" ");
    printf("Welcome to gftos!");
    for (int i = 0; i < 78 / 2 - 17 / 2 - 1; i++)
        printf(" ");
    printf("%c\n", 186);

    for (int i = 0; i < 21; i++) {
        printf(empty_row);
    }

    printf("%c", 200);
    for (int i = 0; i < 78; i++)
        printf("%c", 205);
    printf("%c", 188);
}

void kernel_panic(const char *error) {
    print_set_color(PRINT_COLOR_BLACK, PRINT_COLOR_RED);
    print_clear();
    printf(" PANIC! ERROR: %s", error);
    for (;;)
        ;
}

void kernel_main(void) {
    print_clear();
    motd();
    /* kernel_panic("test_panic."); */
    // for (int r = 0; r < 24; r++) {
    //     for (int c = 0; c < 80; c += 3) {
    //         int color = (r + c) % 15 + 1;
    //         print_set_color(color, PRINT_COLOR_BLACK);
    //         printf(":3");
    //     }
    //     printf("%n");
    // }
    // print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    // printf("printf\n\tint: %d\n\tstring: %s\n\tchar: %c\n\tpercent: "
    //        "%%\n\tnothing: %n",
    //        99, "World! ", 't');

    // printf("\n0x%d\n", *((int *)(0x0010)));
}
