/**
 * @author wcc
 * 2016-10-14
 */

#ifndef _TJYTLXWXHYZQFW_H
#define _TJYTLXWHHYZQFW_H

#include <algorithm>
#include <cstdio>
#include <cstring>

struct prftree {
	struct node {
		char cidx;
		int cnt;
		node **nexts;

		inline struct node& init(const char ch, const char bas, const int len) {
			cidx = ch - bas;
			cnt = 0;
			nexts = new node*[len];
			std::fill(nexts, nexts+len, reinterpret_cast<node*>(0));
			return *this;
		}

		void print(int bas) const {
			printf("%c(cnt: %2d)\n", cidx+bas, cnt);
		}

		void print(void) const {
			printf("%3d(cnt: %2d)\n", cidx, cnt);
		}
	};

	int bas, len;
	node *root;

	inline prftree& init(const int base, const int length) {
		bas = base;
		len = length;
		root = new node();
		root->init(0, base, length);
		return *this;
	}

	prftree& add(const char *s) {
		int i, idx;
		node *curr = spread(s, &i);
		for (; s[i] != 0; ++i) {
			idx = s[i] - bas;
			curr->nexts[idx] = new node();
			curr->nexts[idx]->init(s[i], bas, len);
			curr = curr->nexts[idx];
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

	/**
	 * Math s in trie as long as we can.
	 * This is a key method.
	 *
	 * @param s
	 * @param index - first character unmatched in s
	 * @return - last node matching s
	 */
	node* spread(const char *s, int *index) const {
		node *curr = root;
		int i, idx;

		for (i = 0; s[i] != 0; ++i) {
			idx = s[i] - bas;
			if (curr->nexts[idx] == NULL)
				break;
			curr = curr->nexts[idx];
		}

		*index = i;
		return curr;
	}

	inline void print(void) {
		print(root, 0);
	}

	void print(const node *rt, const int deepth) const {
		for (int i = 0; i < deepth; ++i)
			printf("\t");
		rt->print(bas);
		for (int i = 0; i < len; ++i)
			if (rt->nexts[i] != NULL)
				print(rt->nexts[i], deepth+1);
	}
};

#endif
