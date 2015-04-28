/*
 *  An alternative and simple multiplication algorithm which works in any base.
 *  Copyright (C) 2015 Franz-Josef Anton Friedrich Haider
 *  Copyright (C) 2015 Lorenz Oberhammer
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <vector>
#include <cmath>

#if USE_GMP
#include <gmpxx.h>
typedef mpz_class number;
typedef unsigned long int digit_counter;
#else
typedef unsigned long long int number;
typedef unsigned long int digit_counter;
#endif

using namespace std;

/* this function returns the maximum number of digits after multiplication of a and b */
digit_counter max_num_of_digits_after_multiplication(const vector<number> &a, const vector<number> &b)
{
    return a.size() + b.size();
}

/* this function multiplies a with b and returns a*b, where a and b are numbers
 * represented as vector of their digits, the return value is also represented
 * in the same way.
 */
vector<number> multiply(const vector<number> &a, const vector<number> &b, const number &base)
{
    vector<number> result;

    // multiplication
    number carry = 0;

    for(digit_counter d = 0;d <= max_num_of_digits_after_multiplication(a, b);d++)
    {
        number tmp = 0;

        for(digit_counter i = 0;i <= d;i++)
        {
            if(!(i >= a.size() || d-i >= b.size()))
            {
                tmp += a[i] * b[d-i];
            }
        }

        tmp += carry;
        carry = tmp / base;

        result.push_back(tmp % base);
    }

    // cleanup
    for(vector<number>::size_type i = result.size() - 1;i >= 0;i--)
    {
        if(result[i] == 0)
        {
            result.pop_back();
        }
        else
        {
            break;
        }
    }

    return move(result);
}

/* this function returns x^y */
#if USE_GMP
number my_pow(const number &x, const digit_counter &y)
{
    mpz_class r;
    mpz_pow_ui(r.get_mpz_t(), x.get_mpz_t(), y);
    return r;
}
#else
#define my_pow pow
#endif

/* this function returns the digit_counter-th digit of x in base base) */
number get_digit(const number &x, const digit_counter &digit_number, const number &base)
{
    if(base == 2)
    {
        return (x >> digit_number) & 1;
    }
    else
    {
        return (x / (number)my_pow(base, digit_number)) % base;
    }
}

/* this function returns the number of digits of x in base base */
digit_counter num_of_digits(number x, const number &base)
{
    digit_counter digits = 0;
    while(x) {
        x /= base;
        digits++;
    }
    return digits;
}

int main(int argc, char *argv[])
{
    number base;
    number a, b;
    std::vector<number> av, bv, result;

    if(argc != 4)
    {
        std::cerr << "please specify three (positive integer) arguments (base, a and b)" << std::endl;
        return -1;
    }

#if USE_GMP
    base = argv[1];
    a = argv[2];
    b = argv[3];
#else
    base = strtoull(argv[1], NULL, 10);
    a = strtoull(argv[2], NULL, 10);
    b = strtoull(argv[3], NULL, 10);
#endif

    for(digit_counter i = 0;i < num_of_digits(a, base);i++)
    {
        av.push_back(get_digit(a, i, base));
    }

    for(digit_counter i = 0;i < num_of_digits(b, base);i++)
    {
        bv.push_back(get_digit(b, i, base));
    }

    result = multiply(av, bv, base);

    for(std::vector<number>::size_type j = 0;j < result.size();j++)
    {
        std::cout << "digit " << j << " of a * b in base " << base << " is " << result[j] << std::endl;
    }

    return 0;
}

