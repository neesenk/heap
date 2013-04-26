/**
 * author: Liu Zhiyong<neesenk@gmail.com>
 *
 * fibheap_union  O(1)
 * fibheap_insert O(1)
 * fibheap_min	  O(1)
 * fibheap_delete O(lgn) amortized, O(n) worst case
 * fibheap_extract_min  O(lgn) amortized, O(n) worst case
 * fibheap_decrease_key O(1) amortized, O(lgn) worst case
 */

#ifndef __FIBHEAP_H__
#define __FIBHEAP_H__

#define FIBHEAP_MINKEY	LONG_MIN

#define FIBHEAP_INIT() (fibheap_t){0, NULL}
#define FIBNODE_INIT(_n) do { fibnode_t *n = _n; n->parent=n->child=NULL;n->left=n->right=n;} while (0)

typedef struct fibnode {
	long   key;
	char   mark;		// 标记节点成为另一个节点的子节点后失去过子节点
	unsigned short degree;	// 子节点数目
	struct fibnode *parent;	// 父指针
	struct fibnode *child;	// 字节点
	struct fibnode *left;	// 左兄节点
	struct fibnode *right;	// 右兄节点, 这两个指针窜成一个双向链表, 没有大小次序
} fibnode_t;

typedef struct fibheap {
	size_t nodes;		// 包含的关键字数目
	struct fibnode *min;	// 指向fibheap中最小关键字的根节点
} fibheap_t;

void fibheap_init(fibheap_t *fib);
void fibheap_fini(fibheap_t *fib);

int fibheap_union(fibheap_t *fib, fibheap_t *other);

int fibheap_insert(fibheap_t *fib, fibnode_t *node);
int fibheap_delete(fibheap_t *fib, fibnode_t *node);
int fibheap_decrease_key(fibheap_t *fib, fibnode_t *node, long key);
fibnode_t *fibheap_extract_min(fibheap_t *fib);
fibnode_t *fibheap_min(fibheap_t *fib);

#endif /* _FIBHEAP_H_ */
