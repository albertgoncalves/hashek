#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include "primes.h"

static const size_t INIT_BASE = 2;
static const size_t RESIZE_UP = 50;

static const char* BOLD_PURPLE    = "\033[1;35m";
static const char* BOLD           = "\033[1m";
static const char* BOLD_UNDERLINE = "\033[1;4m";
static const char* CLOSE          = "\033[0m";

#define EXIT_IF(condition)                                  \
    if (condition) {                                        \
        fprintf(stderr,                                     \
                "<%s%s%s>.%s%s%s() @%sline %d%s: %sExit%s", \
                BOLD,                                       \
                __FILE__,                                   \
                CLOSE,                                      \
                BOLD,                                       \
                __func__,                                   \
                CLOSE,                                      \
                BOLD_UNDERLINE,                             \
                __LINE__,                                   \
                CLOSE,                                      \
                BOLD_PURPLE,                                \
                CLOSE);                                     \
        exit(1);                                            \
    }

static ht_item_t NULL_ITEM = {NULL, NULL};

// static size_t get_hash(const char* string, size_t table_size) {
//     uint64_t hash = 5381;
//     const size_t   n    = strlen(string);
//     for (size_t i = 0; i < n; ++i) {
//         hash = ((hash << 5) + hash) + (uint64_t)string[i];
//     }
//     return ((size_t)hash) % table_size;
// }

static size_t get_hash(const char* string, size_t table_size) {
    uint64_t       hash = 0;
    const uint64_t size = table_size;
    const size_t   n    = strlen(string);
    for (size_t i = 0; i < n; ++i) {
        hash += (uint64_t)(powf(163.0f, (float)(n - (i + 1))) * string[i]);
        hash %= size;
    }
    return (size_t)hash;
}

static ht_item_t* new_item(const char* key, const char* value) {
    ht_item_t* item = malloc(sizeof(ht_item_t));
    EXIT_IF(item == NULL);
    char* item_key = strdup(key);
    EXIT_IF(item_key == NULL);
    char* item_value = strdup(value);
    EXIT_IF(item_value == NULL);
    item->key   = item_key;
    item->value = item_value;
    return item;
}

static void free_item(ht_item_t* item) {
    free(item->key);
    free(item->value);
    free(item);
}

static ht_hash_table_t* new_sized(size_t base) {
    ht_hash_table_t* table = malloc(sizeof(ht_hash_table_t));
    EXIT_IF(table == NULL);
    table->base       = base;
    table->size       = next_prime(base);
    table->count      = 0;
    table->collisions = 0;
    ht_item_t** items = calloc((size_t)table->size, sizeof(ht_item_t*));
    EXIT_IF(items == NULL);
    table->items = items;
    return table;
}

ht_hash_table_t* ht_new(void) {
    return new_sized(INIT_BASE);
}

void ht_destroy(ht_hash_table_t* table) {
    const size_t size       = table->size;
    const size_t count      = table->count;
    size_t       free_count = 0;
    for (size_t i = 0; (i < size) && (free_count < count); ++i) {
        ht_item_t* item = table->items[i];
        if ((item != NULL) && (item != &NULL_ITEM)) {
            free_item(item);
            ++free_count;
        }
    }
    free(table->items);
    free(table);
}

static void resize(ht_hash_table_t* table, size_t base) {
    if (base < INIT_BASE) {
        return;
    }
    ht_hash_table_t* new_table    = new_sized(base);
    const size_t     size         = table->size;
    const size_t     count        = table->count;
    size_t           insert_count = 0;
    for (size_t i = 0; (i < size) && (insert_count < count); ++i) {
        ht_item_t* item = table->items[i];
        if ((item != NULL) && (item != &NULL_ITEM)) {
            ht_insert(new_table, item->key, item->value);
            ++insert_count;
        }
    }
    table->base       = base;
    table->size       = new_table->size;
    new_table->size   = size;
    ht_item_t** items = table->items;
    table->items      = new_table->items;
    new_table->items  = items;
    ht_destroy(new_table);
}

void ht_insert(ht_hash_table_t* table, const char* key, const char* value) {
    if (RESIZE_UP < ((table->count * 100) / table->size)) {
        resize(table, table->base * 2);
    }
    const size_t size = table->size;
    size_t       hash = get_hash(key, size);
    for (size_t i = 0; i < size; ++i) {
        size_t index = (hash + i) % size;
        ht_item_t*   item  = table->items[index];
        if ((item == NULL) || (item == &NULL_ITEM)) {
            table->items[index] = new_item(key, value);
            ++(table->count);
            return;
        } else if (strcmp(item->key, key) == 0) {
            free_item(item);
            table->items[index] = new_item(key, value);
            return;
        }
        ++(table->collisions);
    }
}

char* ht_search(ht_hash_table_t* table, const char* key) {
    if (table->count == 0) {
        return NULL;
    }
    const size_t size  = table->size;
    size_t       index = get_hash(key, size);
    ht_item_t*   item  = table->items[index];
    for (size_t i = 1; (item != NULL) && (i <= size); ++i) {
        if ((item != &NULL_ITEM) && (strcmp(item->key, key) == 0)) {
            return item->value;
        }
        index = (index + 1) % size;
        item  = table->items[index];
    }
    return NULL;
}

void ht_delete(ht_hash_table_t* table, const char* key) {
    if (table->count == 0) {
        return;
    }
    const size_t size  = table->size;
    size_t       index = get_hash(key, size);
    ht_item_t*   item  = table->items[index];
    for (size_t i = 1; (item != NULL) && (i <= size) && (table->count != 0);
         ++i) {
        if ((item != &NULL_ITEM) && (strcmp(item->key, key) == 0)) {
            free_item(item);
            table->items[index] = &NULL_ITEM;
            --(table->count);
        }
        index = (index + 1) % size;
        item  = table->items[index];
    }
}

void ht_pretty_print(ht_hash_table_t* table) {
    const size_t size = table->size;
    printf(".size       : %zu\n.count      : %zu\n.collisions : %zu\n.items   "
           "   : {\n",
           size,
           table->count,
           table->collisions);
    for (size_t i = 0; i < size; ++i) {
        ht_item_t* item = table->items[i];
        if (item == &NULL_ITEM) {
            printf("    NULL: NULL,\n");
        } else if (item != NULL) {
            printf("    \"%s\": \"%s\",\n", item->key, item->value);
        } else {
            printf("    _,\n");
        }
    }
    printf("}\n");
}
