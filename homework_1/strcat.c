//------------------------------------------------------------------------
// NAME: Simeon Tsekov
// CLASS: XIa
// NUMBER: 24
// PROBLEM: #2
// FILE NAME: strcat.c
// FILE PURPOSE:
// файла извършва операцията конкатенация(слепване) на два символни низа
//------------------------------------------------------------------------
 	
#include <stdio.h>
#include <stdlib.h>

char* strcat( char* destination, const char* source );

int main(){
	char source[5] = {'a', 'b', 'c', 'd', '\0'};
	char destination[4] = {'e', 'f', 'g', '\0'};

	printf("%s\n", strcat(source, destination));

	return 0;
}

//------------------------------------------------------------------------
// FUNCTION: strcat
// функцията извършва конкатенация на два символни низа
// PARAMETERS:
// char* destination - указател към низът, към който се конкатенира
// char* source - указател към низът, който се конкатенира
//------------------------------------------------------------------------

char* strcat( char* destination, const char* source ){
	int i, j;

	for(i=0; destination[i]!='\0'; i++);
	for(j=0; source[j]!='\0'; j++, i++){
		destination[i] = source[j];
	}
	
	destination[i] = '\0';

	return destination;
}
