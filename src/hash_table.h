#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <stdio.h>

typedef struct {
    char* key;
    char* value;
} ht_item_t;

typedef struct {
    size_t      base;
    size_t      size;
    size_t      count;
    size_t      collisions;
    ht_item_t** items;
} ht_hash_table_t;

ht_hash_table_t* ht_new(size_t);
void             ht_destroy(ht_hash_table_t*);

void  ht_insert(ht_hash_table_t*, const char*, const char*);
void  ht_delete(ht_hash_table_t*, const char*);
char* ht_search(const ht_hash_table_t*, const char*);

void ht_pretty_print(const ht_hash_table_t*);

#endif
