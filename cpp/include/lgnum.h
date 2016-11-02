/**
 * Large Number Operations
 *
 * @author wcc
 * 2016-10-19
 *
 * A char array is employed to denote a large number,
 * low numbers are on the LEFT(e.g. 123 is stored as 
 * ['3', '2', '1']). Thus the unit digit is always at
 * n[0]. Although this is kind of strange, it does 
 * free us from some troubles of alignment.
 *
 * file overview :
 *
 * template <int width> struct lgnum {
 *	static strpls()
 *	static pls()
 *
 *	num, len
 *
 *	lgnum()
 *	setv()
 *
 *	operator==()
 *	operator<()
 * 	operator>()
 * 	operator>=()
 *	operator<=()
 *
 *	print()
 * }
 *
 */

#ifndef __INCLUDE_LGNUM_H
#define __INCLUDE_LGNUM_H

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>

template <int width> struct lgnum {
	/**
	 * large number addition based on char arrays
	 *
	 * It must be guaranteed that both x[] and y[] are not empty string,
	 * namely :
	 *	1. strlen(x) > 0
	 *	2. strlen(y) > 0.
	 *
	 * @param x, y - operands
	 * @param r - result of x + y.
	 * @return - length of r[]
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

	/**
	 * large number addition based on 'lgnum'
	 *
	 * @param x, y - operands
	 * @param r - result of x + y
 	 */
	static void pls(const lgnum &x, const lgnum &y, lgnum &r) {
		r.len = strpls(x.num, y.num, r.num);
	}

	char *num; //number string
	int len; // length of num[]

	lgnum(void):num(NULL), len(0) {
		num = new char[width+1];
	}

	//TODO: destructor

	/**
	 * set num[] with a number string
	 *
	 *      str      --->       num
	 * ['1','2','3'] ---> ['3', '2', '1']
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

	inline bool operator==(const lgnum &rival) const {
		if (len != rival.len)
			return false;
		return strcmp(num, rival.num) == 0;
	}

	bool operator <(const lgnum &rival) const {
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

	inline bool operator>(const lgnum &rival) const {
		return rival < *this;
	}

	inline bool operator>=(const lgnum &rival) const {
		return !(*this < rival);
	}

	inline bool operator<=(const lgnum &rival) const {
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
