#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define BUFFER_SIZE 1000

int main(){
	int fd = open("a.txt", O_RDONLY);
	int i=0;	   
	int new_lines = 0;
	char buffer[BUFFER_SIZE];

	read(fd, buffer, BUFFER_SIZE);

	while(1){
		if(buffer[i] == '\n'){new_lines++;}
		if(new_lines == 10){break;}

		i++;
	}

	write(STDOUT_FILENO, buffer, i);
	write(STDOUT_FILENO, "\n", 1);	

	close(fd);

	return 0;
}
