/**
 * author: Liu Zhiyong<neesenk@gmail.com>
 */

#ifndef __HEAP_H__
#define __HEAP_H__

typedef struct heapnode {
	long	key;
	size_t	pos;
} heapnode_t;

typedef struct heap {
	size_t nodes;
	size_t size;
	heapnode_t **heaps;
} heap_t;

void heap_fini(heap_t *heap);
int heap_init(heap_t *heap, size_t size);

void heap_destroy(heap_t *heap);
heap_t *heap_create(size_t size);

// 将other合并到heap
int heap_union(heap_t *heap, heap_t *other);

heapnode_t *heap_extrace_min(heap_t *heap);
heapnode_t *heap_min(heap_t *heap);
int heap_delete(heap_t *heap, heapnode_t *node);
int heap_insert(heap_t *heap, heapnode_t *node);
int heap_replace_key(heap_t *heap, heapnode_t *node, long key);

#endif /* __HEAP_H__ */
