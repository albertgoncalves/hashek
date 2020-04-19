#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <stdio.h>

typedef struct {
    char* key;
    char* value;
} ht_item_t;

typedef struct {
    size_t      size;
    size_t      count;
    ht_item_t** items;
} ht_hash_table_t;

ht_hash_table_t* ht_new(void);
void             ht_destroy(ht_hash_table_t*);

void  ht_insert(ht_hash_table_t*, const char*, const char*);
void  ht_delete(ht_hash_table_t*, const char*);
char* ht_search(ht_hash_table_t*, const char*);

void ht_pretty_print(ht_hash_table_t*);

#endif
