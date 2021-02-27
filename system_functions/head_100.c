#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define BUFFER_SIZE 20

int main(){
	int fd = open("a.txt", O_RDONLY);	   
	char buffer[BUFFER_SIZE];

	int read_value = read(fd, buffer, BUFFER_SIZE);

	write(STDOUT_FILENO, buffer, read_value);
	write(STDOUT_FILENO, "\n", 1);	

	close(fd);

	return 0;
}
