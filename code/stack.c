#include <stdio.h>
#include <stdlib.h>

#include "../headers/stack.h"
#include "../headers/error.h"


Stack* newStack(unsigned capacity){
	Stack* stack = (Stack *) malloc(sizeof(Stack));
	stack->capacity = capacity;
	stack->top = -1;
	stack->array = (int *) malloc(stack->capacity * sizeof(int));
	return stack;
}

int isFull(Stack* stack)
{ return stack->top == stack->capacity -1; }

int isEmpty(Stack* stack)
{ return stack->top == -1; }

int push(Stack* stack, int data)
{
	if (isFull(stack))
		return STACK_OVERFLOW;
	stack->array[++stack->top] = data;
	return 0;
}

int pop(Stack* stack){
	if(isEmpty(stack))
		return STACK_EMPTY;
	return stack->array[stack->top--]; 
}

int getith(Stack* stack, int i){
	int true_index = stack->top - i;
	if(true_index < 0)
		return STACK_OUT_OF_BOUNDS;
	return stack->array[true_index];
}

int setith(Stack* stack, int i, int data){
	int true_index = stack->top - i;
	if(true_index < 0)
		return STACK_OUT_OF_BOUNDS;
	stack->array[true_index] = data;
	return 0;
}