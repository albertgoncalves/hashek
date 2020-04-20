#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"

/* NOTE: Based on `https://github.com/jamesroutley/write-a-hash-table`. */

static ht_hash_table_t* TABLE;
static char*            RESULT;

#define PRINT_SEARCH(key)                                            \
    RESULT = ht_search(TABLE, key);                                  \
    if (RESULT != NULL) {                                            \
        printf("ht_search(TABLE, \"%s\") -> \"%s\"\n", key, RESULT); \
    } else {                                                         \
        printf("ht_search(TABLE, \"%s\") -> NULL\n", key);           \
    }

int main(void) {
    TABLE = ht_new(4);
    {
        ht_insert(TABLE, "foo", "foo");
        ht_insert(TABLE, "baz", "baz");
        ht_pretty_print(TABLE);
        PRINT_SEARCH("foo");
        PRINT_SEARCH("baz");
        printf("\n");
    }
    {
        ht_insert(TABLE, "foo", "foo2");
        ht_pretty_print(TABLE);
        PRINT_SEARCH("foo");
        printf("\n");
    }
    {
        ht_delete(TABLE, "baz");
        ht_pretty_print(TABLE);
        PRINT_SEARCH("foo");
        PRINT_SEARCH("baz");
        printf("\n");
    }
    {
        ht_insert(TABLE, "baz", "baz2");
        ht_pretty_print(TABLE);
        PRINT_SEARCH("foo");
        PRINT_SEARCH("baz");
        printf("\n");
    }
    {
        ht_delete(TABLE, "foo");
        ht_delete(TABLE, "bar");
        ht_delete(TABLE, "baz");
        ht_pretty_print(TABLE);
        PRINT_SEARCH("foo");
        PRINT_SEARCH("baz");
        printf("\n");
    }
    {
        ht_insert(TABLE, "foo", "foo3");
        ht_insert(TABLE, "foo", "foo4");
        ht_insert(TABLE, "bar", "bar");
        ht_insert(TABLE, "baz", "baz3");
        ht_insert(TABLE, "jazz", "jazz");
        ht_insert(TABLE, "FOO", "FOO");
        ht_insert(TABLE, "BAR", "BAR");
        ht_insert(TABLE, "BAZ", "BAZ");
        ht_insert(TABLE, "JAZZ", "JAZZ");
        ht_pretty_print(TABLE);
        PRINT_SEARCH("foo");
        PRINT_SEARCH("bar");
        PRINT_SEARCH("baz");
        PRINT_SEARCH("jazz");
        PRINT_SEARCH("FOO");
        PRINT_SEARCH("BAR");
        PRINT_SEARCH("BAZ");
        PRINT_SEARCH("JAZZ");
        printf("\n");
    }
    ht_destroy(TABLE);
    return EXIT_SUCCESS;
}
