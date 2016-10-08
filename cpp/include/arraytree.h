/**
 * @author wcc
 */

#include <cassert>

/* common */
int prt(int child);
int lc(int parent);
int rc(int parent);
int valiad(int index);

namespace segutils
{
	/* segment tree */

	bool atleft(int child);
	bool atright(int child);
	bool leftbound(int index);
	bool rightbound(int index);
	/* use valiad() to check return value */
	int leftelder(int child);
	int rightelder(int child);

};

/* common */
inline int prt(int i)
{
	return i >> 1;
}

inline int lc(int i)
{
	return i << 1;
}

inline int rc(int i)
{
	return (i << 1) | 1;
}

inline int valiad(int i)
{
	return i != 0;
}


namespace segutils
{
	/* segment tree */

	inline bool atleft(int i)
	{
		return (i & 1) == 0;
	}

	inline bool atright(int i)
	{
		return (i & 1) == 1;
	}

	inline bool leftbound(int i)
	{
		return (i & (i-1)) == 0;
	}
	
	inline bool rightbound(int i)
	{
		return (i & (i+1)) == 0;
	}

	inline int leftelder(int i)
	{
		if (leftbound(i)) return 0;
		if (atleft(i)) return prt(i) - 1;
		if (atright(i)) return prt(i);
		assert(false);
		return 0;
	}

	inline int rightelder(int i)
	{
		if (rightbound(i)) return 0;
		if (atright(i)) return prt(i) + 1;
		if (atleft(i)) return prt(i);
		assert(false);
		return 0;
	}

}
