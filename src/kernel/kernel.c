#include "print.h"

// Check if the compiler thinks we are targeting the wrong OS
#if defined(__linux__)
#error "Not using cross compiler!"
#endif

// Only works for 32 bit ix86 target
#if !defined(__i386__)
#error "Must use ix86-elf compiler"
#endif

// Color Constants

void kernel_main(void) {
  print_clear();
  print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
  printf("printf\n\tint: %d\n\tstring: %s\n\tchar: %c\n\tpercent: "
         "%%\n\tnothing: %n",
         1287, "World! ", 't');
}
