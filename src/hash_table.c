#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include "primes.h"

static const size_t RESIZE_UP = 64;

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

static uint32_t get_hash(const char* string) {
    /* NOTE: MurmurHash via
     * `https://stackoverflow.com/questions/7666509/hash-function-for-string`.
     */
    uint32_t hash = 3323198485;
    for (; *string; ++string) {
        uint32_t c = (uint32_t)*string;
        hash ^= c;
        hash *= 1540483477;
        hash ^= hash >> 15;
    }
    return hash;
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

ht_hash_table_t* ht_new(size_t base) {
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
    if (base < table->base) {
        return;
    }
    ht_hash_table_t* new_table    = ht_new(base);
    const size_t     size         = table->size;
    const size_t     count        = table->count;
    size_t           insert_count = 0;
    for (size_t i = 0; (i < size) && (insert_count < count); ++i) {
        ht_item_t* item = table->items[i];
        if ((item != NULL) && (item != &NULL_ITEM)) {
            ht_insert(new_table, item->key, item->value);
            free_item(item);
            ++insert_count;
        }
    }
    table->base       = base;
    table->size       = new_table->size;
    table->collisions = 0;
    free(table->items);
    table->items = new_table->items;
    free(new_table);
}

void ht_insert(ht_hash_table_t* table, const char* key, const char* value) {
    if (RESIZE_UP < ((table->count * 100) / table->size)) {
        resize(table, table->base << 2);
    }
    const size_t size = table->size;
    size_t       hash = ((size_t)get_hash(key)) % size;
    for (size_t i = 0; i < size; ++i) {
        size_t     index = (hash + i) % size;
        ht_item_t* item  = table->items[index];
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

void ht_delete(ht_hash_table_t* table, const char* key) {
    if (table->count == 0) {
        return;
    }
    const size_t size = table->size;
    size_t       hash = ((size_t)get_hash(key)) % size;
    for (size_t i = 0; i < size; ++i) {
        size_t     index = (hash + i) % size;
        ht_item_t* item  = table->items[index];
        if (item == NULL) {
            break;
        } else if ((item != &NULL_ITEM) && (strcmp(item->key, key) == 0)) {
            free_item(item);
            table->items[index] = &NULL_ITEM;
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
        ht_item_t* item = table->items[(hash + i) % size];
        if (item == NULL) {
            break;
        } else if ((item != &NULL_ITEM) && (strcmp(item->key, key) == 0)) {
            return item->value;
        }
    }
    return NULL;
}

void ht_pretty_print(const ht_hash_table_t* table) {
    const size_t size = table->size;
    printf(".size       : %zu\n"
           ".count      : %zu\n"
           ".collisions : %zu\n"
           ".items      : {\n",
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
