#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "exit.h"
#include "hash_table.h"
#include "primes.h"

static const size_t RESIZE_UP = 70;

static uint32_t get_hash(const char* string) {
    /* NOTE: MurmurHash via
     * `https://stackoverflow.com/questions/7666509/hash-function-for-string`.
     */
    uint32_t hash = 3323198485;
    for (; *string; ++string) {
        uint32_t c = (uint32_t)(*string);
        hash ^= c;
        hash *= 1540483477;
        hash ^= hash >> 15;
    }
    return hash;
}

static void set_item(ht_item_t* item, const char* key, const char* value) {
    EXIT_IF(item == NULL);
    EXIT_IF(KEY_BUFFER_SIZE <= strlen(key));
    strcpy(item->key, key);
    EXIT_IF(VALUE_BUFFER_SIZE <= strlen(value));
    strcpy(item->value, value);
    item->alive = true;
}

ht_hash_table_t* ht_new(size_t base) {
    ht_hash_table_t* table = malloc(sizeof(ht_hash_table_t));
    EXIT_IF(table == NULL);
    table->base       = base < 4 ? 4 : base;
    table->size       = next_prime(base);
    table->count      = 0;
    table->collisions = 0;
    table->resizes    = 0;
    ht_item_t* items  = calloc((size_t)table->size, sizeof(ht_item_t));
    EXIT_IF(items == NULL);
    table->items = items;
    return table;
}

void ht_destroy(ht_hash_table_t* table) {
    free(table->items);
    free(table);
}

static void resize(ht_hash_table_t* table, size_t base) {
    EXIT_IF(base <= table->base);
    ht_hash_table_t* new_table    = ht_new(base);
    const size_t     size         = table->size;
    const size_t     count        = table->count;
    size_t           insert_count = 0;
    for (size_t i = 0; (i < size) && (insert_count < count); ++i) {
        ht_item_t* item = &table->items[i];
        if (item->alive) {
            ht_insert(new_table, item->key, item->value);
            ++insert_count;
        }
    }
    table->base       = base;
    table->size       = new_table->size;
    table->collisions = 0;
    ++(table->resizes);
    free(table->items);
    table->items = new_table->items;
    free(new_table);
}

void ht_insert(ht_hash_table_t* table, const char* key, const char* value) {
    if (RESIZE_UP < ((table->count * 100) / table->size)) {
        resize(table, table->base << 1);
    }
    const size_t size = table->size;
    size_t       hash = ((size_t)get_hash(key)) % size;
    for (size_t i = 0; i < size; ++i) {
        size_t     index = (hash + i) % size;
        ht_item_t* item  = &(table->items[index]);
        if (!item->alive) {
            set_item(&(table->items[index]), key, value);
            ++(table->count);
            return;
        } else if (strcmp(item->key, key) == 0) {
            set_item(&(table->items[index]), key, value);
            return;
        }
        ++(table->collisions);
    }
}

void ht_delete(ht_hash_table_t* table, const char* key) {
    if (table->count == 0) {
        return;
    }
    const size_t size = table->size;
    size_t       hash = ((size_t)get_hash(key)) % size;
    for (size_t i = 0; i < size; ++i) {
        size_t     index = (hash + i) % size;
        ht_item_t* item  = &table->items[index];
        if (item == NULL) {
            break;
        } else if ((item->alive) && (strcmp(item->key, key) == 0)) {
            table->items[index].alive = false;
            --(table->count);
            return;
        }
    }
}

char* ht_search(const ht_hash_table_t* table, const char* key) {
    if (table->count == 0) {
        return NULL;
    }
    const size_t size = table->size;
    size_t       hash = ((size_t)get_hash(key)) % size;
    for (size_t i = 0; i < size; ++i) {
        ht_item_t* item = &table->items[(hash + i) % size];
        if (item == NULL) {
            break;
        } else if (item->alive && (strcmp(item->key, key) == 0)) {
            return item->value;
        }
    }
    return NULL;
}

void ht_pretty_print(const ht_hash_table_t* table) {
    const size_t size = table->size;
    printf(".base       : %zu\n"
           ".size       : %zu\n"
           ".count      : %zu\n"
           ".collisions : %zu\n"
           ".resizes    : %zu\n"
           ".items      : {\n",
           table->base,
           size,
           table->count,
           table->collisions,
           table->resizes);
    for (size_t i = 0; i < size; ++i) {
        ht_item_t* item = &table->items[i];
        if (!item->alive) {
            printf("    _,\n");
        } else {
            printf("    \"%s\": \"%s\",\n", item->key, item->value);
        }
    }
    printf("}\n");
}
