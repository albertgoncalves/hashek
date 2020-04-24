#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <stdbool.h>
#include <stdint.h>

#define KEY_BUFFER_SIZE   7
#define VALUE_BUFFER_SIZE 24

typedef struct {
    char value[VALUE_BUFFER_SIZE];
    char key[KEY_BUFFER_SIZE];
    bool alive;
} ht_item_t;

typedef struct {
    ht_item_t* items;
    uint32_t   base;
    uint32_t   size;
    uint32_t   count;
    uint32_t   resizes;
    uint32_t   collisions;
    char       _[4];
} ht_table_t;

ht_table_t* ht_new(uint32_t);
void        ht_destroy(ht_table_t*);

void  ht_insert(ht_table_t*, const char*, const char*);
void  ht_delete(ht_table_t*, const char*);
char* ht_search(const ht_table_t*, const char*);

void ht_pretty_print(const ht_table_t*);

#endif
