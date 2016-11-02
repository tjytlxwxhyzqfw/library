/**
 * Prefix Tree (Trie)
 *
 * @author wcc
 * 2016-10-14
 */

#ifndef __INCLUDE_PRFTREE_H
#define __INCLUDE_PRFTREE_H

#include <algorithm>
#include <cstdio>
#include <cstring>

struct prftree {
	struct node {
		int cnt;
		node *nexts;

		node(): cnt(-1), nexts(NULL) {}

		inline node& init(void) {
			cnt = 0;
			return *this;
		}

		inline node& alloc_nexts(const int len) {
			nexts = new node[len];
			return *this;
		}

		inline bool has_next(int i) const {
			return nexts != NULL && nexts[i].cnt >= 0;
		}
		

		void print(int cidx, int bas) const {
			printf("%c(cnt: %2d)\n", cidx+bas, cnt);
		}

		void print(int cidx) const {
			printf("%3d(cnt: %2d)\n", cidx, cnt);
		}
	};

	int bas, len;
	node *root;

	inline prftree& init(const int base, const int length) {
		bas = base;
		len = length;
		root = new node();
		return *this;
	}

	/**
	 * Match s in trie as long as we can.
	 * This is a key method.
	 *
	 * @param s
	 * @param index - first character unmatched in s
	 * @return - last node matching s
	 *
	 * s[i] guides curr, |\ -> || -> |\ || -> |\
	 * Go on if curr can follow s[i], else break
	 */
	node* spread(const char *s, int *index) const {
		node *curr = root;
		int i, idx;

		/**
		 * [0, i) has been matched in trie
		 */
		for (i = 0; s[i] != 0; ++i) {
			idx = s[i] - bas;
			if (!curr->has_next(idx))
				break;
			curr = &curr->nexts[idx];
		}

		*index = i;
		return curr;
	}

	prftree& add(const char *s) {
		int i, idx;
		node *curr = spread(s, &i);
		for (; s[i] != 0; ++i) {
			idx = s[i] - bas;
			if (curr->nexts == NULL)
				curr->alloc_nexts(len);
			curr->nexts[idx].init();
			curr = &curr->nexts[idx];
		}
		++curr->cnt;
		return *this;
	}

	inline node* find(const char *s) const {
		int idx;
		node *curr = spread(s, &idx);
		if (s[idx] == 0 && curr->cnt > 0)
			return curr;
		return NULL;
	}

	void clear(void) {
		clear(root);
	}

	void clear(node *rt) {
		int i;
		if (rt->nexts == NULL)
			return;
		forr(i, bas, len) clear(&root->nexts[i]);
		delete rt->nexts;
		rt->nexts = NULL;
	}

	inline void print(void) {
		print(0, root, 0);
	}

	void print(const int idx, const node *rt, const int deepth) const {
		for (int i = 0; i < deepth; ++i)
			printf("\t");
		rt->print(idx, bas);
		for (int i = 0; i < len; ++i)
			if (rt->has_next(i))
				print(i, &rt->nexts[i], deepth+1);
	}
};

#endif
