#ifndef UTILS_H
#define UTILS_H

#include <winsock2.h>

// Server object
typedef struct Server Server;

// Array print object
typedef struct arr_p arr_p;

// BigInteger object
typedef struct big_integer BigInteger;

// StringBuilder object
typedef struct string_builder StringBuilder;

// ArrayList object
typedef struct array_list ArrayList;

// ArrayList iterator object
typedef struct it {
    void* value;
    ArrayList* al;
    size_t index;
} ali;

// HashTable object
typedef struct hash_table HashTable;

// HastTable iterator object
typedef struct {
    const char* key;
    void* value;
    HashTable* table;
    size_t index;
} hti;

// String addon
char* strrstr(const char* str, const char* substr);

// Array print
arr_p* arr_create(void** arr, size_t len);
void arr_destroy(arr_p* arr_p, void (*free_elem)(void* ptr));
void arr_print(char* format, ...);

// BigInteger
BigInteger* bi_create(char* str);
BigInteger* bi_add(BigInteger* a, BigInteger* b);
BigInteger* bi_subtract(BigInteger* a, BigInteger* b);
BigInteger* bi_multiply(BigInteger* a, BigInteger* b);
char* bi_toString(BigInteger* bi);

// HashTable
HashTable* ht_create(void);
void ht_destroy(HashTable* table);
void* ht_get(HashTable* table, const char* key);
void ht_set(HashTable* table, const char* key, void* value);
size_t ht_length(HashTable* table);
hti ht_iterator(HashTable* table);
bool ht_next(hti* it);

// ArrayList
ArrayList* al_create();
void al_destroy(ArrayList* al, void (*free_elem)(void* ptr));
void al_put(ArrayList* al, void* val);
void* al_get(ArrayList* al, size_t index);
size_t al_length(ArrayList* al);
ali al_iterator(ArrayList* al);
bool al_next(ali* it);
void al_remove_index_wrapper(ArrayList* al, size_t index, ...);
void al_remove_object(ArrayList* al, void* val, bool comp(const void* a, const void* b));
size_t al_index_of(ArrayList* al, void* val, bool (*comp)(const void* a, const void* b));

#define al_remove(al, b, ...) _Generic((b),                \
    size_t:  al_remove_index_wrapper(al, (size_t)(b)),     \
    int:     al_remove_index_wrapper(al, (size_t)(b)),     \
    default: al_remove_object(al, (void*)(b), __VA_ARGS__) \
)

// StringBuilder
StringBuilder* sb_create();
void sb_append_str(StringBuilder* sb, char* str);
void sb_append_char(StringBuilder* sb, char ch);
void sb_append_int(StringBuilder* sb, int n);
char* sb_to_string(StringBuilder* sb);
void sb_destroy(StringBuilder* sb);
size_t sb_length(StringBuilder* sb);
void sb_substring(StringBuilder* sb, size_t start, size_t end);

#define sb_append(sb, b) _Generic((b),             \
    char:          sb_append_char(sb, (char)(b)),  \
    unsigned char: sb_append_char(sb, (char)(b)),  \
    signed char:   sb_append_char(sb, (char)(b)),  \
    int:           sb_append_int(sb, (int)(b)),    \
    default:       sb_append_str(sb, (char*)(b))   \
)

// Server
Server server_constructor(int port, int domain, int service, int protocol, int backlog, u_long e_interface, void (*launch) (Server*));

#endif //UTILS_H
