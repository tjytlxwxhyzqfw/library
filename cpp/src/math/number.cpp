/**
 * @author wcc
 */
#include <algorithm>
#include <cassert>
#include <climits>

#include "../../include/number.h"

using namespace std;
using namespace number;

long long do_gcd(long long a, long long b, long long &x, long long &y);

inline long long number::correct(long long value, long long module)
{
	if (value < 0)
		value += ((-1*value)/module+1) * module;
	return value % module;
}

long long number::quickpow(long long x, long long n, long long module)
{
	llong square, ans;

	assert(x >= 0 && n >= 0 && module > 0);

	if (n == 0)
		return 1;
	if (x == 0)
		return 0;

	square = llong(x, module);
	ans = llong(1, module);

	for (int i = 0; i < 64; ++i) {
		if (n & 1UL)
			ans *= square;
		n >>= 1;
		square *= square;
	}

	return ans.toll();
}

long long number::gcd(long long a, long long b, long long &x, long long &y)
{
	if (a == 0 && b == 0) {
		x = y = 0;
		return 0;
	}

	a = abs(a);
	b = abs(b);

	if (a < b) {
		swap(a, b);
		swap(x, y);
	}

	return do_gcd(a, b, x, y);
}

long long number::gcd(long long a, long long b)
{
	long long foo;
	return gcd(a, b, foo, foo);
}

long long number::perm(long long n, long long k, long long mod)
{
	llong a, b;

	assert(n >= 0 && k >= 0 && mod > 0);

	if (n == 0 && k == 0)
		return 1;
	if (k > n)
		return 0;

	a = llong(1, mod);
	b = llong(k, mod);

	for (long long i = 0; i < k; ++i)
		a *= n - i;

	return a.toll();
}

long long number::fact(long long n, long long mod)
{
	return perm(n, n, mod);
}

long long number::comb(long long n, long long m, long long mod)
{
	llong top(0, mod), bot(0, mod);

	top = perm(n, m, mod);
	bot = fact(m, mod);

	return (top * bot.inv()).toll();
}

llong::llong(void)
{
	value = 0;
	module = LLONG_MAX;
}

llong::llong(const long long value, const long long module)
{
	assert(module > 0);
	this->module = module;
	this->value = correct(value, module);
}

long long llong::pow(long long n) const
{
	return quickpow(value, n, module);
}

long long llong::inv(void) const
{
	return quickpow(value, module-2, module);
}

long long llong::toll(void) const
{
	return value;
}

long long llong::mod(void) const
{
	return module;
}

llong::operator long long () const
{
	return value;
}

llong llong::operator=(long long rhs)
{
	value = correct(rhs, module);
	return *this;
}

llong llong::operator=(const llong & rhs)
{
	module = rhs.mod();
	value = rhs.toll();
	return *this;
}

llong llong::operator+(long long rhs) const
{
	return llong(rhs + value, module);
}

llong llong::operator-(long long rhs) const
{
	return llong(value - rhs, module);
}

llong llong::operator*(long long rhs) const
{
	return llong((value * correct(rhs, module)) % module, module);
}

llong llong::operator+=(long long rhs)
{
	this->value = correct(value + rhs, module);
	return *this;
}

llong llong::operator-=(long long rhs)
{
	this->value = correct(value - rhs, module);
	return *this;
}

llong llong::operator*=(long long rhs)
{
	this->value = correct(value * rhs, module);
	return *this;
}

llong llong::operator+(const llong & rhs) const
{
	assert(module == rhs.mod());
	return *this + rhs.toll();
}

llong llong::operator-(const llong & rhs) const
{
	assert(module == rhs.mod());
	return *this - rhs.toll();
}

llong llong::operator*(const llong & rhs) const
{
	assert(module == rhs.mod());
	return *this * rhs.toll();
}

llong llong::operator+=(const llong &rhs)
{
	return *this += rhs.toll();
}

llong llong::operator-=(const llong &rhs)
{
	return *this -= rhs.toll();
}

llong llong::operator*=(const llong &rhs)
{
	return *this *= rhs.toll();
}

long long do_gcd(long long a, long long b, long long &x, long long &y)
{
	long long d;

	if (b == 0) {
		x = 1;
		y = 0;
		return a;
	}

	d = do_gcd(b, a % b, x, y);
	swap(x, y);
	y -= a / b * x;

	return d;
}
