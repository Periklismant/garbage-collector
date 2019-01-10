#define UTILS_H

#include "./heap.h"
#include "./stack.h"

int garbageCollect(Heap *h, Stack *s);
int DFS(Heap *h, int elem);
int sweep(Heap *h);

