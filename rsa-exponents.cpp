#include <stdarg.h>
#include "rsa-exponents.hpp"

/** Find Euler totient phi(n) from p and q.
 * @param[in]   p       prime number 1
 * @param[in]   q       prime number 2
 * @pre         p and q must be prime
 * @param[out]  phi_n   Euler totient phi(n) */
static void
find_phi_n(const mpz_t &p,
           const mpz_t &q,
           mpz_t &phi_n);

/** Find the greatest common divisor and Bezout coefficients for a and b.
 * Bezout's identity: ax + by = gcd(a,b). When a & b are coprime, a*x % b = 1
 * and b*y % a = 1.
 * @param[in]   a       number 1
 * @param[in]   b       number 2
 * @param[out]  x       Bezout coefficient 1
 * @param[out]  y       Bezout coefficient 2
 * @param[out]  gcd     greatest common divisor */
static void
extended_euclid(const mpz_t &a,
                const mpz_t &b,
                mpz_t &x,
                mpz_t &y,
                mpz_t &gcd);

void
find_rsa_exp(const mpz_t &p,
             const mpz_t &q,
             mpz_t &e,
             mpz_t &d) {
        /* initialize e & d; don't care about old values */
        mpz_inits(d, e, NULL);
        /* calc phi(n); apparently it also works to calculate the Carmichael
         * totient function lambda(n) */
        mpz_t phi_n;
        find_phi_n(p, q, phi_n);
        /* choose e; the best e values have a short bitlength and low hamming
         * weight, making encryption faster. a small e value also guarantees
         * that d be very large and harder to crack */
        mpz_set_si(e, 65537);
        /* 65537 = 2^16 + 1; 17 bits, hamming weight of 2 (common value) */
        /* calc d */
        mpz_t y;
        mpz_t gcd;
        /* e and phi_n are coprime (as e above is actually prime), so x will be
         * the modular multiplicative inverse of a modulo b -> e*x % phi_n = 1,
         * so x is d */
        extended_euclid(e, phi_n, d, y, gcd);
        /* d can return negative, which simply requires adding phi(n) to it to
         * make it positive and fulfill (e*d) % phi(n) = 1 */
        while (mpz_cmp_si(d, 0) < 0) {
                mpz_add(d, d, phi_n);
        }
        /* free memory */
        mpz_clears(phi_n, y, gcd, NULL);
}

static void
find_phi_n(const mpz_t &p,
           const mpz_t &q,
           mpz_t &phi_n) {
        mpz_t pp, qq, one; // tmp vars for subtracting one
        /* initialize */
        mpz_inits(phi_n, pp, qq, one, NULL);
        mpz_set_si(one, 1); // subtractor
        /* calculation */
        mpz_sub(pp, p, one); // p - 1
        mpz_sub(qq, q, one); // q - 1
        mpz_mul(phi_n, pp, qq); // (p-1)*(q-1)
        /* free unneeded variables */
        mpz_clears(pp, qq, one, NULL);
}

static void
extended_euclid(const mpz_t &a,
                const mpz_t &b,
                mpz_t &x,
                mpz_t &y,
                mpz_t &gcd) {
        /* initialize outputs */
        mpz_inits(x, y, gcd, NULL);
        /* euclidean algorithm only maintains the current remainder and the
         * previous remainder */
        mpz_t rem[2];
        mpz_init_set(rem[0], a);
        mpz_init_set(rem[1], b);
        /* extended euclidean does the same for Bezout coefficients */
        mpz_t bez_a[2];
        mpz_init_set_si(bez_a[0], 1);
        mpz_init_set_si(bez_a[1], 0);
        mpz_t bez_b[2];
        mpz_init_set_si(bez_b[0], 0);
        mpz_init_set_si(bez_b[1], 1);

        mpz_t quo, tmp; // quotient + tmp variable
        mpz_inits(quo, tmp, NULL);
        /* gcd is found when the latest remainder is 0 */
        while (mpz_cmp_si(rem[1], 0) != 0) {
                /* calculate quotient + remainder */
                mpz_tdiv_qr(quo, tmp, rem[0], rem[1]);
                /* shift remainders */
                mpz_set(rem[0], rem[1]);
                mpz_set(rem[1], tmp);

                /* repeat for Bezout coeffs */
                mpz_set(tmp, bez_a[0]);
                mpz_submul(tmp, quo, bez_a[1]);
                mpz_set(bez_a[0], bez_a[1]);
                mpz_set(bez_a[1], tmp);

                mpz_set(tmp, bez_b[0]);
                mpz_submul(tmp, quo, bez_b[1]);
                mpz_set(bez_b[0], bez_b[1]);
                mpz_set(bez_b[1], tmp);
        }
        /* the Bezout coefficients in the same index as the greatest common
         * divisor are the final coeffs */
        mpz_set(x, bez_a[0]);
        mpz_set(y, bez_b[0]);
        /* the remainder previous to 0 is the greatest common divisor; gcd(0,0)
         * is defined to be 0 */
        mpz_set(gcd, rem[0]);
        /* free memory */
        mpz_clears(rem[0], rem[1], bez_a[0], bez_a[1], bez_b[0], bez_b[1],
                   quo, tmp, NULL);
}
