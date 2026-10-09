#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define HT_INITIAL_CAPACITY 16

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

static uint64_t hash_key(const char *str) {
    uint64_t hash = FNV_OFFSET;
    for (const char* c = str; *c; c++) {
        hash ^= (uint64_t)(unsigned char) *c;
        hash *= FNV_PRIME;
    }
    return hash;
}

typedef struct {
    const char* key;
    void* value;
} ht_entry;

struct ht {
    ht_entry* entries;
    size_t capacity;
    size_t length;
};

typedef struct ht ht;

ht* ht_create() {
    ht *ht = malloc(sizeof(ht));
    if (!ht) return nullptr;
    ht->capacity = HT_INITIAL_CAPACITY;
    ht->length = 0;
    ht->entries = calloc(ht->capacity, sizeof(ht_entry));
    return ht;
}

void ht_destroy(ht* table) {
    if (!table) return;
    free(table->entries);
    free(table);
}

void* ht_get(ht* table, const char* key) {
    if (!table) return nullptr;
    uint64_t hash = hash_key(key);
    size_t index = hash & (table->capacity - 1);

    ht_entry* entries = table->entries;

    while (entries[index].key) {
        if (strcmp(entries[index].key, key) == 0) return entries[index].value;
        index++;
        if (index == table->capacity) index = 0;
    }

    return nullptr;
}

typedef struct {
    const char* key;
    void* value;
    ht* table;
    size_t index;
} hti;

hti ht_iterator(ht* table) {
    hti it;
    it.table = table;
    it.index = 0;
    return it;
}

bool ht_next(hti* it) {
    if (it->index > it->table->capacity) return false;
    ht_entry* entries = it->table->entries;
    while (!entries[it->index].key) {
        it->index++;
        if (it->index == it->table->capacity) return false;
    }
    it->key = entries[it->index].key;
    it->value = entries[it->index].value;
    it->index++;
    return true;
}

static void ht_expand(ht* table) {
    size_t new_size = table->capacity * 2;
    ht_entry* new_entries = calloc(new_size, sizeof(ht_entry));
    hti it = ht_iterator(table);
    while (ht_next(&it)) {
        uint64_t hash = hash_key(it.key);
        size_t index = hash & (new_size - 1);

        while (new_entries[index].key) {
            index++;
            if (index == new_size) index = 0;
        }

        new_entries[index].key = it.key;
        new_entries[index].value = it.value;
    }
    table->capacity = new_size;
    free(table->entries);
    table->entries = new_entries;
}

void ht_set(ht* table, const char* key, void* value) {
    uint64_t hash = hash_key(key);
    size_t index = hash & (table->capacity - 1);

    ht_entry* entries = table->entries;
    while (entries[index].key) {
        if (strcmp(entries[index].key, key) == 0) {
            printf("Duplicated key\n");
            return;
        }
        index++;
        if (index == table->capacity) index = 0;
    }

    entries[index].key = key;
    entries[index].value = value;

    table->length++;
    if (table->length == table->capacity / 2) ht_expand(table);
}

size_t ht_length(ht* table) {
    return table->length;
}