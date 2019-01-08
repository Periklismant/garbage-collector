#ifndef STACK_H
#define STACK_H

typedef struct s{
	int top;
	unsigned capacity;
	int* array;
} Stack;

Stack* newStack(unsigned capacity);
int isFull(Stack *s);
int isEmpty(Stack *s);
int pop(Stack *s);
int push(Stack *s, int data);
int getith(Stack *s, int i);
int setith(Stack *s, int i, int data);

#endif