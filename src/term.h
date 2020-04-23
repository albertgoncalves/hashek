#ifndef __TERM_H__
#define __TERM_H__

#include <unistd.h>

/* NOTE: See `https://askubuntu.com/a/558422`. */

#define BOLD           "\033[1m"
#define DIM            "\033[2m"
#define BOLD_INVERSE   "\033[1;30;107m"
#define BOLD_PURPLE    "\033[1;35m"
#define BOLD_UNDERLINE "\033[1;4m"
#define CLOSE          "\033[0m"

#define FLUSH_ALL       \
    {                   \
        fflush(stdout); \
        fflush(stderr); \
    }

/* NOTE: See `https://stackoverflow.com/a/13498238`. */

#define DEV_NULL "/dev/null"
#define SUPPRESS(block)                            \
    {                                              \
        FLUSH_ALL;                                 \
        int   pin_stdout  = dup(fileno(stdout));   \
        int   pin_stderr  = dup(fileno(stderr));   \
        FILE* null_stdout = fopen(DEV_NULL, "w");  \
        EXIT_IF(null_stdout == NULL);              \
        dup2(fileno(null_stdout), fileno(stdout)); \
        FILE* null_stderr = fopen(DEV_NULL, "w");  \
        EXIT_IF(null_stderr == NULL);              \
        dup2(fileno(null_stderr), fileno(stderr)); \
        {                                          \
            block;                                 \
        }                                          \
        FLUSH_ALL;                                 \
        fclose(null_stdout);                       \
        fclose(null_stderr);                       \
        dup2(pin_stdout, fileno(stdout));          \
        dup2(pin_stderr, fileno(stderr));          \
    }

#endif
