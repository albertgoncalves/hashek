#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

#define NEW_TABLE_SIZE (size_t)53;
#define PRIME_A        157.0f
#define PRIME_B        163.0f

static ht_item_t NULL_ITEM = {NULL, NULL};

static ht_item_t* new_item(const char* key, const char* value) {
    ht_item_t* item = malloc(sizeof(ht_item_t));
    item->key       = strdup(key);
    item->value     = strdup(value);
    return item;
}

static void free_item(ht_item_t* item) {
    free(item->key);
    free(item->value);
    free(item);
}

static size_t hash(const char*  string,
                   const double seed,
                   const size_t table_size) {
    uint64_t     hash = 0;
    const size_t n    = strlen(string);
    for (size_t i = 0; i < n; ++i) {
        hash += (uint64_t)(pow(seed, (double)(n - (i + 1))) * string[i]);
        hash = hash % (uint64_t)table_size;
    }
    return (size_t)hash;
}

static size_t double_hash(const char*  string,
                          const size_t table_size,
                          const size_t k) {
    return (hash(string, PRIME_A, table_size) +
            (k * (hash(string, PRIME_B, table_size) + 1))) %
           table_size;
}

ht_hash_table_t* ht_new(void) {
    ht_hash_table_t* table = malloc(sizeof(ht_hash_table_t));
    table->size            = NEW_TABLE_SIZE;
    table->count           = 0;
    table->items           = calloc((size_t)table->size, sizeof(ht_item_t*));
    return table;
}

void ht_destroy(ht_hash_table_t* table) {
    for (size_t i = 0; i < table->size; ++i) {
        ht_item_t* item = table->items[i];
        if ((item != NULL) && (item != &NULL_ITEM)) {
            free_item(item);
        }
    }
    free(table->items);
    free(table);
}

void ht_insert(ht_hash_table_t* table, const char* key, const char* value) {
    size_t     index = double_hash(key, table->size, 0);
    ht_item_t* item  = table->items[index];
    for (size_t i = 1; (item != NULL) && (item != &NULL_ITEM); ++i) {
        if (strcmp(item->key, key) == 0) {
            free_item(item);
            table->items[index] = new_item(key, value);
            return;
        }
        index = double_hash(key, table->size, i);
        item  = table->items[index];
    }
    table->items[index] = new_item(key, value);
    ++(table->count);
}

char* ht_search(ht_hash_table_t* table, const char* key) {
    size_t     index = double_hash(key, table->size, 0);
    ht_item_t* item  = table->items[index];
    for (size_t i = 1; item != NULL; ++i) {
        if ((item != &NULL_ITEM) && (strcmp(item->key, key) == 0)) {
            return item->value;
        }
        index = double_hash(key, table->size, i);
        item  = table->items[index];
    }
    return NULL;
}

void ht_delete(ht_hash_table_t* table, const char* key) {
    size_t     index = double_hash(key, table->size, 0);
    ht_item_t* item  = table->items[index];
    for (size_t i = 1; item != NULL; ++i) {
        if ((item != &NULL_ITEM) && (strcmp(item->key, key) == 0)) {
            free_item(item);
            table->items[index] = &NULL_ITEM;
            --(table->count);
        }
        index = double_hash(key, table->size, i);
        item  = table->items[index];
    }
}

void ht_pretty_print(ht_hash_table_t* table) {
    printf(".size : %zu\n.count: %zu\n.items: {\n", table->size, table->count);
    for (size_t i = 0; i < table->size; ++i) {
        ht_item_t* item = table->items[i];
        if (item == &NULL_ITEM) {
            printf("    NULL: NULL,\n");
        } else if (item != NULL) {
            printf("    \"%s\": \"%s\",\n", item->key, item->value);
        }
    }
    printf("}\n");
}
