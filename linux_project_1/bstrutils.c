#include "strutils.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void str_reverse(char *s) {
    int len = strlen(s);
    for (int i = 0; i < len / 2; i++) {
        char tmp = s[i];
        s[i] = s[len - i - 1];
        s[len - i - 1] = tmp;
    }
}

void str_trim(char *s) {
    int start = 0;
    while (isspace((unsigned char)s[start])) start++;

    int end = strlen(s) - 1;
    while (end >= start && isspace((unsigned char)s[end])) end--;

    memmove(s, s + start, end - start + 1);
    s[end - start + 1] = '\0';
}

int str_to_int(const char *s, int *out) {
    char *end;
    long val = strtol(s, &end, 10);
    if (*end != '\0') return 0;  // không phải số hợp lệ
    *out = (int)val;
    return 1;
}
