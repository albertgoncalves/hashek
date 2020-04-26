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

/* NOTE: See `https://stackoverflow.com/a/13498238`. */

#define DEV_NULL "/dev/null"
#define SUPPRESS_STDOUT(block)                      \
    {                                               \
        fflush(stdout);                             \
        int   restore_stdout = dup(fileno(stdout)); \
        FILE* null_stdout = fopen(DEV_NULL, "w");   \
        EXIT_IF(null_stdout == NULL);               \
        dup2(fileno(null_stdout), fileno(stdout));  \
        block;                                      \
        fflush(stdout);                             \
        fclose(null_stdout);                        \
        dup2(restore_stdout, fileno(stdout));       \
    }

#endif
