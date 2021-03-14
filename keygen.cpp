#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "key-files.hpp"
#include "rsa-exponents.hpp"
#include "rsa-primes.hpp"

/** Write the RSA key pair to a file.
 * @param[in]   fp      file pointer to write to
 * @param[in]   key_exp key exponent
 * @param[in]   key_mod key modulus
 * @pre         fp is a valid & open file pointer
 * @pre         key_exp and key_mod are a valid key pair */
static void
write_key(FILE *fp,
          const mpz_t key_exp,
          const mpz_t key_mod);
/** Print a line of help about program usage.
 * @param[in]   argc    argc from main
 * @param[in]   argv    argv from main */
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

        /* get primes */
        mpz_t p;
        mpz_init(p);
        mpz_t q;
        mpz_init(q);
        find_primes(p, q, num_digits);
        /*fputs("p = ", stdout);
        mpz_out_str(stdout, 10, p);
        fputc('\n', stdout);
        fputs("q = ", stdout);
        mpz_out_str(stdout, 10, q);
        fputc('\n', stdout);*/

        /* get exponents */
        mpz_t e;
        mpz_t d;
        find_rsa_exp(p, q, e, d);
        /*fputs("e = ", stdout);
        mpz_out_str(stdout, 10, e);
        fputc('\n', stdout);
        fputs("d = ", stdout);
        mpz_out_str(stdout, 10, d);
        fputc('\n', stdout);*/

        /* get modulus */
        mpz_t n;
        mpz_init(n);
        mpz_mul(n, p, q); // n = p * q
        /*fputs("n = ", stdout);
        mpz_out_str(stdout, 10, n);
        fputc('\n', stdout);*/

        /* print keys to files */
        write_key(priv_fp, d, n);
        write_key(pub_fp, e, n);

        mpz_clear(p);
        mpz_clear(q);
        mpz_clear(e);
        mpz_clear(d);
        mpz_clear(n);
        fclose(priv_fp);
        fclose(pub_fp);
        return 0;
}

static void
write_key(FILE *fp,
          const mpz_t key_exp,
          const mpz_t key_mod) {
        mpz_out_str(fp, 10, key_exp); // write exponent
        fputc('\n', fp); // newline separator
        mpz_out_str(fp, 10, key_mod); // write modulus
        fputc('\n', fp); // eof newline
}

static void
print_help(int argc,
           char **argv) {
        fprintf(stderr, USAGE_FSTR "\n", argv[ARGS_PROG_NAME]);
}
