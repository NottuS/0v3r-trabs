#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* |-- node_set.h */
typedef struct node_set {
	int* nodes;
	ssize_t size;
	ssize_t offset;
} node_set;

static node_set* set_new(ssize_t size);
static void set_insert(node_set* nodes, int node);
static void set_merge(node_set* dest, node_set* source);
static void set_free(node_set* nodes);
node_set* cis(int i, int s);
