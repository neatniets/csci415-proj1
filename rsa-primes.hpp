#ifndef RSA_PRIMES_HPP
#define RSA_PRIMES_HPP

#include <gmp.h>

/** Find primes p and q for RSA of a specified digit size.
 * @param[out]  p               prime 1
 * @param[out]  q               prime 2
 * @param[in]   num_digits      number of decimal digits in each prime */
void
find_primes(mpz_t &p,
            mpz_t &q,
            int num_digits);

#endif /* !RSA_PRIMES_HPP */
