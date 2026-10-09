#include <stdlib.h>

#define INITIAL_CAPACITY 16

typedef struct ArrayList {
    void** values;
    size_t capacity;
    size_t length;
} ArrayList;

ArrayList* arrayList() {
    ArrayList* al = malloc(sizeof(ArrayList));
    if (!al) return nullptr;
    al->capacity = INITIAL_CAPACITY;
    al->length = 0;
    al->values = calloc(al->capacity, sizeof(void*));
    if (!al->values) {
        free(al);
        return nullptr;
    }
    return al;
}

void al_destroy(ArrayList* al, void (*free_elem)(void* ptr)) {
    if (!al) return;
    if (free_elem) {
        for (int i = 0; i < al->length; i++) free_elem(al->values[i]);
    }
    free(al->values);
    free(al);
}

typedef struct it {
    void* value;
    ArrayList* al;
    size_t index;
} ali;

ali al_iterator(ArrayList* al) {
    ali it;
    it.al = al;
    it.index = 0;
    return it;
}

bool al_next(ali* it) {
    if (!it->al || it->index >= it->al->length) return false;
    it->value = it->al->values[it->index];
    it->index++;
    return true;
}

void al_expand(ArrayList* al) {
    size_t new_capacity = al->capacity * 2;
    if (new_capacity < al->capacity) return;
    void** new_values = realloc(al->values, new_capacity * sizeof(void*));
    if (!new_values) return;
    al->values = new_values;
    al->capacity = new_capacity;
}

void al_put(ArrayList* al, void* val) {
    if (!al) return;
    al->values[al->length] = val;
    al->length++;
    if (al->length >= al->capacity*7/8) al_expand(al);
}

void* al_get(ArrayList* al, size_t index) {
    if (!al || index >= al->length) return nullptr;
    return al->values[index];
}

size_t al_length(ArrayList* al) {
    return al ? al->length : 0;
}

void al_remove_index(ArrayList* al, size_t index) {
    if (!al || index >= al->length) return;
    for (;index < al->length - 1; index++) {
        al->values[index] = al->values[index+1];
    }
    al->values[index+1] = nullptr;
    al->length--;
}

void al_remove_index_wrapper(ArrayList* al, size_t index, ...) {
    al_remove_index(al, index);
}

void al_remove_object(ArrayList* al, void* val, bool (*comp)(const void* a, const void* b)) {
    if (!al || !comp) return;
    int i = 0;
    while (!comp(al->values[i], val) && al->values[i]) {
        i++;
    }
    al_remove_index(al, i);
}

size_t al_index_of(ArrayList* al, void* val, bool (*comp)(const void* a, const void* b)) {
    if (!al || !comp) return 0;
    size_t i = 0;
    while (!comp(al->values[i], val) && al->values[i]) {
        i++;
    }
    return i;
}