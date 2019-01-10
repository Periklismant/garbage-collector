#include <stdio.h>
#include <stdlib.h>

#include "../headers/error.h"
#include "../headers/heap.h"

Heap* newHeap(unsigned capacity){
	Heap* heap = (Heap *) malloc(sizeof(Heap));
	heap->capacity = capacity;
	heap->used=0;
	heap->next=0;
	heap->array= (int *) malloc(capacity * sizeof(int));
	heap->marked= (unsigned char *) malloc((capacity)/2 * sizeof(unsigned char));
	for(int i = 0; i< capacity/2; i++)
		heap->marked[i]=0;
	return heap;
}

int fullHeap(Heap *heap){
	return (heap->capacity==heap->used);
}

int addCons(Heap *heap, int a, int b){
	 while (heap->marked[heap->next >> 1] & 0x02)
		if(heap->next < heap->capacity)
			heap->next++;
		else
			heap->next=0;  
	 heap->array[heap->next] = a;
	 heap->marked[heap->next >> 1] = 0x02;
	 heap->array[heap->next+1] = b;
	 int ret = heap->next;
	 heap->next+=2;
	 heap->next%=heap->capacity;
	 heap->used+=2;
	 return(ret);
}

int getHead(Heap *heap, int addr){
	return heap->array[addr];
}
	
int getTail(Heap *heap, int addr){
	return heap->array[addr+1];
}