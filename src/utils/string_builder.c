#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_CAPACITY 32

typedef struct sb {
    char* str;
    size_t capacity;
    size_t length;
} StringBuilder;

StringBuilder* sb_create() {
    StringBuilder* sb = malloc(sizeof(StringBuilder));
    if (!sb) return nullptr;
    char* str = malloc(INITIAL_CAPACITY * sizeof(char));
    if (!str) {
        free(sb);
        return nullptr;
    }
    sb->str = str;
    sb->capacity = INITIAL_CAPACITY;
    sb->length = 0;
}

static void sb_expand(StringBuilder* sb, size_t new_length) {
    size_t new_capacity = sb->capacity * 2;
    while (new_capacity <= new_length) new_capacity *= 2;
    char* new_str = realloc(sb->str, new_capacity * sizeof(char));
    if (!new_str) return;
    sb->str = new_str;
    sb->capacity = new_capacity;
}

void sb_append_str(StringBuilder* sb, char* str) {
    if (!sb || !str || *str == '\0') return;
    size_t new_length = sb->length + strlen(str);
    if (new_length >= sb->capacity) sb_expand(sb, new_length);
    sb->length += sprintf(sb->str+sb->length, "%s", str);
}

void sb_append_char(StringBuilder* sb, char ch) {
    if (!sb || !ch) return;
    size_t new_length = sb->length + 1;
    if (new_length >= sb->capacity) sb_expand(sb, new_length);
    sb->length += sprintf(sb->str+sb->length, "%c", ch);
}

void sb_append_int(StringBuilder* sb, int n) {
    if (!sb || !n) return;
    size_t new_length = sb->length + snprintf(nullptr, 0, "%d", n);
    if (new_length >= sb->capacity) sb_expand(sb, new_length);
    sb->length += sprintf(sb->str+sb->length, "%d", n);
}

char* sb_to_string(StringBuilder* sb) {
    char* str = malloc(sb->length * sizeof(char));
    strcpy(str, sb->str);
    return str;
}

void sb_destroy(StringBuilder* sb) {
    if (!sb) return;
    free(sb->str);
    free(sb);
}

size_t sb_length(StringBuilder* sb) {
    return sb->length;
}

void sb_substring(StringBuilder* sb, size_t start, size_t end) {
    if (!sb || start > end) return;
    if (end >= sb->length) end = sb->length - 1;
    memmove(sb->str+start, sb->str+end+1, sb->length-end);
}