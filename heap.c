/**
 * author: Liu Zhiyong<neesenk@gmail.com>
 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "heap.h"

#define CHILD_INDEX(n)	 (((n) * 2) + 1)
#define PARENT_INDEX(n)	 (((n) - 1) / 2)

#define HEAP_SWAP(h,a,b) do {				\
	struct heapnode *tmp = h->heaps[a];		\
	h->heaps[a] = h->heaps[b], h->heaps[a]->pos = a;\
	h->heaps[b] = tmp, h->heaps[b]->pos = b;	\
} while (0)

static inline void heap_siftdown(heap_t *h, size_t pos)
{
	size_t child = CHILD_INDEX(pos);
	for (; child < h->nodes; pos = child, child = CHILD_INDEX(child)) {
		if (child + 1 < h->nodes) {
			if (h->heaps[child]->key > h->heaps[child+1]->key)
				child++;
		}

		if (h->heaps[pos]->key <= h->heaps[child]->key)
			break;
		HEAP_SWAP(h, pos, child);
	}
}

static inline void heap_siftup(heap_t *h, size_t pos)
{
	size_t parent = 0;
	for (; pos; pos = parent) {
		parent = PARENT_INDEX(pos);
		if (h->heaps[pos]->key >= h->heaps[parent]->key)
			break;
		HEAP_SWAP(h, pos, parent);
	}
}

static inline int heap_resize(heap_t *h, size_t size)
{
	heapnode_t **n = NULL;
	assert(size > h->size);
	if (!(n = realloc(h->heaps, size * sizeof(*n))))
		return -1;
	h->heaps = n;
	h->size = size;
	return 0;
}

int heap_insert(heap_t *h, heapnode_t *node)
{
	if (h->nodes >= h->size && heap_resize(h, h->size * 2))
		return -1;

	node->pos	   = h->nodes;
	h->heaps[h->nodes] = node;
	h->nodes++;

	heap_siftup(h, node->pos);

	return 0;
}

int heap_delete(heap_t *h, heapnode_t *node)
{
	assert(node->pos < h->nodes && h->heaps[node->pos] == node);

	h->nodes--;
	h->heaps[node->pos] = h->heaps[h->nodes];
	h->heaps[node->pos]->pos = node->pos;

	heap_siftdown(h, node->pos);
	node->pos = -1;
	return 0;
}

int heap_replace_key(heap_t *h, heapnode_t *node, long key)
{
	long okey = node->key;
	assert(node->pos < h->nodes && h->heaps[node->pos] == node);

	node->key = key;
	if (okley > key)
		heap_siftup(h, node->pos);
	else if (okey < key)
		heap_siftdown(h, node->pos);

	return 0;
}

heapnode_t *heap_min(heap_t *h)
{
	return h->nodes > 0 ? h->heaps[0] : NULL;
}

heapnode_t *heap_extrace_min(heap_t *h)
{
	heapnode_t *r = NULL;
	if (h->nodes > 0) {
		r = h->heaps[0];
		heap_delete(h, r);
	}

	return r;
}

int heap_union(heap_t *h, heap_t *o)
{
	ssize_t i = 0;
	if (h->nodes + o->nodes > h->size && heap_resize(h, h->size + o->size))
		return -1;

	for (i = h->nodes; i < h->nodes + o->nodes; i++) {
		h->heaps[i] = o->heaps[i - o->nodes];
		h->heaps[i]->pos = i;
	}

	h->nodes += o->nodes;
	for (i = PARENT_INDEX(h->nodes); i >= 0; i--)
		heap_siftdown(h, i);
	heap_fini(o);

	return 0;
}

heap_t *heap_create(size_t size)
{
	heap_t *h = calloc(1, sizeof(*h));
	heapnode_t **n = calloc(size, sizeof(*n));

	if (!h || !n) {
		free(h);
		free(n);
		return NULL;
	}

	h->heaps = n;
	h->size = size;
	return h;
}

void heap_destroy(heap_t *h)
{
	if (h) {
		free(h->heaps);
		free(h);
	}
}

int heap_init(heap_t *h, size_t size)
{
	heapnode_t **n = calloc(size, sizeof(*n));
	if (!n)
		return -1;

	h->nodes = 0;
	h->heaps = n;
	h->size = size;
	return 0;
}

void heap_fini(heap_t *h)
{
	free(h->heaps);
	memset(h, 0, sizeof(*h));
}
