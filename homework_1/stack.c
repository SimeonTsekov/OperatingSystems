//------------------------------------------------------------------------
// NAME: Simeon Tsekov
// CLASS: XIa
// NUMBER: 24
// PROBLEM: #6
// FILE NAME: stack.c
// FILE PURPOSE:
// realizirane na stack(LIFO)
//------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    size_t size;
    size_t capacity;
    int *elements;
} stack;

void stack_init(stack *stack);
void stack_destroy(stack *stack);
int stack_empty(stack  *stack);
void stack_push(stack *stack, int n);
int stack_top(stack *stack);
void stack_pop(stack *stack);

int main(){
	stack stack;
	stack_init(&stack);
	
	printf("%d\n", stack_empty(&stack));
	stack_push(&stack, 5);
	stack_push(&stack, 3);
	stack_push(&stack, 7);
	stack_push(&stack, 8);
	printf("%d\n", stack_top(&stack));
	stack_pop(&stack);
	printf("%d\n", stack_top(&stack));

	stack_destroy(&stack);

	return 0;
}

//------------------------------------------------------------------------
// FUNCTION: stack_init
// zadava parametri na prazen stack
// PARAMETERS:
// stack *stack - stack, na koito se zadavat parametrite
//------------------------------------------------------------------------

void stack_init(stack *stack){
	stack->size = 0;
	stack->capacity = 10;
	stack->elements = NULL;
}

//------------------------------------------------------------------------
// FUNCTION: stack_destroy
// unishtojava stack
// PARAMETERS:
// stack *stack - stack, koito da byde unishtojen
//------------------------------------------------------------------------

void stack_destroy(stack *stack){
	stack->size = 0;
	stack->capacity = 0;
	free(stack->elements);
}

//------------------------------------------------------------------------
// FUNCTION: stack_empty
// proverqva dali stacka e prazen
// PARAMETERS:
// stack *stack - stack, koito da byde proveren
//------------------------------------------------------------------------

int stack_empty(stack  *stack){
	return stack->size == 0;
}

//------------------------------------------------------------------------
// FUNCTION: stack_push
// dobavq element v stacka
// PARAMETERS:
// stack *stack - stack, v koito da se dobavi element
// int n - element, koito da byde dobaven 
//------------------------------------------------------------------------

void stack_push(stack *stack, int n){
	stack->size++;
	stack->elements = realloc(stack->elements, sizeof(int)*stack->size);
	stack->elements[stack->size-1] = n;
}

//------------------------------------------------------------------------
// FUNCTION: stack_top
// vryshta posledniqt element v stacka
// PARAMETERS:
// stack *stack - stack, ot koito da se vzeme posledniq element
//------------------------------------------------------------------------

int stack_top(stack *stack){
	return stack->elements[stack->size-1];
}

//------------------------------------------------------------------------
// FUNCTION: stack_pop
// maha posledniq element ot stacka
// PARAMETERS:
// stack *stack - stack, ot koito da se da se mahne posledniq element
//------------------------------------------------------------------------

void stack_pop(stack *stack){
	stack->size--;
	stack->elements = realloc(stack->elements, sizeof(int)*stack->size);
}
