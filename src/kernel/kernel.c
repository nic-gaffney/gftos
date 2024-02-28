#include "exceptions.h"
#include "gdt.h"
#include "isr.h"
#include "paging.h"
#include "print.h"
#if defined(__linux__)
#error "Not using cross compiler!"
#endif

// Only works for 32 bit ix86 target
#if !defined(__i386__)
#error "Must use ix86-elf compiler"
#endif

void motd() { printf("Welcome to gftos!\n=================\n"); }
extern Page_Table pgtable[1024];
extern Page_Directory page_dir[1024];
extern GDT_t table;

void kernel_main(void) {
    PIC_init(0x20, 0xA0);
    IRQ_mask(0);

    print_set_color(PRINT_COLOR_PINK, PRINT_COLOR_DARK_GRAY);
    print_clear();
    motd();
    printf("Page Directory: %x\n", (Page_Directory *)page_dir[1023]);
    printf("Page Table: %x\n", (Page_Table *)pgtable);
    printf("GDT: %x\n", table);
    for (;;) {
        update_cursor();
        asm("hlt");
    }

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
