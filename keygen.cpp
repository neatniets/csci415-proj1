#include <stdio.h>
#include <stdlib.h>
#include "key-files.hpp"

static void
print_help(int argc,
           char **argv);

#define USAGE_FSTR       "usage: %s num_digits"
enum args {
        ARGS_PROG_NAME,
        ARGS_NUM_DIGITS
};
int main(int argc, char **argv) {
        /* verify proper number of args */
        if (argc < (ARGS_NUM_DIGITS + 1)) {
                fprintf(stderr,
                        "error: incorrect number of arguments. "
                        "expected %d args but got %d.\n",
                        ARGS_NUM_DIGITS + 1, argc);
                print_help(argc, argv);
                exit(EXIT_FAILURE);
        }
        /* validate argument(s) */
        int num_digits = atoi(argv[ARGS_NUM_DIGITS]);
        if (num_digits <= 0) {
                fprintf(stderr,
                        "error: num_digits must be >0. "
                        "entered value = %d.\n",
                        num_digits);
                exit(EXIT_FAILURE);
        }

        /* try to open key files for writing */
        FILE *priv_fp = fopen(PRIV_FNAME_STR, "w");
        FILE *pub_fp = fopen(PUB_FNAME_STR, "w");
        if ((priv_fp == NULL) || (pub_fp == NULL)) {
                fputs("error: failed to open key files.\n", stderr);
                exit(EXIT_FAILURE);
        }

        return 0;
}

static void
print_help(int argc,
           char **argv) {
        fprintf(stderr, USAGE_FSTR "\n", argv[ARGS_PROG_NAME]);
}
