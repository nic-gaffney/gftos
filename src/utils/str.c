#include "str.h"
#include <stddef.h>

size_t strlen(const char *str) {
    size_t size = 0;
    for (; (str[size]) != '\0'; size++) {
    }
    return size;
}

int isalpha(int c) { return (c > 64 && c < 91) || (c > 96 && c < 123); }

char *str_reverse(char *str) {
    char *str_ptr = str;
    size_t size = strlen(str);
    char return_str[size];

    for (size_t i = 0; i < size; i++)
        return_str[size - i - 1] = str[i];

    for (size_t i = 0; i < size; i++)
        *str_ptr++ = return_str[i];

    return str;
}

char *itoa(int num, char *str, int base) {
    char *str_ptr = str;
    if (num == 0)
        return "0";
    if (num < 0) {
        *str_ptr++ = '-';
        num *= -1;
    }
    while (num) {
        *str_ptr++ = num % base + (num % base > 9 ? 55 : 48);
        num /= base;
    }
    str_reverse(str);
    return str;
}
