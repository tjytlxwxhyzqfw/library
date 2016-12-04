/** @file
 *
 * @brief Prefix Tree (Trie)
 * 
 * A prefix tree using ascii indices
 *
 * @author wcc
 * @date 2016-10-14
 */

#ifndef __INCLUDE_PRFTREE_H
#define __INCLUDE_PRFTREE_H

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>

/** @brief Prefix tree
 */
struct Prftree {
	/** @brief Prefix tree node
	 */
	struct Node {
		int cnt;
		Node *nexts;

		Node(void) {
			cnt = 0;
			nexts = NULL;
		}

		~Node(void) {
			if (nexts) {
				delete[] nexts;
			}
		}

		inline void alloc_nexts(const int len) {
			assert(nexts == NULL);
			nexts = new Node[len];
		}

		inline bool has_next(int i) const {
			//TODO: why use '>=' ?
			return nexts != NULL && nexts[i].cnt >= 0;
		}
		

		void print(int cidx, int bas) const {
			printf("%c(%3d): %2d)\n", cidx+bas, cidx+bas, cnt);
		}
	};

	int bas, len;
	Node *root;

	Prftree(void) {
		bas = len = 0;
		root = NULL;
	}

	~Prftree(void) {
		if (root) {
			clear();
			delete root;
		}
	}

	/** @brief Initialize a prefix tree
	 *
	 * @param base - Sequence offset in ascii table
	 * @param length - Length of sequence
	 */
	inline void build(const int base, const int length) {
		bas = base;
		len = length;
		assert(root == NULL);
		root = new Node();
	}

	/** @brief Spread a string in prefix tree
	 *
	 * Find the longgest prefix in of s in tree.
	 *
	 * @param s - string to be spreaded
	 * @param index - index of the first character without
	 *	a corresponding node in prefix tree.
	 * @return The last node in the prefix path
	 *
	 * @par Implementation Details
	 * <pre>
	 * s[i] guides curr, like this:
	 * |\ -> || -> |\ || -> |\
	 * Go on if curr can follow s[i], else break.
	 * </pre>
	 */
	Node* spread(const char *s, int *index) const {
		Node *curr = root;
		int i, idx;

		/* [0, i) has been matched in trie */
		for (i = 0; s[i] != 0; ++i) {
			idx = s[i] - bas;
			if (!curr->has_next(idx))
				break;
			curr = &curr->nexts[idx];
		}

		*index = i;
		return curr;
	}

	/** @brief Add a string to prefix tree
	 *
	 * This function add into trie some nodes representing string *s,
	 * only the 'cnt' of the last node are increased.
	 *
	 * Note that this is a very simple adding procedure,
	 * for it dose a very simple thing and in most cases,
	 * you might have to write your own adding function
	 * <strong>outside</strong> the tree.
	 *
	 * @param s - target string
	 */ 
	void add(const char *s) {
		int i, idx;
		Node *curr = spread(s, &i);
		for (; s[i] != 0; ++i) {
			idx = s[i] - bas;
			if (curr->nexts == NULL)
				curr->alloc_nexts(len);
			curr = &curr->nexts[idx];
		}
		++curr->cnt;
	}

	/** @brief Find given string in prefix string
 	 *
	 * @param s - target string
	 * @return - tail node of s in tree, NULL if no such node.
	 */
	inline Node* find(const char *s) const {
		int idx;
		Node *curr = spread(s, &idx);
		if (s[idx] == 0 && curr->cnt > 0)
			return curr;
		return NULL;
	}

	/** @brief Clear prefix tree
	 *
	 * Driver of recursive function clear(Node*).
	 */
	inline void clear(void) {
		clear(root);
	}

	/** @brief Clear prefix tree recursivly
	 *
	 * @param rt - Root of a subtree
	 */
	void clear(Node *rt) {
		if (rt->nexts == NULL)
			return;
		for (int i = 0; i < len; ++i)
			clear(&rt->nexts[i]);
		delete[] rt->nexts;
		rt->nexts = NULL;
	}

	/** @brief Print prefix tree
	 *
	 * Driver of recursive function print(const int, const Node*, const int)
	 */
	inline void print(void) {
		print(0, root, 0);
	}

	/**
	 * @brief Print prefix tree recursivly
	 *
	 * @param idx - Index of current node
	 * @param rt - Root of subtree
	 * @param deepth - Resursion deepth, used for indent
	 */
	void print(const int idx, const Node *rt, const int deepth) const {
		for (int i = 0; i < deepth; ++i)
			printf("\t");
		rt->print(idx, bas);
		for (int i = 0; i < len; ++i)
			if (rt->has_next(i))
				print(i, &rt->nexts[i], deepth+1);
	}
};

#endif
