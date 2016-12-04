/** @file
 *
 * @author wcc
 * @date 2016-10-26
 * @version 1.01
 *
 * @brief Number Theory And Combinatorics Algorithms.
 */

#ifndef __INCLUDE_NUMBER_H
#define __INCLUDE_NUMBER_H

#include <algorithm>

static long long do_gcd(const long long, const long long, long long *, long long *);

/** @brief Extended greast common divisor algorithm
 *
 * @param left, rght - operands
 * @param x, y - output parameters storing coefficients used in egcd algorithm
 * @return greast common divisor of <strong>absolute</strong> values of 'left' and 'rght',
 *	return 0 if 'left' = 'rght' = 0.
 */
inline long long gcd(const long long left, const long long rght, long long *x, long long *y) {
	long long a = left, b = rght;
	if (a < 0)
		a *= -1;
	if (b < 0)
		b *= -1;
	/* gcd(0, 0) = 0 */
	if (a == 0 && b == 0)
		return *x = *y = 0;
	if (a < b) {
		std::swap(a, b);
		std::swap(x, y);
	}
	return do_gcd(a, b, x, y);
}

inline long long gcd(const long long a, const long long b) {
	long long x, y;
	return gcd(a, b, &x, &y);
}

/** @brief Simple primality test
 * @param n
 * @return 'true' if n is prime else 'false'
 */
bool isprime(const long long n) {
	assert(n >= 0);
	if (n == 1 || n == 0) return false;
	if (n == 2) return true;

	/* Note that we used '<=', must not use '<' */ 
	for (long long i = 2; i * i <= n; ++i)
		if (n % i == 0)
			return false;
	return true;
}

/** @brief Modulo Util
 */
template <long long mod> struct moduler {
	/**
	 * Correct a number x into [0, mod)
	 */
	inline static long long crc(const long long x) {
		if (x >= 0)
			return x % mod;
		return ((-1*x/mod+1)*mod+x) % mod;
	}

	/** @brief Modulo addition
	 *
	 * Note that opreands must be <strong>non-negative</strong> number
	 */
	inline static long long pls(const long long x, const long long y) {
		return ((x%mod)+(y%mod))%mod;
	}

	/** @brief Modulo X
	 *
	 * Note that oprands must be <stong>non-negative</strong> number
	 */
	inline static long long mul(const long long x, const long long y) {
		return ((x%mod)*(y%mod))%mod;
	}

	/** @brief Modulo quick power algorithm
	 *
	 * @param x - base number
	 * @param y - exponent
	 * @return x<sup>y</sup> % mod
	 */
	static long long pow(const long long x, const long long y) {
		long long n = y;
		moduler square, ans;

		ans = 1;
		square = x;
		while (n != 0) {
			if ((n & (long long)1UL) == 1)
				ans = ans * square;
			square = square * square;
			n >>= 1;
		}

		return ans.ll();
	}

	/** @brief Moduler Linear Equation Solver
	 * 
	 * ax = b (mod n)
	 *
	 * Let d = gcd(a, n).
	 * If d is divisible by b, we got d solutions.
	 * Otherwise, no solution.
	 *
	 * @param a
	 * @param b
	 * @param d - *d = gcd(a, mod)
	 * @return any one of the solution(s), -1 if no solution.
	 *
	 * @par Deduction
	 *
 	 * By applying gcd() on (a, n) we get:
	 * >	ax + ny = d
	 * Since d | b:
	 * >	ax + ny = d = b/m
	 * >	m = b / d 
	 * Thus:
	 * >	amx + nmy = b
	 * Yet we get an solution:
	 * >	x<sub>0</sub> = xb/d
	 * Furthermore:
	 * >	x<sub>i</sub> = x<sub>0</sub>+i(n/d) (mod n), i = 0, 1, ..., d-1
	 *
	 * @par Usage
	 *
	 * Use following loop to for accessing all solutions
	 * @code
	 * x0 = moduler<mod>::eqt(a, b, &d);
	 * assert(x0 != -1);
	 * forn(i, d) {
	 *	x = moduler<mod>::pls(x0+i*(n/d));
	 *	//...
	 *}
	 *@endcode
	 */
	inline static long long eqt(const long long a, const long long b, long long *d) {
		long long x, y;
		*d = gcd(a, mod, &x, &y);
		if (b % (*d) != 0)
			return -1; //no solution
		return crc((x*b/(*d)));
	}

	long long num;

	inline const long long correct(void) {
		return num = crc(num);
	}

	/*
	 * Operators =, + and * are overrided here.
	 * Each of the opreators are overrided twice: one for 'moduler<>'
	 * and another for 'long long'.
	 */

	inline const long long operator=(const long long x) {
		return num = crc(x);
	}

	inline const long long operator=(const moduler &x) {
		return num = x.num;
	}

	inline const long long operator+(const long long x) const {
		return pls(num, correct(x));
	}

	inline const long long operator+(const moduler &x) const {
		return pls(num, x.ll());
	}

	inline const long long operator*(const long long x) const {
		return mul(num, correct(x));
	}

	inline const long long operator*(const moduler &x) const {
		return mul(num, x.ll());
	}

	/* 
	 * Cast a 'moduler<>' instance to 'long long'.
	 * Or just access instance.num
	 */
	inline const long long ll(void) const {
		return num;
	}
};

/* implementations */

static inline long long do_gcd(const long long a, const long long b, long long *x, long long *y) {
	if (b == 0) {
		*x = 1;
		*y = 0;
		return a;
	}
	long long d = do_gcd(b, a%b, x, y);
	std::swap(*x, *y);
	*y -= a / b * (*x);

	return d;
}

#endif
