#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "../../include/utils.h"

typedef struct arr_p {
    void** arr;
    size_t len;
} arr_p;

arr_p* arr_creator(void** arr, size_t len) {
    arr_p* arr_p = malloc(sizeof(arr_p));
    if (!arr_p) return nullptr;
    arr_p->arr = arr;
    arr_p->len = len;
    return arr_p;
}

void arr_destroy(arr_p* arr_p, void (*free_elem)(void* ptr)) {
    if (!arr_p) return;
    if (free_elem) {
        for (int i = 0; i < arr_p->len; i++) free_elem(arr_p->arr[i]);
    }
    free(arr_p->arr);
    free(arr_p);
}

void arr_int(StringBuilder* sb, arr_p* arr_p) {
    if (!arr_p) return;
    StringBuilder* str = sb_create();
    sb_append(str, (char) '{');
    for (int i = 0; i < arr_p->len; i++) {
        sb_append(str, *(int*)arr_p->arr[i]);
        if (i >= arr_p->len-1) break;
        sb_append(str, (char) ',');
    }
    sb_append(str, (char) '}');
    char* arr = sb_to_string(str);
    sb_append(sb, arr);
    free(arr);
    arr = nullptr;
    sb_destroy(str);
    // str->str = nullptr;
    str = nullptr;
}

void arr_print(char* format, ...) {
    va_list args;
    va_start(args, format);

    StringBuilder* sb = sb_create();
    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'd':
                    format++;
                    arr_int(sb, va_arg(args, arr_p*));
                    break;
            }
        }
        sb_append(sb, (char) *format);
        format++;
    }
    char* str = sb_to_string(sb);
    printf("%s\n", str);
    va_end(args);
    free(str);
    str = nullptr;
    sb_destroy(sb);
    // sb->str = nullptr;
    sb = nullptr;
}