#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"

/* NOTE: Based on `https://github.com/jamesroutley/write-a-hash-table`. */

static ht_hash_table_t* TABLE;
static char*            RESULT;

#define DEBUG_SEARCH(key)                                            \
    RESULT = ht_search(TABLE, key);                                  \
    if (RESULT != NULL) {                                            \
        printf("ht_search(TABLE, \"%s\") -> \"%s\"\n", key, RESULT); \
    } else {                                                         \
        printf("ht_search(TABLE, \"%s\") -> NULL\n", key);           \
    }

int main(void) {
    TABLE = ht_new();
    {
        ht_insert(TABLE, "foo", "bar");
        ht_insert(TABLE, "baz", "jazz");
        ht_pretty_print(TABLE);
        DEBUG_SEARCH("foo");
        DEBUG_SEARCH("baz");
        printf("\n");
    }
    {
        ht_insert(TABLE, "foo", "BAR");
        ht_pretty_print(TABLE);
        DEBUG_SEARCH("foo");
        printf("\n");
    }
    {
        ht_delete(TABLE, "baz");
        ht_pretty_print(TABLE);
        DEBUG_SEARCH("foo");
        DEBUG_SEARCH("baz");
        printf("\n");
    }
    {
        ht_insert(TABLE, "baz", "JAZZ");
        ht_pretty_print(TABLE);
        DEBUG_SEARCH("foo");
        DEBUG_SEARCH("baz");
        printf("\n");
    }
    {
        ht_delete(TABLE, "foo");
        ht_delete(TABLE, "bar");
        ht_delete(TABLE, "baz");
        ht_pretty_print(TABLE);
        DEBUG_SEARCH("foo");
        DEBUG_SEARCH("baz");
        printf("\n");
    }
    {
        ht_insert(TABLE, "foo", "BAR");
        ht_insert(TABLE, "foo", "bar");
        ht_insert(TABLE, "baz", "jazz");
        ht_insert(TABLE, "FOO", "BAR");
        ht_pretty_print(TABLE);
        DEBUG_SEARCH("foo");
        DEBUG_SEARCH("baz");
        DEBUG_SEARCH("FOO");
        printf("\n");
    }
    ht_destroy(TABLE);
    return EXIT_SUCCESS;
}
