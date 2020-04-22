#include <string.h>
#include <sys/resource.h>

#include "exit.h"
#include "hash_table.h"
#include "term.h"

/* NOTE: Based on `https://github.com/jamesroutley/write-a-hash-table`. */

#define TEST_KEY_VALUE(table, key, value)     \
    {                                         \
        char* result = ht_search(table, key); \
        EXIT_IF(result == NULL);              \
        EXIT_IF(strcmp(result, value) != 0);  \
    }

#define TEST_KEY_NULL(table, key) EXIT_IF(ht_search(table, key) != NULL);

#define PRINT_TABLE(table)  \
    ht_pretty_print(table); \
    printf("\n");

int main(void) {
    /* NOTE: See `http://www.cs.tufts.edu/comp/111/examples/Time/getrusage.c`.
     */
    struct rusage usage;
    {
        ht_table_t* table = ht_new(4);
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
    getrusage(RUSAGE_SELF, &usage);
    printf("================\n"
           "%s  Time Elapsed  %s\n"
           "seconds   : %s%ld\n%s"
           "microsec. : %s%ld\n%s"
           "================\n",
           BOLD_INVERSE,
           CLOSE,
           BOLD,
           usage.ru_utime.tv_sec,
           CLOSE,
           BOLD,
           usage.ru_utime.tv_usec,
           CLOSE);
    return EXIT_SUCCESS;
}
