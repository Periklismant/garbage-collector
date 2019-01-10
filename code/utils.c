#include <stdio.h>
#include <stdlib.h>

#include "../headers/error.h"
#include "../headers/heap.h"
#include "../headers/stack.h"

int DFS(Heap *h, int elem){
	if(!(elem & 0x00000001))
		return 0;
	if(h->marked[(elem-1) >> 1] & 0x02){
		h->marked[(elem-1) >> 1] = 0x03;
		//printf("DFS with elem: %d\n", h->array[elem-1]);
		DFS(h, h->array[elem-1]);
		//printf("DFS with elem: %d\n", h->array[elem]);
		DFS(h, h->array[elem]);
		return 0;
	}
	return 0;
}

int sweep(Heap* h){
	for(int i=0; i<h->capacity; i+=2)
		if(!(h->marked[i >> 1] & 0x01)){
			h->marked[i >> 1] = 0;
			h->used-=2;
		}
		else 
			h->marked[i>>1] = 0x02;
	return 0;
}

int garbageCollect(Heap *h, Stack *s){
	//printf("WE ARE IN GC\n");
	//for(int j=0; j<h->capacity/2; j++)
	//	printf("\tHeap block %d has marked tag: %d\n", j, h->marked[j]);
	for(int i=s->top; i>-1; i--){
	//	printf("DFS with elem: %d\n", s->array[i]);
		DFS(h, s->array[i]);
	}
	//for(int j=0; j<h->capacity/2; j++)
	//	printf("\tBEFORE SWEEP Heap block %d has marked tag: %d\n", j, h->marked[j]);
	sweep(h);
	//printf("GC: BYEE\n");
	//for(int j=0; j<h->capacity/2; j++)
//		printf("\tHeap block %d has marked tag: %d\n", j, h->marked[j]);
	return 0;
}