/** @file
 *
 * @author wcc
 * @date 2016-10-19
 * @version 0.5
 *
 * @brief Large Number Operations
 *
 * A char array is employed to denote a large number,
 * low numbers are on the LEFT(e.g. 123 is stored as 
 * ['3', '2', '1']). Thus the unit digit is always at
 * n[0]. Although this is kind of strange, it does 
 * free us from some troubles of alignment.
 */

#ifndef __INCLUDE_LGNUM_H
#define __INCLUDE_LGNUM_H

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>

/** @brief Large Number
 */
template <int width> struct Lgnum {
	/** @brief Large number addition based on char arrays
	 *
	 * It must be guaranteed that both x[] and y[] are not empty string,
	 * namely :
	 *	1. strlen(x) > 0
	 *	2. strlen(y) > 0.
	 *
	 * @param x, y - Operands
	 * @param r - Result of x + y.
	 * @return Length of r[]
	 */
	static int strpls(const char *x, const char *y, char *r) {
		int i, xlen, ylen;
		int tres, carry;

		xlen = strlen(x), ylen = strlen(y);
		carry = 0;
		for (i = 0; i < xlen && i < ylen; ++i) {
			tres = carry + x[i] + y[i] - 96;
			carry = tres / 10;
			r[i] = tres % 10 + 48;
		}

		for (; i < xlen; ++i) {
			tres = carry + x[i] - 48;
			carry = tres / 10;
			r[i] = tres % 10 + 48;
		}

		for (; i < ylen; ++i) {
			tres = carry + y[i] - 48;
			carry = tres / 10;
			r[i] = tres % 10 + 48;
		}

		if (carry != 0) r[i++] = '1';
		r[i] = 0;

		return i;
	}

	/** @brief Large number addition based on Lgnum
	 *
	 * @param x, y - Operands
	 * @param r - Result of x + y
 	 */
	static void pls(const Lgnum &x, const Lgnum &y, Lgnum &r) {
		r.len = strpls(x.num, y.num, r.num);
	}

	char *num; //number string
	int len; // length of num[]

	Lgnum(void): len(0) {
		num = new char[width+1];
	}

	//TODO: test
	~Lgnum(void) {
		/* num is valiad all the time*/
		assert(num);
		delete[] num;
	}

	/** @brief Set num[] with a number string
	 *
	 * <pre>
	 *      str      --->       num
	 * ['1','2','3'] ---> ['3', '2', '1']
	 *</pre>
	 *
	 * @param str - number string of NORMAL order
	 *	e.g. ['1', '2', '3'] for number 123
	 */
	void setv(const char *str) {
		int i, m;
		len = strlen(str);
		std::copy(str, str+len+1, num);
		m = (len-1) / 2;
		for (i = 0; i <= m; ++i)
			std::swap(num[i], num[len-1-i]);
	}

	inline bool operator==(const Lgnum &rival) const {
		if (len != rival.len)
			return false;
		return strcmp(num, rival.num) == 0;
	}

	bool operator <(const Lgnum &rival) const {
		if (len == rival.len) {
			char *s = num+len, *t = rival.num+len;
			while (s > num && (*s == *t)) {
				s--;
				t--;
			};
			/* One of following two conditions are satisfied here:
			 * 1. s == num && t == rival.num
			 * 2. *s != *t
			 * We just compare *s and *t with both conditions.
			 */
			return *s < *t;
		}
		return len < rival.len;
	}

	inline bool operator>(const Lgnum &rival) const {
		return rival < *this;
	}

	inline bool operator>=(const Lgnum &rival) const {
		return !(*this < rival);
	}

	inline bool operator<=(const Lgnum &rival) const {
		return !(*this > rival);
	}

	void print(void) const {
		int len, i;

		len = strlen(num);
		for (i = len - 1; i >= 0; --i)
			printf("%c", num[i]);
		printf("\n");
	}
};

#endif
