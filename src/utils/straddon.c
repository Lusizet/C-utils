#include <string.h>

char* strrstr(const char* str, const char* substr) {
    int substr_max = (int) strlen(substr) - 1;
    for (int i = (int) strlen(str) - 1, j = substr_max; i >= 0 ; i--) {
        if (str[i] == substr[j]) {
            j--;
            if (j == -1) {
                return strdup(str + i);
            }
        } else {
            j = substr_max;
        }
    }
    return nullptr;
}