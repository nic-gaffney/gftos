#include "str.h"
#include <stddef.h>

size_t strlen(const char *str) {
    size_t size = 0;
    for (; (str[size]) != '\0'; size++) {
    }
    return size;
}

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

char *itoa(int num, char *str) {
    char *str_ptr = str;
    for (; num;) {
        *str_ptr++ = num % 10 + 48;
        num /= 10;
    }
    str_reverse(str);
    return str;
}
