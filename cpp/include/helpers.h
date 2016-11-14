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
 *	init()
 *	check_from()
 *	nxt()
 *	mov()
 *	idx()
 * }
 */

#ifndef __INCLUDE_HELPER_H
#define __INCLUDE_HELPER_H

#include <algorithm>
#include <cstdio>

/** Next element statifying the given condition.
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
