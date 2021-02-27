//------------------------------------------------------------------------
// NAME: Simeon Tsekov
// CLASS: XIa
// NUMBER: 24
// PROBLEM: #1
// FILE NAME: lseek.c
// FILE PURPOSE:
// parser za abstraktna blokova sistema
//------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 2

typedef struct{
	char data;
	unsigned char nextElementAddress;
} block;

void encrypt(char *content, char *filename);

//------------------------------------------------------------------------
// FUNCTION: main
// kriptira syobshtenie ot standartniq vhod vyv file s ime ot konzolni argumenti i go dekriptira, izpolzvaiki lseek
// PARAMETERS:
// argc - broi argumenti
// **argv - masiv ot argumentite
//------------------------------------------------------------------------

int main(int argc, char **argv){
	char *filename = argv[1];
	char content[128];
	char buff[1];
	int i=0;

	while(1){	
		read(STDIN_FILENO, buff, 1);
		
		if(buff[0] == '\n'){break;}
		if(i == 128){break;}
		
		content[i] = buff[0];
		content[i+1] = '\0';	
		i++;
	}

	encrypt(content, filename);

	return 0;
}

//------------------------------------------------------------------------
// FUNCTION: encrypt
// kriptira syobshtenie i go zapisva vyv file
// PARAMETERS:
// *content - ukazatel kym sydyrjanieto
// *filename - ukazatel kym imeto na file
//------------------------------------------------------------------------

void encrypt(char *content, char *filename){
	char buff[BUFFER_SIZE];

	int fd = open(filename, O_WRONLY);

	int i;
	int n = 2;
	for(i=0; i<=strlen(content); i++){
		buff[0] = content[i];
		buff[1] = (i+2)*10;

		if(i==strlen(content)){
			buff[0] = '\0';
			buff[1] = 0;
		} else if(i%2==0) {
			buff[0] = content[i];
			buff[1] = (i+2)*7; 
		}

		write(fd, buff, BUFFER_SIZE);
		lseek(fd, buff[1], SEEK_SET);
	}	

	close(fd);
}
