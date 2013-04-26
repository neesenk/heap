#include <stdio.h>
#include "heap.h"
#include "fibheap.h"
#include <assert.h>
#include <stdlib.h>


#define N 1000000
fibnode_t nodes[N];

#include <sys/time.h>
int main(int argc, char *argv[])
{
	int i = 0;
	int n = atoi(argv[1]);
	fibheap_t heap[1];
	fibnode_t *r = NULL;
	struct timeval tv[2];

	if (n > N)
		n = N;

	fibheap_init(heap);

	gettimeofday(tv, NULL);
	for (i = 0; i < n; i++) {
		nodes[i].key = i;
		assert(fibheap_insert(heap, nodes + i) == 0);
	}
	gettimeofday(tv + 1, NULL);
	printf("insert %lf\n", tv[1].tv_sec + tv[1].tv_usec/1000000.0 - tv[0].tv_sec - tv[0].tv_usec/1000000.0);

	gettimeofday(tv, NULL);
	for (i = 0; i < n; i++) {
		r = fibheap_extract_min(heap);
		assert(r != NULL);
		if (r->key != i) {
			printf("r->key = %ld, i = %d\n", r->key, i);
		}
		assert(r->key == i);
	}
	gettimeofday(tv + 1, NULL);
	printf("extract %lf\n", tv[1].tv_sec + tv[1].tv_usec/1000000.0 - tv[0].tv_sec - tv[0].tv_usec/1000000.0);


	for (i = 0; i < n; i++) {
		nodes[i].key = i;
		assert(fibheap_insert(heap, nodes + i) == 0);
	}

	i = random() % n;
	fibheap_decrease_key(heap, nodes + i, -1);
	assert(fibheap_extract_min(heap) == nodes + i);
	nodes[i].key = -1;

	for (i = 0; i < 100; i++) {
		r = nodes + random() % n;
		if (r->key == -1)
			continue;
		assert(fibheap_delete(heap, r) == 0);
		r->key = -1;
	}

	for (i = 0; i < n; i++) {
		if (nodes[i].key == -1)
			continue;
		r = fibheap_extract_min(heap);
		assert(r != NULL);
		assert(r->key == i);
	}

	gettimeofday(tv, NULL);
	for (i = 0; i < n; i++) {
		nodes[i].key = i;
		assert(fibheap_insert(heap, nodes + i) == 0);
	}
	gettimeofday(tv + 1, NULL);
	printf("insert %lf\n", tv[1].tv_sec + tv[1].tv_usec/1000000.0 - tv[0].tv_sec - tv[0].tv_usec/1000000.0);

	gettimeofday(tv, NULL);
	for (i = 0; i < n; i++) {
		fibheap_decrease_key(heap, nodes + i, i - 1);
	}
	gettimeofday(tv + 1, NULL);
	printf("decrease %lf\n", tv[1].tv_sec + tv[1].tv_usec/1000000.0 - tv[0].tv_sec - tv[0].tv_usec/1000000.0);

	return 0;
}
