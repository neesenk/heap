#include <stdio.h>
#include "heap.h"
#include "fibheap.h"
#include <assert.h>
#include <stdlib.h>


#define N 1000000
heapnode_t nodes[N];

#include <sys/time.h>
int main(int argc, char *argv[])
{
	int i = 0;
	int n = atoi(argv[1]);
	heap_t heap[1];
	heapnode_t *r = NULL;
	struct timeval tv[2];

	if (n > N)
		n = N;

	heap_init(heap, n);

	gettimeofday(tv, NULL);
	for (i = n - 1; i >= 0; i--) {
		nodes[i].key = i;
		nodes[i].pos = -1;
		assert(heap_insert(heap, nodes + i) == 0);
	}
	gettimeofday(tv + 1, NULL);
	printf("insert %lf\n", tv[1].tv_sec + tv[1].tv_usec/1000000.0 - tv[0].tv_sec - tv[0].tv_usec/1000000.0);

	gettimeofday(tv, NULL);
	for (i = 0; i < n; i++) {
		r = heap_extrace_min(heap);
		assert(r != NULL);
		assert(r->key == i);
	}
	gettimeofday(tv + 1, NULL);
	printf("insert %lf\n", tv[1].tv_sec + tv[1].tv_usec/1000000.0 - tv[0].tv_sec - tv[0].tv_usec/1000000.0);

	for (i = 0; i < n; i++) {
		nodes[i].key = i;
		nodes[i].pos = -1;
		assert(heap_insert(heap, nodes + i) == 0);
	}

	i = random() % n;
	heap_replace_key(heap, nodes + i, -1);
	assert(heap_min(heap) == nodes + i);
	heap_replace_key(heap, nodes + i, i);
	assert(heap_min(heap) == nodes);

	for (i = 0; i < 100; i++) {
		r = nodes + random() % n;
		if (r->pos == (size_t)-1)
			continue;
		assert(heap_delete(heap, r) == 0);
		r->pos = (size_t)-1;
	}

	for (i = 0; i < n; i++) {
		if (nodes[i].pos == (size_t) -1)
			continue;
		r = heap_extrace_min(heap);
		assert(r != NULL);
		assert(r->key == i);
	}

	return 0;
}
