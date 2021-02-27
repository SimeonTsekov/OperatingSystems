//------------------------------------------------------------------------
// NAME: Simeon Tsekov
// CLASS: XIa
// NUMBER: 24
// PROBLEM: #5
// FILE NAME: strdup.c
// FILE PURPOSE:
// programata syzdava duplikat na daden simvolen niz
//------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strdup(const char *a);

int main(){
	char a[4] = {'a', 'b', 'c', '\0'};

	char *b = strdup(a);
	printf("%s\n", b);

	free(b);

	return 0;
}

//------------------------------------------------------------------------
// FUNCTION: strdup
// syzdava kopie na simvolen niz i nasochva ukazatel kym nego
// PARAMETERS:
// const char * - ukazatel kym niza, na koito da se syzdade kopie
//------------------------------------------------------------------------

char *strdup(const char *a){ 
	char *b = malloc(sizeof(a));

	strcpy(b, a);

	return b;
}
