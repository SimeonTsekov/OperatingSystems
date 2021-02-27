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

#define BUFFER_SIZE 2

typedef struct{
	char data;
	unsigned char nextElementAddress;
} block;

//------------------------------------------------------------------------
// FUNCTION: main
// sistema ot blokove s chlen za simvol i chlen za adres na sledvashtiq element
// programata chete dva simvola i popylva bloka
// izvejda se simvola v bloka i adresa se premestva na sledvashtiq blok
// PARAMETERS:
// argc - broi argumenti
// **argv - masiv ot argumentite
//------------------------------------------------------------------------

int main(int argc, char **argv){
	int fd = open(argv[1], O_RDONLY);
	char buff[BUFFER_SIZE];

	block block;
	int i = 0;  
	
	while(1){
		read(fd, buff, BUFFER_SIZE);

		block.data = buff[0];
		block.nextElementAddress = buff[1];
		if(!block.nextElementAddress){break;}
		if(i == 128){break;}
		write(STDOUT_FILENO, buff, 1);

		lseek(fd, block.nextElementAddress, SEEK_SET);
		i++;
	}

	write(STDOUT_FILENO, "\n", 1);
	close(fd);

	return 0;
}
