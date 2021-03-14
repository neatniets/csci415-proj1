/*
 * C++ Program to Implement Miller Rabin Primality Test
 */

#include <stdio.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <time.h>
#include <gmp.h>

using namespace std;

void RanNum(mpz_t& x, int y);
bool Miller(const mpz_t x, int iteration,int digits);
void modulo(const mpz_t base, const mpz_t exponent, const mpz_t mod, mpz_t &ret);
void mulmod(const mpz_t a, const mpz_t b, const mpz_t mod, mpz_t &ret);

int main()
{
    srand(time(NULL));

    int iteration = 5;
    int y;
    mpz_t x;
    mpz_init(x);
    
	cout<<"Enter how many digits you want to use: ";
	cin>>y;
	
    RanNum(x, y);

    for (int i = 0; !Miller(x, iteration,y); i++) {
        cout << "Attempt #" << i << ": The number is not prime" << endl;
        RanNum(x, y);
    }

    cout << "Prime found: " << endl;
    mpz_out_str(stdout, 10, x);
    fputc('\n', stdout);

    mpz_clear(x);
    return 0;
}

//Random Number generator takes
void RanNum(mpz_t& x, int y) {
    mpz_set_si(x, 0); // x = 0

    int z;
    z = (rand() % 9) + 1;
    mpz_add_ui(x, x, z); // x += z

    for (int i = 1; i < y; i++) {

        mpz_mul_si(x, x, 10); // x *= 10
        z = rand() % 10;
        mpz_add_ui(x, x, z); // x += z

    }

}

//Miller-Rabin primality test, iteration signifies the accuracy
bool Miller(const mpz_t x, int iteration, int digits)
{
	int secNum = digits-1;			//for randomizing the second number
    if (mpz_cmp_si(x, 2) < 0) // x < 2
    {
        return false;
    }

    if ((mpz_cmp_si(x, 2) == 0) // x == 2
        || (mpz_tdiv_ui(x, 2) == 0) // x % 2 == 0
        || (mpz_tdiv_ui(x, 3) == 0) // x % 3 == 0
        || (mpz_tdiv_ui(x, 5) == 0) // x % 5 == 0
        || (mpz_tdiv_ui(x, 7) == 0)) // x % 7 == 0
    {
        return false;
    }

    mpz_t s;
    mpz_init(s);
    mpz_sub_ui(s, x, 1); // s = x - 1

    while (mpz_tdiv_ui(s, 2) == 0) // s % 2 == 0
    {

        mpz_tdiv_q_ui(s, s, 2); // s = s / 2
    }

    mpz_t a;
    mpz_init(a);
    mpz_t temp;
    mpz_init(temp);
    mpz_t mod;
    mpz_init(mod);
    mpz_t x_sub1;
    mpz_init(x_sub1);
    for (int i = 0; i < iteration; i++)
    {
        RanNum(a, secNum);

        while (mpz_cmp(a, x) > 0) { // a > x
            RanNum(a, secNum);              //not the best

        }

        mpz_set(temp, s); // temp = s

        modulo(a, temp, x, mod); //base,expon,mod

        mpz_sub_ui(x_sub1, x, 1); // x - 1
        while ((mpz_cmp(temp, x_sub1) != 0) // temp != x - 1
               && (mpz_cmp_ui(mod, 1) != 0) // mod != 1
               && (mpz_cmp(mod, x_sub1) != 0)) // mod != x - 1
        {

            mulmod(mod, mod, x, mod);

            mpz_mul_si(temp, temp, 2); // temp = temp * 2
        }

        if ((mpz_cmp(mod, x_sub1) != 0) // mod != x - 1
            && (mpz_tdiv_ui(temp, 2) == 0)) // temp % 2 == 0
        {

            mpz_clear(s);
            mpz_clear(a);
            mpz_clear(temp);
            mpz_clear(mod);
            mpz_clear(x_sub1);
            return false;
        }
    }
    mpz_clear(s);
    mpz_clear(a);
    mpz_clear(temp);
    mpz_clear(mod);
    mpz_clear(x_sub1);
    return true;
}

 //modular exponentiation
void modulo(const mpz_t base, const mpz_t exponent, const mpz_t mod, mpz_t &ret)
{

    mpz_t exp_cpy;
    mpz_init_set(exp_cpy, exponent); // exp_cpy = exponent
    mpz_t x;
    mpz_init_set_si(x, 1); // x = 1
    mpz_t y;
    mpz_init_set(y, base); // y = base
    while (mpz_cmp_ui(exp_cpy, 0) > 0) // exp_cpy > 0
    {
        if (mpz_tdiv_ui(exp_cpy, 2) == 1) { // exp_cpy % 2 == 1
            /* x = (x * y) % mod */
            mpz_mul(x, x, y); // x *= y
            mpz_mod(x, x, mod); // x %= mod
        }

        /* y = (y * y) % mod */
        mpz_mul(y, y, y); // y *= y
        mpz_mod(y, y, mod); // y %= mod

        mpz_tdiv_q_ui(exp_cpy, exp_cpy, 2); // exp_cpy /= 2
    }
    mpz_mod(ret, x, mod); // ret = x % mod

    mpz_clear(exp_cpy);
    mpz_clear(x);
    mpz_clear(y);
}


//calculates (a * b) % c
void mulmod(const mpz_t a, const mpz_t b, const mpz_t mod, mpz_t &ret)
{
    mpz_t x;
    mpz_init_set_si(x, 0); // x = 0
    mpz_t y;
    mpz_init(y);
    mpz_tdiv_r(y, a, mod); // y = a % mod
    mpz_t b_cpy;
    mpz_init_set(b_cpy, b); // b_cpy = b
    while (mpz_cmp_si(b_cpy, 0) > 0) // b_cpy > 0
    {
        if (mpz_tdiv_ui(b_cpy, 2) == 1) // b_cpy % 2 == 1
        {
            /* x = (x + y) % mod */
            mpz_add(x, x, y); // x += y
            mpz_mod(x, x, mod); // x %= mod
        }
        /* y = (y * 2) % mod */
        mpz_mul_ui(y, y, 2); // y *= 2
        mpz_tdiv_r(y, y, mod); // y %= mod

        mpz_tdiv_q_ui(b_cpy, b_cpy, 2); // b_cpy /= 2
    }
    mpz_mod(ret, x, mod); // ret = x % mod

    mpz_clear(x);
    mpz_clear(y);
    mpz_clear(b_cpy);
}
