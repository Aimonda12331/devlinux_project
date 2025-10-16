#include "strutils.h"
#include <stdio.h>

int main() {
    char str1[] = "  Hello World !!! ";
    char str2[] = "devlinux";
    int num;

    printf("Original: '%s'\n", str1);
    str_trim(str1);
    printf("Trimmed: '%s'\n", str1);

    str_reverse(str2);
    printf("Reversed: '%s'\n", str2);

    if (str_to_int("12345", &num))
        printf("Converted: %d\n", num);
    else
        printf("Invalid number\n");

    return 0;
}
