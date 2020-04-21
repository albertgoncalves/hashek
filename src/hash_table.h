#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <stdbool.h>

#define KEY_BUFFER_SIZE   7
#define VALUE_BUFFER_SIZE 24

typedef struct {
    char value[VALUE_BUFFER_SIZE];
    char key[KEY_BUFFER_SIZE];
    bool alive;
} ht_item_t;

typedef struct {
    size_t     base;
    size_t     size;
    size_t     count;
    size_t     collisions;
    size_t     resizes;
    ht_item_t* items;
} ht_table_t;

ht_table_t* ht_new(size_t);
void        ht_destroy(ht_table_t*);

void  ht_insert(ht_table_t*, const char*, const char*);
void  ht_delete(ht_table_t*, const char*);
char* ht_search(const ht_table_t*, const char*);

void ht_pretty_print(const ht_table_t*);

#endif
