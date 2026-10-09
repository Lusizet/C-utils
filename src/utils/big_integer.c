#include "Headers/big_integer.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct big_integer {
    char* str;
    size_t len;
} big_integer;

big_integer* bi_create(char* str) {
    big_integer* bi = malloc(sizeof(big_integer));
    if (!bi) return nullptr;
    bi->str = str;
    bi->len = strlen(str);
    return bi;
}

big_integer* bi_add(big_integer* a, big_integer* b) {
    int len_a = (int) a->len;
    int len_b = (int) b->len;
    int max_len = len_a < len_b ? len_b : len_a;
    char buffer[max_len];
    int k = 0;
    int carry = 0;
    for (int i = len_a-1, j = len_b-1; i >= 0 || j >= 0 || carry > 0; i--, j--, k++) {
        int n1 = i >= 0 ? a->str[i] - '0' : 0;
        int n2 = j >= 0 ? b->str[j] - '0' : 0;
        int sum = n1 + n2 + carry;
        carry = sum / 10;
        sum %= 10;
        buffer[k] = sum + '0';
    }
    char* result = malloc(sizeof(char) * (k+1));
    result[k] = '\0';
    for (int i = 0, j = k-1; i < k; i++, j--) {
        result[i] = buffer[j];
    }
    return bi_create(result);
}

static int bi_comp(const big_integer* a, const big_integer* b) {
    if (a->len > b->len) return 1;
    if (a->len < b->len) return -1;
    for (int i = 0; i < a->len; i++) {
        if (a->str[i] - '0' > b->str[i] - '0') return 1;
        if (a->str[i] - '0' < b->str[i] - '0') return -1;
    }
    return 0;
}

static void sub(big_integer* a, big_integer* b, char* buffer, bool flag) {
    int k = 0;
    int carry = 0;
    for (int i = (int) a->len-1, j = (int) b->len-1; i >= 0 || j >= 0 || carry > 0; i--, j--, k++) {
        int n1 = i >= 0 ? a->str[i] - '0' : 0;
        int n2 = j >= 0 ? b->str[j] - '0' : 0;
        int digit = n1 - (n2+carry);
        carry = 0;
        printf("%d %d %d %d\n", n1, n2, digit, k);
        if (digit < 0) {
            carry++;
            buffer[k] = (digit + 10) % 10 + '0';
        } else {
            buffer[k] = digit % 10 + '0';
        }
        if (j < 0 && buffer[k] == '0') k--;
    }
    if (flag) {
        buffer[k] = '-';
        buffer[k+1] = '\0';
    } else {
        buffer[k] = '\0';
    }
    printf("%s\n", buffer);
}

big_integer* bi_subtract(big_integer* a, big_integer* b) {
    size_t max_len = a->len < b->len ? b->len : a->len;
    char buffer[max_len+2];
    if (bi_comp(a, b) == 1) {
        sub(a, b, buffer, false);
    } else if (bi_comp(a,b) == -1) {
        sub(b, a, buffer, true);
    } else {
        return bi_create("0");
    }
    size_t buf_sz = strlen(buffer);
    char* result = malloc(sizeof(char) * buf_sz);
    int i = 0;
    for (int j = buf_sz - 1; j >= 0; j--, i++) {
        result[i] = buffer[j];
    }
    result[i] = '\0';
    return bi_create(result);
}

big_integer* bi_multiply(big_integer* a, big_integer* b) {
    big_integer* result = bi_create("0");
    for (int i = b->len-1, k = 0, carry = 0; i >= 0; i--, k = 0, k++) {
        int n1 = b->str[i] - '0';
        char buffer[a->len+k+2];
        for (; k > 0; k++) buffer[k-1] = '0';
        for (int j = a->len-1; j >= 0 || carry < 0; j--, k++) {
            int mult = n1 * (a->str[j] - '0') + carry;
            buffer[k] = mult % 10 + '0';
            carry = mult / 10;
        }
        buffer[a->len+k+1] = '\0';
        result = bi_add(result, bi_create(strrev(buffer)));
    }
    return result;
}

char* bi_toString(big_integer* bi) {
    return bi->str;
}