/** @file
 *
 * @author wcc
 * @date 2016-11-03
 * @version 0.1
 *
 * @brief Some Useful Helpers
 *
 *
 * Overview :
 *
 * 	template struct skipper {
 * 		bgn, end, cur, next
 *		st
 *
 *		init() - 
 *		check_from() - 
 *		nxt() - 
 *		mov() - 
 *		idx() - 
 * 	}
 */

#ifndef __INCLUDE_HELPER_H
#define __INCLUDE_HELPER_H

#include <algorithm>
#include <cstdio>

/** @brief Next element statifying the given condition.
 *
 * @par Example
 *
 * This example shows how to copy positive integers from vector<int> a to b
 * with skipper
 *
 * @code
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
 * @endcode
 *
 * Or you could use 'for' loop instead of 'while':
 *
 * @code
 *	for (skp.init(a.begin(). a.end()); skp.nxt(); skp.mov())
 *		b.push_ack(*skp.cur)
 * @endcode
 *
 *
 * @see Codeforces Round #375 C
 */
template <class Iterator, class Function> struct skipper {
	Iterator bgn, end, cur, next;
	Function st;

	/** @brief Bind skpper to a range and point current pointer to the first target in this range
	 */
	inline void init(const Iterator b, const Iterator e) { 
		bgn = b;
		end = e;
		check_from(bgn);
	}

	/** @brief Move current pointer to the next target, stats from arbitrary pointer
	 */
	bool check_from(Iterator from) {
		for (next = from; next != end; ++next) {
			if (st(next))
				break;
		}
		cur = next++;
		return cur != end;
	}
	/** @brief Move current pointer to the next target, starts from current pointer
	 */
	inline bool nxt(void) {
		return check_from(cur);
	}

	/** @brief Move current pointer to the next target, starts from next pointer
	 */
	inline bool mov(void) {
		return check_from(next);
	}

	/** @brief Offset of current pointer
	 */
	inline int idx(void) const {
		return static_cast<int>(cur - bgn);
	}
};

#endif
