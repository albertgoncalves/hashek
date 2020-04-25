#include <string.h>

#include "exit.h"
#include "hash_table.h"
#include "primes.h"

#define BASE_MIN  4
#define RESIZE_UP 70

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
    EXIT_IF(KEY_BUFFER_SIZE <= strlen(key));
    strcpy(item->key, key);
    EXIT_IF(VALUE_BUFFER_SIZE <= strlen(value));
    strcpy(item->value, value);
    item->alive = true;
}

ht_table_t* ht_new(uint32_t _base) {
    const uint32_t base = _base < BASE_MIN ? BASE_MIN : _base;
    const uint32_t size = next_prime(base);
    ht_table_t*    table =
        calloc(sizeof(ht_table_t) + (sizeof(ht_item_t) * size), 1);
    EXIT_IF(table == NULL);
    table->base = base;
    table->size = size;
    return table;
}

static ht_table_t* resize(ht_table_t* table, uint32_t base) {
    EXIT_IF(base <= table->base);
    ht_table_t*    new_table    = ht_new(base);
    const uint32_t size         = table->size;
    const uint32_t count        = table->count;
    uint32_t       insert_count = 0;
    for (uint32_t i = 0; i < size; ++i) {
        ht_item_t* item = &((*table).items[i]);
        if (item->alive) {
            new_table = ht_insert(new_table, item->key, item->value);
            if (count <= ++insert_count) {
                break;
            }
        }
    }
    new_table->resizes = table->resizes + 1;
    free(table);
    return new_table;
}

ht_table_t* ht_insert(ht_table_t* table, const char* key, const char* value) {
    if (RESIZE_UP < ((table->count * 100) / table->size)) {
        table = resize(table, table->base << 1);
    }
    const uint32_t size = table->size;
    uint32_t       hash = get_hash(key) % size;
    for (uint32_t i = 0; i < size; ++i) {
        uint32_t   index = (hash + i) % size;
        ht_item_t* item  = &((*table).items[index]);
        if (!item->alive) {
            set_item(item, key, value);
            ++table->count;
            return table;
        } else if (strcmp(item->key, key) == 0) {
            set_item(item, key, value);
            return table;
        }
        ++table->collisions;
    }
    return table;
}

void ht_delete(ht_table_t* table, const char* key) {
    if (table->count != 0) {
        const uint32_t size = table->size;
        uint32_t       hash = (get_hash(key)) % size;
        for (uint32_t i = 0; i < size; ++i) {
            uint32_t   index = (hash + i) % size;
            ht_item_t* item  = &((*table).items[index]);
            if (item == NULL) {
                return;
            } else if (item->alive && (strcmp(item->key, key) == 0)) {
                item->alive = false;
                --table->count;
                return;
            }
        }
    }
}

char* ht_search(ht_table_t* table, const char* key) {
    if (table->count != 0) {
        const uint32_t size = table->size;
        uint32_t       hash = (get_hash(key)) % size;
        for (uint32_t i = 0; i < size; ++i) {
            ht_item_t* item = &((*table).items[(hash + i) % size]);
            if (item == NULL) {
                break;
            } else if (item->alive && (strcmp(item->key, key) == 0)) {
                return item->value;
            }
        }
    }
    return NULL;
}

void ht_pretty_print(ht_table_t* table) {
    const uint32_t size = table->size;
    printf(".base       : %u\n"
           ".size       : %u\n"
           ".count      : %u\n"
           ".resizes    : %u\n"
           ".collisions : %u\n"
           ".items      : {\n",
           table->base,
           size,
           table->count,
           table->resizes,
           table->collisions);
    for (uint32_t i = 0; i < size; ++i) {
        ht_item_t* item = &((*table).items[i]);
        if (!item->alive) {
            printf("    _,\n");
        } else {
            printf("    \"%s\": \"%s\",\n", item->key, item->value);
        }
    }
    printf("}\n");
}
