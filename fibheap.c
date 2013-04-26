/**
 * author: Liu Zhiyong<neesenk@gmail.com>
 */

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "fibheap.h"

static fibnode_t *fibnode_remove(fibnode_t *node)
{
	fibnode_t *ret = (node != node->left) ? node->left : NULL;

	node->right->left = node->left;
	node->left->right = node->right;
	node->left = node;
	node->right = node;

	return ret;
}

static void fibnode_insert(fibnode_t *a, fibnode_t *b)
{
	b->left = a;
	b->right = a->right;

	a->right->left = b;
	a->right = b;
}

static void fibnode_union(fibnode_t *a, fibnode_t *b)
{
	fibnode_t *atail = a->right;
	fibnode_t *btail = b->right;

	atail->left = b;
	b->right = atail;

	a->right = btail;
	btail->left = a;
}

static void fibheap_link(fibnode_t *node, fibnode_t *parent)
{
	if (parent->child == NULL)
		parent->child = node;
	else
		fibnode_insert(parent->child, node);
	node->parent = parent;
	node->mark = 0;
	parent->degree++;
}

static void fibheap_cut(fibheap_t *fib, fibnode_t *node, fibnode_t *parent)
{
	parent->degree--;
	parent->child = fibnode_remove(node);

	node->parent  = NULL;
	node->mark    = 0;

	fibnode_insert(fib->min, node);
}

static void __fibheap_insert(fibheap_t *fib, fibnode_t *node)
{
	if (fib->min == NULL) {
		fib->min = node;
	} else {
		fibnode_insert(fib->min, node);
		if (node->key < fib->min->key)
			fib->min = node;
	}
}

static void fibheap_consolidate(fibheap_t *fib)
{
	int i, max = 0;
	fibnode_t *tmp, *node, *min, *stack[1 + 8 * sizeof(long)] = {NULL};

	while ((min = fib->min) != NULL) {
		fib->min = fibnode_remove(min);
		for (i = min->degree; stack[i] != NULL; i++) {
			node = stack[i], stack[i] = NULL;
			if (min->key > node->key)
				tmp = min, min = node, node = tmp;

			fibheap_link(node, min);
		}
		stack[i] = min;
		if (i > max)
			max = i;
	}

	assert(max < 1 + 8 * sizeof(long));
	for (i = 0; i <= max; i++) {
		if (stack[i])
			__fibheap_insert(fib, stack[i]);
	}
}

static void fibheap_cascading_cut(fibheap_t *fib, fibnode_t *node)
{
	fibnode_t *parent;
	while ((parent = node->parent) != NULL) {
		if (node->mark == 0) {
			node->mark = 1;
			return;
		}
		fibheap_cut(fib, node, parent);
		node = parent;
	}
}

static fibnode_t *__fibheap_extract_min(fibheap_t *fib)
{
	fibnode_t *n, *min = fib->min;

	// 将min的子节点合并到堆的根节点上
	if (min->child) {
		min->child->parent = NULL;
		for (n = min->child->left; n != min->child; n = n->left)
			n->parent = NULL;
		fibnode_union(min, min->child);
	}

	fib->nodes--;
	fib->min = fibnode_remove(min);
	fibheap_consolidate(fib);

	FIBNODE_INIT(min);
	return min;
}

void fibheap_init(fibheap_t *fib)
{
	*fib = FIBHEAP_INIT();
}

void fibheap_fini(fibheap_t *fib)
{
	while (fib->min) {
		fibnode_t *min = __fibheap_extract_min(fib);
		FIBNODE_INIT(min);
	}
	*fib = FIBHEAP_INIT();
}

int fibheap_insert(fibheap_t *fib, fibnode_t *node)
{
	assert(node->key > FIBHEAP_MINKEY);

	FIBNODE_INIT(node);
	__fibheap_insert(fib, node);
	fib->nodes++;

	return 0;
}

fibnode_t *fibheap_min(fibheap_t *fib)
{
	return fib->min ? fib->min : NULL;
}

fibnode_t *fibheap_extract_min(fibheap_t *fib)
{
	return fib->min ? __fibheap_extract_min(fib) : NULL;
}

int fibheap_union(fibheap_t *fib, fibheap_t *other)
{
	if (!fib->min || !other->min) {
		if (!fib->min)
			*fib = *other;
		goto DONE;
	}

	// 链接两个堆的根的双向链表
	fibnode_union(fib->min, other->min);
	// 比较最小的节点
	if (fib->min->key > other->min->key)
		fib->min = other->min;
	fib->nodes += other->nodes;
DONE:
	*other = FIBHEAP_INIT();
	return 0;
}

static int __fibheap_decrease_key(fibheap_t *fib, fibnode_t *node, long key)
{
	fibnode_t *parent = node->parent;
	node->key = key;

	if (parent && node->key <= parent->key) {
		fibheap_cut(fib, node, parent);
		fibheap_cascading_cut(fib, parent);
	}

	if (node->key <= fib->min->key)
		fib->min = node;

	return 0;
}

int fibheap_decrease_key(fibheap_t *fib, fibnode_t *node, long key)
{
	assert(key > FIBHEAP_MINKEY && node->key > key);
	return node->key != key ? __fibheap_decrease_key(fib, node, key) : 0;
}

int fibheap_delete(fibheap_t *fib, fibnode_t *node)
{
	long okey = node->key;

	__fibheap_decrease_key(fib, node, FIBHEAP_MINKEY);
	assert(node == fib->min);
	__fibheap_extract_min(fib);

	node->key = okey;

	return 0;
}
