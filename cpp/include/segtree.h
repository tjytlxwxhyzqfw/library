/**
 * @author wcc
 *
 * 2016-10-09
 */
#ifndef _TJYTLXWXHYZQFW_SEGTREE_H
#define _TYYTLXWXHYZQFW_SEGTREE_H

#include <cassert>
#include <cstdio>
#include <vector>

struct segnode_base {
	int b, e;

	inline bool p(void) const { return b == e; }
	inline bool eq(int bgn, int end) const { return b == bgn and e == end; }
	inline bool ct(int bgn, int end) const { return b <= bgn and end <= e; }
	inline void itv(int bgn, int end) { b = bgn, e = end; }
	inline bool spl(int bgn, int end) const { return bgn <= (b+e)/2 and e > (b+e)/2; }
	inline int len(void) const { return e - b + 1; }

	void print(int idx) const {
		printf("\t%2d: [%2d, %2d]\n", idx, b, e);
	}
};

template<class segnode_t>
struct segtree_base {
	std::vector<segnode_t> seg;
	segnode_t *arr;
	int size, half;

	inline int prt(int i) const { return i >> 1; }
	inline int li(int i) const { return i << 1; }
	inline int ri(int i) const { return (i << 1) | 1; }
	inline int le(int i) const { return lb(i) ? 0 : prt(i) - (atl(i) ? 1 : 0); }
	inline int re(int i) const { return rb(i) ? 0 : prt(i) + (atr(i) ? 1 : 0); }
	inline bool lb(int i) const { return (i & (i-1)) == 0; }
	inline bool rb(int i) const { return (i & (i+1)) == 0; }
	inline int valiad(int i) const {  return i != 0; }
	inline int lstprt(void) const { return half-1; }
	inline bool atl(int i) const { return (i & 1) == 0; }
	inline bool atr(int i) const { return (i & 1) == 1; }
	inline bool itvld(int i, int b, int e) const { return (b <= e and seg[i].ct(b, e)); }
	inline segnode_t& lc(int i) { return seg[li(i)]; }
	inline segnode_t& rc(int i) { return seg[ri(i)]; }
	inline segnode_t& get(int i) { return seg[i]; }

	void init(int sz) {
		assert(lb(sz));
		seg.reserve(sz);
		size = sz, half = sz / 2;
		arr = seg.data() + half;
	}

	segtree_base& builditv(void) {
		for (int i = 0; i < half; ++i) arr[i].itv(i, i);
		for (int i = lstprt(); i >= 1; --i) seg[i].itv(lc(i).b, rc(i).e);
		return *this;
	}

	segtree_base& print(void) {
		for (int i = 1; i < half; ++i) seg[i].print(i);
		for (int i = 0; i < half; ++i) { printf("%3d: ", i); arr[i].print(i+half); }
		return *this;
	}
};

#endif
