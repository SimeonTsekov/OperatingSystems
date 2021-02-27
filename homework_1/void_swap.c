//------------------------------------------------------------------------
// NAME: Simeon Tsekov
// CLASS: XIa
// NUMBER: 24
// PROBLEM: #4
// FILE NAME: void_swap.c
// FILE PURPOSE:
// programata smenq stoinostite na dve promenlivi ot proizvolen tip
//------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(void *a, void *b, size_t size);

int main(){
	int a = 2;
	int b = 4;

	swap(&a, &b, sizeof(int));

	printf("%d %d\n", a, b);

	return 0;
}

//------------------------------------------------------------------------
// FUNCTION: swap
// smenq stoinostite na dve promenlivi ot proizvolen tip
// PARAMETERS:
// void *a, void *b - promenlivi, chiito stoinosti da bydat razmeneni
// size_t size - razmer na tipa na promenlivite
//------------------------------------------------------------------------

void swap(void *a, void *b, size_t size){
	void *temp = malloc(size);
	
	memcpy(temp, b, size);
	memcpy(b, a, size);
	memcpy(a, temp, size);

	free(temp);
}
