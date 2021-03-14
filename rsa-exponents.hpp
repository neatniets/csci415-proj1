#ifndef RSA_EXPONENTS_HPP
#define RSA_EXPONENTS_HPP

#include <gmp.h>

/** Find the RSA exponents e and d for a given p and q.
 * @param[in]   p       prime number 1
 * @param[in]   q       prime number 2
 * @pre         p and q must be prime
 * @param[out]  e       public key exponent
 * @param[out]  q       private key exponent */
void
find_rsa_exp(const mpz_t &p,
             const mpz_t &q,
             mpz_t &e,
             mpz_t &d);

#endif /* !RSA_EXPONENTS_HPP */
