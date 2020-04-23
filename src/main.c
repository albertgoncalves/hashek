#include <string.h>

#include "bench.h"
#include "exit.h"
#include "hash_table.h"
#include "term.h"

/* NOTE: Based on `https://github.com/jamesroutley/write-a-hash-table`. */

#define TEST_KEY_NULL(table, key) EXIT_IF(ht_search(table, key) != NULL);
#define TEST_KEY_VALUE(table, key, value)     \
    {                                         \
        char* result = ht_search(table, key); \
        EXIT_IF(result == NULL);              \
        EXIT_IF(strcmp(result, value) != 0);  \
    }

#define PRINT_TABLE(table)  \
    ht_pretty_print(table); \
    printf("\n");

#define N 10000
#define WORK(table)                       \
    {                                     \
        ht_insert(table, "foo", "bar4");  \
        ht_insert(table, "bar", "baz");   \
        ht_insert(table, "baz", "jazz3"); \
        ht_insert(table, "jazz", "foo");  \
        ht_insert(table, "FOO", "BAR");   \
        ht_insert(table, "BAR", "BAZ");   \
        ht_insert(table, "BAZ", "JAZZ");  \
        ht_insert(table, "JAZZ", "FOO");  \
        ht_delete(table, "foo");          \
        ht_delete(table, "bar");          \
        ht_delete(table, "baz");          \
        ht_delete(table, "jazz");         \
        ht_delete(table, "FOO");          \
        ht_delete(table, "BAR");          \
        ht_delete(table, "BAZ");          \
        ht_delete(table, "JAZZ");         \
    }

int main(void) {
    {
        ht_table_t* table = ht_new(2);
        {
            ht_insert(table, "foo", "bar");
            ht_insert(table, "baz", "jazz");
            PRINT_TABLE(table);
            TEST_KEY_VALUE(table, "foo", "bar");
            TEST_KEY_VALUE(table, "baz", "jazz");
        }
        {
            ht_insert(table, "foo", "bar2");
            PRINT_TABLE(table);
            TEST_KEY_VALUE(table, "foo", "bar2");
        }
        {
            ht_delete(table, "baz");
            PRINT_TABLE(table);
            TEST_KEY_NULL(table, "baz");
        }
        {
            ht_insert(table, "baz", "jazz2");
            PRINT_TABLE(table);
            TEST_KEY_VALUE(table, "baz", "jazz2");
        }
        {
            ht_delete(table, "foo");
            ht_insert(table, "foo", "bar3");
            ht_delete(table, "bar");
            ht_delete(table, "baz");
            PRINT_TABLE(table);
            TEST_KEY_VALUE(table, "foo", "bar3");
            TEST_KEY_NULL(table, "bar");
            TEST_KEY_NULL(table, "baz");
        }
        {
            ht_insert(table, "foo", "bar4");
            ht_insert(table, "bar", "baz");
            ht_insert(table, "baz", "jazz3");
            ht_insert(table, "jazz", "foo");
            ht_insert(table, "FOO", "BAR");
            ht_insert(table, "BAR", "BAZ");
            ht_insert(table, "BAZ", "JAZZ");
            ht_insert(table, "JAZZ", "FOO");
            PRINT_TABLE(table);
            TEST_KEY_VALUE(table, "foo", "bar4");
            TEST_KEY_VALUE(table, "bar", "baz");
            TEST_KEY_VALUE(table, "baz", "jazz3");
            TEST_KEY_VALUE(table, "jazz", "foo");
            TEST_KEY_VALUE(table, "FOO", "BAR");
            TEST_KEY_VALUE(table, "BAR", "BAZ");
            TEST_KEY_VALUE(table, "BAZ", "JAZZ");
            TEST_KEY_VALUE(table, "JAZZ", "FOO");
        }
        ht_destroy(table);
    }
    BENCH("new, work, destroy", N, {
        ht_table_t* table = ht_new(8);
        WORK(table);
        ht_destroy(table);
    });
    {
        ht_table_t* table = ht_new(8);
        BENCH("work only", N, WORK(table));
        ht_destroy(table);
    }
    return EXIT_SUCCESS;
}
