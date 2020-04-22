#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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

/* NOTE: See
 * `https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/gettime.html`.
 */
#define timespec_t struct timespec
#define TIME_TO_NS(start, end) \
    (1000000000l * (end.tv_sec - start.tv_sec)) + end.tv_nsec - start.tv_nsec
#define BENCH(block)                                           \
    {                                                          \
        timespec_t mono_start, mono_end, proc_start, proc_end; \
        clock_gettime(CLOCK_MONOTONIC, &mono_start);           \
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &proc_start);  \
        block;                                                 \
        clock_gettime(CLOCK_MONOTONIC, &mono_end);             \
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &proc_end);    \
        int64_t mono_ns = TIME_TO_NS(mono_start, mono_end);    \
        int64_t proc_ns = TIME_TO_NS(proc_start, proc_end);    \
        printf("%s Benchmark %s\n"                             \
               "monotonic : %s%8li%s ns\n"                     \
               "process   : %s%8li%s ns\n",                    \
               BOLD_INVERSE,                                   \
               CLOSE,                                          \
               BOLD,                                           \
               mono_ns,                                        \
               CLOSE,                                          \
               BOLD,                                           \
               proc_ns,                                        \
               CLOSE);                                         \
    }

int main(void) {
    BENCH({
        usleep(1000);
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
    });
    return EXIT_SUCCESS;
}
