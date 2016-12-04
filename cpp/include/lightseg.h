/** @file
 *
 * @author wcc
 * @date 2016-11-20
 * @version 0.1-SNAPSHOT
 *
 * @brief Lightweight Segment Tree
 *
 * This is for single point updating only.
 *
 * There are no ready-made functions or classes for query as
 * it's too complicated. But there are some carefully designed
 * tools(such as Segtree::Pos) that could simplify your programing
 * for query procedure.
 *
 * @par Nodes
 *
 *	To avoid runtime error, nodes of Segtree must
 *	define/implement variables and methods lists as following,
 *	for that they are all used by methods in Segtree.
 * @code
	struct Segnode {
 * 		//used y Segnode#build, Segnode::Pos#at
 * 		Segitv itv;
 *
 * 		//used by Segnode#{build,update}
 *		template <class Data> void create(int i, Data *a, int n);
 *		void update(const Node &left, const Node &rght);
 *
 *		//used by Segnode#print 
 *		void print(int segidx);
 * 	}
 * @endcode
 * 
 * @par References
 *
 *	Generally, I found that there are three types of query with segment tree:
 *
 *	- root - Answer remains in root all the time
 *	- reducing - Check and reduce some intervals sequencly from left to right
 *	- merging - you must locate a point first, then accumulate the answer
 *		with a bottom-up approch.
 *
 *	The following are problems having been solved with this library:
 *
 * 	1. C, Codeforces Intel Code Channenge Elimination Round, root
 * 	2. 1166, HDOJ, reducing
 *	3. 1540, HDOJ, merging
 *	4. 1754, HDOJ, merging
 *	5. 3308, HDOJ, merging/reduce
 *
 * @see online-judge/hdoj/10-SegmentTree/2
 *
 */

/** @brief Round a number to a power of 2
 * 
 * @param n - Non-negetive number which must be smaller than INT_MAX
 * @return First power of 2 which is great than n.
 */
int round2power(const int n) {
	int i = n, j = 1;
	while (i != 0)
		j <<= 1, i >>= 1;
	return j;
}

/** @brief Interval
 *
 * TODO This class could be more general
 */
struct Segitv {
	int b, e;

	inline void set(const int b, const int e) {
		this->b = b;
		this->e = e;
	}

	inline bool p(void) const {
		return b == e;
	}

	inline bool ct(const int b, const int e) const {
		return this->b <= b && e <= this->e;
	}

	inline bool eq(const int b, const int e) const {
		return this->b == b && e == this->e;
	}

	inline int len(void) const {
		return e - b + 1;
	}
};

/** @brief Segment Tree
 *
 */
template <class Node> struct Segtree {
	/** @brief Position in Segtree
	 *
	 * Configure some variables indicating the relavent positions
	 * with the given index.
	 */
	struct Pos {
		Node *curr, *left, *rght;
		int curri, lefti, rghti;

		Pos(void) {
			curr = left = rght = NULL;
			curri = lefti = rghti = 0;
		}

		void at(const int segidx, const Segtree &seg) {
			curri = segidx;
			lefti = (segidx << 1);
			rghti = ((segidx << 1) | 1);
			curr = &seg[curri];
			if (!curr->itv.p()) {
				left = &seg[lefti];
				rght = &seg[rghti];
			}
		}
	};

	Node *seg, *arr;
	int cap, round;

	Segtree(void):
		seg(NULL),
		arr(NULL),
		cap(0),
		round(0) {
	}

	/* kind of subtle */
	~Segtree(void) {
		if (seg != NULL)
			delete seg;
	}

	template <class Data> void build(const Data *a, const int n) {
		int i;

		/* calculate size of both segment array and data array */
		round = round2power(n);
		cap = round << 1;

		/* allocate memory */
		assert(seg == NULL && arr == NULL);
		seg = new Node[cap];
		arr = seg + round;

		/* create nodes in data array region */
		for (i = 0; i < round; ++i) {
			//FIXME: This may incur bugs
			seg[i+round].itv.set(i, i);
			/* cpp is smart enough thus we needn't specify template type here */
			seg[i+round].create(i, a, n);
		}

		/* updade parents with already created primary nodes*/
		for (i = round-1; i > 0; --i) {
			//FIXME: This may incur bugs
			seg[i].itv.set(seg[2*i].itv.b, seg[2*i+1].itv.e);
			seg[i].update(seg[2*i], seg[2*i+1]);
		}
	}

	/** @brief Update tree
	 *
	 * This is to be invoked right after a point is updated,
	 * and then update all anscestors of the changed point.
	 */
	void update(int segidx) {
		for (int i = segidx / 2; i > 0; i /= 2)
			seg[i].update(seg[2*i], seg[(2*i+1)]);
	}

	void clear(void) {
		if (seg != NULL)
			delete seg;
		seg = arr = NULL;
		cap = round = 0;
	}

	Node& operator[](int idx) const {
		return seg[idx];
	}

	void print(char *title=NULL) const {
		int i;
		if (title != NULL)
			printf("%s\n", title);
		for (i = 1; i < round; ++i)
			seg[i].print(i);
		for (i = round; i < cap; ++i) {
			printf("%3d: ", i-round);
			seg[i].print(i);
		}
	}
};

