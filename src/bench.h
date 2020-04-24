#ifndef __BENCH_H__
#define __BENCH_H__

#include <time.h>

#include "term.h"

/* NOTE: See
 * `https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/gettime.html`.
 */

#define timespec_t struct timespec

#define TIME_TO_NS(start, end) \
    (1000000000l * (end.tv_sec - start.tv_sec)) + end.tv_nsec - start.tv_nsec

#define BENCH(label, n, block)                                    \
    {                                                             \
        timespec_t mono_start, mono_end, proc_start, proc_end;    \
        SUPPRESS_STDOUT({                                         \
            clock_gettime(CLOCK_MONOTONIC, &mono_start);          \
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &proc_start); \
            for (size_t _ = 0; _ < n; ++_) {                      \
                block;                                            \
            }                                                     \
            clock_gettime(CLOCK_MONOTONIC, &mono_end);            \
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &proc_end);   \
        })                                                        \
        int64_t mono_ns     = TIME_TO_NS(mono_start, mono_end);   \
        int64_t proc_ns     = TIME_TO_NS(proc_start, proc_end);   \
        double  m           = (double)n;                          \
        double  mono_ns_per = (double)mono_ns / m;                \
        double  proc_ns_per = (double)proc_ns / m;                \
        printf("\n%s%-28s%s\n"                                    \
               "iter.     : %s%9d%s\n"                            \
               "monotonic : %s%9.2f%s %sns/per%s\n"               \
               "process   : %s%9.2f%s %sns/per%s\n",              \
               BOLD_INVERSE,                                      \
               label,                                             \
               CLOSE,                                             \
               BOLD,                                              \
               n,                                                 \
               CLOSE,                                             \
               BOLD,                                              \
               mono_ns_per,                                       \
               CLOSE,                                             \
               DIM,                                               \
               CLOSE,                                             \
               BOLD,                                              \
               proc_ns_per,                                       \
               CLOSE,                                             \
               DIM,                                               \
               CLOSE);                                            \
    }

#endif
