#ifndef HEAP_H
#define HEAP_H 

typedef struct heap{
	int next;
	unsigned capacity;
	unsigned used;
	int* array;
	unsigned char* marked;
} Heap;

Heap* newHeap(unsigned capacity);
int fullHeap(Heap *h);
int addCons(Heap *h, int a, int b);
int getHead(Heap *h, int addr);
int getTail(Heap *h, int addr);

#endif