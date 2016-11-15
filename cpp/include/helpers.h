/** Some Useful Helpers
 *
 * @author wcc
 * create : 2016-11-03
 * modified: 2016-11-03
 *
 * overview :
 *
 * template struct skipper {
 * 	bgn, end, cur, next
 *	st
 *
 *	init() - set a range and point current pointer to the first target
 *	check_from() - move current pointer to the next target, stats from arbitrary pointer
 *	nxt() - move current pointer to the next target, starts from current pointer
 *	mov() - move current pointer to the next target, starts from next pointer
 *	idx() - offset of current pointer
 * }
 */

#ifndef __INCLUDE_HELPER_H
#define __INCLUDE_HELPER_H

#include <algorithm>
#include <cstdio>

/** Next element statifying the given condition.
 *
 * Example:
 *	//copy positive integers from vector<int> a to b
 *
 *	//We need a unary_function first.
 *	class pos: public unary_function<vector<int>::iterator, bool> {
 *		bool operator()(vector<int>::iterator it) {
 *			return *it > 0;
 *		}
 *	}
 *
 *	skipper<vector<int>::iterator, pos> skp;
 *
 *	skp.init(a.begin(), a.end())
 *	while (skp.nxt()) {
 *		b.push_back(*skp.cur);
 *		skp.mov();
 *	}
 *
 *	//or:
 *	for (skp.init(a.begin(). a.end()); skp.nxt(); skp.mov())
 *		b.push_ack(*skp.cur)
 *
 *
 * @see cf-r375-c
 */
template <class Iterator, class Function> struct skipper {
	Iterator bgn, end, cur, next;
	Function st;

	inline void init(const Iterator b, const Iterator e) { 
		bgn = b;
		end = e;
		check_from(bgn);
	}

	bool check_from(Iterator from) {
		for (next = from; next != end; ++next) {
			if (st(next))
				break;
		}
		cur = next++;
		return cur != end;
	}

	inline bool nxt(void) {
		return check_from(cur);
	}

	inline bool mov(void) {
		return check_from(next);
	}

	inline int idx(void) const {
		return static_cast<int>(cur - bgn);
	}
};

#endif
